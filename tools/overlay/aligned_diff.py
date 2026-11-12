#!/usr/bin/env python3
"""Aligned target-vs-compiled disassembly for one overlay function.

Usage: python3 tools/overlay/aligned_diff.py <OV> <FUNC> [SRC.c]

Compiles SRC (default src/overlay/<OV>/<FUNC>.c) through the normal
resolver pipeline (hooks + PostProc + name-derived link), then aligns the
two instruction streams on MNEMONICS with difflib so an insertion does not
smear every later line.  Columns: target index, compiled index, marker
("  " identical text, " ~" same mnemonic / different operands, "##"
structural), target | compiled.  Last line: target words, compiled words
(the compiled stream is NOT sliced to the symbol, so trailing pad nops and
over-long bodies are visible).  Read-only; SRC may be a scratch copy.
A diagnostic only -- the verdict is still resolve_overlay_relocs.py.
"""
import sys, os, struct, tempfile, subprocess, re
from pathlib import Path
import os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import resolve_overlay_relocs as R
ov, fn = sys.argv[1], sys.argv[2]
src = Path(sys.argv[3]) if len(sys.argv) > 3 else R.SRC_OVERLAY_DIR/ov/f"{fn}.c"
funcs = R.parse_sym_file(ov); addr, size = funcs[fn]
content = src.read_text(errors='ignore')
link_opts = R.link_options_from_source(content)  # @RodataBase/@StaticAddr
rb = link_opts['rodata_base']
tgt = R.get_target_bytes(ov, addr, size)
with tempfile.TemporaryDirectory() as td:
    td = Path(td)
    obj = R.compile_object(ov, fn, src, addr, td)
    import os
    rawsz=size+400
    comp, _, _ = R.link_resolved(obj, addr, rawsz, td, rodata_base=rb, func_name=fn,
                                  static_addrs=link_opts['static_addrs'], overlay_name=ov)
def dis(b, tag):
    p = Path(tempfile.mktemp()); p.write_bytes(b)
    o = subprocess.run(['mipsel-linux-gnu-objdump','-D','-b','binary','-mmips:3000','-EL','-z',f'--adjust-vma={addr}',str(p)],capture_output=True,text=True).stdout
    out=[]
    for l in o.splitlines():
        mm = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', l)
        if mm: out.append(mm.group(3).replace('\t',' '))
    return out
t = dis(tgt,'t'); c = dis(comp,'c')
import difflib
key=lambda x: x.split()[0] if x else ''
sm=difflib.SequenceMatcher(None,[key(x) for x in t],[key(x) for x in c],autojunk=False)
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag=='equal':
        for k in range(i2-i1):
            a,b=t[i1+k],c[j1+k]
            print(f"{i1+k:3d} {j1+k:3d} {'  ' if a==b else ' ~'} {a:34s} | {b}")
    else:
        for k in range(max(i2-i1,j2-j1)):
            a=t[i1+k] if i1+k<i2 else ''; b=c[j1+k] if j1+k<j2 else ''
            print(f"{i1+k if a else -1:3d} {j1+k if b else -1:3d} ## {a:34s} | {b}")
print(len(t),len(c))
