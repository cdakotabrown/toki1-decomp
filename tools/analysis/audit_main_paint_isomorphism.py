"""Reaching-definition isomorphism audit for a MAIN-BINARY register paint.

The overlays have audit_paint_isomorphism.py; the main binary had nothing, which
is why CLAUDE.md's "a refusal shifts the burden of proof onto you" could not be
discharged here and why several honest semis were left unpainted in 2026-09.
This closes that gap.

Given the object compiled WITHOUT the paint directives and the object compiled
WITH them, it rebuilds basic blocks, runs a reaching-definition fixpoint over
the CFG, and keys every instruction by

    (opcode + immediate/shamt, tuple of DEFINING-INSTRUCTION INDICES per source)

If those keys are identical at every word, the paint renamed registers without
disturbing a single reaching definition -- a pure permutation, which is the
ALLOWED patch_insn_reg_field class. Any difference is reported as a producer
change, which means the paint altered dataflow and must NOT be shipped.

    python3 tools/analysis/audit_main_paint_isomorphism.py PRE.o POST.o SYMBOL [--source=F.c] [--raw=RAW.o]
    python3 tools/analysis/audit_main_paint_isomorphism.py --all [--jobs=N] [--json=OUT]

2026-10-03: `--all` builds PRE (the chain minus the PAINT LAYER: the rename
steps plus custom_script / asm_intercept / patch_insn_imm16 -- red-team
finding 1: a script editing register fields or immediates was never part of
the audited layer), POST and RAW itself through build_level3_c for every
registered function that carries a paint-layer step, and adds a FIELD check:
a paint may change register fields only, so any opcode / immediate /
displacement difference between PRE and POST is reported (a pure-reorder
script is recognised as a permutation and judged by the unpermute mode).

Exit status is informational; read the "producer changes=N" line. Always run a
NEGATIVE CONTROL too: re-run with one deliberately bogus paint and confirm it
reports a nonzero count, otherwise you have proved nothing.

2026-09-28: the verdict now comes from the OVERLAY audit's model
(tools/overlay/audit_paint_isomorphism.py compare / unpermute_check), fed
this object's own jump table, relocations and -- with --source -- callee
arities.  The legacy model (build()/compare() below, kept for comparison)
gave jump-table case bodies no predecessor, so a case-body read of a value
computed before the dispatch was the untagged 'IN' on both sides and a
corrupted paint there read "0 producer changes" (3 of 4 targeted controls).
Two main-only details: strip_leading_instructions moves the code up without
rewriting the .rodata jump table or a local `j` field, so both are shifted
back (--shift=N, the stripped word count, or inferred from the table); and
with --raw=RAW.o a swap_instructions layer
is un-permuted, as in the overlay audit (2026-10-04: RAW = the chain minus
the paint layer and the reorder steps, and --all MEASURES the strip shift;
see --self-test).  Controls: 299/299 random corrupted
reads (legacy 293), 4/4 targeted case-body corruptions (legacy 1).  Over the
314 registered main functions with a rename layer: 14 flag (legacy 66).

Caveats, stated so nobody over-trusts it:
  * `jal`/`jalr` conservatively clobber the caller-saved set, so a paint that
    moves a value across a call is flagged rather than silently accepted.
  * `$zero` reads are folded to a constant, so painting a register to `$zero`
    (or away from it) is a real change and shows up.
  * It compares the two objects' .text for ONE symbol and requires equal
    lengths -- it audits renaming only, never a count change.
"""
import sys, struct, subprocess, os

def decode(w):
    op=w>>26; rs=(w>>21)&31; rt=(w>>16)&31; rd=(w>>11)&31; sa=(w>>6)&31; fn=w&63
    imm=w&0xffff; tgt=w&0x3ffffff
    return op,rs,rt,rd,sa,fn,imm,tgt

