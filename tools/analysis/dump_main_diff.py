#!/usr/bin/env python3
"""dump_main_diff.py - positional target-vs-compiled dump for a MAIN-BINARY function.

    python3 tools/analysis/dump_main_diff.py <FUNC> <ADDR> <SIZE> <src.c> [skip-step ...]

Compiles <src.c> with the @CompileEnv / @CompileFlags the file itself declares,
runs its @PostProc chain (strip_leading_instructions, swap_instructions,
remap_stack_slots, scheduler_normalizer, convert_or_to_addu,
patch_insn_reg_field), disassembles both sides and prints them word by word
with a `<<` on every line whose mnemonic or REGISTERS differ.  Immediates and
memory displacements are masked, because at an unlinked relocation site they
are 0 by construction.

Naming a step on the command line SKIPS it, which is how you find out what a
PostProc step is actually contributing -- `... src.c scheduler_normalizer`
answers "is the C right, or is the normalizer carrying it?".

Why this and not test_single_match: that tool gives a percentage and the first
twenty mismatch indices.  For anything bigger than a leaf you need the two
streams side by side, because one surplus word early misaligns everything
after it and the percentage then means nothing.

⚠️ Directive lines are matched ANCHORED to the start of a comment line.  File
headers narrate retired directives in prose, and an unanchored search picks
those up -- the same prose-scrape trap CLAUDE.md warns about for grep counts.
"""

import subprocess,re,sys,os,shutil,tempfile
ROOT='.'
def parse(src):
    txt=open(src).read()
    env={}; flags=[]; steps=[]
    # ⚠️ Only ANCHORED directive lines.  Headers narrate retired directives in
    # prose ("`@PostProc: custom_script ...`, now retired"), and an unanchored
    # search picks those up -- the same prose-scrape trap CLAUDE.md warns about
    # for `grep -c` counts.
    for m in re.finditer(r'(?m)^\s*(?:/\*|\*)?\s*@CompileEnv:\s*(.+)',txt):
        for kv in m.group(1).split():
            if '=' in kv: k,v=kv.split('=',1); env[k]=v.rstrip('*/ ')
    for m in re.finditer(r'(?m)^\s*(?:/\*|\*)?\s*@CompileFlags:\s*(.+)',txt):
        flags+= [x for x in m.group(1).replace('*/','').split() if x.startswith('-')]
    for m in re.finditer(r'(?m)^\s*(?:/\*|\*)?\s*@PostProc:\s*(.+)',txt):
        steps.append(m.group(1).replace('*/','').strip())
    return env,flags,steps
_TMP=os.path.join(tempfile.gettempdir(),'mdiff-%d'%os.getpid())
os.makedirs(_TMP,exist_ok=True)
def _t(n):
    """Per-process temp path.  These were fixed /tmp/m*.o names, so two of
    these tools running at once silently read each other's objects -- which is
    what made a parallel screen of this sweep produce nonsense on 2026-09-20."""
    return os.path.join(_TMP,n)
def target(addr,size):
    off=(addr-0x80041000)+0x800
    d=open(ROOT+'/disks/SLPS_000.65','rb').read()[off:off+size]
    open(_t('mt.bin'),'wb').write(d)
    out=subprocess.run(['mips-linux-gnu-objdump','-D','-z','-b','binary','-m','mips:3000','-EL',
                        '--adjust-vma=0x%x'%addr,_t('mt.bin')],capture_output=True,text=True).stdout
    return [(int(m.group(1),16),int(m.group(2),16),m.group(3).strip()) for m in
            (re.match(r'^\s*([0-9a-f]{8}):\s+([0-9a-f]{8})\s+(.*)$',l) for l in out.splitlines()) if m]
def compiler(src):
    """@Compiler: value, defaulting to ido52 exactly like parse_postproc does."""
    m=re.search(r'(?m)^\s*(?:/\*|\*)?\s*@Compiler:\s*(\S+)',open(src).read())
    cc=m.group(1).rstrip('*/ ') if m else 'ido52'
    return cc if cc in {'ido52','ido53','gcc260','gcc263','gcc263p','gcc272','gcc281'} else 'ido52'
