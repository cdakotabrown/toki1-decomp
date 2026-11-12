#!/usr/bin/env python3
"""Report overlay `.s` files that cover FEWER bytes than their `.sym` symbol.

Surfaced 2026-08-29 when a decompilation pass could not match ~10 EN_NICHI
functions until it stopped trusting `asm/overlay/<OV>/<func>.s` and
disassembled the real window instead.

⚠️ CORRECTION (same day, second pass): the first version of this tool — and the
CLAUDE.md note written from it — said the gap was at the END, a "shared epilogue
tail". **That is wrong for 83% of cases.** The gap is measured by comparing a
byte count against a symbol size, which says nothing about WHICH END is missing.
Checking the symbol's recorded ADDRESS against the address in its own NAME
settles it:

  FRONT (1,644)  `.sym` address < the address in the name, by exactly the gap.
                 The symbol window was deliberately widened BACKWARDS over a
                 hoisted cross-prologue head (IDO -O2 lifts the first global
                 read above the prologue), and the `.s` still starts at the
                 prologue.  The `.sym` is RIGHT; the `.s` was never regenerated.
                 Most are 8 bytes — the `lui`/load pair.
  TAIL    (388)  `.sym` address == the name's address, so the missing bytes are
                 genuinely at the end.
  BOTH     (87)  the head was absorbed AND the gap is larger than that delta.

Either way the practical rule is the same, and for the FRONT class it is the
more serious of the two: **C written from a head-less window cannot match**,
because the body reads a register nothing inside the window ever wrote — the
same trap the main binary's boundary doctrine covers. Use
`tools/overlay/dis_overlay.py <OV> <FUNC>`, which disassembles the full `.sym`
window out of the EXN.

The `.s` files are NOT regenerated to fix this: they feed the byte-perfect ASM
build whose SHA1 is a project invariant, so rewriting ~2,000 of them is separate,
gated work.  A gap of all-zero words is IDO's 16-byte `.text` padding and is not
reported.
"""
import os, glob, struct, sys, argparse, collections

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import overlay_names as ON  # noqa: E402  (landed overlay names -> addresses)


def exn(ov):
    for c in ('assets/overlays/%s.EXN' % ov, 'assets/overlays/%s.exn' % ov):
        p = os.path.join(ROOT, c)
        if os.path.exists(p):
            return open(p, 'rb').read()
    return None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--list', action='store_true')
    args = ap.parse_args()

    cache, rows = {}, []
    for p in sorted(glob.glob(os.path.join(ROOT, 'config/overlay/*.sym'))):
        ov = os.path.basename(p)[:-4]
        if ov not in cache:
            cache[ov] = exn(ov)
        img = cache[ov]
        if img is None:
            continue
        for line in open(p, errors='replace'):
            line = line.strip()
            if not line or line.startswith('#'):
                continue
            parts = line.split(':')
            if len(parts) < 3:
                continue
            name = parts[2].strip()
            if not ON.is_func_symbol(ov, name):
                continue
            named = ON.func_addr(ov, name)       # func_<ADDR>, or a landed name
            try:
                addr, size = int(parts[0], 16), int(parts[1])
            except ValueError:
                continue
            if named is None:
                continue
            src = os.path.join(ROOT, 'asm/overlay/%s/%s.s' % (ov, name))
            if not os.path.exists(src):
                continue
            have = 4 * len([l for l in open(src, errors='replace')
                            if l.startswith('    ') and l.strip()])
            if have >= size:
                continue
            gap = size - have
            body = img[addr - 0x80132000: addr - 0x80132000 + size]
            if not body or not any(body[have:]):
                continue                     # .text padding, benign
            if named - addr == gap:
                kind = 'FRONT'
            elif named == addr:
                kind = 'TAIL'
            else:
                kind = 'BOTH'
            rows.append((gap, kind, ov, name, size, have, named - addr))

    by = collections.Counter(r[1] for r in rows)
    print("overlay .s files covering fewer bytes than their .sym symbol: %d" % len(rows))
    for k, why in (('FRONT', 'the .sym was widened back over a hoisted head; the .s starts at the prologue'),
                   ('TAIL', 'the missing bytes are genuinely at the end'),
                   ('BOTH', 'head absorbed AND a further gap')):
        print("  %-6s %5d  %s" % (k, by[k], why))
    print("\nUse tools/overlay/dis_overlay.py <OV> <FUNC> for the real window.\n")
    print("worst 12 by bytes missing:")
    for g, k, ov, nm, sz, have, headdelta in sorted(rows, reverse=True)[:12]:
        print("  %-5s %-10s %-18s sym=%5dB  .s=%5dB  missing %5dB"
              % (k, ov, nm, sz, have, g))
    if args.list:
        print()
        for g, k, ov, nm, sz, have, hd in sorted(rows, reverse=True):
            print("%-5s %-10s %-18s %5d %5d %5d" % (k, ov, nm, sz, have, g))
    return 0


if __name__ == '__main__':
    sys.exit(main())