# returns (kind, defs(list of regs), uses(list of regs), key)
def analyze(w):
    op,rs,rt,rd,sa,fn,imm,tgt=decode(w)
    if op==0:
        if fn in (0x00,0x02,0x03):          # sll srl sra
            return ('r',[rd],[rt],('R',fn,sa))
        if fn in (0x04,0x06,0x07):          # sllv srlv srav
            return ('r',[rd],[rt,rs],('R',fn,0))
        if fn in (0x0c,0x0d):
            # syscall / break: the 20 bits between opcode and funct are a CODE,
            # not rs/rt.  Decoding them as registers made every `break 0x6` /
            # `break 0x7` div guard report a spurious producer change whenever
            # an unrelated paint renamed $a2 or $a3 (code 6 and 7 decode as
            # those register numbers).  They read no registers.
            return ('r',[],[],('R',fn,(w>>6)&0xfffff))
        if fn==0x08: return ('jr',[],[rs],('JR',))
        if fn==0x09: return ('jalr',[rd],[rs],('JALR',))
        if fn in (0x18,0x19,0x1a,0x1b):     # mult/div
            return ('r',[],[rs,rt],('R',fn,0))
        if fn in (0x10,0x12): return ('r',[rd],[],('R',fn,0))
        return ('r',[rd],[rs,rt],('R',fn,0))
    if op==2: return ('j',[],[],('J',))
    if op==3: return ('jal',[],[],('JAL',))
    if op in (4,5):
        # `b off` assembles as `beq $0,$0,off` and NEVER falls through.
        # Classifying it as an ordinary conditional branch added a
        # phantom fall-through edge, which let defs from the block
        # ABOVE an unconditional branch reach the code below it and
        # produced false 'producer change' reports (2026-09-20).
        if op==4 and rs==0 and rt==0:
            return ('bu',[],[],('B',op,imm))
        return ('br',[],[rs,rt],('B',op,imm))
    if op in (1,6,7):return ('br',[],[rs],('B',op,rt,imm))
    if op==15:       return ('lui',[rt],[],('LUI',imm))
    if op in (8,9,10,11,12,13,14,24,25):
        return ('i',[rt],[rs],('I',op,imm))
    if op in (32,33,34,35,36,37,38,39,48,49):   # lb..lwr, lwc1/2
        return ('load',[rt],[rs],('L',op,imm))
    if op in (40,41,42,43,46,56,57,58,59):
        return ('store',[],[rs,rt],('S',op,imm))
    return ('?',[],[],('?',op,w))