def build(src,fn,extra_env=None,skip=()):
    env,flags,steps=parse(src)
    if extra_env: env.update(extra_env)
    e=dict(os.environ); e.update(env)
    cc=compiler(src)
    if cc.startswith('gcc'):
        # Library-region files are Psy-Q GCC.  Hardcoding the IDO cc here made
        # this tool reject their flags outright (`-fno-delayed-branch`), which
        # is why GCC functions could not be diffed at all.  Same wrapper and
        # flag handling test_single_match.py uses, so both agree.
        # gccwrap's own ASPSX-model flags are spelled `--fill-earliest`,
        # `--macro-lui-fill`, ... and must be passed as gccwrap ARGUMENTS.
        # Folding them into --extra-flags handed them to cc1, which rejects
        # them outright ("Invalid option `--fill-earliest'"), so every GCC
        # main-binary function carrying one was undiffable.  Split the way
        # test_single_match.py does, so the two agree.  (Every wrapper flag
        # in the tree spells its value with `=`, so a prefix split is exact.)
        wrap=[f for f in flags if f.startswith('--')]
        cc1 =[f for f in flags if not f.startswith('--')]
        cmd=[sys.executable,ROOT+'/tools/gcc/gccwrap.py',src,'-o',_t('m0.o'),
             '--gcc-version',cc[3:]]+wrap+['--extra-flags=%s'%' '.join(cc1)]
    else:
        cmd=[ROOT+'/tools/ido-static-recomp/build/5.2/out/cc','-O2',*flags,'-G0','-mips1','-EL','-non_shared','-c','-I','include',src,'-o',_t('m0.o')]
    r=subprocess.run(cmd,cwd=ROOT,env=e,capture_output=True,text=True)
    if r.returncode: print(r.stderr[-3000:]); sys.exit(1)
    cur=_t('m0.o'); i=0
    for st in steps:
        name=st.split()[0]
        if name in skip: continue
        nxt=_t('m%d.o'%(i+1)); i+=1
        if name=='strip_leading_instructions':
            cnt=st.split('--count')[1].strip()
            c=[sys.executable,'tools/ido/strip_leading_instructions.py',cur,nxt,'--count',cnt]
        elif name=='scheduler_normalizer':
            c=[sys.executable,'tools/postproc/scheduler_normalizer.py',cur,'--target','disks/SLPS_000.65','--address',ADDR_S,'-o',nxt]
            if '--block-aware' in st: c.append('--block-aware')
        elif name=='remap_stack_slots':
            mp=re.search(r"--map\s+'?([^'\s]+)'?",st).group(1)
            c=[sys.executable,'tools/ido/remap_stack_slots.py',cur,nxt,'--map',mp]
        elif name=='swap_instructions':
            o1=re.search(r'--offset1\s+(\S+)',st).group(1); o2=re.search(r'--offset2\s+(\S+)',st).group(1)
            c=[sys.executable,'tools/ido/swap_instructions.py',cur,nxt,'--offset1',o1,'--offset2',o2]
        elif name=='patch_stack_complete':
            old=re.search(r'--old\s+(\S+)',st).group(1); new=re.search(r'--new\s+(\S+)',st).group(1)
            c=[sys.executable,'tools/ido/patch_stack_complete.py',cur,nxt,'--old',old,'--new',new]
        elif name=='convert_or_to_addu':
            c=[sys.executable,'tools/ido/convert_or_to_addu.py',cur,nxt]
        elif name=='swap_branch_operands':
            offs=re.findall(r'0x[0-9a-fA-F]+|\b\d+\b',st.split('swap_branch_operands',1)[1])
            c=[sys.executable,'tools/ido/swap_branch_operands.py',cur,nxt,'--offsets']+offs
        elif name=='patch_insn_reg_field':
            c=[sys.executable,'tools/ido/patch_insn_reg_field.py',cur,nxt]
            for p in st.split()[1:]:
                if p!='--patch': c+= ['--patch',p]
        else:
            print('SKIP unsupported step:',name); shutil.copy(cur,nxt); cur=nxt; continue
        r=subprocess.run(c,cwd=ROOT,capture_output=True,text=True)
        if r.returncode: print('STEP FAIL',name,r.stdout[-1500:],r.stderr[-1500:]); sys.exit(1)
        cur=nxt
    return cur
