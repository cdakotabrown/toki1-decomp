#!/usr/bin/env python3
"""
Build hybrid executable with C code and raw assembly.

This tool:
1. Reads the original executable
2. Reads a list of decompiled functions with their addresses/sizes
3. Generates assembly that includes raw bytes EXCEPT for decompiled regions
4. Generates a linker script that places C code at exact addresses

Usage:
    python3 build_with_c.py --exe disks/SLPS_000.65 \
        --functions config/decompiled_functions.json \
        --asm build/jp/hybrid/main.s \
        --ld build/jp/hybrid/main.ld
"""

import argparse
import json
import struct
import sys
from pathlib import Path


HEADER_SIZE = 2048  # PS1 EXE header


def load_function_config(config_path: Path) -> list:
    """Load function configuration from JSON file."""
    with open(config_path) as f:
        data = json.load(f)

    functions = data.get('functions', [])

    # Sort by address
    functions.sort(key=lambda f: f.get('address', 0))

    return functions


def generate_hybrid_assembly(exe_data: bytes, functions: list, dest_addr: int, output_path: Path) -> None:
    """Generate assembly with gaps for C functions."""

    code_data = exe_data[HEADER_SIZE:]
    header_data = exe_data[:HEADER_SIZE]

    # Build list of regions to exclude (decompiled functions)
    excluded = []
    for func in functions:
        addr = func.get('address', 0)
        size = func.get('size', 0)
        name = func.get('name', 'unknown')
        if addr and size:
            # Convert to file offset
            offset = addr - dest_addr
            if 0 <= offset < len(code_data):
                excluded.append({
                    'name': name,
                    'address': addr,
                    'offset': offset,
                    'size': size,
                })

    # Sort excluded regions by offset
    excluded.sort(key=lambda r: r['offset'])

    with open(output_path, 'w') as f:
        f.write("/* Auto-generated hybrid assembly for SLPS-00065 */\n")
        f.write("/* Contains raw bytes with gaps for decompiled C functions */\n\n")

        f.write(".set noat\n")
        f.write(".set noreorder\n\n")

        # Header section
        f.write(".section .header, \"a\"\n")
        f.write(".balign 4\n")
        f.write("header_start:\n")

        for i in range(0, len(header_data), 4):
            word = struct.unpack('<I', header_data[i:i+4])[0]
            f.write(f"    .word 0x{word:08X}\n")

        f.write("\n")

        # Code section
        f.write(".section .text, \"ax\"\n")
        f.write(".balign 4\n")
        f.write(f".org 0x{dest_addr:08X}\n")
        f.write("code_start:\n\n")

        current_offset = 0

        for region in excluded:
            # Write bytes before this excluded region
            if current_offset < region['offset']:
                f.write(f"/* Raw bytes: offset 0x{current_offset:X} - 0x{region['offset']:X} */\n")
                for i in range(current_offset, region['offset'], 4):
                    if i + 4 <= region['offset']:
                        word = struct.unpack('<I', code_data[i:i+4])[0]
                        f.write(f"    .word 0x{word:08X}\n")

                f.write("\n")

            # Add symbol reference for the C function
            f.write(f"/* C function: {region['name']} @ 0x{region['address']:08X} ({region['size']} bytes) */\n")
            f.write(f".global {region['name']}\n")
            f.write(f"{region['name']}:\n")
            f.write(f"    .space {region['size']}\n")
            f.write("\n")

            current_offset = region['offset'] + region['size']

        # Write remaining bytes after last excluded region
        if current_offset < len(code_data):
            f.write(f"/* Raw bytes: offset 0x{current_offset:X} - end */\n")
            for i in range(current_offset, len(code_data), 4):
                if i + 4 <= len(code_data):
                    word = struct.unpack('<I', code_data[i:i+4])[0]
                    f.write(f"    .word 0x{word:08X}\n")
                else:
                    for j in range(i, len(code_data)):
                        f.write(f"    .byte 0x{code_data[j]:02X}\n")

        f.write("\ncode_end:\n")

    print(f"Generated hybrid assembly: {output_path}")
    print(f"  Excluded {len(excluded)} function regions for C code")


