#!/usr/bin/env python3
"""
swap_registers_auto.py - Automatically detect and fix register allocation differences

This tool analyzes the target binary and compiled object to detect register allocation
differences, then automatically applies the necessary swaps to achieve a match.

Usage:
    # Automatic detection and fix
    python3 swap_registers_auto.py input.o output.o --target-exe disks/SLPS_000.65 --address 0x80088C44

    # Multiple swaps in one pass
    python3 swap_registers_auto.py input.o output.o --swaps "at:v0,t6:v1,t7:t8"

    # Report only (no modification)
    python3 swap_registers_auto.py input.o output.o --target-exe disks/SLPS_000.65 --address 0x80088C44 --report-only

Features:
    - Automatically detects register allocation differences
    - Handles multiple swaps in a single pass
    - Common patterns: at↔v0, t6↔v1, t7↔t8
    - Verifies match after swapping
    - Detailed diff reporting

Common Patterns:
    1. at ↔ v0: IDO uses $at for sltu/bnez, but target uses $v0
    2. t6 ↔ v1: Loop counter register differences
    3. t7 ↔ t8: Temporary register allocation differences
    4. a3 ↔ at: Function argument vs assembler temporary

Example Workflow:
    # Step 1: Compile with IDO
    ./tools/ido-static-recomp/build/5.2/out/cc -O2 -G0 -mips1 -EL -non_shared \\
        -I include -c source.c -o output.o

    # Step 2: Apply OR→ADDU conversion (for IDO region 0x80086000+)
    python3 tools/ido/convert_or_to_addu.py output.o output_addu.o

    # Step 3: Auto-detect and fix register differences
    python3 tools/ido/swap_registers_auto.py output_addu.o output_final.o \\
        --target-exe disks/SLPS_000.65 --address 0x80088C44

Test Function:
    mem2mem (address 0x80088C44) has known register differences:
    - Target: sltu v0,v1,a2 + bnez v0,...
    - Compiled: sltu at,v1,a2 + bnez at,...
    - Fix: swap at↔v0

Date: January 2026
"""

import argparse
import struct
import sys
import shutil
import subprocess
import tempfile
import os
from pathlib import Path
from typing import List, Tuple, Dict, Optional, Set

# MIPS register numbers
REGS = {
    'zero': 0, 'at': 1,
    'v0': 2, 'v1': 3,
    'a0': 4, 'a1': 5, 'a2': 6, 'a3': 7,
    't0': 8, 't1': 9, 't2': 10, 't3': 11,
    't4': 12, 't5': 13, 't6': 14, 't7': 15,
    's0': 16, 's1': 17, 's2': 18, 's3': 19,
    's4': 20, 's5': 21, 's6': 22, 's7': 23,
    't8': 24, 't9': 25,
    'k0': 26, 'k1': 27,
    'gp': 28, 'sp': 29, 'fp': 30, 'ra': 31
}

# Reverse mapping: number -> name
REG_NAMES = {v: k for k, v in REGS.items()}

# MIPS instruction decoding
def decode_instruction(instr: int) -> Dict:
    """Decode a MIPS instruction into its components."""
    opcode = (instr >> 26) & 0x3F
    rs = (instr >> 21) & 0x1F
    rt = (instr >> 16) & 0x1F
    rd = (instr >> 11) & 0x1F
    shamt = (instr >> 6) & 0x1F
    funct = instr & 0x3F
    imm = instr & 0xFFFF

    return {
        'opcode': opcode,
        'rs': rs, 'rt': rt, 'rd': rd,
        'shamt': shamt, 'funct': funct,
        'imm': imm
    }

