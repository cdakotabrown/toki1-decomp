#!/usr/bin/env python3
#
# Written 2026-08-14. Part of the
# mnemonic-SHAPE alignment harness: shape_tdis.py (target disasm) + shape_dis.py
# (explicit-env-only compile, dodges the @CompileEnv prose-scrape trap) +
# shape_align.py (difflib opcode-stream alignment) + shape_sbs.py (side-by-side).
# Shape alignment finds real holes that exact%% hides — exact%% moves wholesale on
# allocation shifts and rewards count-compensating hooks. Intermediates in /tmp.
"""tdis.py <OV> <FUNC> [SIZE] -- disassemble TARGET bytes from the EXN with the
same objdump used for compiled output.  Writes /tmp/<FUNC>.tdis"""
import sys, os, re, subprocess

ROOT = "."
os.chdir(ROOT)
ov, fn = sys.argv[1], sys.argv[2]
OVERLAY_BASE = 0x80132000

# Window start: @SymbolStart (absorbed heads begin BEFORE the name-derived
# address — the name alone mis-windows e.g. TACO/func_801427E0 by 13 words),
# then the .sym START field, then the name as last resort.
addr = None
size = None
src = f"src/overlay/{ov}/{fn}.c"
if os.path.exists(src):
    txt = open(src).read()
    m = re.search(r'@SymbolStart:\s*(0x[0-9A-Fa-f]+)', txt)
    if m:
        addr = int(m.group(1), 16)
    m = re.search(r'@SymbolSize:\s*(\d+)', txt)
    if m:
        size = int(m.group(1))
if addr is None or size is None:
    for l in open(f"config/overlay/{ov}.sym"):
        p = l.strip().split(':')
        if len(p) >= 3 and p[2] == fn:
            if addr is None:
                addr = int(p[0], 16)
            if size is None:
                size = int(p[1], 0)
            break
if addr is None:
    addr = int(fn.split('_')[1], 16)
if len(sys.argv) > 3:
    size = int(sys.argv[3])

data = open(f"assets/overlays/{ov}.EXN", 'rb').read()[addr-OVERLAY_BASE:][:size]
open(f"/tmp/{fn}.bin", 'wb').write(data)
r = subprocess.run(["mips-linux-gnu-objdump", "-D", "-b", "binary",
                    "-m", "mips:isa32", "-EL", f"/tmp/{fn}.bin"],
                   capture_output=True, text=True)
n = 0
with open(f"/tmp/{fn}.tdis", "w") as f:
    for l in r.stdout.splitlines():
        m = re.match(r'^\s*([0-9a-f]+):\t([0-9a-f]{8}) \t(.*)$', l)
        if m:
            f.write(f"{int(m.group(1),16)//4:5d} {m.group(2)} {m.group(3).strip()}\n")
            n += 1
print(f"target {n} words -> /tmp/{fn}.tdis")
