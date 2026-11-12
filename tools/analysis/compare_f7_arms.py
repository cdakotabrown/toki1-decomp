#!/usr/bin/env python3
"""Per-arm comparison of serifu_cmd_exec against retail."""
import subprocess,re,sys,os
ROOT=os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
ADDR=0x8007F08C; SIZE=8868
ARMS=[('pro',0x8007F08C),('7',0x8007F174),('16',0x8007F78C),('17',0x8007F7D8),('9',0x8007F820),
      ('0',0x8007F8BC),('14',0x8007FAA0),('1',0x8007FB64),('15',0x8007FEB4),('12',0x800800C0),
      ('13',0x800803E8),('18',0x80080630),('11',0x800807FC),('10',0x80080AE0),('4/6',0x80080DC4),
      ('3',0x80080E90),('8',0x80080F18),('19',0x80080F74),('2/5',0x80080FD0),('255',0x80081300),('def',0x80081310),('END',0x80081330)]
# NB: the last entry deliberately runs to the end of the window: case 255 and the
# default arm are not in the jump table, so the compiled object gives no anchor for
# them and they are counted with 2/5 (204 + 4 + 8 = 216 target words).
def tgt_words():
    off=(ADDR-0x80041000)+0x800
    d=open(ROOT+'/disks/SLPS_000.65','rb').read()[off:off+SIZE]
    open('/tmp/tg.bin','wb').write(d)
    out=subprocess.run(['mips-linux-gnu-objdump','-D','-b','binary','-m','mips:3000','-EL',
                        '--adjust-vma=0x%x'%ADDR,'/tmp/tg.bin'],capture_output=True,text=True).stdout
    return [(int(m.group(1),16),m.group(2),m.group(3).strip()) for m in
            (re.match(r'^\s*([0-9a-f]{8}):\s+([0-9a-f]{8})\s+(.*)$',l) for l in out.splitlines()) if m]
def compile_o(src,env=None):
    e=dict(os.environ); e.update(env or {})
    r=subprocess.run([ROOT+'/tools/ido-static-recomp/build/5.2/out/cc','-O2','-G0','-mips1','-EL',
                      '-non_shared','-c','-I','include',src,'-o','/tmp/arm.o'],cwd=ROOT,env=e,capture_output=True,text=True)
    if r.returncode: print(r.stderr[-4000:]); sys.exit(1)
    return '/tmp/arm.o'
def cmp_words(o):
    d=subprocess.run(['mips-linux-gnu-objdump','-d',o],capture_output=True,text=True).stdout
    rows=[];st=False
    for line in d.splitlines():
        if '<serifu_cmd_exec>:' in line: st=True; continue
        if not st: continue
        if re.match(r'^[0-9a-f]{8} <',line): break
        m=re.match(r'^\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)$',line)
        if m: rows.append((int(m.group(1),16),m.group(2),m.group(3).strip()))
    return rows
def jt(o):
    """compiled case -> .text offset"""
    d=subprocess.run(['mips-linux-gnu-objdump','-s','-j','.rodata',o],capture_output=True,text=True).stdout
    words=[]
    for l in d.splitlines():
        m=re.match(r'^\s*([0-9a-f]{4})\s((?:[0-9a-f]{8}\s){1,4})',l)
        if m:
            for w in m.group(2).split():
                words.append(int.from_bytes(bytes.fromhex(w),'little'))
    return words[:20]
def norm(s):
    s=re.sub(r'<[^>]*>','',s); s=re.sub(r'\s+',' ',s.strip())
    parts=re.split(r'\s+',s,1)
    if len(parts)==1: return parts[0]
    mn,rest=parts
    toks=[]
    for tk in rest.split(','):
        tk=tk.strip()
        m=re.match(r'^(-?(?:0x)?[0-9a-fA-F]+)\((\w+)\)$',tk)
        if m: toks.append('#(%s)'%m.group(2))
        elif re.match(r'^(zero|at|v[01]|a[0-3]|t\d|s\d|k[01]|gp|sp|fp|ra)$',tk): toks.append(tk)
        elif re.match(r'^-?(0x)?[0-9a-fA-F]+$',tk): toks.append('#')
        else: toks.append(tk)
    return mn+' '+','.join(toks)