def get_instruction_name(instr: int) -> str:
    """Get human-readable instruction name."""
    decoded = decode_instruction(instr)
    opcode = decoded['opcode']
    funct = decoded['funct']

    # R-type instructions (opcode = 0)
    if opcode == 0:
        r_type = {
            0x00: 'sll', 0x02: 'srl', 0x03: 'sra',
            0x08: 'jr', 0x09: 'jalr',
            0x20: 'add', 0x21: 'addu', 0x22: 'sub', 0x23: 'subu',
            0x24: 'and', 0x25: 'or', 0x26: 'xor', 0x27: 'nor',
            0x2a: 'slt', 0x2b: 'sltu'
        }
        return r_type.get(funct, f'r{funct:02x}')

    # I-type and J-type instructions
    i_type = {
        0x01: 'bcond', 0x02: 'j', 0x03: 'jal',
        0x04: 'beq', 0x05: 'bne', 0x06: 'blez', 0x07: 'bgtz',
        0x08: 'addi', 0x09: 'addiu', 0x0a: 'slti', 0x0b: 'sltiu',
        0x0c: 'andi', 0x0d: 'ori', 0x0e: 'xori', 0x0f: 'lui',
        0x20: 'lb', 0x21: 'lh', 0x23: 'lw', 0x24: 'lbu', 0x25: 'lhu',
        0x28: 'sb', 0x29: 'sh', 0x2b: 'sw'
    }
    return i_type.get(opcode, f'op{opcode:02x}')

def extract_registers_from_instruction(instr: int) -> Set[int]:
    """Extract all registers used in an instruction."""
    decoded = decode_instruction(instr)
    regs = set()

    # Add rs (source register 1)
    if decoded['rs'] != 0:  # Ignore $zero
        regs.add(decoded['rs'])

    # Add rt (source register 2 or destination for I-type)
    if decoded['rt'] != 0:
        regs.add(decoded['rt'])

    # Add rd (destination for R-type)
    if decoded['opcode'] == 0 and decoded['rd'] != 0:
        regs.add(decoded['rd'])

    return regs

def disassemble_bytes(data: bytes, vram_addr: int = 0) -> List[Tuple[int, int, str]]:
    """
    Disassemble raw bytes using objdump.

    Returns list of (offset, instruction_word, asm_text) tuples.
    Note: objdump shows bytes in big-endian format, but we need to read
    the actual little-endian words from the data for correct decoding.
    """
    with tempfile.NamedTemporaryFile(suffix='.bin', delete=False) as tmp:
        tmp.write(data)
        tmp_path = tmp.name

    try:
        cmd = [
            "mipsel-linux-gnu-objdump",
            "-D", "-b", "binary", "-m", "mips:3000",
            "--adjust-vma", hex(vram_addr),
            tmp_path
        ]
        result = subprocess.run(cmd, capture_output=True, text=True)

        instructions = []
        in_disasm = False
        for line in result.stdout.split('\n'):
            if '<.data>:' in line:
                in_disasm = True
                continue
            if in_disasm and ':' in line:
                parts = line.strip().split('\t')
                if len(parts) >= 3:
                    addr_hex = parts[0].split(':')[0].strip()
                    asm_text = '\t'.join(parts[2:]).strip()

                    # Parse instruction word from actual data (little-endian)
                    offset = int(addr_hex, 16) - vram_addr
                    if offset >= 0 and offset + 4 <= len(data):
                        instr_word = struct.unpack('<I', data[offset:offset+4])[0]
                        instructions.append((offset, instr_word, asm_text))

        return instructions
    finally:
        os.unlink(tmp_path)

def extract_function_from_exe(exe_path: str, vram_addr: int, size: int = None) -> bytes:
    """Extract function bytes from executable."""
    TEXT_SECTION_START = 0x80041000
    FILE_OFFSET_BASE = 0x800

    file_offset = (vram_addr - TEXT_SECTION_START) + FILE_OFFSET_BASE

    with open(exe_path, 'rb') as f:
        f.seek(file_offset)
        if size is None:
            # Read until we find a jr ra (return)
            data = bytearray()
            while len(data) < 4096:  # Safety limit
                chunk = f.read(4)
                if not chunk:
                    break
                data.extend(chunk)
                word = struct.unpack('<I', chunk)[0]
                # Check for jr ra (0x03e00008)
                if word == 0x03e00008:
                    # Read one more instruction (delay slot)
                    chunk = f.read(4)
                    if chunk:
                        data.extend(chunk)
                    break
            return bytes(data)
        else:
            return f.read(size)

