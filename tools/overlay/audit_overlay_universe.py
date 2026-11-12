#!/usr/bin/env python3
"""Report the overlay completion figure over the TRUE universe.

`verify_overlay_bytes.py` enumerates its function list from
`src/overlay/<OV>/*.c` -- the C files that exist -- not from the `.sym` map.
So a function that has byte-perfect ASM and no C is invisible to it: absent
from the numerator AND from the denominator.  The published percentage is
therefore "of the functions someone has already written C for", not "of the
overlay".

Measured 2026-08-29: the `.sym` files hold 6325 `func_` symbols; the status
JSON holds 6141 and the verifier enumerates 6125.  **184 functions, 25,896
bytes of real code, are counted nowhere** -- among them a 3,192-byte function
in DATE.  None of them carries a blocker entry ANYWHERE: they have no `.c`, so they have
no header either, and none appears in `config/c_unmatchable.txt`.  By the
project's own completion rule they are undecided work, not a terminal
`asm_only`.

⚠️ Do not read the blocker line as an indictment of the COUNTED `asm_only`
population.  Only 121 of those 338 appear in `config/c_unmatchable.txt`, but
all 217 of the rest document their blocker in the `.c` header instead -- a
difference in where the category is recorded, not a missing one.  Checked
2026-08-29 before drawing the wrong conclusion.

This tool prints both figures side by side so the difference is explicit. It
does not rewrite anything.
"""
import json, os, glob, re, sys, argparse

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import overlay_names as ON      # a renamed overlay function is still a function symbol


def sym_functions(ov):
    out = {}
    p = os.path.join(ROOT, 'config/overlay/%s.sym' % ov)
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
        try:
            out[name] = int(parts[1])
        except ValueError:
            out[name] = 0
    return out


def sym_addresses(ov):
    out = {}
    for line in open(os.path.join(ROOT, 'config/overlay/%s.sym' % ov), errors='replace'):
        line = line.strip()
        if not line or line.startswith('#'):
            continue
        parts = line.split(':')
        if len(parts) >= 3 and ON.is_func_symbol(ov, parts[2].strip()):
            try:
                out[parts[2].strip()] = int(parts[0], 16)
            except ValueError:
                pass
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--status', default='config/overlay_match_status.json')
    args = ap.parse_args()

    status = json.load(open(os.path.join(ROOT, args.status)))
    blocked = set()
    for line in open(os.path.join(ROOT, 'config/c_unmatchable.txt'), errors='replace'):
        line = line.split('#')[0].strip()
        if line:
            blocked.add(line.split()[0])

    tot = counted = eff = uncounted = unc_bytes = unc_blocked = covered = 0
    per_ov = []
    for p in sorted(glob.glob(os.path.join(ROOT, 'config/overlay/*.sym'))):
        ov = os.path.basename(p)[:-4]
        syms = sym_functions(ov)
        addr_of = sym_addresses(ov)
        st = status.get(ov, {})
        st = st if isinstance(st, dict) else {}
        o_tot = len(syms)
        # A symbol can be absent from the status JSON and still be COVERED:
        # a neighbour's .c may declare an @SymbolSize window that spans it
        # (an absorbed cross-prologue head, or a merged alt-entry block).
        # Those are not work.  Measured 2026-08-29: 11 of 107, including all
        # seven DATE head/body pairs.
        wins = []
        for f, sz in syms.items():
            c = os.path.join(ROOT, 'src/overlay/%s/%s.c' % (ov, f))
            if not os.path.exists(c):
                continue
            m = re.search(r'@SymbolSize:\s*(\d+)', open(c, errors='replace').read())
            wins.append((addr_of.get(f, -1), int(m.group(1)) if m else sz))
        o_cov = [f for f in syms if f not in st
                 and any(wa <= addr_of.get(f, -1) < wa + ws
                         for wa, ws in wins if wa != addr_of.get(f, -1))]
        o_unc = [f for f in syms if f not in st and f not in o_cov]
        covered += len(o_cov)
        o_eff = sum(1 for f, v in st.items()
                    if (v.get('status') if isinstance(v, dict) else v)
                    in ('byte_perfect', 'asm_only', 'reloc_only'))  # reloc_only = rodata-blocked, effective by rule
        tot += o_tot
        counted += len(st)
        eff += o_eff
        uncounted += len(o_unc)
        unc_bytes += sum(syms[f] for f in o_unc)
        unc_blocked += sum(1 for f in o_unc
                           if f in blocked or '%s/%s' % (ov, f) in blocked)
        if o_unc:
            per_ov.append((len(o_unc), ov, o_tot, len(st)))

    print("TRUE universe (func_ symbols in config/overlay/*.sym): %d" % tot)
    print("counted by the status JSON:                            %d" % counted)
    print("COVERED by a neighbour's @SymbolSize (absorbed, not work): %d" % covered)
    print("UNCOUNTED (byte-perfect ASM, no .c, in neither half):  %d  (%d bytes)"
          % (uncounted, unc_bytes))
    print("   of those, carrying a blocker entry anywhere:        %d" % unc_blocked)
    print("   (they have no .c, so no header blocker either -- this is")
    print("    undecided work, not a terminal asm_only)")
    print()
    print("effective (byte_perfect + asm_only + rodata-blocked reloc_only):                  %d" % eff)
    print("  as published, over the counted set:  %d/%d = %.2f%%"
          % (eff, counted, 100.0 * eff / max(counted, 1)))
    print("  over the TRUE universe:              %d/%d = %.2f%%"
          % (eff, tot, 100.0 * eff / max(tot, 1)))
    print("  + the %d COVERED symbols (their bytes are already reproduced by a"
          % covered)
    print("    byte-perfect neighbour's window):  %d/%d = %.2f%%"
          % (eff + covered, tot, 100.0 * (eff + covered) / max(tot, 1)))
    print()
    print("overlays with uncounted functions, worst first:")
    for n, ov, t, c in sorted(per_ov, reverse=True):
        print("  %-10s sym=%-5d counted=%-5d uncounted=%d" % (ov, t, c, n))
    return 0


if __name__ == '__main__':
    sys.exit(main())