def build(words, base=0):
    n=len(words)
    # basic blocks from branch targets
    leaders={0}
    for i,w in enumerate(words):
        k,d,u,key=analyze(w)
        op=w>>26
        if k=='br':
            t=i+1+struct.unpack('<h',struct.pack('<H',w&0xffff))[0]
            if 0<=t<n: leaders.add(t)
            if i+2<n: leaders.add(i+2)
        if k=='bu':
            t=i+1+struct.unpack('<h',struct.pack('<H',w&0xffff))[0]
            if 0<=t<n: leaders.add(t)
            if i+2<n: leaders.add(i+2)
        if k in ('j','jr') and i+2<n: leaders.add(i+2)
    leaders=sorted(leaders)
    blkof={}
    for bi,s in enumerate(leaders):
        e=leaders[bi+1] if bi+1<len(leaders) else n
        for i in range(s,e): blkof[i]=bi
    # predecessors
    preds={b:set() for b in range(len(leaders))}
    for i,w in enumerate(words):
        k,_,_,_=analyze(w)
        b=blkof[i]
        if k=='bu':
            t=i+1+struct.unpack('<h',struct.pack('<H',w&0xffff))[0]
            if t in blkof: preds[blkof[t]].add(b)
            # no fall-through edge: `b` is unconditional
        elif k=='br':
            t=i+1+struct.unpack('<h',struct.pack('<H',w&0xffff))[0]
            if t in blkof: preds[blkof[t]].add(b)
            if i+2 in blkof and blkof[i+2]!=b: preds[blkof[i+2]].add(b)
        elif k in ('j','jr','jal','jalr'):
            pass
        elif i+1 in blkof and blkof[i+1]!=b:
            # ⚠️ The fall-through edge is emitted by the word that ENDS the
            # block, which for a branch is its DELAY SLOT, not the branch.
            # Skipping it only at the branch (above) left the delay slot to
            # re-add the very edge an unconditional transfer does not have --
            # so defs above a `b`/`j`/`jr` leaked into the code below it and
            # were reported as producer changes (2026-09-20).
            prevk = analyze(words[i-1])[0] if i>0 else None
            if prevk not in ('bu','j','jr'):
                preds[blkof[i+1]].add(b)
    # unconditional `b` (op4 rs==rt==0) is kind 'bu': taken edge only.
    # iterate dataflow: state[b] = dict reg -> frozenset of def ids
    IN=frozenset(['IN'])
    outs={b:{} for b in range(len(leaders))}
    ins={b:{} for b in range(len(leaders))}
    keys=[None]*n
    for _ in range(12):
        changed=False
        for bi,s in enumerate(leaders):
            e=leaders[bi+1] if bi+1<len(leaders) else n
            st={}
            if bi==0:
                pass
            for p in preds[bi]:
                for r,v in outs[p].items(): st[r]=st.get(r,frozenset())|v
            if st!=ins[bi]: ins[bi]=st; changed=True
            cur=dict(st)
            for i in range(s,e):
                k,defs,uses,key=analyze(words[i])
                src=tuple(sorted(cur.get(r,IN), key=repr) if r!=0 else ('ZERO',) for r in uses)
                # add/addu/and/or/xor/nor/mult/multu are commutative: a paint
                # that only swaps rs<->rt on one of them leaves every reaching
                # definition in place, so compare the operand set, not the
                # order (false positive on srn_tpage_show's six addu, 2026-09-24).
                w=words[i]
                if (w>>26)==0 and (w&0x3f) in (0x20,0x21,0x24,0x25,0x26,0x27,0x18,0x19):
                    src=tuple(sorted(src, key=repr))
                keys[i]=(key,src)
                if k in ('jal','jalr'):
                    for r in list(range(1,16))+[24,25,31]: cur[r]=frozenset([('CALL',i,r)])
                for r in defs:
                    if r!=0: cur[r]=frozenset([i])
            if cur!=outs[bi]: outs[bi]=cur; changed=True
        if not changed: break
    return keys

