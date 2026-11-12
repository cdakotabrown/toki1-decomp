"""Side-by-side target/compiled disassembly for an overlay function.

Usage: .venv/bin/python wip-scratch/sbs.py OVERLAY FUNC [lo] [hi]
"""
import sys, struct, tempfile, os
from pathlib import Path
from capstone import Cs, CS_ARCH_MIPS, CS_MODE_MIPS32, CS_MODE_LITTLE_ENDIAN

sys.path.insert(0, os.path.abspath('tools/overlay'))
import resolve_overlay_relocs as R
import re

ov, fn = sys.argv[1], sys.argv[2]
lo = int(sys.argv[3]) if len(sys.argv) > 3 else 0
hi = int(sys.argv[4]) if len(sys.argv) > 4 else 10 ** 9

md = Cs(CS_ARCH_MIPS, CS_MODE_MIPS32 | CS_MODE_LITTLE_ENDIAN)


def dis(word, addr):
    b = struct.pack('<I', word)
    for i in md.disasm(b, addr):
        return f"{i.mnemonic} {i.op_str}"
    return ".word 0x%08x" % word


os.chdir(R.ROOT)
funcs = R.parse_sym_file(ov)
func_addr, func_size = funcs[fn]
src_file = R.SRC_OVERLAY_DIR / ov / f"{fn}.c"
content = src_file.read_text(errors='ignore')
_sz = R.parse_symbol_size_text(content)
if _sz is not None:
    func_size = _sz
_st = R.parse_symbol_start_text(content)
if _st is not None:
    func_addr = _st
# @RodataBase / @StaticAddr, parsed exactly as the resolver does.
link_opts = R.link_options_from_source(content)
rodata_base = link_opts['rodata_base']

target = R.get_target_bytes(ov, func_addr, func_size)
with tempfile.TemporaryDirectory() as td:
    td = Path(td)
    obj = R.compile_object(ov, fn, src_file, func_addr, td)
    if obj is None:
        print("compile failed")
        sys.exit(1)
    linked = R.link_resolved(obj, func_addr, func_size, td,
                             rodata_base=rodata_base, func_name=fn,
                             static_addrs=link_opts['static_addrs'], overlay_name=ov)
    compiled, resolved, unresolved = linked

nt = len(target) // 4
nc = len(compiled) // 4
print(f"words target={nt} compiled={nc}  (offset = idx*4+0x10 for PostProc)")
for i in range(max(nt, nc)):
    if i < lo or i > hi:
        continue
    tw = struct.unpack('<I', target[i*4:i*4+4])[0] if i < nt else None
    cw = struct.unpack('<I', compiled[i*4:i*4+4])[0] if i < nc else None
    mark = ' ' if tw == cw else '*'
    ts = dis(tw, func_addr + i*4) if tw is not None else '--'
    cs = dis(cw, func_addr + i*4) if cw is not None else '--'
    print(f"{mark}[{i:3d}] {func_addr + i*4:08x} {tw if tw is None else '%08x' % tw} "
          f"{ts:<34} | {cw if cw is None else '%08x' % cw} {cs}")
