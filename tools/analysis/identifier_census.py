#!/usr/bin/env python3
"""Identifier census over the PS1 C and headers (token-based, ~2 s).

Counts distinct identifiers by kind and how many are still address-style
(func_/D_XXXXXXXX) or placeholder-style (unk/pad/var/temp/a0...). Tracks the
naming goal in docs/SYMBOL-MAPPING-PLAN.md. Run from the repo root.
"""
import re,glob,collections,sys
TOK=re.compile(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'|^[ \t]*#[^\n]*(?:\\\n[^\n]*)*|[A-Za-z_]\w*|0[xX][0-9A-Fa-f]+|\d+|->|[{}()\[\];,.:=*]|\S',re.S|re.M)
KW=set('auto break case char const continue default do double else enum extern float for goto if int long register return short signed sizeof static struct switch typedef union unsigned void volatile while'.split())
TYPES=set('u8 s8 u16 s16 u32 s32 f32 int char short long void unsigned signed float double struct union enum const volatile register static'.split())
addr=re.compile(r'^(func|D|sub|jtbl|L|B|S)_[0-9A-Fa-f]{6,8}(_\w+)?$')
unk=re.compile(r'^(unk|pad|field|arg|var|temp|tmp|sp|a|b|c|i|j|k|n|p|q|r|s|t|v|x|y|w|v[0-9]|s[0-9]|t[0-9]|a[0-9])[0-9A-Fa-f_]*$')
cat=collections.defaultdict(set); local_occ=0
files=glob.glob('src/main/matched/*.c')+glob.glob('src/overlay/*/*.c')+glob.glob('include/**/*.h',recursive=True)
for f in files:
    ov=f.split('/')[2] if f.startswith('src/overlay') else 'main'
    toks=[m.group(0) for m in TOK.finditer(open(f,errors='replace').read())]
    toks=[t for t in toks if not (t.startswith('/*') or t.startswith('//') or t.lstrip().startswith('#') or t[0] in '"\'')]
    depth=0; stmt_start=True; paren=0; last=0
    for i,t in enumerate(toks):
        nxt=toks[i+1] if i+1<len(toks) else ''
        prv=toks[i-1] if i else ''
        if t=='{': depth+=1; stmt_start=True; last=i+1; continue
        if t=='}': depth=max(0,depth-1); stmt_start=True; last=i+1; continue
        if t==';': stmt_start=True; last=i+1; continue
        if t in ('struct','union','enum') and re.match(r'[A-Za-z_]\w*$',nxt): cat['type tags'].add(nxt)
        if t=='typedef':
            j=i; d=0
            while j<len(toks) and not (toks[j]==';' and d==0):
                d+= toks[j]=='{'; d-= toks[j]=='}'; j+=1
            k=j-1
            while k>i and not re.match(r'[A-Za-z_]\w*$',toks[k]): k-=1
            if k>i: cat['typedefs'].add(toks[k])
        if prv in ('->','.') and re.match(r'[A-Za-z_]\w*$',t): cat['struct fields'].add(t)
        if not re.match(r'[A-Za-z_]\w*$',t) or t in KW: 
            stmt_start=False if t not in ('{','}',';') else stmt_start; continue
        if depth>0 and nxt==':' and stmt_start and toks[i+2:i+3]!=[':']: cat['goto labels'].add(ov+':'+t)
        if depth==0 and nxt=='(' and 'extern' not in toks[last:i] and 'typedef' not in toks[last:i]:
            j=i+1; d=0
            while j<len(toks):
                d+= toks[j]=='('; d-= toks[j]==')'
                if d==0: break
                j+=1
            n=toks[j+1] if j+1<len(toks) else ''
            if n=='{' or n in TYPES or (n and n not in (';',',','=','(',')','[')):
                cat['functions defined'].add(t if ov=='main' else ov+':'+t)
        if depth==0 and 'extern' in toks[last:i] and nxt in (';','[',',','='): cat['globals (extern data)'].add(t)
        if depth>0 and nxt in (';','=','[',',') and (prv in TYPES or prv=='*' or re.match(r'[A-Z]\w*$',prv)) and not prv in ('return',):
            local_occ+=1; cat['local names (distinct)'].add(t)
        stmt_start=False
for k in sorted(cat):
    v=cat[k]; a=sum(1 for x in v if addr.match(x.split(':')[-1])); u=sum(1 for x in v if unk.match(x.split(':')[-1]))
    print(f'{k:28s} {len(v):7d}  address-style {a:6d}  placeholder-style {u:6d}')
print('local declarations (occurrences)',local_occ, ' files',len(files))
