#!/usr/bin/env python3
"""
fix_sym_boundaries.py - Correct overlay .sym function sizes that over-count
into the next function's hoisted-load instructions.

>>> VALIDATED 2026-06-14: applying this yields ~0 effective-% gain. <<<
The trailing-2 overcount is REAL (1,287 functions, 1,198 by exactly 8 bytes),
but it is NEVER the sole blocker. Measured on ETC: of 71 semi functions that
over-count, ZERO are blocked only by the trailing words (min diff count = 26;
they are broken/stub C with dozens-to-hundreds of real diffs). The genuinely
otherwise-matching functions already carry per-file @SymbolSize / strip_trailing_nop
workarounds (which OVERRIDE this .sym edit anyway, see test_overlay_match.py:220).
Keep this tool only as a correctness/cleanup pass (e.g. if extract_symbols.py is
fixed or @SymbolSize annotations are removed); do NOT expect it to flip semis.

extract_symbols.py systematically attributes the next function's first 1-3
instructions (the global/selector load IDO -O2 hoists ABOVE the next function's
prologue) to the PREVIOUS function's symbol size. This makes the over-counted
function impossible to byte-match (those trailing words are foreign code), so it
is stuck as semi_matched no matter how correct its C is.

Ground truth for where a function ends: its last `jr ra` (0x03e00008) plus the
one delay-slot instruction. This tool clamps a Functions-section symbol's size
to (last_jr_ra + 2 words) ONLY when:
  * the .sym size exceeds that boundary by exactly 4/8/12 bytes,
  * every trailing word past the boundary looks foreign (a next-function
    prologue `addiu sp,sp,-N`, a `jal`, or a `lui $r,0x80xx` selector-load high
    half), and
  * none of the trailing words is itself a `jr ra` (would indicate a merged
    multi-function symbol — left alone).

Clamping can only REMOVE foreign trailing bytes from the comparison window, so it
never turns a real match into a mismatch; it only unblocks the trailing-artifact
class. NOT a binary patch - it edits a config size field, the same value the
@SymbolSize source annotation overrides per-file.

Usage:
    python3 tools/overlay/fix_sym_boundaries.py --overlay ETC [--dry-run]
    python3 tools/overlay/fix_sym_boundaries.py --all [--dry-run]
"""
import argparse
import glob
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
CONFIG_DIR = ROOT / "config" / "overlay"
ASSETS_DIR = ROOT / "assets" / "overlays"
OVERLAY_BASE = 0x80132000
JR_RA = 0x03E00008


def is_foreign(w):
    """True if the word looks like the start of the NEXT function, not padding."""
    # addiu sp,sp,-N  (next prologue)
    if (w & 0xFFFF0000) == 0x27BD0000 and (w & 0x8000):
        return True
    # jal target
    if (w >> 26) == 3:
        return True
    # lui $r, 0x80xx  (hoisted global/selector load high half)
    if (w >> 26) == 0x0F and 0x8000 <= (w & 0xFFFF) <= 0x8017:
        return True
    return False


def true_end_size(exn, addr, size):
    """Return clamped size if the symbol over-counts foreign trailing words,
    else None."""
    off = addr - OVERLAY_BASE
    if off < 0 or off + size > len(exn) or size < 12 or size % 4:
        return None
    words = [struct.unpack("<I", exn[off + i * 4: off + i * 4 + 4])[0]
             for i in range(size // 4)]
    jrs = [i for i, w in enumerate(words) if w == JR_RA]
    if not jrs:
        return None
    last = jrs[-1]
    true_size = (last + 2) * 4
    if true_size >= size:
        return None
    over = size - true_size
    if over not in (4, 8, 12):
        return None
    tail = words[last + 2:]
    if JR_RA in tail:
        return None
    # Everything past the last jr-ra+delay is post-return: it cannot be this
    # function's C output. Require at least one clearly-foreign word (next
    # prologue / jal / hoisted lui) as a sanity signal that this is the next
    # function bleeding in (vs. an oddity we should leave alone).
    if not any(is_foreign(w) for w in tail):
        return None
    return true_size


def process_overlay(ov, dry_run):
    sym_path = CONFIG_DIR / f"{ov}.sym"
    exn_path = ASSETS_DIR / f"{ov}.EXN"
    if not sym_path.exists() or not exn_path.exists():
        return 0
    exn = exn_path.read_bytes()
    lines = sym_path.read_text().splitlines(keepends=True)
    in_funcs = False
    changes = []
    out = []
    for line in lines:
        s = line.strip()
        if s.startswith("# === Functions"):
            in_funcs = True
            out.append(line)
            continue
        if s.startswith("# === Data"):
            in_funcs = False
            out.append(line)
            continue
        m = re.match(r"(0x[0-9A-Fa-f]+):(\d+):(\S+)\s*$", s)
        if not in_funcs or not m:
            out.append(line)
            continue
        addr = int(m.group(1), 16)
        size = int(m.group(2))
        name = m.group(3)
        new_size = true_end_size(exn, addr, size)
        if new_size is not None:
            changes.append((name, size, new_size))
            out.append(f"{m.group(1)}:{new_size}:{name}\n")
        else:
            out.append(line)
    if changes and not dry_run:
        sym_path.write_text("".join(out))
    return changes


def main():
    ap = argparse.ArgumentParser()
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument("--overlay")
    g.add_argument("--all", action="store_true")
    ap.add_argument("--dry-run", action="store_true")
    args = ap.parse_args()

    overlays = ([args.overlay] if args.overlay
                else sorted(p.stem for p in CONFIG_DIR.glob("*.sym")))
    grand = 0
    for ov in overlays:
        changes = process_overlay(ov, args.dry_run)
        if changes:
            grand += len(changes)
            verb = "would clamp" if args.dry_run else "clamped"
            print(f"[{ov}] {verb} {len(changes)} symbol(s):")
            for name, old, new in changes[:6]:
                print(f"    {name}: {old} -> {new} (-{old - new}b)")
            if len(changes) > 6:
                print(f"    ... and {len(changes) - 6} more")
    print(f"\nTotal {'would-clamp' if args.dry_run else 'clamped'}: {grand}")


if __name__ == "__main__":
    main()
