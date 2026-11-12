#!/usr/bin/env python3
"""Audit overlay .sym sizes for cross-prologue/data tails.

For every function in the requested status set, scan the target EXN bytes
for the LAST `jr $rs` instruction (the function's epilogue, whether
`jr $ra` or `jr $tN` jumptable dispatcher), include its delay slot, count
trailing nops, and report the gap between that boundary and the .sym
declared size.

When the gap is small (typically 4-32 bytes) it usually means the .sym
inflated the function with the cross-prologue tail of the next function
or trailing jump-table `.word` entries. Emitting `@SymbolSize: <actual>`
in the C header clips the target window and may unblock the match.

The script is dry-run by default; pass `--apply` to write annotations
into matching `src/overlay/<OVL>/<func>.c` files.

Usage:
  python3 tools/overlay/audit_symbol_sizes.py
  python3 tools/overlay/audit_symbol_sizes.py GEKO
  python3 tools/overlay/audit_symbol_sizes.py --status count_mismatch
  python3 tools/overlay/audit_symbol_sizes.py --status semi_matched --apply

Tier A6 of the 2026-05-11 blocker brainstorm (kept in the project's private archive).
"""
from __future__ import annotations

import argparse
import json
import re
import struct
import sys
from pathlib import Path
from typing import Dict, List, Optional, Tuple

ROOT = Path(__file__).parent.parent.parent
SRC_OVERLAY_DIR = ROOT / "src" / "overlay"
CONFIG_DIR = ROOT / "config" / "overlay"
ASSETS_DIR = ROOT / "assets" / "overlays"
STATUS_FILE = ROOT / "config" / "overlay_match_status.json"

OVERLAY_BASE = 0x80132000

sys.path.insert(0, str(ROOT / "tools" / "build"))
from parse_postproc import parse_symbol_size_text  # noqa: E402

# --- MIPS-I instruction decode helpers -------------------------------------

def is_jr(word: int) -> bool:
    """True iff word encodes `jr $rs` (any rs)."""
    return (word & 0xFC1FFFFF) == 0x00000008


def jr_rs(word: int) -> int:
    return (word >> 21) & 0x1F


def reg_name(rs: int) -> str:
    names = [
        "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
        "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
        "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
        "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra",
    ]
    return names[rs & 0x1F]


# --- .sym / EXN loaders ---------------------------------------------------

def parse_sym_file(overlay_name: str) -> List[Tuple[int, int, str]]:
    """Returns sorted [(addr, size, name), ...]."""
    sym_file = CONFIG_DIR / f"{overlay_name}.sym"
    if not sym_file.exists():
        return []
    funcs = []
    for line in sym_file.read_text().splitlines():
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        parts = line.split(":")
        if len(parts) < 3:
            continue
        try:
            addr = int(parts[0], 16)
            size = int(parts[1])
        except ValueError:
            continue
        name = parts[2]
        funcs.append((addr, size, name))
    funcs.sort()
    return funcs


def load_exn(overlay_name: str) -> Optional[bytes]:
    path = ASSETS_DIR / f"{overlay_name}.EXN"
    if not path.exists():
        return None
    return path.read_bytes()


# --- Core analysis --------------------------------------------------------

