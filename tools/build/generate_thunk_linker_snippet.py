#!/usr/bin/env python3
"""
Generate linker ordering artifacts for thunk+body pairing.

Consumes build/thunk_stub_metadata.json and writes:
- build/thunk_link_order.txt (ordered object list)
- build/thunk_linker_snippets.ld (optional ld script snippet)
- build/thunk_linker_notes.md (integration notes)

This does NOT modify build scripts automatically; it generates helpers
so you can integrate in a controlled, DRY way.
"""

import argparse
import json
from pathlib import Path
from typing import List


def load_json(path: Path) -> dict:
    if not path.exists():
        raise FileNotFoundError(f"File not found: {path}")
    with path.open() as f:
        return json.load(f)


def build_link_order(stubs: List[dict]) -> List[str]:
    lines = ["# Thunk + body object ordering", "# thunk.o then body.o"]
    for stub in stubs:
        thunk_obj = Path(stub["stub_path"]).with_suffix(".o")
        body_obj = Path(f"src/main/matched/{stub['body_symbol']}.o")
        lines.append(str(thunk_obj))
        lines.append(str(body_obj))
    return lines


def build_ld_snippet(stubs: List[dict]) -> List[str]:
    lines = ["/* Thunk linker snippet (optional) */", "SECTIONS", "{"]
    for stub in stubs:
        thunk_name = stub["thunk_name"]
        body_symbol = stub["body_symbol"]
        lines.append(
            f"  .text.{thunk_name} :" \
            f" {{ KEEP(*(.text.{thunk_name}_thunk)) " \
            f"KEEP(*(.text.{body_symbol}*)) }}"
        )
    lines.append("}")
    return lines


def build_notes(stubs: List[dict]) -> List[str]:
    lines = [
        "# Thunk Linker Integration Notes",
        "",
        "This helper assumes:",
        "- Thunk stubs are assembled into objects named like asm/thunks/func_XXXXXXXX_thunk.o",
        "- C bodies compile into objects named like src/main/matched/func_XXXXXXXX_body.o",
        "- You place thunk objects immediately before body objects in the link order.",
        "",
        "If your build system differs, adjust the generated paths accordingly.",
        "",
        f"Total stubs: {len(stubs)}",
    ]
    return lines


def main() -> int:
    parser = argparse.ArgumentParser(description="Generate linker helpers for thunk stubs")
    parser.add_argument(
        "--metadata",
        default="build/thunk_stub_metadata.json",
        help="Thunk stub metadata JSON",
    )
    parser.add_argument(
        "--link-order",
        default="build/thunk_link_order.txt",
        help="Output link order file",
    )
    parser.add_argument(
        "--ld-snippet",
        default="build/thunk_linker_snippets.ld",
        help="Output ld snippet",
    )
    parser.add_argument(
        "--notes",
        default="build/thunk_linker_notes.md",
        help="Output integration notes",
    )
    args = parser.parse_args()

    metadata = load_json(Path(args.metadata))
    stubs = metadata.get("stubs", [])

    Path(args.link_order).write_text("\n".join(build_link_order(stubs)) + "\n")
    Path(args.ld_snippet).write_text("\n".join(build_ld_snippet(stubs)) + "\n")
    Path(args.notes).write_text("\n".join(build_notes(stubs)) + "\n")

    print(f"Wrote {args.link_order}")
    print(f"Wrote {args.ld_snippet}")
    print(f"Wrote {args.notes}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