def _scan(d,fn):
    rows=[];st=False
    for line in d.splitlines():
        if f'<{fn}>:' in line: st=True; continue
        if not st: continue
        if re.match(r'^[0-9a-f]{8} <',line): break
        m=re.match(r'^\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)$',line)
        if m: rows.append((int(m.group(1),16),int(m.group(2),16),m.group(3).strip()))
    return rows

def words(o,fn):
    """Disassembled words of symbol `fn` in object `o`.

    ⚠️ This used to return [] when `fn` was not in the object, and EVERY caller
    (find_main_swaps, gen_main_reg_patches) reads an empty list as "nothing
    differs" -- so a name mismatch printed a perfectly clean `0 not paintable`
    on a function that was nowhere near matching.  That is how `get_alarm`
    (whose .c actually defines `get_alarm`) and three others screened as
    fully closeable on 2026-09-20.  A missing symbol is now an ERROR, and the
    unambiguous case -- exactly one global text symbol -- resolves to it with
    a warning instead of a wrong answer.
    """
    d=subprocess.run(['mips-linux-gnu-objdump','-d','-z',o],capture_output=True,text=True).stdout
    rows=_scan(d,fn)
    if rows: return rows
    defined=[m.group(1) for m in re.finditer(r'(?m)^[0-9a-f]{8} <([^>]+)>:',d)]
    if len(defined)==1 and defined[0]!=fn:
        sys.stderr.write(f"dump_main_diff: WARNING symbol {fn!r} not in {o}; "
                         f"using the object's only text symbol {defined[0]!r}\n")
        return _scan(d,defined[0])
    raise SystemExit(f"dump_main_diff: ERROR symbol {fn!r} not found in {o} "
                     f"(text symbols: {defined or 'none'}). An empty word list "
                     f"would read downstream as a perfect match -- refusing.")
def norm(s):
    s=re.sub(r'<[^>]*>','',s); s=re.sub(r'\s+',' ',s.strip())
    parts=re.split(r'\s+',s,maxsplit=1)
    if len(parts)==1: return parts[0]
    mn,rest=parts; toks=[]
    for tk in rest.split(','):
        tk=tk.strip()
        m=re.match(r'^(-?(?:0x)?[0-9a-fA-F]+)\((\w+)\)$',tk)
        if m: toks.append('#(%s)'%m.group(2))
        elif re.match(r'^(zero|at|v[01]|a[0-3]|t\d|s\d|k[01]|gp|sp|fp|ra)$',tk): toks.append(tk)
        elif re.match(r'^-?(0x)?[0-9a-fA-F]+$',tk): toks.append('#')
        else: toks.append(tk)
    return mn+' '+','.join(toks)
if __name__=='__main__':
    fn=sys.argv[1]; ADDR=int(sys.argv[2],16); SIZE=int(sys.argv[3]); src=sys.argv[4]
    ADDR_S='0x%08X'%ADDR
    skip=tuple(sys.argv[5:])
    o=build(src,fn,skip=skip)
    t=target(ADDR,SIZE); c=words(o,fn)
    print(f"target={len(t)} compiled={len(c)}")
    bad=0
    for i in range(max(len(t),len(c))):
        T=t[i] if i<len(t) else (0,0,'')
        C=c[i] if i<len(c) else (0,0,'')
        same = norm(T[2])==norm(C[2])
        if not same: bad+=1
        print(f"{i:4d} {T[0]:08x} {T[2][:42]:42s} | {C[2][:42]:42s}{'' if same else ' <<'}")
    print("mnemonic/register mismatches:",bad)