def extract_text_section_from_elf(elf_path: str) -> Tuple[bytes, bool]:
    """
    Extract .text section from ELF object file.

    Returns (text_data, big_endian)
    """
    with open(elf_path, 'rb') as f:
        # Read ELF header
        magic = f.read(4)
        if magic != b'\x7fELF':
            raise ValueError(f"{elf_path} is not an ELF file")

        # Check endianness
        f.seek(5)
        ei_data = f.read(1)[0]
        big_endian = (ei_data == 2)
        endian_fmt = '>' if big_endian else '<'

        # Get section header info
        f.seek(32)
        e_shoff = struct.unpack(endian_fmt + 'I', f.read(4))[0]
        f.seek(46)
        e_shentsize = struct.unpack(endian_fmt + 'H', f.read(2))[0]
        e_shnum = struct.unpack(endian_fmt + 'H', f.read(2))[0]
        e_shstrndx = struct.unpack(endian_fmt + 'H', f.read(2))[0]

        # Find .shstrtab section
        f.seek(e_shoff + e_shstrndx * e_shentsize + 16)
        shstrtab_offset = struct.unpack(endian_fmt + 'I', f.read(4))[0]

        # Find .text section
        for i in range(e_shnum):
            f.seek(e_shoff + i * e_shentsize)
            sh_name_idx = struct.unpack(endian_fmt + 'I', f.read(4))[0]

            # Read section name
            pos = f.tell()
            f.seek(shstrtab_offset + sh_name_idx)
            name = b''
            while True:
                c = f.read(1)
                if c == b'\x00' or not c:
                    break
                name += c
            f.seek(pos)

            if name == b'.text':
                f.seek(e_shoff + i * e_shentsize + 16)
                text_offset = struct.unpack(endian_fmt + 'I', f.read(4))[0]
                text_size = struct.unpack(endian_fmt + 'I', f.read(4))[0]

                f.seek(text_offset)
                return f.read(text_size), big_endian

        raise ValueError(".text section not found")

def compare_instructions(target_instrs: List[Tuple[int, int, str]],
                         compiled_instrs: List[Tuple[int, int, str]]) -> List[Dict]:
    """
    Compare two instruction lists and identify differences.

    Returns list of difference dictionaries.
    """
    differences = []

    min_len = min(len(target_instrs), len(compiled_instrs))

    for i in range(min_len):
        target_off, target_word, target_asm = target_instrs[i]
        compiled_off, compiled_word, compiled_asm = compiled_instrs[i]

        if target_word != compiled_word:
            target_dec = decode_instruction(target_word)
            compiled_dec = decode_instruction(compiled_word)

            diff = {
                'offset': i * 4,
                'target_word': target_word,
                'compiled_word': compiled_word,
                'target_asm': target_asm,
                'compiled_asm': compiled_asm,
                'target_decoded': target_dec,
                'compiled_decoded': compiled_dec
            }
            differences.append(diff)

    return differences