def load(path, sym):
    subprocess.run(['mips-linux-gnu-objcopy','-O','binary','-j','.text',path,path+'.bin'],check=True,capture_output=True)
    b=open(path+'.bin','rb').read()
    nm=subprocess.run(['mips-linux-gnu-nm','-S',path],capture_output=True,text=True)
    st=sz=None
    for ln in nm.stdout.splitlines():
        p=ln.split()
        if len(p)>=4 and p[3]==sym: st=int(p[0],16); sz=int(p[1],16)
    if st is None:
        # a gcc object can carry the function without an nm size (or under a
        # local label): take the symbol's address, else .text start, to end
        for ln in nm.stdout.splitlines():
            p=ln.split()
            if len(p)>=3 and p[-1]==sym:
                st=int(p[0],16)
        st=st or 0
    if sz is None:
        sz=len(b)-st
    sz=min(sz, len(b)-st)
    w=[struct.unpack_from('<I',b,st+4*i)[0] for i in range(sz//4)]
    return w

def compare(a, c):
    """LEGACY model (kept for comparison): [(word, pre key, post key)]."""
    ka=build(a); kc=build(c)
    return [(i,x,y) for i,(x,y) in enumerate(zip(ka,kc)) if x!=y]


def _sym_start(path, sym):
    nm=subprocess.run(['mips-linux-gnu-nm','-S',path],capture_output=True,text=True)
    for ln in nm.stdout.splitlines():
        p=ln.split()
        if len(p)>=4 and p[3]==sym:
            return int(p[0],16)//4
    for ln in nm.stdout.splitlines():
        p=ln.split()
        if len(p)>=3 and p[-1]==sym:
            return int(p[0],16)//4
    return 0


def _case_starts(words, raw):
    """Jump-table targets as word indices of `words`.  The table's entries
    are .text offsets baked into .rodata, and strip_leading_instructions
    (the types.h thunks) moves the code up WITHOUT rewriting them, so they
    can sit N words past the case bodies they name.  Pick the shift that
    lands every entry on a word that follows an unconditional transfer's
    delay slot (how every case body starts), preferring the smallest."""
    if not raw:
        return []

    def uncond(w):
        op=w>>26
        return (op==4 and ((w>>21)&31)==0 and ((w>>16)&31)==0) or op==2 or \
            (op==0 and (w&63)==8)
    best=None
    for s in range(0, 17):
        ts=[x-s for x in raw]
        if not all(2 <= x < len(words) for x in ts):
            continue
        ok=sum(1 for x in ts if uncond(words[x-2]))
        if best is None or ok > best[0]:
            best=(ok, ts)
    return sorted(set(best[1])) if best else []


def _unshift_j(words, s):
    """A local `j` keeps its PRE-strip .text word index in the field (the
    same staleness as the jump table); move it back by s words."""
    if not s:
        return list(words)
    return [((w & 0xFC000000) | (((w & 0x3FFFFFF) - s) & 0x3FFFFFF))
            if (w >> 26) == 2 else w for w in words]


def compare_objects(pre, post, sym, source_text=None, raw=None, shift=None):
    """Producer changes between PRE.o and POST.o for SYMBOL, with the overlay
    audit's reaching-definition model (2026-09-28): jump-table case bodies
    inherit the dispatch state from the object's own table, entry values are
    register-tagged, call results $v0/$v1-tagged, relocated words value-equal
    only when their relocations agree, and -- given the C source -- argument
    registers compared at each call up to the callee's arity.

    The legacy model above gave a case body NO predecessor, so a read there
    of a value computed before the dispatch was the untagged 'IN' before and
    after any paint: a corrupted case-body paint read "0 producer changes"
    (3 of 4 such controls on 2026-09-28).

    `shift` = words strip_leading_instructions removed ahead of SYMBOL
    (None: inferred from the jump table, else 0)."""
    sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
        os.path.abspath(__file__))), 'overlay'))
    import audit_paint_isomorphism as iso
    a=load(pre, sym); c=load(post, sym)
    n_=min(len(a), len(c))
    if len(a)!=len(c) and not any(a[n_:]) and not any(c[n_:]):
        a, c = a[:n_], c[:n_]          # only trailing nops differ (CdRead.py)
    if len(a)!=len(c):
        return None
    st=_sym_start(pre, sym)
    stc=_sym_start(post, sym)

    def relshift(rel, base):
        return {k-base: v for k, v in rel.items() if base <= k < base+len(a)}
    rawjt=[x-st for x in (iso.jump_targets(pre) or [])]
    ra=relshift(iso.text_relocs(pre), st)
    rc=relshift(iso.text_relocs(post), stc)
    arity=iso.call_arities(source_text, ra) if source_text else None
    r=rr=rarity=None
    if raw is not None:
        # RAW.o = the compile minus the paint layer AND the reorder steps
        # (_raw_steps).  When PRE is a pure permutation of it (a
        # swap_instructions layer), judge the rename and the reorder
        # together, exactly as the overlay audit does (its "unpermuted"
        # mode) -- a swap applied to the compiled registers that the paints
        # then repair is not a producer change.
        r=load(raw, sym)
        rr=relshift(iso.text_relocs(raw), _sym_start(raw, sym))
        rarity=iso.call_arities(source_text, rr) if source_text else None
    return compare_words(a, c, r, rawjt, ra, rc, rr, arity, rarity, shift)


def compare_words(a, c, r, rawjt, ra, rc, rr=None, arity=None, rarity=None,
                  shift=None):
    """The verdict on word lists: PRE `a`, POST `c`, RAW `r` (or None), the
    jump-table targets as read from PRE (`rawjt`, PRE-strip coordinates),
    and each object's {word: relocation}.  `shift` = words the thunk strip
    removed (None: inferred from the jump table, else 0 -- see _all_one,
    which measures it)."""
    sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
        os.path.abspath(__file__))), 'overlay'))
    import audit_paint_isomorphism as iso
    if shift is None:
        jt=_case_starts(a, rawjt)
        shift=(rawjt[0]-jt[0]) if jt else 0
    else:
        jt=sorted(set(x-shift for x in rawjt if 0 <= x-shift < len(a)))
    a=_unshift_j(a, shift); c=_unshift_j(c, shift)
    bad=iso.compare(a, c, jt, (), arity, (ra, rc))
    if bad and r is not None and len(r)==len(a):
        u=iso.unpermute_check(_unshift_j(r, shift), a, c, jt, (), rr, ra, rc,
                              rarity)
        if u is not None:
            bad=u[0]
    return bad


