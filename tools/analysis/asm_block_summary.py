import re, sys
lines = open(sys.argv[1]).read().split('\n')
blocks=[]; cur=[]
for L in lines:
    cur.append(L)
    if re.search(r'beq zero, zero, 0x', L) or 'JRTABLE' in L:
        blocks.append(cur); cur=[]
if cur: blocks.append(cur)
print('blocks:', len(blocks))
for bi,b in enumerate(blocks):
    first = b[0].split()[0] if b and b[0].split() else '?'
    calls=[]; stores=[]
    for x in b:
        if 'CALL' in x: calls.append(x.split('CALL ')[1].strip())
        m = re.search(r'(\w+) = (-?\d+)$', x)
        st = re.search(r'-> (D_\w+)', x)
        if st: stores.append(st.group(1))
    print(f'[{bi:3}] L{first:>5} calls:{",".join(c.replace("0x","") for c in calls)} stores:{",".join(stores)}')