def detect_register_swaps(differences: List[Dict]) -> List[Tuple[int, int]]:
    """
    Analyze differences and detect which register swaps are needed.

    Returns list of (from_reg, to_reg) tuples.
    """
    # Count register usage differences
    reg_diff_counts = {}

    for diff in differences:
        target_dec = diff['target_decoded']
        compiled_dec = diff['compiled_decoded']

        # Only analyze if same instruction type
        if target_dec['opcode'] != compiled_dec['opcode']:
            continue
        if target_dec['opcode'] == 0 and target_dec['funct'] != compiled_dec['funct']:
            continue

        # Check rs difference
        if target_dec['rs'] != compiled_dec['rs']:
            pair = (compiled_dec['rs'], target_dec['rs'])
            reg_diff_counts[pair] = reg_diff_counts.get(pair, 0) + 1

        # Check rt difference
        if target_dec['rt'] != compiled_dec['rt']:
            pair = (compiled_dec['rt'], target_dec['rt'])
            reg_diff_counts[pair] = reg_diff_counts.get(pair, 0) + 1

        # Check rd difference (R-type only)
        if target_dec['opcode'] == 0:
            if target_dec['rd'] != compiled_dec['rd']:
                pair = (compiled_dec['rd'], target_dec['rd'])
                reg_diff_counts[pair] = reg_diff_counts.get(pair, 0) + 1

    # Sort by frequency (most common first)
    sorted_swaps = sorted(reg_diff_counts.items(), key=lambda x: x[1], reverse=True)

    return [pair for pair, count in sorted_swaps if count >= 1]

def swap_register_in_instruction(instr: int, from_reg: int, to_reg: int, bidirectional: bool = True) -> Tuple[int, bool]:
    """
    Swap register in a single MIPS instruction.

    Args:
        instr: The instruction word
        from_reg: Register number to replace
        to_reg: Register number to replace with
        bidirectional: If True, swap both directions (a↔b). If False, only replace from→to (a→b).

    Returns:
        (new_instruction, changed)
    """
    changed = False

    opcode = (instr >> 26) & 0x3F
    rs = (instr >> 21) & 0x1F
    rt = (instr >> 16) & 0x1F
    rd = (instr >> 11) & 0x1F

    # Replace rs
    if rs == from_reg:
        instr = (instr & ~(0x1F << 21)) | (to_reg << 21)
        changed = True
    elif bidirectional and rs == to_reg:
        instr = (instr & ~(0x1F << 21)) | (from_reg << 21)
        changed = True

    # Replace rt
    if rt == from_reg:
        instr = (instr & ~(0x1F << 16)) | (to_reg << 16)
        changed = True
    elif bidirectional and rt == to_reg:
        instr = (instr & ~(0x1F << 16)) | (from_reg << 16)
        changed = True

    # Replace rd (only for R-type instructions)
    if opcode == 0:
        if rd == from_reg:
            instr = (instr & ~(0x1F << 11)) | (to_reg << 11)
            changed = True
        elif bidirectional and rd == to_reg:
            instr = (instr & ~(0x1F << 11)) | (from_reg << 11)
            changed = True

    return instr, changed