MAIN_PAINT_LAYER = {'patch_insn_reg_field', 'swap_registers', 'swap_registers_liveness',
                    'swap_registers_liveness_arrow', 'rename_register_chain',
                    'custom_script', 'asm_intercept', 'patch_insn_imm16'}


MAIN_REORDER = {'swap_instructions', 'scheduler_normalizer'}


def _raw_steps(pp):
    """RAW's chain: PRE's (the chain minus the paint layer) minus its
    reorder steps, so PRE is a pure permutation of RAW.  It used to be the
    thunk strip alone, which left convert_or_to_addu / remap_stack_slots /
    patch_frame in PRE but not RAW: PRE was then no permutation of RAW, the
    unpermute mode was skipped, and a swap layer the paints repair was judged
    by the direct compare (2026-10-04: GsTMDdivF3L, GsTMDdivF3LFG and
    GsTMDdivTG4NL flagged on exactly that)."""
    return [x for x in pp if x.get('type') not in MAIN_PAINT_LAYER
            and x.get('type') not in MAIN_REORDER]


def _all_one(e):
    """PRE (chain minus the paint layer), POST (full chain) and RAW (PRE
    minus its reorder steps, _raw_steps) for one registered main function,
    built through the build's own compile_c_function, then the iso verdict
    + the field check."""
    import io, contextlib, tempfile, shutil
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
        os.path.abspath(__file__))), 'overlay'))
    import audit_plain_vs_shipped as P
    import audit_paint_isomorphism as iso
    from parse_postproc import (parse_postproc_header, parse_compile_flags,
                                parse_compiler, parse_compile_env)
    root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    src = os.path.join(root, e['source'])
    out = {'id': 'main/0x%08X' % int(e['address'], 16), 'name': e['name']}
    td = tempfile.mkdtemp(prefix='miso_')
    try:
        with contextlib.redirect_stdout(io.StringIO()), contextlib.redirect_stderr(io.StringIO()):
            pp = parse_postproc_header(src) or []
            parsed = (parse_compile_flags(src), parse_compiler(src), parse_compile_env(src))
            pre, post, raw = (os.path.join(td, x + '.o') for x in ('pre', 'post', 'raw'))
            P._main_compile(e, [x for x in pp if x.get('type') not in MAIN_PAINT_LAYER],
                            parsed, keep=pre)
            P._main_compile(e, pp, parsed, keep=post)
            P._main_compile(e, _raw_steps(pp),
                            parsed, keep=raw)
            # The words strip_leading_instructions removed ahead of the
            # symbol, measured (the symbol's start in the same chain without
            # the strip) rather than inferred from a jump table: a gcc local
            # `j` keeps its PRE-strip .text index, and a function with no
            # table read shift 0, so VSync's `j 0xd0` landed 18 words late,
            # mid-block, and fed the entry $a0 into an unrelated load
            # (2026-10-04; GsGetLws / GsGetLs the same).
            shift = None
            if any(x.get('type') == 'strip_leading_instructions' for x in pp):
                ns = os.path.join(td, 'nostrip.o')
                P._main_compile(e, [x for x in _raw_steps(pp)
                                    if x.get('type') != 'strip_leading_instructions'],
                                parsed, keep=ns)
                shift = _sym_start(ns, e['name']) - _sym_start(raw, e['name'])
                if shift < 0:
                    shift = None
            a = load(pre, e['name']); c = load(post, e['name'])
            n_ = min(len(a), len(c))
            if len(a) != len(c) and not any(a[n_:]) and not any(c[n_:]):
                a, c = a[:n_], c[:n_]      # a trimmed trailing nop (CdRead.py)
            if len(a) != len(c):
                out['error'] = 'length %d vs %d' % (len(a), len(c))
                return out
            fc = iso.field_changes(a, c)
            scripted = any(x.get('type') in ('custom_script', 'asm_intercept',
                                             'swap_instructions', 'scheduler_normalizer')
                           for x in pp)
            if fc and scripted and sorted(map(iso._shape, a)) == \
                    sorted(map(iso._shape, c)):
                fc = []                    # a reorder script: judged by the unpermute below
            text = open(src, errors='replace').read()
            bad = compare_objects(pre, post, e['name'], text, raw, shift)
        if bad is None:
            out['error'] = 'length mismatch'
            return out
        out.update(bad=len(bad), field=len(fc), bad_at=[b[0] for b in bad][:8],
                   field_at=fc[:8])
    except BaseException as ex:                      # noqa: BLE001
        out['error'] = repr(ex)[:160]
    finally:
        shutil.rmtree(td, ignore_errors=True)
    return out


