#!/usr/bin/env python3
"""
extract_function_asm.py - Extract original assembly for a specific function

Extracts the original disassembly for a function from SLPM_86.053 and saves
it in a format compatible with asm-differ and m2c.

Features:
- Extract assembly by function name or address
- Output in multiple formats (raw, annotated, m2c-compatible)
- Save to build/function_analysis/asm/ directory
- Compatible with existing toolchain

Usage:
    python3 tools/extract_function_asm.py func_8004B338
    python3 tools/extract_function_asm.py 0x8004B338
    python3 tools/extract_function_asm.py func_8004B338 --format m2c
    python3 tools/extract_function_asm.py func_8004B338 --output my_func.s
    python3 tools/extract_function_asm.py --list-leaf  # List all leaf functions
"""

import argparse
import json
import os
import subprocess
import sys
from pathlib import Path
from typing import Dict, List, Optional, Tuple

# Project paths
ROOT_DIR = Path(__file__).parent.parent.parent
ORIGINAL_EXE = ROOT_DIR / "disks" / "SLPS_000.65"
FUNCTION_MAP = ROOT_DIR / "tools" / "build" / "function_analysis" / "function_map.json"
ASM_OUTPUT_DIR = ROOT_DIR / "build" / "function_analysis" / "asm"

# Constants
TEXT_SECTION_START = 0x80041000
FILE_OFFSET_BASE = 0x800


def load_function_map() -> Dict:
    """Load the function map containing all function metadata."""
    if not FUNCTION_MAP.exists():
        print(f"Error: Function map not found at {FUNCTION_MAP}")
        print("Run: make analyze")
        sys.exit(1)

    with open(FUNCTION_MAP, 'r') as f:
        return json.load(f)


def get_function_by_name(func_name: str, function_map: Dict) -> Optional[Dict]:
    """Get function metadata by name."""
    for func in function_map.get('functions', []):
        if func['name'] == func_name:
            return func
    return None


def get_function_by_address(addr: int, function_map: Dict) -> Optional[Dict]:
    """Get function metadata by VRAM address."""
    addr_str = f"0x{addr:08X}"
    for func in function_map.get('functions', []):
        if func['vram_start'].upper() == addr_str.upper():
            return func
    return None


def vram_to_file_offset(vram_addr: int) -> int:
    """Convert VRAM address to file offset."""
    return (vram_addr - TEXT_SECTION_START) + FILE_OFFSET_BASE


def extract_raw_bytes(vram_start: int, size: int) -> bytes:
    """Extract function bytes from the original executable."""
    if not ORIGINAL_EXE.exists():
        print(f"Error: Original executable not found at {ORIGINAL_EXE}")
        sys.exit(1)

    file_offset = vram_to_file_offset(vram_start)

    with open(ORIGINAL_EXE, 'rb') as f:
        f.seek(file_offset)
        return f.read(size)


def disassemble_to_objdump(raw_bytes: bytes, vram_start: int) -> List[str]:
    """
    Disassemble raw bytes using objdump.

    Returns list of disassembly lines.
    """
    import tempfile

    with tempfile.NamedTemporaryFile(suffix='.bin', delete=False) as tmp:
        tmp.write(raw_bytes)
        tmp_path = tmp.name

    try:
        cmd = [
            "mipsel-linux-gnu-objdump",
            "-D", "-b", "binary", "-m", "mips:3000",
            "--adjust-vma", hex(vram_start),
            tmp_path
        ]
        result = subprocess.run(cmd, capture_output=True, text=True)

        lines = []
        in_disasm = False
        for line in result.stdout.split('\n'):
            if '<.data>:' in line:
                in_disasm = True
                continue
            if in_disasm and ':' in line:
                lines.append(line)

        return lines
    finally:
        os.unlink(tmp_path)


def format_raw(func_info: Dict, disasm_lines: List[str]) -> str:
    """Format as raw assembly with VRAM addresses."""
    vram_start = int(func_info['vram_start'], 16)
    func_name = func_info['name']

    output = []
    output.append(f"# Function: {func_name}")
    output.append(f"# VRAM: {func_info['vram_start']} - {func_info['vram_end']}")
    output.append(f"# Size: {func_info['size']} bytes ({func_info['num_instructions']} instructions)")
    output.append(f"# Complexity: {func_info['complexity']}")
    output.append("")
    output.append(".set noat")
    output.append(".set noreorder")
    output.append("")
    output.append(f".globl {func_name}")
    output.append(f"{func_name}:")

    for line in disasm_lines:
        # Parse objdump output: "80041000:\td0ffbd27 \taddiu\tsp,sp,-48"
        parts = line.strip().split('\t')
        if len(parts) >= 2:
            addr_hex = parts[0].split(':')[0].strip()
            hex_bytes = parts[1].strip() if len(parts) > 1 else ""
            # Join parts 2+ as instruction (opcode + operands)
            instr = '\t'.join(parts[2:]).strip() if len(parts) > 2 else ""

            # Format instruction
            if instr:
                output.append(f"    {instr:<40} # {addr_hex}")
            else:
                # Just hex bytes (data or invalid instruction)
                output.append(f"    .word 0x{hex_bytes}  # {addr_hex}")

    return '\n'.join(output)


