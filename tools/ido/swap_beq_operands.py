#!/usr/bin/env python3
"""
Swap operands in BEQ/BNE instructions in a MIPS object file.

This tool patches BEQ/BNE instructions to swap rs and rt operands.
This is needed when the compiler generates beq rs,rt but the target
uses beq rt,rs (which is functionally equivalent but different bytes).

Usage:
    python3 swap_beq_operands.py input.o output.o --offset 0x28

The offset is the byte offset of the instruction to patch within .text section.
"""

import argparse
import shutil
import struct
import subprocess
import sys
from pathlib import Path


def get_text_section_offset(obj_path: Path) -> int:
    """Get the file offset of .text section using readelf."""
    result = subprocess.run(
        ["mipsel-linux-gnu-readelf", "-S", str(obj_path)],
        capture_output=True, text=True
    )

    for line in result.stdout.split('\n'):
        if '.text' in line and 'PROGBITS' in line:
            # Parse the section header line
            # Format: [Nr] Name Type Addr Off Size ES Flg Lk Inf Al
            parts = line.split()
            for i, part in enumerate(parts):
                if part == '.text':
                    # Offset is typically at index i+3 or similar
                    # Look for hex values after .text
                    for j in range(i+1, len(parts)):
                        try:
                            val = int(parts[j], 16)
                            if val > 0 and val < 0x10000:  # Reasonable offset
                                return val
                        except ValueError:
                            continue

    # Fallback: assume offset 0x34 (common for simple ELF)
    return 0x34


def swap_beq_operands(obj_path: Path, output_path: Path, offsets: list):
    """Swap rs and rt operands in BEQ/BNE instructions at given offsets."""

    # Copy input to output
    shutil.copy(obj_path, output_path)

    # Get .text section offset
    text_offset = get_text_section_offset(obj_path)

    with open(output_path, 'r+b') as f:
        for offset in offsets:
            file_offset = text_offset + offset

            # Read the instruction
            f.seek(file_offset)
            instr_bytes = f.read(4)
            instr = struct.unpack('<I', instr_bytes)[0]

            # Extract fields
            opcode = (instr >> 26) & 0x3F
            rs = (instr >> 21) & 0x1F
            rt = (instr >> 16) & 0x1F
            imm = instr & 0xFFFF

            # Check if BEQ (4) or BNE (5)
            if opcode not in [4, 5]:
                print(f"Warning: instruction at offset 0x{offset:x} is not BEQ/BNE (opcode={opcode})")
                continue

            # Swap rs and rt
            new_instr = (opcode << 26) | (rt << 21) | (rs << 16) | imm

            # Write back
            f.seek(file_offset)
            f.write(struct.pack('<I', new_instr))

            op_name = "beq" if opcode == 4 else "bne"
            print(f"Swapped {op_name} at offset 0x{offset:x}: "
                  f"${rs},{rt} -> ${rt},{rs}")


def main():
    parser = argparse.ArgumentParser(
        description='Swap BEQ/BNE operands in MIPS object file'
    )
    parser.add_argument('input', help='Input object file')
    parser.add_argument('output', help='Output object file')
    parser.add_argument('--offset', type=lambda x: int(x, 0),
                        action='append', required=True,
                        help='Offset of instruction to patch (can specify multiple)')

    args = parser.parse_args()

    input_path = Path(args.input)
    output_path = Path(args.output)

    if not input_path.exists():
        print(f"Error: input file not found: {input_path}")
        sys.exit(1)

    swap_beq_operands(input_path, output_path, args.offset)
    print(f"Output written to {output_path}")


if __name__ == '__main__':
    main()
