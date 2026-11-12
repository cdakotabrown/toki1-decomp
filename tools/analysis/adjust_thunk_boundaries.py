#!/usr/bin/env python3
"""
Adjust Function Boundaries for Thunk Patterns

For functions with thunks, the true entry point is the thunk address,
not the prologue address. This script outputs adjusted boundaries and
optional symbol override mappings.

Usage:
    python3 tools/analysis/adjust_thunk_boundaries.py \
        --thunks build/thunks.json \
        --function-map build/function_analysis/function_map.json \
        --output build/thunk_boundary_adjustments.json \
        --symbol-overrides build/thunk_symbol_overrides.txt
"""

import argparse
import json
from pathlib import Path
from typing import Dict, List


def load_json(path: Path) -> dict:
    if not path.exists():
        raise FileNotFoundError(f"File not found: {path}")
    with path.open() as f:
        return json.load(f)


def load_function_map(path: Path) -> Dict[str, dict]:
    if not path.exists():
        return {}
    data = load_json(path)
    return {f["vram_start"]: f for f in data.get("functions", [])}


def collect_thunks(thunk_data: dict, include_both: bool) -> List[dict]:
    thunks = []
    thunk_groups = thunk_data.get("thunks", {})
    thunks.extend(thunk_groups.get("thunk_called", []))
    if include_both:
        thunks.extend(thunk_groups.get("both_called", []))
    return thunks


def build_adjustments(thunks: List[dict], functions: Dict[str, dict]) -> List[dict]:
    adjustments = []
    for thunk in thunks:
        thunk_addr = thunk["thunk_address"]
        func_addr = thunk["function_address"]
        thunk_size = thunk["thunk_size"]

        orig_func = functions.get(func_addr, {})
        orig_end = orig_func.get("vram_end", "unknown")
        orig_size = orig_func.get("size", 0)

        adjustments.append({
            "original_name": f"func_{int(func_addr, 16):08X}",
            "adjusted_name": f"func_{int(thunk_addr, 16):08X}",
            "thunk_address": thunk_addr,
            "prologue_address": func_addr,
            "function_end": orig_end,
            "thunk_size": thunk_size,
            "adjusted_size": orig_size + thunk_size if orig_size else "unknown",
            "pattern": thunk.get("pattern_type", "unknown"),
            "caller_count": len(thunk.get("thunk_callers", [])),
            "function_callers": len(thunk.get("function_callers", [])),
        })

    return adjustments


def write_symbol_overrides(path: Path, adjustments: List[dict]) -> None:
    lines = [
        "# Thunk symbol override map",
        "# Format: original_name adjusted_name thunk_address prologue_address",
    ]
    for adj in adjustments:
        lines.append(
            f"{adj['original_name']} {adj['adjusted_name']} "
            f"{adj['thunk_address']} {adj['prologue_address']}"
        )

    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text("\n".join(lines) + "\n")


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Adjust function boundaries to include thunks"
    )
    parser.add_argument(
        "--thunks",
        default="build/thunks.json",
        help="Thunk analysis JSON (from detect_thunks.py --verify-calls)",
    )
    parser.add_argument(
        "--function-map",
        default="build/function_analysis/function_map.json",
        help="Function map JSON (optional)",
    )
    parser.add_argument(
        "--output",
        default="build/thunk_boundary_adjustments.json",
        help="Output JSON file",
    )
    parser.add_argument(
        "--symbol-overrides",
        default="build/thunk_symbol_overrides.txt",
        help="Output symbol override mapping",
    )
    parser.add_argument(
        "--include-both",
        action="store_true",
        help="Include thunks where both thunk and function are called",
    )

    args = parser.parse_args()

    thunk_path = Path(args.thunks)
    func_map_path = Path(args.function_map)

    thunk_data = load_json(thunk_path)
    functions = load_function_map(func_map_path)

    thunks = collect_thunks(thunk_data, include_both=args.include_both)
    adjustments = build_adjustments(thunks, functions)

    output = {
        "metadata": {
            "description": "Functions with adjusted boundaries to include thunks",
            "count": len(adjustments),
            "source": str(thunk_path),
            "function_map": str(func_map_path) if func_map_path.exists() else "missing",
            "include_both": args.include_both,
        },
        "adjustments": adjustments,
    }

    output_path = Path(args.output)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(json.dumps(output, indent=2) + "\n")

    symbol_override_path = Path(args.symbol_overrides)
    write_symbol_overrides(symbol_override_path, adjustments)

    print(f"Wrote {len(adjustments)} boundary adjustments to {output_path}")
    print(f"Wrote symbol overrides to {symbol_override_path}")

    if adjustments:
        print("\n=== Sample Adjustments ===")
        for adj in adjustments[:10]:
            print(
                f"  {adj['original_name']} -> {adj['adjusted_name']} "
                f"(thunk {adj['thunk_address']}, size {adj['thunk_size']} bytes)"
            )
        if len(adjustments) > 10:
            print(f"  ... and {len(adjustments) - 10} more")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
