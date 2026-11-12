#!/usr/bin/env python3
"""
next_stub.py - Rank the next TRACTABLE fresh-stub jr-table dispatchers to decode.

The productive overlay-matching vein is fresh dispatcher stubs: a small .c that is
still semi_matched, whose target is a jr-table dispatch with a static IN-EXN case
table and simple call-per-case bodies. This tool finds and ranks them so the
autonomous loop can just `next_stub.py`, decode the top hit, commit, repeat.

Screening:
  * status is NOT byte_perfect / reloc_only / asm_only (work remains),
  * the .c is a stub: <= MAXLINES lines, or carries a BLOCKER/stub marker,
  * extract_jr_table reports strategy=IN-EXN with >= 4 cases (a real dispatch
    whose table we can read statically),
  * the target body contains NO multu/div (strength-reduction blocker) and
    exactly one `jr ra` (no merged multi-function symbol).
Ranking: fewer .c lines first (closer to a bare stub), then more cases.

RUNTIME-table dispatchers (base 0x80120000, ~10 total) are skipped: their tables
are loader-relocated and only recoverable from a ram.bin dump.

Usage:
    python3 tools/overlay/next_stub.py [--count 10] [--overlay ETC] [--maxlines 120]
"""
import argparse
import glob
import json
import os
import re
import struct
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SRC = ROOT / "src" / "overlay"
ASSETS = ROOT / "assets" / "overlays"
STATUS = ROOT / "config" / "overlay_match_status.json"
BASE = 0x80132000
MULTU_DIV = {0x18, 0x19, 0x1A, 0x1B}  # special funct: mult/multu/div/divu
JR_RA = 0x03E00008


def load_status():
    try:
        return json.load(open(STATUS))
    except Exception:
        return {}


def status_of(status, ov, fn):
    d = status.get(ov, {})
    e = d.get(fn) if isinstance(d, dict) else None
    return (e.get("status") if isinstance(e, dict) else e)


def body_words(ov, addr, size):
    exn = ASSETS / f"{ov}.EXN"
    if not exn.exists():
        return None
    data = exn.read_bytes()
    off = addr - BASE
    if off < 0 or off + size > len(data):
        return None
    return [struct.unpack("<I", data[off + i * 4: off + i * 4 + 4])[0]
            for i in range(size // 4)]


def has_multu_div(words):
    for w in words:
        if (w >> 26) == 0 and (w & 0x3F) in MULTU_DIV:
            return True
    return False


def sym_size(ov, fn):
    symf = ROOT / "config" / "overlay" / f"{ov}.sym"
    if not symf.exists():
        return None
    for line in open(symf):
        m = re.match(rf"(0x[0-9A-Fa-f]+):(\d+):{re.escape(fn)}\s*$", line.strip())
        if m:
            return int(m.group(1), 16), int(m.group(2))
    return None


def probe(ov, fn):
    """Return dict with cases / lines / flags, or None if not a tractable stub."""
    r = subprocess.run(
        ["python3", str(ROOT / "tools/overlay/extract_jr_table.py"), ov, fn],
        capture_output=True, text=True)
    if "strategy=IN-EXN" not in r.stdout:
        return None
    cases = r.stdout.count("  case ")
    if cases < 4:
        return None
    info = sym_size(ov, fn)
    if not info:
        return None
    addr, size = info
    words = body_words(ov, addr, size)
    if not words:
        return None
    if has_multu_div(words):
        return None
    if sum(1 for w in words if w == JR_RA) != 1:
        return None
    return {"cases": cases}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--count", type=int, default=10)
    ap.add_argument("--overlay")
    ap.add_argument("--maxlines", type=int, default=120)
    args = ap.parse_args()

    status = load_status()
    stub_re = re.compile(r"BLOCKER|[Ss]tub|actual implementation|jump table", re.I)
    cands = []
    pat = f"{args.overlay}/*.c" if args.overlay else "*/*.c"
    for f in glob.glob(str(SRC / pat)):
        ov = Path(f).parent.name
        fn = Path(f).stem
        st = status_of(status, ov, fn)
        if st in ("byte_perfect", "reloc_only", "asm_only"):
            continue
        txt = open(f, errors="ignore").read()
        nlines = txt.count("\n") + 1
        is_stub = nlines <= args.maxlines or bool(stub_re.search(txt))
        if not is_stub:
            continue
        npp = txt.count("@PostProc")
        info = probe(ov, fn)
        if not info:
            continue
        cands.append((nlines, -info["cases"], ov, fn, info["cases"], npp, st))

    cands.sort()
    print(f"{'OVERLAY/FUNC':<34} {'cases':>5} {'lines':>5} {'pp':>3} status")
    print("-" * 66)
    for nlines, _, ov, fn, cases, npp, st in cands[:args.count]:
        print(f"{ov+'/'+fn:<34} {cases:>5} {nlines:>5} {npp:>3} {st}")
    print(f"\n{len(cands)} tractable fresh-stub dispatchers found"
          f"{' in '+args.overlay if args.overlay else ''}.")


if __name__ == "__main__":
    main()
