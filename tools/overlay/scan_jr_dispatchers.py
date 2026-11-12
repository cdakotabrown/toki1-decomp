#!/usr/bin/env python3
"""Scan ALL overlays for jr-table dispatchers and classify IN-EXN vs RUNTIME.

Reuses extract_jr_table's dispatcher detector over every function declared in
each overlay's .sym, and buckets by the dispatcher's table base:
  - IN-EXN  (base >= 0x80132000): table is static rodata inside the .EXN.
  - RUNTIME (base  < 0x80132000, e.g. 0x80120000): table is written by the
            overlay loader's relocation pass at load time (not in code/rodata).

Usage:
    python3 tools/overlay/scan_jr_dispatchers.py            # summary table
    python3 tools/overlay/scan_jr_dispatchers.py --runtime  # list RUNTIME funcs
    python3 tools/overlay/scan_jr_dispatchers.py --json
"""
import argparse, glob, json, os, struct, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from extract_jr_table import load_exn, find_dispatcher, TEXT_BASE


def parse_sym(path):
    funcs = []
    for line in open(path):
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        parts = line.split(":")
        if len(parts) < 3:
            continue
        try:
            addr = int(parts[0], 16)
        except ValueError:
            continue
        funcs.append((addr, parts[2]))
    return funcs


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--runtime", action="store_true", help="list RUNTIME dispatchers")
    ap.add_argument("--json", action="store_true")
    args = ap.parse_args()

    rows = []
    runtime_list = []
    for sym in sorted(glob.glob(os.path.join(ROOT, "config", "overlay", "*.sym"))):
        overlay = os.path.basename(sym)[:-4]
        exn = os.path.join(ROOT, "assets", "overlays", f"{overlay}.EXN")
        if not os.path.exists(exn):
            continue
        words, _ = load_exn(overlay)
        funcs = parse_sym(sym)
        in_exn = runtime = 0
        for addr, name in funcs:
            fstart = (addr - TEXT_BASE) // 4
            if fstart < 0 or fstart >= len(words):
                continue
            disp = find_dispatcher(words, fstart)
            if not disp:
                continue
            base = disp["table_base"]
            if base >= TEXT_BASE:
                in_exn += 1
            else:
                runtime += 1
                runtime_list.append({
                    "overlay": overlay, "func": name, "addr": f"0x{addr:08x}",
                    "table_base": f"0x{base:08x}", "bound": disp["bound"]})
        rows.append({"overlay": overlay, "in_exn": in_exn, "runtime": runtime})

    if args.json:
        print(json.dumps({"by_overlay": rows, "runtime": runtime_list}, indent=2))
        return

    if args.runtime:
        for r in runtime_list:
            print(f"{r['overlay']:10} {r['func']:22} {r['addr']} "
                  f"table={r['table_base']} bound={r['bound']}")
        print(f"\nTotal RUNTIME dispatchers: {len(runtime_list)}")
        return

    print(f"{'overlay':12} {'IN-EXN':>7} {'RUNTIME':>8}")
    print("-" * 30)
    tot_in = tot_rt = 0
    for r in sorted(rows, key=lambda x: -x["runtime"]):
        if r["in_exn"] or r["runtime"]:
            print(f"{r['overlay']:12} {r['in_exn']:7} {r['runtime']:8}")
        tot_in += r["in_exn"]
        tot_rt += r["runtime"]
    print("-" * 30)
    print(f"{'TOTAL':12} {tot_in:7} {tot_rt:8}")


if __name__ == "__main__":
    main()
