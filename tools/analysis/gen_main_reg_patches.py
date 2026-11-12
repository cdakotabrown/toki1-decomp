#!/usr/bin/env python3
"""gen_main_reg_patches.py - derive @PostProc: patch_insn_reg_field for a
MAIN-BINARY function (the main-side counterpart of
tools/overlay/gen_reg_patches.py).

    python3 tools/analysis/gen_main_reg_patches.py <FUNC> <ADDR> <SIZE> <src.c>

Runs the file's own PostProc chain, then walks the compiled and retail streams
in lockstep.  For every differing word it emits register-field patches ONLY
when the two words are identical once the named fields are renamed -- so a
patch is by construction a same-opcode, same-immediate rename.  Everything
else is printed under NOT paintable, with the reason:

  opcode          different instruction -- a real codegen difference, fix the C
  funct/sa        R-type with a different function code or shift amount
  r-type-residue  R-type that still differs after the register rename
  imm16           I-type immediate difference at a NON-relocation site.  These
                  are the words `fixup_relocations` would silently copy from
                  the target; they must be zero before a match is claimed.

Deliberately ignored: `j`/`jal` (bits 16-20 are part of the target address,
not a register field -- treating them as `rt` produces nonsense patches), and
branch displacements, which follow the layout rather than the C.

⚠️ A clean run is NOT a licence to paint.  Read the list: it should be a
rename of ugen temps applied to a def and all of its uses.  Check that no
patch lands on a compiled `nop` (a nop is `sll $zero,$zero,0`; painting one
fabricates an instruction).  Prefer closing the diffs in codegen first --
IDO52_UGEN_TEMP_REG (Hook 36) forces the same choice at the source of the
problem and costs no PostProc debt.
"""

import sys,re,subprocess,os
import os as _os
sys.path.insert(0,_os.path.dirname(_os.path.abspath(__file__)))
import dump_main_diff as mdump
REGN={0:'zero',1:'at',2:'v0',3:'v1',4:'a0',5:'a1',6:'a2',7:'a3',8:'t0',9:'t1',10:'t2',11:'t3',
      12:'t4',13:'t5',14:'t6',15:'t7',16:'s0',17:'s1',18:'s2',19:'s3',20:'s4',21:'s5',22:'s6',
      23:'s7',24:'t8',25:'t9',26:'k0',27:'k1',28:'gp',29:'sp',30:'fp',31:'ra'}
REV={v:k for k,v in REGN.items()}
fn,ADDR,SIZE,src=sys.argv[1],int(sys.argv[2],16),int(sys.argv[3]),sys.argv[4]
mdump.ADDR_S='0x%08X'%ADDR
o=mdump.build(src,fn)
t=mdump.target(ADDR,SIZE); c=mdump.words(o,fn)
rel=set(); sec=False
for l in subprocess.run(['mips-linux-gnu-readelf','-r',o],capture_output=True,text=True).stdout.splitlines():
    if l.startswith('Relocation section'): sec='.rel.text' in l
    m=re.match(r'^([0-9a-f]{8})\s+[0-9a-f]{8}\s+R_MIPS',l)
    if m and sec: rel.add(int(m.group(1),16)//4)
patches=[]; bad=[]
BR={4,5,1,6,7,20,21,22,23}   # beq/bne/regimm/blez/bgtz + likely
for i in range(min(len(t),len(c))):
    tw,cw=t[i][1],c[i][1]
    if tw==cw: continue
    top,cop=(tw>>26)&0x3f,(cw>>26)&0x3f
    if top!=cop: bad.append((i,'opcode',t[i][2],c[i][2])); continue
    if top in (2,3): continue                    # j/jal: target field, not registers
    fields=(('rs',21),('rt',16),('rd',11)) if top==0 else (('rs',21),('rt',16))
    if top==0 and (tw&0x7ff)!=(cw&0x7ff): bad.append((i,'funct/sa',t[i][2],c[i][2])); continue
    diffs=[]
    for f,sh in fields:
        a,b=(tw>>sh)&0x1f,(cw>>sh)&0x1f
        if a!=b: diffs.append((f,REGN[b],REGN[a]))
    test=cw
    for f,frm,to in diffs:
        sh={'rs':21,'rt':16,'rd':11}[f]
        test=(test & ~(0x1f<<sh)) | (REV[to]<<sh)
    if test!=tw:
        if top==0: bad.append((i,'r-type-residue',t[i][2],c[i][2])); continue
        if i in rel: pass                       # unlinked immediate, harness fills it
        elif top in BR: pass                    # branch displacement follows layout
        else: bad.append((i,'imm16',t[i][2],c[i][2])); continue
    for f,frm,to in diffs: patches.append(f"0x{i*4:x}:{f}:{frm}:{to}")
print("patch_insn_reg_field "+" ".join(patches))
print("\nfields",len(patches),"words",len({p.split(':')[0] for p in patches}))
print("NOT paintable:",len(bad))
for b in bad[:25]: print("   ",b)
