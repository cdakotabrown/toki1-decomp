import glob,os,re,struct,subprocess,json,sys
from collections import Counter
R='.'; BASE=0x80132000
def syms_of(ov):
    syms=[]
    for l in open(f'{R}/config/overlay/{ov}.sym'):
        m=re.match(r'\s*0x([0-9a-fA-F]+):(\d+):(\S+)',l)
        if m: syms.append((int(m.group(1),16),int(m.group(2)),m.group(3)))
    return sorted(set(syms))
EXN={}
def exn(ov):
    if ov not in EXN: EXN[ov]=open(f'{R}/assets/overlays/{ov}.EXN','rb').read()
    return EXN[ov]
def dis(ov,addr,n):
    d=exn(ov); off=addr-BASE
    open('/tmp/hole.bin','wb').write(d[off:off+n])
    od=subprocess.run(['mips-linux-gnu-objdump','-D','-z','-b','binary','-m','mips:3000','-EL','--no-show-raw-insn','--adjust-vma=%#x'%addr,'/tmp/hole.bin'],capture_output=True,text=True).stdout
    return [(int(m.group(1),16),m.group(2).strip()) for l in od.splitlines() for m in [re.match(r'\s*([0-9a-f]+):\s+(.*)',l)] if m]
def cwin(ov,name,pa):
    p=f'{R}/src/overlay/{ov}/{name}.c'
    if not os.path.exists(p): return None
    h=open(p).read()[:6000]
    m=re.search(r'@SymbolSize:\s*(\d+)',h); s=re.search(r'@SymbolStart:\s*0x([0-9A-Fa-f]+)',h)
    return (int(s.group(1),16) if s else pa, int(m.group(1)) if m else None)
res=[]
for p in sorted(glob.glob(f'{R}/config/overlay/*.sym')):
    ov=os.path.basename(p)[:-4]; syms=syms_of(ov); d=exn(ov)
    W=[struct.unpack_from('<I',d,i)[0] for i in range(0,len(d)-3,4)]; Wset=set(W)
    jals=set(((w&0x3ffffff)<<2)|0x80000000 for w in W if (w>>26)==3)
    for (a,sz,n),(b,_,n2) in zip(syms,syms[1:]):
        gap=b-(a+sz)
        if gap<=0: continue
        start=a+sz; ins=dis(ov,start,gap)
        cw=cwin(ov,n,a)
        i=0
        while i<len(ins) and ins[i][1]=='nop': i+=1
        cur=None; pieces=[]; skip=-1
        for j in range(i,len(ins)):
            if j==skip: continue
            addr,t=ins[j]
            if cur is None:
                if t=='nop': continue
                cur=addr
            if t.startswith('jr\tra'):
                end=ins[j+1][0]+4 if j+1<len(ins) else addr+4
                pieces.append((cur,end-cur,True)); cur=None; skip=j+1
        if cur is not None: pieces.append((cur,start+gap-cur,False))
        # host info: does host branch into hole / has jr reg
        brin=set(); jrreg=False
        for k in range(a,a+sz,4):
            w=W[(k-BASE)//4]; op=w>>26
            if op in (1,4,5,6,7):
                off=struct.unpack('<h',struct.pack('<H',w&0xffff))[0]; tgt=k+4+off*4
                if start<=tgt<start+gap: brin.add(tgt)
            if op==0 and (w&0x3f)==8 and ((w>>21)&31)!=31: jrreg=True
        for (pc,psz,hasjr) in pieces:
            body=dis(ov,pc,psz); ops=[t.split()[0] for _,t in body]
            if all(o=='nop' for o in ops): continue
            init=sum(1 for o in ops if o in ('lui','ori','sw','addiu','lw','sh','sb','jr','nop'))
            kind='ptrinit' if init/len(body)>0.95 and len(body)>8 else 'code'
            covered = bool(cw and cw[1] and cw[0]<=pc and pc+psz<=cw[0]+cw[1])
            ref='jal' if pc in jals else ('ptr' if pc in Wset else ('branch-in' if any(pc<=t<pc+psz for t in brin) else ('after-jrtab' if jrreg else 'unref')))
            res.append(dict(ov=ov,addr=pc,size=psz,jr=hasjr,kind=kind,prev=n,prev_addr=a,prev_size=sz,cwin=cw,covered=covered,ref=ref,first=body[0][1] if body else ''))
json.dump(res,open(sys.argv[1],'w'))
c=Counter((r['ref'],r['jr'],r['covered']) for r in res)
for k,v in sorted(c.items(),key=lambda x:-x[1]): print(k,v)
print('pieces',len(res),'bytes',sum(r['size'] for r in res),'ptrinit',sum(1 for r in res if r['kind']=='ptrinit'))
print('== after-jrtab / branch-in / nojr')
for r in res:
    if r['ref'] in ('after-jrtab','branch-in') or not r['jr']:
        print(r['ov'],hex(r['addr']),r['size'],'jr' if r['jr'] else 'NOJR',r['kind'],r['ref'],'host',r['prev'],r['prev_size'],'cwin',r['cwin'],'|',r['first'])