if __name__=='__main__':
    src=sys.argv[1]
    o=compile_o(src)
    t=tgt_words(); c=cmp_words(o); table=jt(o)
    fbase=0x10
    # compiled arm offsets, function-relative
    carm={}
    names={'7':7,'16':16,'17':17,'9':9,'0':0,'14':14,'1':1,'15':15,'12':12,'13':13,'18':18,
           '11':11,'10':10,'4/6':4,'3':3,'8':8,'19':19,'2/5':2}
    for n,k in names.items(): carm[n]=table[k]-fbase
    carm['pro']=0
    # 255/def/END: derive from layout after 2/5 by scanning? use end of body
    print(f"{'arm':5s} {'tgt_w':>6s} {'cmp_w':>6s} {'d':>4s} {'exact':>7s}")
    # order arms by target
    tstart={n:(a-ADDR)//4 for n,a in ARMS}
    order=[n for n,_ in ARMS if n!='END']
    body_end=len(c)
    cstart={}
    for n in order:
        if n in carm: cstart[n]=carm[n]//4
    # infer 255/def by target-relative distance from 2/5 end using remaining words
    known=[n for n in order if n in cstart]
    tot=0
    LAST=known[-1]
    for i,n in enumerate(known):
        ns=cstart[known[i+1]] if i+1<len(known) else None
        te=tstart['END'] if n==LAST else tstart[order[order.index(n)+1]]
        ts=tstart[n]
        ce=ns if ns is not None else body_end
        cs=cstart[n]
        tl=te-ts; cl=ce-cs
        ex=sum(1 for k in range(min(tl,cl)) if norm(t[ts+k][2])==norm(c[cs+k][2]))
        tot+=cl-tl
        print(f"{n:5s} {tl:6d} {cl:6d} {cl-tl:+4d} {ex:4d}/{tl:<4d} {'':2s}{'OK' if tl==cl and ex==tl else ''}")
    print("total delta (excl 255/def tail lumped in last arm):",tot)
    print("target words",len(t),"compiled",len(c))

def arm_diff(src,arm):
    import difflib
    o=compile_o(src); t=tgt_words(); c=cmp_words(o); table=jt(o)
    names={'7':7,'16':16,'17':17,'9':9,'0':0,'14':14,'1':1,'15':15,'12':12,'13':13,'18':18,
           '11':11,'10':10,'4/6':4,'3':3,'8':8,'19':19,'2/5':2}
    order=[n for n,_ in ARMS if n!='END']
    tstart={n:(a-ADDR)//4 for n,a in ARMS}
    cstart={n:(table[k]-0x10)//4 for n,k in names.items()}; cstart['pro']=0
    known=[n for n in order if n in cstart]
    i=known.index(arm)
    ts,te=tstart[arm],tstart[order[order.index(arm)+1]]
    cs=cstart[arm]; ce=cstart[known[i+1]] if i+1<len(known) else len(c)
    A=[norm(x[2]) for x in t[ts:te]]; B=[norm(x[2]) for x in c[cs:ce]]
    sm=difflib.SequenceMatcher(None,A,B,autojunk=False)
    for tag,i1,i2,j1,j2 in sm.get_opcodes():
        if tag=='equal':
            print(f"  equal {i2-i1} words  (tgt {ts+i1}..{ts+i2-1} @0x{t[ts+i1][0]:x})")
            continue
        print(f"{tag.upper()} @0x{t[ts+i1][0]:08x} tgt[{i1}:{i2}] cmp[{j1}:{j2}] delta={(j2-j1)-(i2-i1):+d}")
        for k in range(i1,i2): print(f"    T {t[ts+k][0]:08x} {t[ts+k][2]}")
        for k in range(j1,j2): print(f"    C          {c[cs+k][2]}")

def arm_dump(src,arm,lo=0,hi=10**9):
    o=compile_o(src); t=tgt_words(); c=cmp_words(o); table=jt(o)
    names={'7':7,'16':16,'17':17,'9':9,'0':0,'14':14,'1':1,'15':15,'12':12,'13':13,'18':18,
           '11':11,'10':10,'4/6':4,'3':3,'8':8,'19':19,'2/5':2}
    order=[n for n,_ in ARMS if n!='END']
    tstart={n:(a-ADDR)//4 for n,a in ARMS}
    cstart={n:(table[k]-0x10)//4 for n,k in names.items()}; cstart['pro']=0
    known=[n for n in order if n in cstart]
    i=known.index(arm)
    ts,te=tstart[arm],tstart[order[order.index(arm)+1]]
    cs=cstart[arm]; ce=cstart[known[i+1]] if i+1<len(known) else len(c)
    for k in range(min(hi,max(te-ts,ce-cs))):
        if k<lo: continue
        T=t[ts+k][2] if ts+k<te else ''
        Ta=f"{t[ts+k][0]:08x}" if ts+k<te else '        '
        C=c[cs+k][2] if cs+k<ce else ''
        mark='' if norm(T)==norm(C) else ' <<'
        print(f"{k:4d} {Ta} {T[:40]:40s} | {C[:40]:40s}{mark}")