def analyze(
    exn: bytes, addr: int, declared_size: int
) -> Optional[Dict]:
    """Locate function's terminating jump and report gap to declared end.

    Returns dict with:
      actual_size:  jr_pos + 8 + trailing_nop_bytes
      jr_offset:    byte offset of the last `jr` within the function
      jr_rs:        rs register of the last jr
      trailing_nops: number of nop instructions after jr+delay
      gap_bytes:    declared_size - actual_size (>= 0)
      tail_kind:    "none" | "nop_only" | "data" | "code"

    Returns None when the declared range can't be analyzed (out of EXN,
    no jr found, etc.).
    """
    base_offset = addr - OVERLAY_BASE
    if base_offset < 0 or base_offset + declared_size > len(exn) or declared_size < 8:
        return None
    if declared_size % 4:
        return None  # malformed

    last_jr_pos = None
    last_jr_rs = None
    # Walk forward; record the last jr we see. A `jr` cannot be in the final
    # 4 bytes of the function (it needs a delay slot).
    for off in range(0, declared_size - 4, 4):
        word = struct.unpack_from("<I", exn, base_offset + off)[0]
        if is_jr(word):
            last_jr_pos = off
            last_jr_rs = jr_rs(word)

    if last_jr_pos is None:
        return None

    end_after_delay = last_jr_pos + 8
    # Count any 0x00000000 trailing nops past the delay slot.
    nop_words = 0
    cursor = end_after_delay
    while cursor < declared_size:
        word = struct.unpack_from("<I", exn, base_offset + cursor)[0]
        if word == 0:
            nop_words += 1
            cursor += 4
        else:
            break

    actual_size = end_after_delay + nop_words * 4
    gap = declared_size - actual_size

    if gap == 0:
        tail_kind = "none" if nop_words == 0 else "nop_only"
    else:
        # Look at the first non-nop after the trailing-nop run.
        tail_word = struct.unpack_from("<I", exn, base_offset + actual_size)[0]
        # Plausible cross-prologue tail starts with `lui $rd, K` (opcode 0x0F).
        # Plausible jumptable entry is a code pointer in the .text address range.
        opcode = (tail_word >> 26) & 0x3F
        if opcode == 0x0F:
            tail_kind = "code"  # very likely cross-prologue lui
        elif 0x80100000 <= tail_word <= 0x80200000:
            tail_kind = "data"  # jump-table .word pointing into overlay
        else:
            tail_kind = "code"  # any other opcode — treat as code

    return {
        "actual_size": actual_size,
        "jr_offset": last_jr_pos,
        "jr_rs": last_jr_rs,
        "trailing_nops": nop_words,
        "gap_bytes": gap,
        "tail_kind": tail_kind,
    }


# --- Source-file helpers --------------------------------------------------

_SYMBOL_SIZE_RE = re.compile(r"@SymbolSize:\s*(\d+)")
_HEADER_RE = re.compile(r"\A(/\*(?:[^*]|\*(?!/))*\*/)", re.MULTILINE)


def existing_symbol_size(c_text: str) -> Optional[int]:
    """ANCHORED @SymbolSize (a prose mention must not count as a window)."""
    return parse_symbol_size_text(c_text)


def inject_symbol_size(c_text: str, actual_size: int) -> str:
    """Insert `@SymbolSize: N` into the C file's leading block comment.

    If no block comment exists, prepend a new one.
    """
    annot_line = f" * @SymbolSize: {actual_size}\n"
    m = _HEADER_RE.match(c_text)
    if m:
        header = m.group(1)
        # Insert before the closing */
        close_idx = header.rfind("*/")
        new_header = header[:close_idx].rstrip() + "\n" + annot_line + " */"
        return new_header + c_text[m.end() :]
    new_header = f"/*\n{annot_line} */\n"
    return new_header + c_text


# --- Orchestration --------------------------------------------------------

