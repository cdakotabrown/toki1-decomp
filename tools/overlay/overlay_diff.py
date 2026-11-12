#!/usr/bin/env python3
"""overlay_diff.py — per-function byte diff between built EXN and reference EXN.

Phase 2 of overlay matching: the main goal is byte-perfect EXN files
(not just instruction-count match). This tool reports which functions
inside a built overlay differ from the reference, so PostProc work can
target them.

Usage:
    python3 tools/overlay/overlay_diff.py GEKO
    python3 tools/overlay/overlay_diff.py GEKO --details
    python3 tools/overlay/overlay_diff.py --all
    python3 tools/overlay/overlay_diff.py --all --summary-only

Outputs, per function, one of:
    MATCH   — bytes identical
    DIFF N  — N mismatching bytes out of M (with first-diff offset)
    SIZE    — function lengths differ (different instruction count)
    MISSING — function absent from built EXN

Exit code: 0 if every function matches across every overlay scanned,
1 otherwise. Useful as a CI gate once phase 2 is done.
"""
import argparse
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
REF_DIR = ROOT / "pc" / "assets" / "overlays"
BUILT_DIR = ROOT / "build" / "overlay"
SYM_DIR = ROOT / "config" / "overlay"

BASE_ADDR = 0x80132000


def parse_sym(path: Path):
    """Yield (addr, size, name) for function entries."""
    in_funcs = False
    for line in path.read_text().splitlines():
        line = line.strip()
        if not line or line.startswith("#"):
            if "Functions" in line:
                in_funcs = True
            elif "===" in line and "Functions" not in line:
                in_funcs = False
            continue
        if not in_funcs:
            continue
        parts = line.split(":")
        if len(parts) < 3:
            continue
        try:
            addr = int(parts[0], 0)
            size = int(parts[1])
        except ValueError:
            continue
        name = parts[2]
        yield addr, size, name


def _mask_relocs(buf: bytes) -> bytearray:
    """Return `buf` with JAL/BAL target immediates zeroed so unresolved
    call sites don't dominate the diff. MIPS little-endian encoding:
    a 32-bit instruction `[opcode:6][target:26]` lives in 4 bytes where
    opcode bits 31-26 end up in byte index 3 (high 6 bits). JAL=0x03,
    JALX=0x1D, J=0x02. We mask when opcode == 0x02 or 0x03 by clearing
    the low 26 bits across bytes [0..3]."""
    out = bytearray(buf)
    n = len(out) & ~3
    for i in range(0, n, 4):
        op = (out[i + 3] >> 2) & 0x3F
        if op == 0x02 or op == 0x03:  # J, JAL
            out[i] = 0
            out[i + 1] = 0
            out[i + 2] = 0
            out[i + 3] &= 0xFC  # preserve opcode bits 31-26
    return out


def diff_overlay(name: str, details: bool = False, ignore_relocs: bool = False):
    ref = REF_DIR / f"{name}.EXN"
    built = BUILT_DIR / name / f"{name}.EXN"
    sym = SYM_DIR / f"{name}.sym"
    if not ref.exists():
        print(f"[{name}] SKIP: reference missing ({ref})")
        return None
    if not built.exists():
        print(f"[{name}] SKIP: built missing ({built}) — run build_overlay.py first")
        return None
    if not sym.exists():
        print(f"[{name}] SKIP: sym missing ({sym})")
        return None

    ref_bytes = ref.read_bytes()
    built_bytes = built.read_bytes()

    matched = 0
    diffed = 0
    size_bad = 0
    missing = 0
    total = 0
    first_diffs = []

    for addr, size, fname in parse_sym(sym):
        total += 1
        off = addr - BASE_ADDR
        if off + size > len(ref_bytes):
            missing += 1
            if details:
                print(f"  [{fname}] MISSING in reference (offset {off:#x} size {size})")
            continue
        if off + size > len(built_bytes):
            size_bad += 1
            if details:
                print(f"  [{fname}] SIZE: built file shorter than function end")
            continue
        r = ref_bytes[off:off + size]
        b = built_bytes[off:off + size]
        if ignore_relocs:
            r = bytes(_mask_relocs(r))
            b = bytes(_mask_relocs(b))
        if r == b:
            matched += 1
            continue
        # Count mismatch bytes + find first mismatch offset within the function
        mismatches = sum(1 for i in range(size) if r[i] != b[i])
        first_off = next(i for i in range(size) if r[i] != b[i])
        diffed += 1
        if len(first_diffs) < 5:
            first_diffs.append((fname, addr + first_off, mismatches, size))
        if details:
            print(f"  [{fname}] DIFF {mismatches}/{size} bytes, first @ {addr + first_off:#x}")

    status = "OK" if (diffed == 0 and size_bad == 0 and missing == 0) else "DIFF"
    print(f"[{name}] {status}: {matched}/{total} match"
          + (f", {diffed} diff" if diffed else "")
          + (f", {size_bad} size" if size_bad else "")
          + (f", {missing} missing" if missing else ""))
    if first_diffs and not details:
        for fname, at, mm, sz in first_diffs[:3]:
            print(f"    first diff: {fname} @ {at:#x} ({mm}/{sz} bytes)")
    return {"name": name, "matched": matched, "diffed": diffed,
            "size_bad": size_bad, "missing": missing, "total": total}


def discover_overlays():
    return sorted(p.stem for p in REF_DIR.glob("*.EXN"))


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("names", nargs="*", help="Overlay names (e.g. GEKO). Empty + --all to scan every overlay.")
    ap.add_argument("--all", action="store_true")
    ap.add_argument("--details", action="store_true", help="Per-function diff lines")
    ap.add_argument("--summary-only", action="store_true")
    ap.add_argument("--ignore-relocs", action="store_true",
                    help="Mask J/JAL target immediates before comparing — "
                         "shows true byte divergence beyond linker relocations.")
    args = ap.parse_args()

    if args.all or not args.names:
        names = discover_overlays()
    else:
        names = args.names

    results = []
    for n in names:
        r = diff_overlay(n, details=args.details,
                         ignore_relocs=args.ignore_relocs)
        if r is not None:
            results.append(r)

    if results:
        print()
        total_funcs = sum(r["total"] for r in results)
        total_match = sum(r["matched"] for r in results)
        total_diff = sum(r["diffed"] for r in results)
        total_size = sum(r["size_bad"] for r in results)
        total_missing = sum(r["missing"] for r in results)
        pct = 100.0 * total_match / total_funcs if total_funcs else 0.0
        print(f"[overlay_diff] {total_match}/{total_funcs} functions byte-perfect ({pct:.1f}%)"
              f"  diff={total_diff} size={total_size} missing={total_missing}")

    return 0 if all(r["diffed"] == 0 and r["size_bad"] == 0 and r["missing"] == 0
                    for r in results) else 1


if __name__ == "__main__":
    sys.exit(main())
