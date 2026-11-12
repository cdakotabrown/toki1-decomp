#!/usr/bin/env python3
"""Disassemble an overlay function over its FULL `.sym` window.

Use this instead of reading `asm/overlay/<OV>/<func>.s`, which is SHORTER than
the symbol for 1,978 functions (`tools/overlay/audit_asm_truncation.py`).

Cause, for the record: `extract_exn_functions.py` ends a function at the FIRST
`jr ra` it sees (its epilogue test sets `in_function = False` immediately), so
any function with an early return is cut off there. The `.sym` size comes from
a different pass and is right. Write C from the short `.s` and you are
targeting a different function than the resolver compares against, and cannot
match — that blocked ~10 functions in one batch on 2026-08-29 until it
stopped trusting the `.s`.

The `.s` files are not regenerated to fix this: they feed the byte-perfect ASM
build whose SHA1 is a project invariant, so rewriting ~2,000 of them is a
separate, gated piece of work. This tool sidesteps them.

    python3 tools/overlay/dis_overlay.py TT func_80135848
    python3 tools/overlay/dis_overlay.py TT func_80135848 --raw   # words only
"""
import os, sys, glob, struct, subprocess, argparse

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
OVERLAY_BASE = 0x80132000


def sym_lookup(ov, func):
    p = os.path.join(ROOT, 'config/overlay/%s.sym' % ov)
    for line in open(p, errors='replace'):
        line = line.strip()
        if not line or line.startswith('#'):
            continue
        parts = line.split(':')
        if len(parts) >= 3 and parts[2].strip() == func:
            return int(parts[0], 16), int(parts[1])
    return None, None


def exn_path(ov):
    for c in ('assets/overlays/%s.EXN' % ov, 'assets/overlays/%s.exn' % ov):
        p = os.path.join(ROOT, c)
        if os.path.exists(p):
            return p
    return None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('overlay')
    ap.add_argument('func')
    ap.add_argument('--raw', action='store_true', help='print words, no mnemonics')
    args = ap.parse_args()

    addr, size = sym_lookup(args.overlay, args.func)
    if addr is None:
        print("not in config/overlay/%s.sym: %s" % (args.overlay, args.func))
        return 1
    p = exn_path(args.overlay)
    if p is None:
        print("no EXN for overlay %s" % args.overlay)
        return 1
    data = open(p, 'rb').read()
    off = addr - OVERLAY_BASE
    body = data[off:off + size]
    if len(body) < size:
        print("EXN too short for %s+%d" % (hex(addr), size))
        return 1

    s = os.path.join(ROOT, 'asm/overlay/%s/%s.s' % (args.overlay, args.func))
    have = 0
    if os.path.exists(s):
        have = 4 * len([l for l in open(s, errors='replace')
                        if l.startswith('    ') and l.strip()])
    note = ''
    if have and have < size:
        note = ("   ⚠️  asm/overlay/%s/%s.s has only %d bytes — %d SHORT"
                % (args.overlay, args.func, have, size - have))
    print("%s / %s  @ 0x%08X  size %d bytes (%d insns)%s"
          % (args.overlay, args.func, addr, size, size // 4, note))

    if args.raw:
        for i in range(0, size, 4):
            print("  0x%08X  %08x" % (addr + i, struct.unpack_from('<I', body, i)[0]))
        return 0

    tmp = '/tmp/dis_overlay_%d.bin' % os.getpid()
    open(tmp, 'wb').write(body)
    try:
        out = subprocess.run(
            ['mips-linux-gnu-objdump', '-D', '-b', 'binary', '-m', 'mips:3000',
             '-EL', '--adjust-vma=0x%X' % addr, tmp],
            capture_output=True, text=True).stdout
        started = False
        for line in out.splitlines():
            if line.strip().endswith('<.data>:'):
                started = True
                continue
            if started and line.strip():
                print(line)
    finally:
        os.unlink(tmp)
    return 0


if __name__ == '__main__':
    sys.exit(main())