def generate_hybrid_linker_script(functions: list, dest_addr: int, output_path: Path) -> None:
    """Generate linker script that places C functions at correct addresses."""

    with open(output_path, 'w') as f:
        f.write("/* Auto-generated hybrid linker script for SLPS-00065 */\n\n")

        f.write("OUTPUT_FORMAT(\"elf32-tradlittlemips\")\n")
        f.write("OUTPUT_ARCH(mips)\n\n")

        f.write("MEMORY {\n")
        f.write("    header : ORIGIN = 0x00000000, LENGTH = 0x800\n")
        f.write(f"    ram (rwx) : ORIGIN = 0x{dest_addr:08X}, LENGTH = 2M\n")
        f.write("}\n\n")

        f.write("SECTIONS {\n")
        f.write("    .header : {\n")
        f.write("        *(.header)\n")
        f.write("    } > header\n\n")

        f.write(f"    .text 0x{dest_addr:08X} : {{\n")
        f.write("        *(.text)\n")

        # Place each C function at its exact address
        for func in functions:
            addr = func.get('address', 0)
            name = func.get('name', '')
            c_file = func.get('c_file', '')
            if addr and name:
                f.write(f"\n        /* {name} @ 0x{addr:08X} */\n")
                f.write(f"        . = 0x{addr:08X};\n")
                if c_file:
                    f.write(f"        {c_file}(.text.{name})\n")
                else:
                    f.write(f"        *(.text.{name})\n")

        f.write("\n        *(.rodata)\n")
        f.write("        *(.rodata.*)\n")
        f.write("        *(.data)\n")
        f.write("        *(.data.*)\n")
        f.write("    } > ram\n\n")

        f.write("    /DISCARD/ : {\n")
        f.write("        *(.MIPS.abiflags)\n")
        f.write("        *(.reginfo)\n")
        f.write("        *(.pdr)\n")
        f.write("        *(.comment)\n")
        f.write("        *(.note*)\n")
        f.write("        *(.eh_frame*)\n")
        f.write("        *(.mdebug*)\n")
        f.write("    }\n")
        f.write("}\n")

    print(f"Generated hybrid linker script: {output_path}")


def main():
    parser = argparse.ArgumentParser(
        description='Build hybrid executable with C code and raw assembly'
    )
    parser.add_argument('--exe', required=True, help='Path to original PS1 executable')
    parser.add_argument('--functions', required=True, help='Path to function config JSON')
    parser.add_argument('--asm', required=True, help='Output assembly file path')
    parser.add_argument('--ld', required=True, help='Output linker script path')

    args = parser.parse_args()

    exe_path = Path(args.exe)
    func_path = Path(args.functions)
    asm_path = Path(args.asm)
    ld_path = Path(args.ld)

    if not exe_path.exists():
        print(f"ERROR: Original executable not found: {exe_path}")
        sys.exit(1)

    if not func_path.exists():
        print(f"ERROR: Function config not found: {func_path}")
        sys.exit(1)

    # Create output directories
    asm_path.parent.mkdir(parents=True, exist_ok=True)
    ld_path.parent.mkdir(parents=True, exist_ok=True)

    # Read original
    with open(exe_path, 'rb') as f:
        exe_data = f.read()

    # Parse header for dest address
    dest_addr = struct.unpack('<I', exe_data[0x18:0x1C])[0]

    # Load functions
    functions = load_function_config(func_path)
    print(f"Loaded {len(functions)} decompiled functions from {func_path}")

    # Generate files
    generate_hybrid_assembly(exe_data, functions, dest_addr, asm_path)
    generate_hybrid_linker_script(functions, dest_addr, ld_path)


if __name__ == '__main__':
    main()
