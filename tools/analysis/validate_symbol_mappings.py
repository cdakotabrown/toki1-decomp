#!/usr/bin/env python3
"""
Validate that every named C file's address matches a known symbol
within the expected offset threshold.

Usage:
    python3 tools/analysis/validate_symbol_mappings.py [--strict]
"""

import json
import re
import sys
from pathlib import Path
from bisect import bisect_left

BASE_DIR = Path(__file__).parent.parent.parent
SYMBOL_MAP = BASE_DIR / "build/slps00065/symbols/symbol_map.json"
SRC_DIRS = [
    BASE_DIR / "src/main/matched",
    BASE_DIR / "src/main/semi-matched",
    BASE_DIR / "src/main/wip",
]


def load_symbols():
    with open(SYMBOL_MAP) as f:
        data = json.load(f)
    syms = []
    for s in data["symbols"]:
        syms.append((int(s["addr"], 16), s["name"]))
    return sorted(syms)


def extract_address_from_header(filepath):
    try:
        with open(filepath) as f:
            header = f.read(2000)
        m = re.search(r'Address:\s*(0x[0-9A-Fa-f]{8})', header)
        if m:
            return int(m.group(1), 16)
    except Exception:
        pass
    return None


def find_nearest(addr, symbols):
    addrs = [s[0] for s in symbols]
    idx = bisect_left(addrs, addr)
    best = None
    for i in range(max(0, idx - 2), min(len(symbols), idx + 2)):
        dist = abs(addr - symbols[i][0])
        if best is None or dist < best[0]:
            best = (dist, symbols[i][1], symbols[i][0])
    return best


def main():
    strict = "--strict" in sys.argv
    symbols = load_symbols()

    issues = []
    ok = 0

    for src_dir in SRC_DIRS:
        if not src_dir.exists():
            continue
        for f in sorted(src_dir.glob("*.c")):
            stem = f.stem
            # Skip func_8* files (unnamed)
            if re.match(r'func_8[0-9A-Fa-f]{7}$', stem, re.IGNORECASE):
                continue
            if re.match(r'FUN_8[0-9A-Fa-f]{7}$', stem, re.IGNORECASE):
                continue

            addr = extract_address_from_header(f)
            if addr is None:
                issues.append(("NO_ADDR", str(f.relative_to(BASE_DIR)), stem, "No address in header"))
                continue

            result = find_nearest(addr, symbols)
            if result is None:
                issues.append(("NO_SYMBOL", str(f.relative_to(BASE_DIR)), stem, "No symbol found"))
                continue

            dist, sym_name, sym_addr = result

            if sym_name == stem:
                if dist == 0:
                    ok += 1
                elif dist <= 16:
                    ok += 1
                elif dist <= 64:
                    if strict:
                        issues.append(("MEDIUM", str(f.relative_to(BASE_DIR)), stem,
                                       f"offset={dist}, sym_addr=0x{sym_addr:08X}"))
                    else:
                        ok += 1
                elif dist <= 128:
                    issues.append(("LOW", str(f.relative_to(BASE_DIR)), stem,
                                   f"offset={dist}, sym_addr=0x{sym_addr:08X}"))
                else:
                    issues.append(("REJECT", str(f.relative_to(BASE_DIR)), stem,
                                   f"offset={dist}, sym_addr=0x{sym_addr:08X}"))
            else:
                issues.append(("NAME_MISMATCH", str(f.relative_to(BASE_DIR)), stem,
                               f"nearest sym={sym_name} at offset {dist}"))

    print(f"Validated: {ok} OK")
    if issues:
        print(f"Issues: {len(issues)}")
        for severity, path, name, note in sorted(issues):
            print(f"  [{severity}] {name}: {note}")
            print(f"    {path}")
    return 1 if issues and strict else 0


if __name__ == "__main__":
    sys.exit(main())