def audit_all(jobs=16, json_out=None):
    """--all: every registered main C function whose chain carries a paint-
    layer step (2026-10-03: nothing ran this audit tree-wide -- red-team
    finding 4)."""
    import json, multiprocessing as mp
    sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
        os.path.abspath(__file__))), 'build'))
    from parse_postproc import parse_postproc_header
    import io, contextlib
    root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    reg = json.load(open(os.path.join(root, 'config/level3_functions.json')))['functions']
    tasks = []
    for e in reg:
        if not e.get('source', '').endswith('.c'):
            continue
        with contextlib.redirect_stdout(io.StringIO()):
            pp = parse_postproc_header(os.path.join(root, e['source'])) or []
        if any(x.get('type') in MAIN_PAINT_LAYER for x in pp):
            tasks.append(e)
    with mp.get_context('fork').Pool(max(1, jobs)) as pool:
        res = list(pool.imap(_all_one, tasks, chunksize=2))
    flagged = [r for r in res if r.get('bad') or r.get('field')]
    errs = [r for r in res if 'error' in r]
    for r in flagged:
        print("FLAG  %-16s %-28s producer_changes=%d field_changes=%d at=%s"
              % (r['id'], r['name'], r['bad'], r['field'], r['bad_at'] or r['field_at']))
    for r in errs:
        print("ERROR %-16s %-28s %s" % (r['id'], r['name'], r['error']))
    print("\naudited %d painted main function(s): %d flagged, %d error(s)"
          % (len(res), len(flagged), len(errs)))
    if json_out:
        open(json_out, 'w').write(json.dumps(res, indent=0) + "\n")
    return 1 if flagged else (2 if errs else 0)


def _fx_words():
    """Encoders for the --self-test fixtures."""
    def I(op, rt, rs, imm):
        return (op << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)
    E = dict(
        addiu=lambda rt, rs, imm: I(9, rt, rs, imm),
        sh=lambda rt, base, imm: I(41, rt, base, imm),
        lw=lambda rt, base, imm: I(35, rt, base, imm),
        lui=lambda rt, imm: I(15, rt, 0, imm),
        bgez=lambda rs, off: I(1, 1, rs, off),
        addu=lambda d, s_, t: (s_ << 21) | (t << 16) | (d << 11) | 0x21,
        or_=lambda d, s_, t: (s_ << 21) | (t << 16) | (d << 11) | 0x25,
        j=lambda idx: (2 << 26) | idx, JR=0x03E00008, NOP=0)
    return E


