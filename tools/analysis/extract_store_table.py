#!/usr/bin/env python3
"""Turn a straight-line init function into a table of resolved stores.

Long main-binary functions that are mostly data movement -- sprite/packet
builders, struct initialisers -- are hundreds of `lui at` / `s{b,h,w}
reg,off(at)` pairs.  Reading those by eye is how transcription errors get in.
This resolves each pair to an absolute address and carries register provenance
through the load/addu/subu chains, so what comes out is a table you can
generate C from:

    80063124 sw  80129cb0   = HI8100
    800631d0 sh  80129cb4   = ((G_80127a12+arg2[+0][+78])-arg2[+0][+2])
    800631ec sb  800ef805   = #11

Read that as: absolute destination, and the VALUE as an expression over
arguments, globals and literals.  Six near-identical records become six rows of
a table you can loop over in a generator script.

    python3 tools/analysis/extract_store_table.py 0x80062F5C 1856
    python3 tools/analysis/extract_store_table.py 0x80062F5C 1856 --args a0=arg0,a1=arg1

Recovered on bustup_sprite_init (1856 bytes, 464 instructions), which went
byte-perfect the same day: the table made the six-record structure obvious and
the C was generated from it rather than typed.

⚠️ Provenance is best-effort and single-pass, so a value can be stale where a
register is reloaded inside a branch -- it is a reading aid, not a decompiler.
Always confirm anything surprising against the raw disassembly.
"""
import argparse
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
TEXT_START = 0x80041000
FILE_BASE = 0x800

INSN = re.compile(r'^\s*([0-9a-f]{8}):\s+[0-9a-f]{8}\s+(\S+)\s*(.*)$')
MEMOP = re.compile(r'(-?\d+)\((\w+)\)')


def disassemble(addr, size, exe):
    data = open(exe, 'rb').read()
    off = (addr - TEXT_START) + FILE_BASE
    tmp = ROOT / '.extract_store_table.bin'
    tmp.write_bytes(data[off:off + size])
    try:
        r = subprocess.run(
            ['mips-linux-gnu-objdump', '-D', '-b', 'binary', '-m', 'mips:3000',
             '-EL', '--adjust-vma=0x%x' % addr, str(tmp)],
            capture_output=True, text=True)
    finally:
        tmp.unlink(missing_ok=True)
    return r.stdout.splitlines()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('address')
    ap.add_argument('size', type=int)
    ap.add_argument('--exe', default=str(ROOT / 'disks/SLPS_000.65'))
    ap.add_argument('--args', default='a0=arg0,a1=arg1,a2=arg2,a3=arg3',
                    help='initial register names, e.g. "a0=self,a1=count"')
    ap.add_argument('--homes', default='',
                    help='caller arg homes as "80=arg2,84=arg3" (sp offsets)')
    args = ap.parse_args()

    val = {}
    for pair in args.args.split(','):
        if '=' in pair:
            k, v = pair.split('=', 1)
            val[k.strip()] = v.strip()
    homes = {}
    for pair in args.homes.split(','):
        if '=' in pair:
            k, v = pair.split('=', 1)
            homes[int(k.strip())] = v.strip()

    hi = {}
    rows = []
    for line in disassemble(int(args.address, 16), args.size, args.exe):
        m = INSN.match(line.replace('\t', ' '))
        if not m:
            continue
        addr, op, ops = m.group(1), m.group(2), m.group(3)
        o = [x.strip() for x in ops.split(',')] if ops else []
        if op == 'lui' and len(o) == 2:
            hi[o[0]] = int(o[1], 16)
            val[o[0]] = 'HI%04x' % int(o[1], 16)
        elif op == 'li' and len(o) == 2:
            val[o[0]] = '#' + o[1]
        elif op in ('lh', 'lw', 'lb', 'lbu', 'lhu') and len(o) > 1:
            mm = MEMOP.match(o[1])
            if not mm:
                continue
            off, base = int(mm.group(1)), mm.group(2)
            if base in hi and str(val.get(base, '')).startswith('HI'):
                val[o[0]] = 'G_%08x' % ((hi[base] << 16) + off)
            elif base == 'sp':
                val[o[0]] = homes.get(off, 'sp%d' % off)
            else:
                val[o[0]] = '%s[+%d]' % (val.get(base, base), off)
        elif op in ('addu', 'subu', 'addiu') and len(o) == 3:
            sym = {'addu': '+', 'addiu': '+', 'subu': '-'}[op]
            val[o[0]] = '(%s%s%s)' % (val.get(o[1], o[1]), sym,
                                      val.get(o[2], o[2]))
        elif op == 'move' and len(o) == 2:
            val[o[0]] = val.get(o[1], o[1])
        elif op in ('sw', 'sh', 'sb') and len(o) > 1:
            mm = MEMOP.match(o[1])
            if not mm:
                continue
            off, base = int(mm.group(1)), mm.group(2)
            if base == 'at' and 'at' in hi:
                rows.append((addr, op, '%08x' % ((hi['at'] << 16) + off),
                             val.get(o[0], o[0])))
            elif base == 'sp':
                rows.append((addr, op, 'sp+%d' % off, val.get(o[0], o[0])))
            elif base in val:
                rows.append((addr, op, '%s[+%d]' % (val[base], off),
                             val.get(o[0], o[0])))

    for a, op, dst, v in rows:
        print('%s %-3s %-22s = %s' % (a, op, dst, v))
    print('\n%d stores' % len(rows), file=sys.stderr)


if __name__ == '__main__':
    main()
