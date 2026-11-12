#!/usr/bin/env python3
"""Scan all semi-matched overlay functions for A2/A4 PostProc candidates.

A2 (convert_addiu_to_slti):
  target  : `slti rt, rs, K; b{ne,eq} rt, $zero, dest`
  compiled: `addiu rt, $zero, K; b{ne,eq} rs, rt, dest`

A4 (unfold_dead_or_move):
  target  : `or rd, rs, $zero` (any rs, rd != 0)
  compiled: `nop` at the same offset

For each semi-matched function, this compiles the existing C through the
full PostProc pipeline, extracts the .text bytes, and compares each
instruction against the target EXN bytes at the equivalent offset. When
the (compiled, target) pair matches one of the A2/A4 patterns, it emits a
candidate row.

Usage:
    python3 tools/overlay/scan_a2_a4_candidates.py
    python3 tools/overlay/scan_a2_a4_candidates.py --parallel 8
    python3 tools/overlay/scan_a2_a4_candidates.py --pattern a2
    python3 tools/overlay/scan_a2_a4_candidates.py --json
"""
from __future__ import annotations

import argparse
import json
import struct
import sys
from concurrent.futures import ProcessPoolExecutor, as_completed
from pathlib import Path
from typing import Dict, List, Optional, Tuple

ROOT = Path(__file__).parent.parent.parent
sys.path.insert(0, str(ROOT / "tools" / "overlay"))
sys.path.insert(0, str(ROOT / "tools" / "build"))

from test_overlay_match import test_overlay_function  # noqa: E402

SRC_OVERLAY_DIR = ROOT / "src" / "overlay"
CONFIG_DIR = ROOT / "config" / "overlay"
ASSETS_DIR = ROOT / "assets" / "overlays"
STATUS_FILE = ROOT / "config" / "overlay_match_status.json"

OVERLAY_BASE = 0x80132000

OP_SPECIAL = 0x00
OP_ADDIU = 0x09
OP_SLTI = 0x0A
OP_BEQ = 0x04
OP_BNE = 0x05
FUNC_OR = 0x25


def decode_iform(word: int) -> Tuple[int, int, int, int]:
    op = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    imm = word & 0xFFFF
    return op, rs, rt, imm


def is_addiu_zero_const(word: int) -> Optional[Tuple[int, int]]:
    """If word is `addiu rt, $zero, K` with K small (>0), return (rt, K)."""
    op, rs, rt, imm = decode_iform(word)
    if op != OP_ADDIU or rs != 0:
        return None
    if imm == 0 or imm >= 0x8000:
        return None
    return rt, imm


def is_slti_form(word: int) -> Optional[Tuple[int, int, int]]:
    """If word is `slti rt, rs, K`, return (rt, rs, K)."""
    op, rs, rt, imm = decode_iform(word)
    if op != OP_SLTI:
        return None
    if imm == 0 or imm >= 0x8000:
        return None
    return rt, rs, imm


def is_branch_with_reg_zero(word: int) -> Optional[Tuple[int, int]]:
    """If word is `b{ne,eq} rt, $zero, off` or `b{ne,eq} $zero, rt, off`,
    return (op, rt)."""
    op, rs, rt, _ = decode_iform(word)
    if op not in (OP_BNE, OP_BEQ):
        return None
    if rs == 0 and rt != 0:
        return op, rt
    if rt == 0 and rs != 0:
        return op, rs
    return None


def is_branch_with_two_regs(word: int) -> Optional[Tuple[int, int, int]]:
    """If word is `b{ne,eq} rs, rt, off` with both rs, rt non-zero,
    return (op, rs, rt)."""
    op, rs, rt, _ = decode_iform(word)
    if op not in (OP_BNE, OP_BEQ):
        return None
    if rs == 0 or rt == 0:
        return None
    return op, rs, rt


def is_or_with_zero(word: int) -> Optional[Tuple[int, int]]:
    """If word is `or rd, rs, $zero` (rd != 0, rs != 0), return (rs, rd)."""
    op = (word >> 26) & 0x3F
    if op != OP_SPECIAL:
        return None
    func = word & 0x3F
    if func != FUNC_OR:
        return None
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    rd = (word >> 11) & 0x1F
    if rt != 0 or rd == 0 or rs == 0:
        return None
    return rs, rd


def parse_sym(ovl: str) -> Dict[str, Tuple[int, int]]:
    p = CONFIG_DIR / f"{ovl}.sym"
    if not p.exists():
        return {}
    out: Dict[str, Tuple[int, int]] = {}
    for line in p.read_text().splitlines():
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        parts = line.split(":")
        if len(parts) < 3:
            continue
        try:
            out[parts[2]] = (int(parts[0], 16), int(parts[1]))
        except ValueError:
            continue
    return out


def load_exn(ovl: str) -> Optional[bytes]:
    p = ASSETS_DIR / f"{ovl}.EXN"
    return p.read_bytes() if p.exists() else None


