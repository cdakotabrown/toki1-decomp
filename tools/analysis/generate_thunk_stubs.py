#!/usr/bin/env python3
"""
Generate ASM thunk stubs from detect_thunks.py output.

This creates per-thunk assembly stubs and a metadata file for downstream
linker ordering. Supports two modes:

- fallthrough: emit only thunk instructions (expects body placed immediately after)
- jump: append a jump to body symbol (adds 8 bytes)

Usage:
    python3 tools/analysis/generate_thunk_stubs.py \
        --thunks build/thunks.json \
        --output-dir asm/thunks \
        --metadata-out build/thunk_stub_metadata.json
"""

import argparse
import json
from pathlib import Path
from typing import Iterable, List


def load_json(path: Path) -> dict:
    if not path.exists():
        raise FileNotFoundError(f"File not found: {path}")
    with path.open() as f:
        return json.load(f)


def extract_instr(line: str) -> str:
    # line format: "0x80046730: lui t6, 0x8012"
    if ":" not in line:
        return line.strip()
    return line.split(":", 1)[1].strip()


def format_stub(
    thunk_name: str,
    thunk_addr: str,
    func_addr: str,
    pattern: str,
    instructions: Iterable[str],
    mode: str,
    body_symbol: str,
) -> str:
    header = [
        f"/* Auto-generated thunk stub",
        f" * Thunk: {thunk_name} @ {thunk_addr}",
        f" * Body:  {body_symbol} (prologue {func_addr})",
        f" * Pattern: {pattern}",
        f" * Mode: {mode}",
        " */",
        "",
        ".set noreorder",
        ".set noat",
        f".section .text.{thunk_name}_thunk, \"ax\"",
        f".globl {thunk_name}",
        f"{thunk_name}:",
    ]

    body = [f"    {instr}" for instr in instructions if instr]

    if mode == "jump":
        body.append(f"    j {body_symbol}")
        body.append("    nop")

    return "\n".join(header + body) + "\n"


def collect_thunks(data: dict, include_both: bool) -> List[dict]:
    groups = data.get("thunks", {})
    thunks = list(groups.get("thunk_called", []))
    if include_both:
        thunks.extend(groups.get("both_called", []))
    return thunks


def main() -> int:
    parser = argparse.ArgumentParser(description="Generate thunk ASM stubs")
    parser.add_argument(
        "--thunks",
        default="build/thunks.json",
        help="Thunk analysis JSON (from detect_thunks.py --verify-calls)",
    )
    parser.add_argument(
        "--output-dir",
        default="asm/thunks",
        help="Output directory for .s stubs",
    )
    parser.add_argument(
        "--metadata-out",
        default="build/thunk_stub_metadata.json",
        help="Metadata JSON output",
    )
    parser.add_argument(
        "--mode",
        choices=["fallthrough", "jump"],
        default="fallthrough",
        help="Stub mode (fallthrough or jump)",
    )
    parser.add_argument(
        "--include-both",
        action="store_true",
        help="Include thunks where both thunk and prologue are called",
    )
    parser.add_argument(
        "--pattern",
        action="append",
        default=[],
        help="Filter by pattern_type (repeatable)",
    )
    parser.add_argument(
        "--limit",
        type=int,
        default=0,
        help="Limit number of stubs generated (0 = no limit)",
    )
    args = parser.parse_args()

    thunk_data = load_json(Path(args.thunks))
    thunks = collect_thunks(thunk_data, include_both=args.include_both)

    if args.pattern:
        thunks = [t for t in thunks if t.get("pattern_type") in args.pattern]

    if args.limit:
        thunks = thunks[: args.limit]

    output_dir = Path(args.output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    metadata = {
        "metadata": {
            "source": args.thunks,
            "mode": args.mode,
            "count": len(thunks),
            "include_both": args.include_both,
            "patterns": args.pattern,
        },
        "stubs": [],
    }

    for thunk in thunks:
        thunk_addr = thunk["thunk_address"]
        func_addr = thunk["function_address"]
        thunk_name = f"func_{int(thunk_addr, 16):08X}"
        body_symbol = f"func_{int(func_addr, 16):08X}_body"

        instructions = [extract_instr(line) for line in thunk.get("thunk_disasm", [])]

        stub_text = format_stub(
            thunk_name=thunk_name,
            thunk_addr=thunk_addr,
            func_addr=func_addr,
            pattern=thunk.get("pattern_type", "unknown"),
            instructions=instructions,
            mode=args.mode,
            body_symbol=body_symbol,
        )

        output_path = output_dir / f"{thunk_name}_thunk.s"
        output_path.write_text(stub_text)

        metadata["stubs"].append(
            {
                "thunk_name": thunk_name,
                "body_symbol": body_symbol,
                "thunk_address": thunk_addr,
                "function_address": func_addr,
                "pattern": thunk.get("pattern_type", "unknown"),
                "thunk_size": thunk.get("thunk_size"),
                "stub_path": str(output_path),
                "mode": args.mode,
            }
        )

    metadata_path = Path(args.metadata_out)
    metadata_path.parent.mkdir(parents=True, exist_ok=True)
    metadata_path.write_text(json.dumps(metadata, indent=2) + "\n")

    print(f"Generated {len(thunks)} thunk stubs in {output_dir}")
    print(f"Metadata written to {metadata_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