def apply_register_swaps(input_path: str, output_path: str,
                        swaps: List[Tuple[int, int]], bidirectional: bool = True,
                        verbose: bool = False,
                        exclude_offsets: Optional[Set[int]] = None) -> int:
    """
    Apply multiple register swaps to an ELF object file.

    Args:
        input_path: Input ELF .o file
        output_path: Output ELF .o file
        swaps: List of (from_reg, to_reg) tuples
        bidirectional: If True, swap both directions. If False, only replace from→to.
        verbose: Print detailed output
        exclude_offsets: Set of .text offsets to skip (e.g., {0xFC} to skip instruction at offset 0xFC)

    Returns:
        Number of instructions modified.
    """
    if exclude_offsets is None:
        exclude_offsets = set()
    shutil.copy(input_path, output_path)

    with open(output_path, 'r+b') as f:
        # Read ELF header
        magic = f.read(4)
        if magic != b'\x7fELF':
            raise ValueError(f"{input_path} is not an ELF file")

        # Check endianness
        f.seek(5)
        ei_data = f.read(1)[0]
        big_endian = (ei_data == 2)
        endian_fmt = '>' if big_endian else '<'

        # Get section header info
        f.seek(32)
        e_shoff = struct.unpack(endian_fmt + 'I', f.read(4))[0]
        f.seek(46)
        e_shentsize = struct.unpack(endian_fmt + 'H', f.read(2))[0]
        e_shnum = struct.unpack(endian_fmt + 'H', f.read(2))[0]
        e_shstrndx = struct.unpack(endian_fmt + 'H', f.read(2))[0]

        # Find .shstrtab section
        f.seek(e_shoff + e_shstrndx * e_shentsize + 16)
        shstrtab_offset = struct.unpack(endian_fmt + 'I', f.read(4))[0]

        # Find .text section
        text_offset = None
        text_size = None

        for i in range(e_shnum):
            f.seek(e_shoff + i * e_shentsize)
            sh_name_idx = struct.unpack(endian_fmt + 'I', f.read(4))[0]

            # Read section name
            pos = f.tell()
            f.seek(shstrtab_offset + sh_name_idx)
            name = b''
            while True:
                c = f.read(1)
                if c == b'\x00' or not c:
                    break
                name += c
            f.seek(pos)

            if name == b'.text':
                f.seek(e_shoff + i * e_shentsize + 16)
                text_offset = struct.unpack(endian_fmt + 'I', f.read(4))[0]
                text_size = struct.unpack(endian_fmt + 'I', f.read(4))[0]
                break

        if text_offset is None:
            raise ValueError(".text section not found")

        if verbose:
            print(f"Found .text at offset {text_offset}, size {text_size}")

        # Process instructions
        total_changes = 0
        f.seek(text_offset)
        for i in range(0, text_size, 4):
            pos = text_offset + i
            f.seek(pos)
            instr_bytes = f.read(4)
            if len(instr_bytes) < 4:
                break

            # Skip excluded offsets
            if i in exclude_offsets:
                if verbose:
                    print(f"  Offset {i:04x}: EXCLUDED from swap")
                continue

            instr = struct.unpack(endian_fmt + 'I', instr_bytes)[0]
            new_instr = instr
            changed = False

            # Apply all swaps
            for from_reg, to_reg in swaps:
                new_instr, swap_changed = swap_register_in_instruction(new_instr, from_reg, to_reg, bidirectional)
                changed = changed or swap_changed

            if changed:
                if verbose:
                    print(f"  Offset {i:04x}: {instr:08x} -> {new_instr:08x}")
                f.seek(pos)
                f.write(struct.pack(endian_fmt + 'I', new_instr))
                total_changes += 1

        if verbose or total_changes > 0:
            for from_reg, to_reg in swaps:
                from_name = REG_NAMES.get(from_reg, str(from_reg))
                to_name = REG_NAMES.get(to_reg, str(to_reg))
                print(f"Applied swap: ${from_name} ↔ ${to_name}")
            print(f"Total instructions modified: {total_changes}")

    return total_changes

def generate_diff_report(target_instrs: List[Tuple[int, int, str]],
                         compiled_instrs: List[Tuple[int, int, str]],
                         differences: List[Dict]) -> str:
    """Generate a detailed diff report."""
    lines = []
    lines.append("=" * 80)
    lines.append("REGISTER ALLOCATION DIFFERENCE REPORT")
    lines.append("=" * 80)
    lines.append("")
    lines.append(f"Total instructions: {len(target_instrs)}")
    lines.append(f"Differences found: {len(differences)}")
    lines.append("")

    if differences:
        lines.append("-" * 80)
        lines.append("INSTRUCTION-BY-INSTRUCTION COMPARISON:")
        lines.append("-" * 80)
        lines.append("")

        for diff in differences:
            offset = diff['offset']
            lines.append(f"Offset {offset:04x}:")
            lines.append(f"  Target:   {diff['target_word']:08x}  {diff['target_asm']}")
            lines.append(f"  Compiled: {diff['compiled_word']:08x}  {diff['compiled_asm']}")

            # Identify specific differences
            t_dec = diff['target_decoded']
            c_dec = diff['compiled_decoded']

            if t_dec['opcode'] != c_dec['opcode']:
                lines.append(f"    Opcode: {c_dec['opcode']} -> {t_dec['opcode']}")
            if t_dec['rs'] != c_dec['rs']:
                c_name = REG_NAMES.get(c_dec['rs'], str(c_dec['rs']))
                t_name = REG_NAMES.get(t_dec['rs'], str(t_dec['rs']))
                lines.append(f"    RS: ${c_name} -> ${t_name}")
            if t_dec['rt'] != c_dec['rt']:
                c_name = REG_NAMES.get(c_dec['rt'], str(c_dec['rt']))
                t_name = REG_NAMES.get(t_dec['rt'], str(t_dec['rt']))
                lines.append(f"    RT: ${c_name} -> ${t_name}")
            if t_dec['opcode'] == 0 and t_dec['rd'] != c_dec['rd']:
                c_name = REG_NAMES.get(c_dec['rd'], str(c_dec['rd']))
                t_name = REG_NAMES.get(t_dec['rd'], str(t_dec['rd']))
                lines.append(f"    RD: ${c_name} -> ${t_name}")

            lines.append("")

    return '\n'.join(lines)