def format_m2c(func_info: Dict, disasm_lines: List[str]) -> str:
    """
    Format for m2c (MIPS to C decompiler).

    m2c expects a specific format for input.
    """
    vram_start = int(func_info['vram_start'], 16)
    func_name = func_info['name']

    output = []
    output.append(f"glabel {func_name}")

    for line in disasm_lines:
        # Parse objdump output
        parts = line.strip().split('\t')
        if len(parts) >= 3:
            addr_hex = parts[0].split(':')[0].strip()
            # Join parts 2+ as instruction (opcode + operands)
            instr = '\t'.join(parts[2:]).strip()

            # Clean up instruction for m2c
            # m2c expects: "/* ADDR */ instruction"
            output.append(f"/* {addr_hex} */ {instr}")

    return '\n'.join(output)


def format_annotated(func_info: Dict, disasm_lines: List[str], raw_bytes: bytes) -> str:
    """
    Format with full annotations including hex bytes.

    Useful for debugging and manual analysis.
    """
    vram_start = int(func_info['vram_start'], 16)
    func_name = func_info['name']

    output = []
    output.append("=" * 80)
    output.append(f"Function: {func_name}")
    output.append("=" * 80)
    output.append("")
    output.append("Metadata:")
    output.append(f"  VRAM Range:   {func_info['vram_start']} - {func_info['vram_end']}")
    output.append(f"  Size:         {func_info['size']} bytes")
    output.append(f"  Instructions: {func_info['num_instructions']}")
    output.append(f"  Complexity:   {func_info['complexity']}")
    output.append(f"  Category:     {func_info['category']}")
    output.append(f"  JAL calls:    {func_info['num_jal_calls']}")
    output.append(f"  Branches:     {func_info['num_branches']}")
    output.append(f"  Loads:        {func_info['num_loads']}")
    output.append(f"  Stores:       {func_info['num_stores']}")
    output.append("")
    output.append("-" * 80)
    output.append("Disassembly:")
    output.append("-" * 80)
    output.append("")
    output.append("VRAM         Hex        Instruction")
    output.append("-" * 80)

    offset = 0
    for line in disasm_lines:
        parts = line.strip().split('\t')
        if len(parts) >= 2:
            addr_hex = parts[0].split(':')[0].strip()

            # Get the 4 bytes at this offset
            if offset < len(raw_bytes):
                word = int.from_bytes(raw_bytes[offset:offset+4], 'little')
                hex_str = f"{word:08x}"
            else:
                hex_str = "????????"

            # Join parts 2+ as instruction (opcode + operands)
            instr = '\t'.join(parts[2:]).strip() if len(parts) > 2 else "(data)"

            output.append(f"{addr_hex}   {hex_str}   {instr}")
            offset += 4

    output.append("")
    output.append("-" * 80)
    output.append("Raw Bytes (hex dump):")
    output.append("-" * 80)

    # Hex dump
    for i in range(0, len(raw_bytes), 16):
        chunk = raw_bytes[i:i+16]
        hex_str = ' '.join(f'{b:02x}' for b in chunk)
        addr = vram_start + i
        output.append(f"{addr:08x}  {hex_str}")

    return '\n'.join(output)


def format_diff(func_info: Dict, disasm_lines: List[str]) -> str:
    """
    Format for asm-differ comparison.

    Simple format that asm-differ can parse.
    """
    func_name = func_info['name']

    output = []
    output.append(f"{func_name}:")

    for line in disasm_lines:
        parts = line.strip().split('\t')
        if len(parts) >= 3:
            # Join parts 2+ as instruction (opcode + operands)
            instr = '\t'.join(parts[2:]).strip()
            output.append(f"  {instr}")

    return '\n'.join(output)


def list_functions(function_map: Dict, complexity: Optional[str] = None,
                   limit: int = 50) -> None:
    """List functions with optional filtering."""
    functions = function_map.get('functions', [])

    if complexity:
        functions = [f for f in functions if f['complexity'] == complexity]

    # Sort by difficulty score (easiest first)
    functions.sort(key=lambda f: f.get('difficulty_score', 999))

    print(f"{'Name':<20} {'VRAM':<12} {'Size':>6} {'Instr':>6} {'Complexity':<10} {'Score':>8}")
    print("-" * 72)

    for func in functions[:limit]:
        print(f"{func['name']:<20} {func['vram_start']:<12} {func['size']:>6} "
              f"{func['num_instructions']:>6} {func['complexity']:<10} "
              f"{func.get('difficulty_score', 0):>8.1f}")

    if len(functions) > limit:
        print(f"... and {len(functions) - limit} more functions")


