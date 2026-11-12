#!/usr/bin/env python3
"""
Audit all named (non-func_8*) C files against symbol_map.json.
Outputs audit_results.json with confidence scores for every named function.

Usage:
    python3 tools/analysis/audit_named_functions.py
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
UNMATCHABLE = BASE_DIR / "config/c_unmatchable.txt"


def load_symbols():
    with open(SYMBOL_MAP) as f:
        data = json.load(f)
    symbols = sorted([(int(s["addr"], 16), s["name"]) for s in data["symbols"]])
    sym_by_name = {s["name"]: int(s["addr"], 16) for s in data["symbols"]}
    return symbols, sym_by_name


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


def extract_address_from_name(name):
    """Try to extract address from func_8XXXXXXX naming in unmatchable config."""
    m = re.match(r'(?:func_)?(8[0-9A-Fa-f]{7})$', name, re.IGNORECASE)
    if m:
        return int(m.group(1), 16)
    return None


def find_nearest(addr, symbols):
    addrs = [s[0] for s in symbols]
    idx = bisect_left(addrs, addr)
    best = None
    for i in range(max(0, idx - 2), min(len(symbols), idx + 2)):
        d = abs(addr - symbols[i][0])
        if best is None or d < best[0]:
            best = (d, symbols[i][1], symbols[i][0])
    return best


def classify_tier(abs_offset):
    if abs_offset == 0:
        return "EXACT"
    elif abs_offset <= 16:
        return "HIGH"
    elif abs_offset <= 64:
        return "MEDIUM"
    elif abs_offset <= 128:
        return "LOW"
    else:
        return "REJECT"


def main():
    symbols, sym_by_name = load_symbols()
    results = []

    # Collect all named files from source directories
    named_files = []
    for src_dir in SRC_DIRS:
        if not src_dir.exists():
            continue
        for f in sorted(src_dir.glob("*.c")):
            stem = f.stem
            if re.match(r'(?:func|FUN)_8[0-9A-Fa-f]{7}$', stem, re.IGNORECASE):
                continue
            named_files.append(f)

    # Also check named entries in c_unmatchable.txt
    unmatchable_names = set()
    if UNMATCHABLE.exists():
        with open(UNMATCHABLE) as f:
            for line in f:
                line = line.strip()
                if not line or line.startswith('#'):
                    continue
                name = line.split()[0]
                if not re.match(r'(?:func|FUN)_8[0-9A-Fa-f]{7}$', name, re.IGNORECASE):
                    unmatchable_names.add(name)

    print(f"Auditing {len(named_files)} named source files + {len(unmatchable_names)} named unmatchable entries")
    print()

    for f in named_files:
        stem = f.stem
        addr = extract_address_from_header(f)
        if addr is None:
            results.append({
                "name": stem,
                "file": str(f.relative_to(BASE_DIR)),
                "status": "NO_ADDRESS",
                "action": "investigate",
            })
            continue

        if stem in sym_by_name:
            sym_addr = sym_by_name[stem]
            offset = addr - sym_addr
            abs_off = abs(offset)
            tier = classify_tier(abs_off)
            action = "keep" if tier in ("EXACT", "HIGH", "MEDIUM") else "investigate"
            results.append({
                "name": stem,
                "file": str(f.relative_to(BASE_DIR)),
                "address": f"0x{addr:08X}",
                "sym_addr": f"0x{sym_addr:08X}",
                "offset": offset,
                "tier": tier,
                "source": "symbol_map",
                "action": action,
            })
        else:
            nearest = find_nearest(addr, symbols)
            results.append({
                "name": stem,
                "file": str(f.relative_to(BASE_DIR)),
                "address": f"0x{addr:08X}",
                "nearest_sym": nearest[1] if nearest else None,
                "nearest_addr": f"0x{nearest[2]:08X}" if nearest else None,
                "nearest_offset": nearest[0] if nearest else None,
                "tier": "SDK_OR_MANUAL",
                "source": "not_in_symbol_map",
                "action": "keep",
            })

    # Write results
    out_path = BASE_DIR / "build/symbol_audit/audit_results.json"
    out_path.parent.mkdir(parents=True, exist_ok=True)
    with open(out_path, "w") as f:
        json.dump({"total": len(results), "results": results}, f, indent=2)

    # Summary
    tiers = {}
    for r in results:
        t = r.get("tier", "UNKNOWN")
        tiers[t] = tiers.get(t, 0) + 1

    print("Audit Results:")
    for t in ["EXACT", "HIGH", "MEDIUM", "LOW", "REJECT", "SDK_OR_MANUAL", "NO_ADDRESS"]:
        if t in tiers:
            print(f"  {t}: {tiers[t]}")
    print(f"  Total named files: {len(results)}")

    # Flag problematic entries
    flagged = [r for r in results if r.get("action") == "investigate"]
    if flagged:
        print(f"\nFLAGGED for investigation ({len(flagged)}):")
        for r in flagged:
            if "offset" in r:
                print(f"  {r['name']}: offset {r['offset']}, tier {r['tier']}")
                print(f"    file: {r['file']}")
                print(f"    func addr: {r['address']}, sym addr: {r['sym_addr']}")
            else:
                print(f"  {r['name']}: {r.get('status', 'unknown')}")
                print(f"    file: {r['file']}")

    print(f"\nWrote: {out_path}")


if __name__ == "__main__":
    main()