def self_test():
    """Synthetic fixtures for the 2026-10-04 fixes (all three were the
    whole of the 8 --all flags; prove_equivalence proved all 8):

      P1/P2  a reorder custom_script that also widens the frame (P1,
             system_config_icon_mode_init) or remaps every slot +8 (P2, system_config_mouse_menu_init)
             while renaming v1<->a1: the $sp-masked pairing paired the two
             sp-addresses crosswise and the coupled group repair settled in
             a local optimum.  Must PASS.
      G1     RAW built from the thunk strip alone (or-form) while PRE carries
             convert_or_to_addu: no permutation, the unpermute mode was
             skipped (GsTMDdiv*).  Old RAW must FLAG, _raw_steps' RAW PASS.
      S1     a gcc local `j` keeps its pre-strip index; with no jump table
             the shift was inferred as 0 and the edge landed past the def
             (VSync, GsGetLws, GsGetLs).  Inferred must FLAG, measured PASS.
      N1-N4  negative controls: a use hoisted above its def, a store
             re-pointed at another value, a remap+reorder that stores the
             pointer for the value, and S1 with a genuinely skipped def.
             Must FLAG.
    """
    sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
        os.path.abspath(__file__))), 'overlay'))
    import audit_paint_isomorphism as iso
    E = _fx_words()
    ad, sh, JR, NOP = E['addiu'], E['sh'], E['JR'], E['NOP']
    V0, V1, A0, A1, A2, A3, T0, T1, SP, ZR = 2, 3, 4, 5, 6, 7, 8, 9, 29, 0

    def loop_r(f, p0, p1):
        return [ad(SP, SP, -f), ad(V1, ZR, -64), ad(A0, SP, p0),
                ad(A1, SP, p1), ad(A0, A0, 2), ad(A1, A1, 2), ad(V1, V1, 16),
                sh(V1, A0, 0), sh(V1, A1, -2), ad(SP, SP, f), JR, NOP]

    def loop_b(f, p0, p1, sa=(A1, 0, A0), sb=(A1, -2, V1)):
        return [ad(SP, SP, -f), ad(V1, SP, p1), ad(A0, SP, p0),
                ad(A1, ZR, -64), ad(A1, A1, 16), ad(V1, V1, 2), ad(A0, A0, 2),
                sh(sa[0], sa[2], sa[1]), sh(sb[0], sb[2], sb[1]),
                ad(SP, SP, f), JR, NOP]

    def unperm(R, B):
        u = iso.unpermute_check(R, list(R), B, [], (), {}, {}, {}, None)
        return True if u is None else bool(u[0])

    R1 = loop_r(104, 88, 96)
    R2 = loop_r(96, 80, 88)
    b_hoist = loop_b(112, 88, 96)
    b_hoist[6], b_hoist[7] = b_hoist[7], b_hoist[6]   # sh above a0 += 2
    # G1: li t0,1 ; move v0,t0 ; li t0,2 ; addu v1,t0,v0 -- a swap of words
    # 1/2 breaks v0, the t0->t1 paint on words 0/2 repairs it.
    g = lambda mv: [ad(T0, ZR, 1), mv(V0, T0, ZR), ad(T0, ZR, 2),
                    E['addu'](V1, T0, V0), JR, NOP]
    gR_old, gR_new = g(E['or_']), g(E['addu'])
    gA = [gR_new[0], gR_new[2], gR_new[1], gR_new[3], JR, NOP]
    gB = [ad(T1, ZR, 1), ad(T0, ZR, 2), E['addu'](V0, T1, ZR), gR_new[3],
          JR, NOP]

    # S1: bgez skips to the true join at 8; the `j` at 3 targets 8 too, but
    # its field holds the pre-strip index 8+2.  A paint renames a0->v1 on
    # the lui/lw pair at 8..10 (v1's word-0 value is dead there).
    def s1(lui_at):
        w = [ad(V1, ZR, 7), E['bgez'](A1, 5), NOP, E['j'](8 + 2), NOP, NOP,
             NOP, ad(V0, ZR, 3), NOP, NOP, E['lw'](A0, A0, 0), JR, NOP]
        w[lui_at] = E['lui'](A0, 0)
        return w

    def ren(w, frm, to, idx):
        out = list(w)
        for i in idx:
            x = out[i]
            if ((x >> 21) & 31) == frm:
                x = (x & ~(31 << 21)) | (to << 21)
            if ((x >> 16) & 31) == frm:
                x = (x & ~(31 << 16)) | (to << 16)
            out[i] = x
        return out
    sA = s1(8)
    sB = ren(sA, A0, V1, (8, 10))
    nA = s1(7)                          # def on the bgez path only
    nB = ren(nA, A0, V1, (7, 10))

    def cw(a, c, r=None, shift=None):
        return bool(compare_words(a, c, r, [], {}, {}, {}, None, None, shift))
    cases = [
        ("P1 reorder script + frame widen + v1<->a1",
         unperm(R1, loop_b(112, 88, 96)), False),
        ("P2 reorder script + slot remap +8 + v1<->a1",
         unperm(R2, loop_b(112, 88, 96)), False),
        ("G1 swap repaired by a paint, RAW per _raw_steps (addu)",
         cw(gA, gB, gR_new), False),
        ("G1 same, old RAW (thunk strip only: or-form)",
         cw(gA, gB, gR_old), True),
        ("S1 stale local j, measured shift 2", cw(sA, sB, None, 2), False),
        ("S1 stale local j, shift inferred (old: 0)", cw(sA, sB), True),
        ("N1 use hoisted above its def", unperm(R1, b_hoist), True),
        ("N2 store re-pointed (sh a1,-2(a0))",
         unperm(R1, loop_b(112, 88, 96, sb=(A1, -2, A0))), True),
        ("N3 remap+reorder, stores the pointer for the value",
         unperm(R2, loop_b(112, 88, 96, sa=(V1, 0, A0))), True),
        ("N4 S1 with the def genuinely skipped, measured shift",
         cw(nA, nB, None, 2), True),
    ]
    fails = 0
    for name, got, want in cases:
        ok = got == want
        fails += not ok
        print("  %s %s: flagged=%s want=%s" % ('ok ' if ok else 'FAIL', name,
                                               got, want))
    raw = [x['type'] for x in _raw_steps([
        {'type': t} for t in ('strip_leading_instructions', 'convert_or_to_addu',
                              'remap_stack_slots', 'swap_instructions',
                              'patch_insn_reg_field', 'custom_script',
                              'scheduler_normalizer', 'patch_frame')])]
    ok = raw == ['strip_leading_instructions', 'convert_or_to_addu',
                 'remap_stack_slots', 'patch_frame']
    fails += not ok
    print("  %s _raw_steps keeps non-reorder steps: %s" % ('ok ' if ok else 'FAIL', raw))
    print("self-test:", "PASS" if not fails else "%d FAILED" % fails)
    return 1 if fails else 0


