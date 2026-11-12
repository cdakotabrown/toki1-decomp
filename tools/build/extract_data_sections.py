#!/usr/bin/env python3
"""
Extract data sections from the game executable.

These are regions incorrectly identified as functions but are actually
jump tables, string literals, or other static data that must be included
in the build as raw binary data.

Usage:
    python3 tools/build/extract_data_sections.py [--exe PATH] [--output-dir DIR]
"""

import argparse
import json
import os
import sys
from pathlib import Path

# Constants
DEFAULT_EXE = "disks/SLPS_000.65"
DEFAULT_OUTPUT = "assets/data/extracted"
CONFIG_FILE = "config/data_sections.json"
VRAM_BASE = 0x80010000
EXE_HEADER_SIZE = 0x800


def vram_to_file_offset(vram_addr: int) -> int:
    """Convert VRAM address to file offset in PS-EXE."""
    return (vram_addr - VRAM_BASE) + EXE_HEADER_SIZE


def extract_data_section(exe_path: str, vram_addr: int, size: int, output_path: str) -> bool:
    """Extract a data section from the executable."""
    file_offset = vram_to_file_offset(vram_addr)

    try:
        with open(exe_path, 'rb') as f:
            f.seek(file_offset)
            data = f.read(size)

        if len(data) != size:
            print(f"  WARNING: Only read {len(data)} bytes, expected {size}")
            return False

        with open(output_path, 'wb') as f:
            f.write(data)

        return True
    except Exception as e:
        print(f"  ERROR: {e}")
        return False


def main():
    parser = argparse.ArgumentParser(description="Extract data sections from game executable")
    parser.add_argument("--exe", default=DEFAULT_EXE, help="Path to SLPS_000.65")
    parser.add_argument("--output-dir", default=DEFAULT_OUTPUT, help="Output directory")
    parser.add_argument("--config", default=CONFIG_FILE, help="Data sections config JSON")
    parser.add_argument("--verbose", "-v", action="store_true", help="Verbose output")
    args = parser.parse_args()

    # Load config
    if not os.path.exists(args.config):
        print(f"ERROR: Config file not found: {args.config}")
        sys.exit(1)

    with open(args.config, 'r') as f:
        config = json.load(f)

    # Check executable
    if not os.path.exists(args.exe):
        print(f"ERROR: Executable not found: {args.exe}")
        print("Run 'make extract-exe' first to extract from disc.")
        sys.exit(1)

    # Create output directory
    os.makedirs(args.output_dir, exist_ok=True)

    # Extract each section
    sections = config.get("sections", {})
    success_count = 0

    print(f"Extracting {len(sections)} data section(s)...")

    for addr_str, info in sections.items():
        vram_addr = int(addr_str, 16)
        size = info["size"]
        desc = info.get("description", "Unknown")

        output_name = f"data_{addr_str[2:]}.bin"  # Strip 0x prefix
        output_path = os.path.join(args.output_dir, output_name)

        print(f"  {addr_str}: {desc}")
        if args.verbose:
            print(f"    Size: {size} bytes")
            print(f"    Output: {output_path}")

        if extract_data_section(args.exe, vram_addr, size, output_path):
            success_count += 1
            print(f"    -> Extracted to {output_path}")
        else:
            print(f"    -> FAILED")

    print(f"\nDone: {success_count}/{len(sections)} sections extracted")

    # Generate include file for build system
    inc_path = os.path.join(args.output_dir, "data_sections.inc")
    with open(inc_path, 'w') as f:
        f.write("/* Auto-generated data section includes */\n")
        f.write("/* Run: python3 tools/build/extract_data_sections.py */\n\n")
        for addr_str, info in sections.items():
            output_name = f"data_{addr_str[2:]}.bin"
            f.write(f"/* {addr_str}: {info.get('description', '')} */\n")
            f.write(f"/* .incbin \"{output_name}\" */\n\n")

    print(f"Generated include hints: {inc_path}")


if __name__ == "__main__":
    main()
