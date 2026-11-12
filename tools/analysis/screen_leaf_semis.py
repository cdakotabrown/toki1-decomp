"""Rank open leaf + loop-free main-binary semis by instruction-count delta.

A function with no `jal` cannot hit the promote-vs-split (saved-register)
wall, because nothing is live across a call; with no back-branch it cannot
hit LICM or induction-variable walls either.  That makes leaf + loop-free the
most tractable open shape, and a count delta of 0 usually means the residual
is register naming, store order or scheduling rather than semantics.

Usage: python3 tools/analysis/screen_leaf_semis.py
"""
import json,struct,subprocess,re,os,sys
S=os.environ.get('TMPDIR','/tmp')
d=json.load(open('config/main_function_audit_by_addr.json'))['addresses']
img=open('disks/SLPS_000.65','rb').read()
CC='./tools/ido-static-recomp/build/5.2/out/cc'
rows=[]
for k,v in d.items():
    if v.get('kind') not in ('semi_real','semi_stub'): continue
    src=v.get('source')
    if not src or not src.endswith('.c') or not os.path.exists(src): continue
    a=int(k,16); n=v.get('size') or 0
    if n<64 or n>1600: continue
    o=(a-0x80041000)+0x800
    if o<0 or o+n>len(img): continue
    w=struct.unpack('<%dI'%(n//4), img[o:o+n])
    if any((x>>26)==3 for x in w): continue                     # leaf only
    if any(((x>>26) in (4,5,6,7,1)) and (x&0xffff)>=0x8000 for x in w): continue  # loop-free
    rows.append((a,n,src,v['kind']))
print("screening",len(rows),"leaf loop-free open semis",file=sys.stderr)
out=[]
for a,n,src,kind in rows:
    r=subprocess.run([CC,'-O2','-Wc,-pic1','-G0','-mips1','-EL','-non_shared','-I','include',
                      '-c',src,'-o',S+'/s.o'],capture_output=True,text=True)
    if r.returncode!=0:
        out.append((999,a,n,0,src,kind,'COMPILE-FAIL')); continue
    dis=subprocess.run(['mips-linux-gnu-objdump','-d','--section=.text',S+'/s.o'],
                       capture_output=True,text=True).stdout
    ins=[l for l in dis.splitlines() if re.match(r'\s*[0-9a-f]+:\s+[0-9a-f]{8}',l)]
    i=0
    while i+1<len(ins) and 'jr' in ins[i] and 'nop' in ins[i+1]: i+=2
    c=len(ins)-i
    out.append((abs(c-n//4),a,n,c,src,kind,''))
out.sort()
print("%-12s %5s %5s %6s %-10s %s"%("addr","targ","comp","delta","kind","source"))
for dl,a,n,c,src,kind,note in out:
    print("0x%08X %5d %5d %+6d %-10s %s %s"%(a,n//4,c,c-(n//4),kind,os.path.basename(src),note))
