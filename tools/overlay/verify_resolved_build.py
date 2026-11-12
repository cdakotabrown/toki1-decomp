#!/usr/bin/env python3
"""verify_resolved_build.py - Prove reloc_only -> byte_perfect via honest linking.

This quantifies how many `reloc_only` overlay functions become *true*
byte_perfect once GNU ld resolves their cross-references to the name-encoded
symbol addresses (the resolution build_overlay.py now wires in via the
INCLUDE'd defsym fragment). The comparison is STRICT and per function: each
function's PostProc'd object is linked at its TRUE address with every func_/D_
symbol DEFINED at its name-encoded address, then compared byte-for-byte against
the shipped EXN (no masking, no copy-from-target).

It delegates the strict link+compare to resolve_overlay_relocs.scan_overlay --
the authoritative honest discriminator -- and reports the conversion count per
overlay plus a grand total.

Why per function (not whole-EXN): the overlay's per-function match STATUS is
defined per function (verify_overlay_bytes.py / overlay_match_status.json). A
whole-EXN byte compare is NOT achievable through this build because (a) every
IDO object carries 16 bytes of types.h `jr ra;nop` thunks ahead of the body
that the shipped EXN lacks, and (b) the shipped EXN has ~95 inter-function gaps
filled with foreign/asm-only content our C sources do not model. Those are
packaging concerns orthogonal to relocation resolution; the per-function strict
link is the honest unit.

Usage:
    python3 tools/overlay/verify_resolved_build.py EVENT
    python3 tools/overlay/verify_resolved_build.py EVENT DATE
    python3 tools/overlay/verify_resolved_build.py --all
"""
import sys
import json
from pathlib import Path

ROOT = Path(__file__).parent.parent.parent
sys.path.insert(0, str(ROOT / "tools" / "overlay"))

from resolve_overlay_relocs import scan_overlay

STATUS_FILE = ROOT / "config" / "overlay_match_status.json"


def main():
    args = sys.argv[1:]
    if not args:
        print(__doc__)
        sys.exit(1)
    status = json.load(open(STATUS_FILE)) if STATUS_FILE.exists() else {}
    if args == ['--all']:
        overlays = sorted(k for k in status.keys() if not k.startswith('_'))
    else:
        overlays = [a.upper() for a in args]

    grand_bp = grand_total = grand_resid = grand_err = 0
    print(f"{'overlay':12} {'reloc_only':>10} {'->byte_perfect':>15} "
          f"{'residual':>9} {'err':>4}")
    print("-" * 56)
    for ov in overlays:
        res = scan_overlay(ov)
        if res['reloc_only_total'] == 0:
            continue
        print(f"{ov:12} {res['reloc_only_total']:>10} "
              f"{res['byte_perfect']:>15} {res['residual']:>9} "
              f"{res['errors']:>4}")
        grand_bp += res['byte_perfect']
        grand_total += res['reloc_only_total']
        grand_resid += res['residual']
        grand_err += res['errors']
    print("-" * 56)
    print(f"{'TOTAL':12} {grand_total:>10} {grand_bp:>15} "
          f"{grand_resid:>9} {grand_err:>4}")
    print(f"\n>>> {grand_bp} reloc_only functions are PROVABLY byte_perfect "
          f"(strict, name-derived link).")


if __name__ == '__main__':
    main()
