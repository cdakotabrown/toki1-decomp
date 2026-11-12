import re, sys
# Digest MIPS asm into per-line pseudo-ops to make case-map writing mechanical.
lines = [l.rstrip() for l in open(sys.argv[1])]
base = int(sys.argv[2], 16)
GLB = {}
def gname(hi, lo):
    addr = (hi << 16) + (lo - 0x10000 if lo >= 0x8000 else lo)
    return f'D_{addr:08X}'
out=[]
lui = {}   # reg -> hi
for i, l in enumerate(lines):
    m = re.match(r'\s*(\w+)\s+(.*)', l)
    if not m: continue
    op, args = m.group(1), m.group(2).replace('$','')
    addr = base + i*4
    A = [a.strip() for a in args.split(',')]
    txt = None
    if op == 'lui':
        lui[A[0]] = int(A[1],16)
    elif op in ('lw','lh','lhu','lb','lbu','sw','sh','sb'):
        mm = re.match(r'(\w+), (-?\w+)\((\w+)\)', args)
        if mm:
            rt, off, rs = mm.group(1), int(mm.group(2),0), mm.group(3)
            if rs in lui:
                g = gname(lui[rs], off & 0xffff if off>=0 else off+0x10000)
                txt = f'{op} {rt} {"<-" if op[0]=="l" else "->"} {g}'
    elif op == 'jal':
        txt = f'CALL {A[0]}'
    elif op == 'jr' and A[0] != 'ra':
        txt = f'JRTABLE {A[0]}'
    elif op in ('addiu','ori') and len(A)==3 and A[1]=='zero':
        txt = f'{A[0]} = {int(A[2],0)}'
    elif op in ('beq','bne') :
        txt = f'{op} {args}'
    elif op in ('addiu',) and len(A)==3:
        txt = f'{A[0]} = {A[1]} + {int(A[2],0)}'
    if txt:
        out.append(f'{i+1:5} {addr:08X}  {txt}')
    elif op not in ('lui','nop'):
        out.append(f'{i+1:5} {addr:08X}  {op} {args}')
open(sys.argv[3],'w').write('\n'.join(out))
print(len(out), 'digest lines')
