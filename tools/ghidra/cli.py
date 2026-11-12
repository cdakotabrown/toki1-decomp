#!/usr/bin/env python3
"""Command-line comprehension oracle over a running ghidra-mcp plugin.

Read-only. Fetches Ghidra's view of a function so you can understand it -- it
does NOT produce byte-perfect C (that's m2c / decomp-permuter / ido_diff).

    python3 tools/ghidra/cli.py ping
    python3 tools/ghidra/cli.py decompile 0x80042304
    python3 tools/ghidra/cli.py disasm    0x80042304
    python3 tools/ghidra/cli.py xrefs     0x80042304
    python3 tools/ghidra/cli.py callers   0x80042304

Every command fails with a clear message (exit 1) when the plugin is not up.
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

# Allow running as a bare script (python3 tools/ghidra/cli.py ...).
sys.path.insert(0, str(Path(__file__).resolve().parent.parent.parent))

from tools.ghidra.client import (  # noqa: E402
    DEFAULT_URL,
    GhidraClient,
    GhidraError,
    decompiled_text,
)


def _client(args: argparse.Namespace) -> GhidraClient:
    return GhidraClient(base_url=args.url or DEFAULT_URL)


def _emit(result) -> None:
    print(result if isinstance(result, str) else json.dumps(result, indent=2))


def cmd_ping(args: argparse.Namespace) -> int:
    gc = _client(args)
    try:
        print(gc.check_connection())
        print(gc.get_version())
    except GhidraError as exc:
        print(f"[offline] {exc}", file=sys.stderr)
        return 1
    return 0


def _read(args: argparse.Namespace, method: str, transform=None) -> int:
    gc = _client(args)
    try:
        result = getattr(gc, method)(args.address)
    except GhidraError as exc:
        print(f"[error] {exc}", file=sys.stderr)
        return 1
    _emit(transform(result) if transform else result)
    return 0


def cmd_decompile(args: argparse.Namespace) -> int:
    return _read(args, "decompile", transform=decompiled_text)


def cmd_disasm(args: argparse.Namespace) -> int:
    return _read(args, "disassemble")


def cmd_xrefs(args: argparse.Namespace) -> int:
    return _read(args, "xrefs_to")


def cmd_callers(args: argparse.Namespace) -> int:
    return _read(args, "callers")


def cmd_callees(args: argparse.Namespace) -> int:
    return _read(args, "callees")


def build_parser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser(
        prog="tools/ghidra/cli.py",
        description="Read-only Ghidra comprehension oracle (not a byte-perfect matcher).",
    )
    p.add_argument(
        "--url",
        default=None,
        help="Ghidra plugin URL (default: $GHIDRA_MCP_URL or http://127.0.0.1:8089).",
    )
    sub = p.add_subparsers(dest="command", required=True)

    sp = sub.add_parser("ping", help="Health-check the Ghidra plugin.")
    sp.set_defaults(func=cmd_ping)

    for name, func, help_text in [
        ("decompile", cmd_decompile, "Decompile a function to C (comprehension only)."),
        ("disasm", cmd_disasm, "Disassemble a function."),
        ("xrefs", cmd_xrefs, "List references to an address."),
        ("callers", cmd_callers, "List functions that call this address."),
        ("callees", cmd_callees, "List functions this address calls."),
    ]:
        sp = sub.add_parser(name, help=help_text)
        sp.add_argument("address", help="e.g. 0x80042304")
        sp.set_defaults(func=func)

    return p


def main(argv: list[str] | None = None) -> int:
    args = build_parser().parse_args(argv)
    return args.func(args)


if __name__ == "__main__":
    raise SystemExit(main())
