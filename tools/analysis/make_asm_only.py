#!/usr/bin/env python3
"""
make_asm_only.py -- extract a byte-verified asm_only .s for a main-binary function.

    python3 tools/analysis/make_asm_only.py 0x800A5F88 692 ApplyMatrixLV \
        "GTE hand-asm (25 COP2 words in 173)" asm/main/matched/ApplyMatrixLV.s

Disassembles the retail window (objdump, no aliases, zero runs kept), turns
in-window branch targets into local labels, writes the file in the
asm/main/matched house style, re-assembles it and byte-compares the result
against disks/SLPS_000.65 (trailing section padding tolerated). Prints MATCH
or the first differing offset. asm/main/matched/*.s is gitignored (retail
code): regenerate locally with this tool; the audit
(tools/analysis/audit_main_functions.py) counts the result as asm_only.
Pair every use with a blocker line in config/c_unmatchable.txt.
"""
import sys,subprocess,re,os
ROOT=os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
addr=int(sys.argv[1],16); size=int(sys.argv[2]); name=sys.argv[3]; note=sys.argv[4]; out=sys.argv[5]
d=open(os.path.join(ROOT,'disks/SLPS_000.65'),'rb').read(); o=(addr-0x80041000)+0x800
blob=d[o:o+size]; open('/tmp/make_asm_only.bin','wb').write(blob)
od=subprocess.run(['mips-linux-gnu-objdump','-D','-z','-b','binary','-m','mips:3000','-EL','--no-show-raw-insn','-M','no-aliases','--adjust-vma=%#x'%addr,'/tmp/make_asm_only.bin'],capture_output=True,text=True).stdout
ins=[(int(m.group(1),16),m.group(2).strip()) for l in od.splitlines() for m in [re.match(r'\s*([0-9a-f]+):\s+(.*)',l)] if m]
targets=set()
for a,t in ins:
    m=re.match(r'(b\w*|j)\s+(?:.*,)?\s*0x([0-9a-f]+)$',t)
    if m and not t.startswith('jal'):
        tg=int(m.group(2),16)
        if addr<=tg<addr+size: targets.add(tg)
lines=['.set noreorder','.set noat','','.text',f'.globl {name}','',f'/*',f' * {name}',f' * Address: 0x{addr:08X}',f' * Size: {size} bytes ({size//4} instructions)',' * Region: Library (0x80086000-0x800BFFFF)',' * Status: asm_only (byte-perfect ASM, no C)',' *',f' * BLOCKER: {note}',' */',f'{name}:']
for a,t in ins:
    if a in targets: lines.append(f'.L{a:08x}:')
    m=re.match(r'(b\w*|j)\s+((?:.*,)?)\s*0x([0-9a-f]+)$',t)
    if m and not t.startswith('jal'):
        tg=int(m.group(3),16)
        if addr<=tg<addr+size: t=f'{m.group(1)}\t{m.group(2)}.L{tg:08x}'.replace(',.L',',.L')
    t=re.sub(r'\b(zero|at|v[01]|a[0-3]|t[0-9]|s[0-8]|k[01]|gp|sp|fp|ra)\b',r'$\1',t)
    t=re.sub(r'\bc0_(\w+)',lambda m:{'index':'$0','random':'$1','entrylo':'$2','context':'$4','badvaddr':'$8','entryhi':'$10','sr':'$12','status':'$12','cause':'$13','epc':'$14','prid':'$15'}.get(m.group(1),'c0_'+m.group(1)),t)
    lines.append('    '+t)
open(out,'w').write('\n'.join(lines)+'\n')
r=subprocess.run(['mipsel-linux-gnu-as','-march=mips1','-EL','-o','/tmp/make_asm_only.o',out],capture_output=True,text=True)
if r.returncode: print('ASMERR',r.stderr[:600]); sys.exit(1)
subprocess.run(['mipsel-linux-gnu-objcopy','-O','binary','-j','.text','/tmp/make_asm_only.o','/tmp/make_asm_only2.bin'],check=True)
got=open('/tmp/make_asm_only2.bin','rb').read()
print(name,'len',len(got),size,'MATCH' if got[:size]==blob and not any(got[size:]) else 'DIFF at '+str(next((i for i in range(min(len(got),size)) if got[i]!=blob[i]),-1)))