def main():
    if '--self-test' in sys.argv:
        sys.exit(self_test())
    if '--all' in sys.argv:
        jobs = 16
        jo = None
        for x in sys.argv[1:]:
            if x.startswith('--jobs='):
                jobs = int(x.split('=', 1)[1])
            if x.startswith('--json='):
                jo = x.split('=', 1)[1]
        sys.exit(audit_all(jobs, jo))
    args=[x for x in sys.argv[1:] if not x.startswith('--')]
    src=raw=shift=None
    for x in sys.argv[1:]:
        if x.startswith('--source='):
            src=open(x.split('=',1)[1], errors='replace').read()
        if x.startswith('--raw='):
            raw=x.split('=',1)[1]
        if x.startswith('--shift='):
            shift=int(x.split('=',1)[1], 0)
    if len(args) != 3:
        print(__doc__)
        print("usage: audit_main_paint_isomorphism.py PRE.o POST.o SYMBOL "
              "[--source=FILE.c] [--raw=RAW.o] [--shift=N]")
        sys.exit(2)
    a=load(args[0], args[2]); c=load(args[1], args[2])
    if len(a)!=len(c):
        print("LENGTH MISMATCH", len(a), len(c)); sys.exit(1)
    bad=compare_objects(args[0], args[1], args[2], src, raw, shift)
    sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(
        os.path.abspath(__file__))), 'overlay'))
    import audit_paint_isomorphism as iso
    fc=iso.field_changes(a, c)
    for i in fc:
        print("FIELD CHANGE at word %d (off 0x%x): %08x -> %08x  (a non-register "
              "field: opcode/immediate/displacement)" % (i, i*4, a[i], c[i]))
    if fc:
        print("field changes=%d  (a register paint may only touch register fields; "
              "a reorder custom_script shows here too -- check it is a permutation)" % len(fc))
    for i,ua,ub,sa,sb in bad:
        print("PRODUCER CHANGE at word %d (off 0x%x): %08x -> %08x  producers %s -> %s"
              % (i, i*4, a[i], c[i], [sorted(x, key=str) for x in sa],
                 [sorted(x, key=str) for x in sb]))
    print("words=%d  producer changes=%d"%(len(a),len(bad)))


if __name__ == '__main__':
    main()
