import sys,re,subprocess,struct
R='.'; BASE=0x80132000
ov=sys.argv[1]; addr=int(sys.argv[2],16); size=int(sys.argv[3]); out=sys.argv[4]
d=open(f'{R}/assets/overlays/{ov}.EXN','rb').read(); off=addr-BASE
open('/tmp/pi.bin','wb').write(d[off:off+size])
od=subprocess.run(['mips-linux-gnu-objdump','-D','-z','-b','binary','-m','mips:3000','-EL','--no-show-raw-insn','--adjust-vma=%#x'%addr,'/tmp/pi.bin'],capture_output=True,text=True).stdout
ins=[m.group(2).strip() for l in od.splitlines() for m in [re.match(r'\s*([0-9a-f]+):\s+(.*)',l)] if m]
regs={}   # reg -> ('hi',hi) | ('const',val) | ('sym',addr) | ('load',srcaddr,width,signed)
stmts=[]; externs={}; sym_arrays=set()
W={'lw':4,'lh':2,'lhu':2,'lb':1,'lbu':1}
name='func_%08X'%addr
def sym(a): return 'D_%08X'%a
for idx,t in enumerate(ins):
  try:
      m=re.match(r'(\w+)\s*(.*)',t); op=m.group(1); args=[x.strip() for x in m.group(2).split(',')] if m.group(2) else []
      if op=='lui': regs[args[0]]=('hi',int(args[1],16)<<16)
      elif op=='ori':
          base=regs[args[1]][1]; regs[args[0]]=('const',base|int(args[2],16))
      elif op=='addiu' and args[1] in regs and regs[args[1]][0]=='hi':
          regs[args[0]]=('sym',regs[args[1]][1]+int(args[2]))
      elif op in W:
          mm=re.match(r'(-?\d+)\((\w+)\)',args[1]); src=regs[mm.group(2)][1]+int(mm.group(1))
          regs[args[0]]=('rawload' if args[0]!=mm.group(2) else 'load',src,W[op],op in ('lw','lh','lb'))
      elif op in ('sw','sh','sb'):
          mm=re.match(r'(-?\d+)\((\w+)\)',args[1]); dst=regs[mm.group(2)][1]+int(mm.group(1))
          v=regs.get(args[0]); width={'sw':4,'sh':2,'sb':1}[op]
          stmts.append((dst,width,v))
      elif op in ('jr','nop','sll'): pass
      else: print('UNHANDLED',t,file=sys.stderr); sys.exit(1)
  except Exception as e:
    print('FAIL at',idx,t,'::',repr(e),file=sys.stderr); sys.exit(1)
lines=[]
def ctype(width,signed=True,ptr=False):
    if ptr: return 'void *'
    return {4:'s32',2:'s16' if signed else 'u16',1:'s8' if signed else 'u8'}[width]
for dst,width,v in stmts:
    if v is None: v=('const',0)
    kind=v[0]
    if kind in ('hi','const'):
        val=v[1]
        if width==4 and 0x80000000<=val<0x80200000:
            externs[sym(dst)]='void *'; lines.append(f'    {sym(dst)} = (void *)0x{val:08X};')
        else:
            externs.setdefault(sym(dst),ctype(width)); lines.append(f'    {sym(dst)} = 0x{val:X};' if val>9 else f'    {sym(dst)} = {val};')
    elif kind=='sym':
        externs[sym(dst)]='void *'; sym_arrays.add(sym(v[1])); lines.append(f'    {sym(dst)} = {sym(v[1])};')
    elif kind=='load':
        src,w,sg=v[1],v[2],v[3]
        externs.setdefault(sym(src),ctype(w,sg)); externs.setdefault(sym(dst),ctype(width,sg))
        lines.append(f'    {sym(dst)} = {sym(src)};')
    elif kind=='rawload':
        src,w,sg=v[1],v[2],v[3]
        externs.setdefault(sym(dst),ctype(width,sg))
        lines.append(f'    {sym(dst)} = *({ctype(w,sg)} *)0x{src:08X};')
hdr=f'''/* {name} -- {ov} overlay
 * Address: 0x{addr:08X}
 * Size: {size} bytes ({size//4} instructions)
 *
 * Found 2026-09-02 by tools/overlay/scan_sym_holes.py: a complete function
 * (ends in jr ra) that no .sym recorded.  Straight-line table/pointer
 * initialiser, C generated from the store sequence (raw address constants
 * are the lui/ori form, symbol addresses the lui/addiu form, copies are
 * load/store pairs).
 *
 * @SymbolSize: {size}
 */
#include "types.h"

'''
ext=''.join(f'extern u8 {s}[];\n' for s in sorted(sym_arrays))+''.join(f'extern {t} {s};\n' for s,t in sorted(externs.items()) if s not in sym_arrays)
body=f'\nvoid {name}(void)\n{{\n'+'\n'.join(lines)+'\n}\n'
open(out,'w').write(hdr+ext+body); print('wrote',out,len(stmts),'stores')