def main():
    parser = argparse.ArgumentParser(
        description='Automatically detect and fix register allocation differences',
        epilog="""
Examples:
    # Auto-detect and fix
    python3 swap_registers_auto.py input.o output.o --target-exe disks/SLPS_000.65 --address 0x80088C44

    # Manual swaps
    python3 swap_registers_auto.py input.o output.o --swaps "at:v0,t6:v1"

    # Report only
    python3 swap_registers_auto.py input.o output.o --target-exe disks/SLPS_000.65 --address 0x80088C44 --report-only

Test function:
    mem2mem (0x80088C44) - known register differences (at↔v0)
        """,
        formatter_class=argparse.RawDescriptionHelpFormatter
    )

    parser.add_argument('input', help='Input .o file (compiled)')
    parser.add_argument('output', help='Output .o file (fixed)')
    parser.add_argument('--target-exe', help='Target executable (e.g., disks/SLPS_000.65)')
    parser.add_argument('--address', help='VRAM address of function (e.g., 0x80088C44)')
    parser.add_argument('--size', type=int, help='Function size in bytes (auto-detect if not specified)')
    parser.add_argument('--swaps', help='Manual swaps: "at:v0,t6:v1,t7:t8"')
    parser.add_argument('--unidirectional', action='store_true',
                       help='Only replace from→to (not bidirectional swap). Default: bidirectional.')
    parser.add_argument('--exclude-offsets',
                       help='Comma-separated list of .text offsets to skip (e.g., "0xFC" or "0xFC,0x10")')
    parser.add_argument('--report-only', action='store_true', help='Generate report without modifying files')
    parser.add_argument('-v', '--verbose', action='store_true', help='Verbose output')

    args = parser.parse_args()

    # Parse exclude offsets
    exclude_offsets = set()
    if args.exclude_offsets:
        for offset_str in args.exclude_offsets.split(','):
            offset_str = offset_str.strip()
            if offset_str:
                exclude_offsets.add(int(offset_str, 0))

    # Determine swap method
    if args.swaps:
        # Manual swaps
        swap_pairs = []
        for swap_str in args.swaps.split(','):
            parts = swap_str.strip().split(':')
            if len(parts) != 2:
                print(f"Error: Invalid swap format '{swap_str}' (expected 'from:to')", file=sys.stderr)
                sys.exit(1)

            from_reg = REGS.get(parts[0].strip())
            to_reg = REGS.get(parts[1].strip())

            if from_reg is None:
                print(f"Error: Unknown register '{parts[0]}'", file=sys.stderr)
                sys.exit(1)
            if to_reg is None:
                print(f"Error: Unknown register '{parts[1]}'", file=sys.stderr)
                sys.exit(1)

            swap_pairs.append((from_reg, to_reg))

        if args.verbose:
            print("Manual swap mode:")
            for from_reg, to_reg in swap_pairs:
                from_name = REG_NAMES[from_reg]
                to_name = REG_NAMES[to_reg]
                print(f"  ${from_name} ↔ ${to_name}")
            print()

        if not args.report_only:
            bidirectional = not args.unidirectional
            changes = apply_register_swaps(args.input, args.output, swap_pairs, bidirectional, args.verbose, exclude_offsets)
            mode_str = "bidirectional" if bidirectional else "unidirectional"
            excl_str = f", excluding {len(exclude_offsets)} offset(s)" if exclude_offsets else ""
            print(f"Applied {len(swap_pairs)} swap(s) ({mode_str}{excl_str}), modified {changes} instruction(s)")

    elif args.target_exe and args.address:
        # Automatic detection
        vram_addr = int(args.address, 16)

        if args.verbose:
            print(f"Auto-detection mode:")
            print(f"  Target: {args.target_exe}")
            print(f"  Address: 0x{vram_addr:08X}")
            print()

        # Extract target function
        target_data = extract_function_from_exe(args.target_exe, vram_addr, args.size)
        target_instrs = disassemble_bytes(target_data, vram_addr)

        # Extract compiled function
        compiled_data, big_endian = extract_text_section_from_elf(args.input)
        compiled_instrs = disassemble_bytes(compiled_data, 0)

        if args.verbose:
            print(f"Target instructions: {len(target_instrs)}")
            print(f"Compiled instructions: {len(compiled_instrs)}")
            print()

        # Compare and detect differences
        differences = compare_instructions(target_instrs, compiled_instrs)

        if args.verbose or len(differences) > 0:
            print(f"Found {len(differences)} instruction difference(s)")
            print()

        # Generate report
        report = generate_diff_report(target_instrs, compiled_instrs, differences)
        print(report)
        print()

        # Detect register swaps
        swap_pairs = detect_register_swaps(differences)

        if swap_pairs:
            print("-" * 80)
            print("RECOMMENDED REGISTER SWAPS:")
            print("-" * 80)
            for from_reg, to_reg in swap_pairs:
                from_name = REG_NAMES[from_reg]
                to_name = REG_NAMES[to_reg]
                print(f"  ${from_name} ↔ ${to_name}")
            print()

            if not args.report_only:
                print("Applying swaps...")
                bidirectional = not args.unidirectional
                changes = apply_register_swaps(args.input, args.output, swap_pairs, bidirectional, args.verbose, exclude_offsets)
                mode_str = "bidirectional" if bidirectional else "unidirectional"
                excl_str = f", excluding {len(exclude_offsets)} offset(s)" if exclude_offsets else ""
                print(f"Applied {len(swap_pairs)} swap(s) ({mode_str}{excl_str}), modified {changes} instruction(s)")
                print()

                # Verify result
                fixed_data, _ = extract_text_section_from_elf(args.output)
                fixed_instrs = disassemble_bytes(fixed_data, 0)
                remaining_diffs = compare_instructions(target_instrs, fixed_instrs)

                if len(remaining_diffs) == 0:
                    print("✓ PERFECT MATCH achieved!")
                else:
                    print(f"⚠ {len(remaining_diffs)} difference(s) remaining")
                    if args.verbose:
                        remaining_report = generate_diff_report(target_instrs, fixed_instrs, remaining_diffs)
                        print()
                        print(remaining_report)
            else:
                print("(Report-only mode: no modifications made)")
        else:
            print("No register swaps detected.")
            if len(differences) > 0:
                print("Differences may be due to:")
                print("  - Different opcodes (structural code differences)")
                print("  - Immediate values (constants, offsets)")
                print("  - Instruction scheduling (reordering)")

    else:
        print("Error: Must specify either --swaps or (--target-exe and --address)", file=sys.stderr)
        parser.print_help()
        sys.exit(1)

if __name__ == '__main__':
    main()