def find_a2_a4(
    compiled_bytes: bytes, target_bytes: bytes, max_len: int
) -> Tuple[List[Dict], List[Dict]]:
    """Returns (a2_hits, a4_hits)."""
    a2: List[Dict] = []
    a4: List[Dict] = []

    n = min(len(compiled_bytes), len(target_bytes), max_len)
    if n < 8:
        return a2, a4

    for off in range(0, n - 4, 4):
        cw = struct.unpack_from("<I", compiled_bytes, off)[0]
        tw = struct.unpack_from("<I", target_bytes, off)[0]

        # A4 — target=or rd,rs,zero, compiled=nop
        if cw == 0:
            ro = is_or_with_zero(tw)
            if ro is not None:
                rs, rd = ro
                a4.append({"offset": off, "rs": rs, "rd": rd})

        # A2 — needs a 2-insn window (off+0 and off+4 within range)
        if off + 8 <= n:
            cw2 = struct.unpack_from("<I", compiled_bytes, off + 4)[0]
            tw2 = struct.unpack_from("<I", target_bytes, off + 4)[0]

            c_addiu = is_addiu_zero_const(cw)
            c_branch = is_branch_with_two_regs(cw2)
            t_slti = is_slti_form(tw)
            t_branch = is_branch_with_reg_zero(tw2)

            if (
                c_addiu is not None
                and c_branch is not None
                and t_slti is not None
                and t_branch is not None
            ):
                c_rt, c_K = c_addiu
                c_op, c_rs, c_rt2 = c_branch
                t_rt, t_rs, t_K = t_slti
                t_op, t_rt2 = t_branch
                # Verify the pairs match — same rt for addiu/slti, same rs
                # for the compare, immediate matches.
                if (
                    c_rt == t_rt
                    and c_K == t_K
                    and c_op == t_op
                    and (c_rs == t_rs)
                    and (c_rt2 == c_rt)
                    and (t_rt2 == t_rt)
                ):
                    a2.append({"offset": off, "rt": c_rt, "rs": c_rs, "K": c_K})

    return a2, a4


def scan_one(arg: Tuple[str, str]) -> Optional[Dict]:
    ovl, fn = arg
    sym = parse_sym(ovl)
    if fn not in sym:
        return None
    addr, decl_size = sym[fn]
    exn = load_exn(ovl)
    if exn is None:
        return None
    base_off = addr - OVERLAY_BASE
    if base_off < 0 or base_off + decl_size > len(exn):
        return None
    target_bytes = exn[base_off : base_off + decl_size]

    try:
        result = test_overlay_function(ovl, fn, verbose=False, return_bytes=True)
    except Exception:
        return None
    if not result or "compiled_bytes" not in result:
        return None
    compiled_bytes = result["compiled_bytes"]
    if not compiled_bytes:
        return None

    a2, a4 = find_a2_a4(compiled_bytes, target_bytes, min(decl_size, len(compiled_bytes)))
    if not a2 and not a4:
        return None
    return {
        "overlay": ovl,
        "function": fn,
        "size": decl_size,
        "a2": a2,
        "a4": a4,
    }


def main() -> int:
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument(
        "--pattern",
        choices=["a2", "a4", "both"],
        default="both",
        help="filter results to one pattern (default: both)",
    )
    p.add_argument("--parallel", type=int, default=4, help="worker count (default 4)")
    p.add_argument("--json", action="store_true", help="emit JSON")
    p.add_argument("--limit", type=int, default=0, help="probe at most N candidates (0=all)")
    p.add_argument(
        "--min-reloc",
        type=float,
        default=30.0,
        help="only consider semi_matched with reloc%% >= threshold (default 30)",
    )
    args = p.parse_args()

    if not STATUS_FILE.exists():
        print("status file missing — run verify_overlay_bytes first", file=sys.stderr)
        return 1
    status = json.loads(STATUS_FILE.read_text())

    targets: List[Tuple[str, str]] = []
    for ovl, funcs in status.items():
        for fn, info in funcs.items():
            if info.get("status") != "semi_matched":
                continue
            if info.get("reloc_pct", 0) < args.min_reloc:
                continue
            targets.append((ovl, fn))

    if args.limit:
        targets = targets[: args.limit]

    print(f"Scanning {len(targets)} semi_matched functions in parallel "
          f"(workers={args.parallel})...", file=sys.stderr)

    hits: List[Dict] = []
    with ProcessPoolExecutor(max_workers=args.parallel) as ex:
        futures = {ex.submit(scan_one, t): t for t in targets}
        done = 0
        for f in as_completed(futures):
            done += 1
            if done % 50 == 0:
                print(f"  scanned {done}/{len(targets)}...", file=sys.stderr)
            try:
                r = f.result()
            except Exception as e:
                print(f"  worker error on {futures[f]}: {e}", file=sys.stderr)
                continue
            if r is None:
                continue
            if args.pattern == "a2" and not r["a2"]:
                continue
            if args.pattern == "a4" and not r["a4"]:
                continue
            hits.append(r)

    if args.json:
        print(json.dumps(hits, indent=2))
        return 0

    a2_count = sum(len(r["a2"]) for r in hits)
    a4_count = sum(len(r["a4"]) for r in hits)
    print(f"\nFound {len(hits)} functions with A2/A4 candidates "
          f"({a2_count} A2 sites, {a4_count} A4 sites)\n")

    for r in sorted(hits, key=lambda x: (-len(x["a2"]) - len(x["a4"]), x["overlay"])):
        print(f"{r['overlay']:<10} {r['function']:<22} size={r['size']:>4}")
        for h in r["a2"]:
            print(
                f"  A2 @ 0x{h['offset']:04x}: addiu rt={h['rt']}, K={h['K']}, rs={h['rs']} -> slti"
            )
        for h in r["a4"]:
            print(f"  A4 @ 0x{h['offset']:04x}: nop -> or rd={h['rd']}, rs={h['rs']}, zero")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