def audit_overlay(
    overlay_name: str,
    status_filter: Optional[set],
    status_map: Dict,
    min_gap: int,
    max_gap: int,
    require_cross_prologue: bool,
) -> List[Dict]:
    funcs = parse_sym_file(overlay_name)
    exn = load_exn(overlay_name)
    if not funcs or exn is None:
        return []

    out = []
    for addr, declared, name in funcs:
        if status_filter is not None:
            status = status_map.get(overlay_name, {}).get(name, {}).get("status")
            if status not in status_filter:
                continue

        info = analyze(exn, addr, declared)
        if info is None:
            continue
        gap = info["gap_bytes"]
        if gap < min_gap or gap > max_gap:
            continue
        if require_cross_prologue and info["tail_kind"] not in ("code", "data"):
            continue

        c_file = SRC_OVERLAY_DIR / overlay_name / f"{name}.c"
        c_exists = c_file.exists()
        existing = None
        if c_exists:
            try:
                existing = existing_symbol_size(c_file.read_text())
            except OSError:
                pass

        out.append({
            "overlay": overlay_name,
            "function": name,
            "addr": addr,
            "declared_size": declared,
            "actual_size": info["actual_size"],
            "gap_bytes": gap,
            "trailing_nops": info["trailing_nops"],
            "terminator": "jr_" + reg_name(info["jr_rs"]),
            "tail_kind": info["tail_kind"],
            "c_exists": c_exists,
            "existing_annotation": existing,
        })
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("overlay", nargs="?", help="restrict to one overlay")
    ap.add_argument(
        "--status",
        action="append",
        choices=["count_mismatch", "semi_matched", "reloc_only", "byte_perfect"],
        help="filter by status (repeatable; default: count_mismatch+semi_matched)",
    )
    ap.add_argument(
        "--min-gap",
        type=int,
        default=4,
        help="minimum declared-vs-actual gap in bytes (default 4)",
    )
    ap.add_argument(
        "--max-gap",
        type=int,
        default=32,
        help="maximum gap to surface (default 32; raise to see jump-table tails)",
    )
    ap.add_argument(
        "--all-tails",
        action="store_true",
        help="include functions whose tail looks like neither code nor data",
    )
    ap.add_argument(
        "--apply",
        action="store_true",
        help="write @SymbolSize annotations into source files (dry-run otherwise)",
    )
    ap.add_argument(
        "--json",
        action="store_true",
        help="emit JSON instead of human-readable table",
    )
    args = ap.parse_args()

    status_filter = set(args.status) if args.status else {"count_mismatch", "semi_matched"}

    status_map: Dict = {}
    if STATUS_FILE.exists():
        try:
            status_map = json.loads(STATUS_FILE.read_text())
        except json.JSONDecodeError:
            print(f"warning: {STATUS_FILE} is not valid JSON", file=sys.stderr)

    if args.overlay:
        overlays = [args.overlay]
    else:
        overlays = sorted(
            d.name for d in SRC_OVERLAY_DIR.iterdir() if d.is_dir() and not d.name.startswith(".")
        )

    all_cands: List[Dict] = []
    for ovl in overlays:
        all_cands.extend(
            audit_overlay(
                ovl,
                status_filter,
                status_map,
                args.min_gap,
                args.max_gap,
                require_cross_prologue=not args.all_tails,
            )
        )

    if args.json:
        print(json.dumps(all_cands, indent=2))
        return 0

    if not all_cands:
        print("No candidates found.")
        return 0

    by_overlay: Dict[str, int] = {}
    by_gap: Dict[int, int] = {}
    by_term: Dict[str, int] = {}
    applied = 0
    skipped_annotated = 0

    print(
        f"{'OVERLAY':<10} {'FUNCTION':<20} {'DECL':>5} {'ACTUAL':>6} "
        f"{'GAP':>4} {'NOPS':>4} {'TERM':<10} {'TAIL':<6} {'NOTE'}"
    )
    for c in all_cands:
        by_overlay[c["overlay"]] = by_overlay.get(c["overlay"], 0) + 1
        by_gap[c["gap_bytes"]] = by_gap.get(c["gap_bytes"], 0) + 1
        by_term[c["terminator"]] = by_term.get(c["terminator"], 0) + 1

        note_parts = []
        if not c["c_exists"]:
            note_parts.append("no-c-file")
        if c["existing_annotation"] is not None:
            note_parts.append(f"existing=@{c['existing_annotation']}")
            skipped_annotated += 1
        note = ",".join(note_parts)

        print(
            f"{c['overlay']:<10} {c['function']:<20} "
            f"{c['declared_size']:>5} {c['actual_size']:>6} "
            f"{c['gap_bytes']:>4} {c['trailing_nops']:>4} "
            f"{c['terminator']:<10} {c['tail_kind']:<6} {note}"
        )

        if args.apply and c["c_exists"] and c["existing_annotation"] is None:
            c_file = SRC_OVERLAY_DIR / c["overlay"] / f"{c['function']}.c"
            try:
                text = c_file.read_text()
                new_text = inject_symbol_size(text, c["actual_size"])
                if new_text != text:
                    c_file.write_text(new_text)
                    applied += 1
            except OSError as e:
                print(f"  ! failed to write {c_file}: {e}", file=sys.stderr)

    print()
    print(f"Total candidates: {len(all_cands)}")
    print(f"By overlay: {dict(sorted(by_overlay.items(), key=lambda x: -x[1])[:10])}")
    print(f"By terminator: {by_term}")
    print(f"By gap size:   {dict(sorted(by_gap.items()))}")
    print(f"Already annotated (skipped): {skipped_annotated}")
    if args.apply:
        print(f"Annotations written: {applied}")
    else:
        print("Dry-run only. Re-run with --apply to write annotations.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
