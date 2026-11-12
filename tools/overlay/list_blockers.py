#!/usr/bin/env python3
"""List functions with documented blockers from overlay_match_status.json"""

import json
import argparse
from pathlib import Path

def main():
    parser = argparse.ArgumentParser(description="List documented overlay blockers")
    parser.add_argument("--type", help="Filter by blocker type (e.g., insert_instruction, cross_prologue)")
    parser.add_argument("--min-pct", type=float, default=0, help="Minimum masked percentage")
    parser.add_argument("--overlay", help="Filter by overlay name")
    args = parser.parse_args()
    
    config_path = Path(__file__).parent.parent.parent / "config" / "overlay_match_status.json"
    with open(config_path) as f:
        data = json.load(f)
    
    results = []
    for overlay, funcs in data.items():
        if args.overlay and overlay != args.overlay:
            continue
        for func, info in funcs.items():
            if "blocker" not in info:
                continue
            bt = info.get("blocker_type", "unknown")
            if args.type and bt != args.type:
                continue
            pct = info.get("reloc_pct", 0)
            if pct < args.min_pct:
                continue
            results.append((pct, overlay, func, bt, info["blocker"]))
    
    results.sort(reverse=True)
    
    print(f"Found {len(results)} blocked functions")
    print("=" * 80)
    for pct, overlay, func, bt, blocker in results:
        print(f"{pct:5.1f}% {overlay}/{func} [{bt}]")
        print(f"       {blocker}")
        print()

if __name__ == "__main__":
    main()