def extract_function(func_name_or_addr: str, output_format: str = 'raw',
                     output_file: Optional[str] = None, verbose: bool = False) -> str:
    """
    Extract and format function assembly.

    Args:
        func_name_or_addr: Function name (func_8004B338) or address (0x8004B338)
        output_format: 'raw', 'm2c', 'annotated', or 'diff'
        output_file: Optional output file path
        verbose: Print verbose information

    Returns:
        Formatted assembly string
    """
    function_map = load_function_map()

    # Determine if input is name or address
    if func_name_or_addr.startswith('0x') or func_name_or_addr.startswith('80'):
        # Address
        addr = int(func_name_or_addr, 16)
        func_info = get_function_by_address(addr, function_map)
    else:
        # Name
        func_info = get_function_by_name(func_name_or_addr, function_map)

    if not func_info:
        print(f"Error: Function '{func_name_or_addr}' not found")
        sys.exit(1)

    func_name = func_info['name']
    vram_start = int(func_info['vram_start'], 16)
    size = func_info['size']

    if verbose:
        print(f"Extracting {func_name}")
        print(f"  VRAM: {func_info['vram_start']} - {func_info['vram_end']}")
        print(f"  Size: {size} bytes ({func_info['num_instructions']} instructions)")
        print()

    # Extract raw bytes
    raw_bytes = extract_raw_bytes(vram_start, size)

    # Disassemble
    disasm_lines = disassemble_to_objdump(raw_bytes, vram_start)

    # Format output
    if output_format == 'raw':
        result = format_raw(func_info, disasm_lines)
    elif output_format == 'm2c':
        result = format_m2c(func_info, disasm_lines)
    elif output_format == 'annotated':
        result = format_annotated(func_info, disasm_lines, raw_bytes)
    elif output_format == 'diff':
        result = format_diff(func_info, disasm_lines)
    else:
        print(f"Error: Unknown format '{output_format}'")
        sys.exit(1)

    # Output
    if output_file:
        output_path = Path(output_file)
    else:
        # Default output path
        ASM_OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
        output_path = ASM_OUTPUT_DIR / f"{func_name}.s"

    with open(output_path, 'w') as f:
        f.write(result)

    if verbose:
        print(f"Written to: {output_path}")

    return result


def main():
    parser = argparse.ArgumentParser(
        description="Extract original assembly for a specific function",
        epilog="""
Examples:
    python3 tools/extract_function_asm.py func_8004B338
    python3 tools/extract_function_asm.py 0x8004B338
    python3 tools/extract_function_asm.py func_8004B338 --format m2c
    python3 tools/extract_function_asm.py func_8004B338 --format annotated
    python3 tools/extract_function_asm.py func_8004B338 --output my_func.s
    python3 tools/extract_function_asm.py --list-leaf
    python3 tools/extract_function_asm.py --list-all --limit 100
        """,
        formatter_class=argparse.RawDescriptionHelpFormatter
    )

    parser.add_argument(
        'function',
        nargs='?',
        help='Function name (func_8004B338) or VRAM address (0x8004B338)'
    )
    parser.add_argument(
        '-f', '--format',
        choices=['raw', 'm2c', 'annotated', 'diff'],
        default='raw',
        help='Output format (default: raw)'
    )
    parser.add_argument(
        '-o', '--output',
        help='Output file path (default: build/function_analysis/asm/func_XXXX.s)'
    )
    parser.add_argument(
        '-v', '--verbose',
        action='store_true',
        help='Print verbose information'
    )
    parser.add_argument(
        '-p', '--print',
        action='store_true',
        help='Print to stdout instead of file'
    )
    parser.add_argument(
        '--list-leaf',
        action='store_true',
        help='List all leaf functions (easiest decompilation targets)'
    )
    parser.add_argument(
        '--list-simple',
        action='store_true',
        help='List all simple functions'
    )
    parser.add_argument(
        '--list-all',
        action='store_true',
        help='List all functions sorted by difficulty'
    )
    parser.add_argument(
        '--limit',
        type=int,
        default=50,
        help='Limit number of functions to list (default: 50)'
    )

    args = parser.parse_args()

    # Handle list commands
    if args.list_leaf or args.list_simple or args.list_all:
        function_map = load_function_map()

        if args.list_leaf:
            print("LEAF FUNCTIONS (no function calls - easiest targets)")
            print("=" * 72)
            list_functions(function_map, 'leaf', args.limit)
        elif args.list_simple:
            print("SIMPLE FUNCTIONS (basic wrappers)")
            print("=" * 72)
            list_functions(function_map, 'simple', args.limit)
        else:
            print("ALL FUNCTIONS (sorted by difficulty)")
            print("=" * 72)
            list_functions(function_map, None, args.limit)

        sys.exit(0)

    # Need a function name for extraction
    if not args.function:
        parser.print_help()
        sys.exit(1)

    # Extract function
    result = extract_function(
        args.function,
        args.format,
        args.output,
        args.verbose
    )

    if args.print:
        print(result)


if __name__ == "__main__":
    main()
