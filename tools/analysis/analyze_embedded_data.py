#!/usr/bin/env python3
"""
Analyze the Tokimeki Memorial executable to identify embedded data regions.

This script scans the entire code section and identifies regions that contain
invalid MIPS I instructions (data embedded within code). It produces precise
byte boundaries for creating a proper splat configuration.

The key insight: PS1 uses MIPS I (R3000). Any MIPS II/III/IV instructions or
invalid opcodes indicate DATA, not CODE.
"""

import struct
import json
import sys
from pathlib import Path
from dataclasses import dataclass
from typing import List, Tuple, Dict, Optional

# PS1 is MIPS I (R3000) - these are the ONLY valid primary opcodes
# Anything else is either invalid or MIPS II+ (which means it's DATA on PS1)
VALID_PRIMARY_OPCODES = {
    0x00,  # SPECIAL (uses funct field)
    0x01,  # REGIMM (uses rt field for branch type)
    0x02,  # J
    0x03,  # JAL
    0x04,  # BEQ
    0x05,  # BNE
    0x06,  # BLEZ
    0x07,  # BGTZ
    0x08,  # ADDI
    0x09,  # ADDIU
    0x0A,  # SLTI
    0x0B,  # SLTIU
    0x0C,  # ANDI
    0x0D,  # ORI
    0x0E,  # XORI
    0x0F,  # LUI
    0x10,  # COP0
    0x12,  # COP2 (GTE - PS1's graphics transformation engine)
    0x20,  # LB
    0x21,  # LH
    0x22,  # LWL
    0x23,  # LW
    0x24,  # LBU
    0x25,  # LHU
    0x26,  # LWR
    0x28,  # SB
    0x29,  # SH
    0x2A,  # SWL
    0x2B,  # SW
    0x2E,  # SWR
    0x32,  # LWC2 (GTE load)
    0x3A,  # SWC2 (GTE store)
}

# DEFINITELY invalid on PS1 - MIPS II/III/IV or unused opcodes
INVALID_PRIMARY_OPCODES = {
    0x11,  # COP1 (FPU - PS1 has NO FPU!)
    0x13,  # COP3 (unused on PS1)
    0x14,  # BEQL (MIPS II)
    0x15,  # BNEL (MIPS II)
    0x16,  # BLEZL (MIPS II)
    0x17,  # BGTZL (MIPS II)
    0x18,  # DADDI (MIPS III - 64-bit)
    0x19,  # DADDIU (MIPS III)
    0x1A,  # LDL (MIPS III)
    0x1B,  # LDR (MIPS III)
    0x1C,  # SPECIAL2 (MIPS32 - much later)
    0x1D,  # JALX (MIPS16e - much later)
    0x1E,  # Reserved
    0x1F,  # SPECIAL3 (MIPS32r2)
    0x27,  # LWU (MIPS III)
    0x2C,  # Reserved
    0x2D,  # Reserved
    0x2F,  # Reserved
    0x30,  # LL (MIPS II)
    0x31,  # LWC1 (FPU)
    0x33,  # PREF (MIPS IV)
    0x34,  # LLD (MIPS III)
    0x35,  # LDC1 (FPU, MIPS II)
    0x36,  # LDC2 (MIPS II)
    0x37,  # LD (MIPS III)
    0x38,  # SC (MIPS II)
    0x39,  # SWC1 (FPU)
    0x3B,  # Reserved
    0x3C,  # SCD (MIPS III)
    0x3D,  # SDC1 (FPU, MIPS II)
    0x3E,  # SDC2 (MIPS II)
    0x3F,  # SD (MIPS III)
}

# SPECIAL (opcode 0x00) - valid function codes for MIPS I
VALID_SPECIAL_FUNCTS = {
    0x00,  # SLL
    0x02,  # SRL
    0x03,  # SRA
    0x04,  # SLLV
    0x06,  # SRLV
    0x07,  # SRAV
    0x08,  # JR
    0x09,  # JALR
    0x0C,  # SYSCALL
    0x0D,  # BREAK
    0x10,  # MFHI
    0x11,  # MTHI
    0x12,  # MFLO
    0x13,  # MTLO
    0x18,  # MULT
    0x19,  # MULTU
    0x1A,  # DIV
    0x1B,  # DIVU
    0x20,  # ADD
    0x21,  # ADDU
    0x22,  # SUB
    0x23,  # SUBU
    0x24,  # AND
    0x25,  # OR
    0x26,  # XOR
    0x27,  # NOR
    0x2A,  # SLT
    0x2B,  # SLTU
}

# REGIMM (opcode 0x01) - valid rt field values for MIPS I
VALID_REGIMM_RT = {
    0x00,  # BLTZ
    0x01,  # BGEZ
    0x10,  # BLTZAL
    0x11,  # BGEZAL
}


def is_valid_mips1_instruction(word: int) -> Tuple[bool, str]:
    """
    Check if a 32-bit word is a valid MIPS I instruction.

    Returns (is_valid, reason) tuple.
    """
    # NOP (sll $zero, $zero, 0) is valid
    if word == 0:
        return True, "NOP"

    opcode = (word >> 26) & 0x3F

    # Check if definitely invalid
    if opcode in INVALID_PRIMARY_OPCODES:
        return False, f"Invalid opcode 0x{opcode:02X} (MIPS II+ or FPU)"

    # Check if valid primary opcode
    if opcode not in VALID_PRIMARY_OPCODES:
        return False, f"Unknown opcode 0x{opcode:02X}"

    # For SPECIAL instructions, check funct field
    if opcode == 0x00:
        funct = word & 0x3F
        if funct not in VALID_SPECIAL_FUNCTS:
            # Some unused funct codes - suspicious but could be data
            return False, f"Invalid SPECIAL funct 0x{funct:02X}"

    # For REGIMM, check rt field
    if opcode == 0x01:
        rt = (word >> 16) & 0x1F
        if rt not in VALID_REGIMM_RT:
            return False, f"Invalid REGIMM rt 0x{rt:02X}"

    # For COP0/COP2, there are many suboperations - allow them
    # (GTE operations use COP2 extensively)

    return True, "Valid"


@dataclass
class Region:
    """A contiguous region of code or data."""
    start_offset: int  # File offset
    end_offset: int    # File offset (exclusive)
    vram_start: int
    vram_end: int
    region_type: str   # 'code' or 'data'
    invalid_count: int = 0
    total_instructions: int = 0

    @property
    def size(self) -> int:
        return self.end_offset - self.start_offset


def analyze_executable(exe_path: str, verbose: bool = False) -> Tuple[List[Region], Dict]:
    """
    Analyze the executable to find all code and data regions.

    Returns (regions, stats) tuple.
    """
    with open(exe_path, 'rb') as f:
        data = f.read()

    # Parse PS-X EXE header
    def read_u32(offset):
        return struct.unpack('<I', data[offset:offset+4])[0]

    text_file_start = read_u32(0x08)  # Usually 0x800
    text_size = read_u32(0x0C)
    pc_entry = read_u32(0x10)
    text_vram_start = read_u32(0x18)

    text_file_end = text_file_start + text_size
    text_vram_end = text_vram_start + text_size

    # IMPORTANT: Everything after text_file_end is pure DATA
    # The text section contains both code and embedded data
    # But after text_file_end, it's all data (no code)

    print(f"Executable: {exe_path}")
    print(f"Text section: file 0x{text_file_start:X} - 0x{text_file_end:X}")
    print(f"Text section: VRAM 0x{text_vram_start:08X} - 0x{text_vram_end:08X}")
    print(f"Entry point: 0x{pc_entry:08X}")
    print()

    # Scan entire text section word by word
    instruction_validity = []  # List of (file_offset, vram, is_valid, reason)

    offset = text_file_start
    while offset < text_file_end:
        word = struct.unpack('<I', data[offset:offset+4])[0]
        vram = text_vram_start + (offset - text_file_start)
        is_valid, reason = is_valid_mips1_instruction(word)
        instruction_validity.append((offset, vram, is_valid, reason, word))
        offset += 4

    # Count invalid instructions
    total_valid = sum(1 for _, _, v, _, _ in instruction_validity if v)
    total_invalid = sum(1 for _, _, v, _, _ in instruction_validity if not v)

    print(f"Total instructions scanned: {len(instruction_validity)}")
    print(f"Valid MIPS I: {total_valid}")
    print(f"Invalid/Data: {total_invalid}")
    print(f"Invalid ratio: {total_invalid / len(instruction_validity) * 100:.2f}%")
    print()

    # Find contiguous regions of valid/invalid instructions
    regions = []
    current_region_start = text_file_start
    current_is_code = instruction_validity[0][2] if instruction_validity else True
    current_invalid_count = 0
    current_total = 0

    # Sliding window approach: mark region as data if >20% invalid in a 16-word window
    WINDOW_SIZE = 16  # 64 bytes
    INVALID_THRESHOLD = 0.20  # 20% invalid = data region

    # First pass: classify each word as code or data based on local context
    word_classification = []  # List of (offset, vram, is_code)

    for i, (foffset, vram, is_valid, reason, word) in enumerate(instruction_validity):
        # Look at surrounding context
        window_start = max(0, i - WINDOW_SIZE // 2)
        window_end = min(len(instruction_validity), i + WINDOW_SIZE // 2)

        window = instruction_validity[window_start:window_end]
        invalid_in_window = sum(1 for _, _, v, _, _ in window if not v)
        invalid_ratio = invalid_in_window / len(window) if window else 0

        # If this word is invalid OR we're in a high-invalid region, mark as data
        is_code = is_valid and invalid_ratio < INVALID_THRESHOLD
        word_classification.append((foffset, vram, is_code))

    # Second pass: merge adjacent classifications into regions
    # Also apply minimum region size (8 bytes = 2 instructions)
    MIN_REGION_SIZE = 8

    current_start = text_file_start
    current_is_code = word_classification[0][2] if word_classification else True

    for i, (foffset, vram, is_code) in enumerate(word_classification):
        if is_code != current_is_code:
            # Region boundary - check if current region is big enough
            region_size = foffset - current_start

            if region_size >= MIN_REGION_SIZE:
                vram_start = text_vram_start + (current_start - text_file_start)
                vram_end = text_vram_start + (foffset - text_file_start)

                # Count invalid instructions in this region
                region_invalid = sum(1 for o, v, c, r, w in instruction_validity
                                    if current_start <= o < foffset and not c)
                region_total = (foffset - current_start) // 4

                regions.append(Region(
                    start_offset=current_start,
                    end_offset=foffset,
                    vram_start=vram_start,
                    vram_end=vram_end,
                    region_type='code' if current_is_code else 'data',
                    invalid_count=region_invalid,
                    total_instructions=region_total,
                ))

                current_start = foffset
                current_is_code = is_code

    # Add final region
    final_vram_start = text_vram_start + (current_start - text_file_start)
    regions.append(Region(
        start_offset=current_start,
        end_offset=text_file_end,
        vram_start=final_vram_start,
        vram_end=text_vram_end,
        region_type='code' if current_is_code else 'data',
    ))

    # Merge very small regions into neighbors
    merged_regions = []
    for region in regions:
        if merged_regions and region.size < MIN_REGION_SIZE:
            # Merge with previous region
            prev = merged_regions[-1]
            merged_regions[-1] = Region(
                start_offset=prev.start_offset,
                end_offset=region.end_offset,
                vram_start=prev.vram_start,
                vram_end=region.vram_end,
                region_type=prev.region_type,  # Keep previous type
                invalid_count=prev.invalid_count + region.invalid_count,
                total_instructions=prev.total_instructions + region.total_instructions,
            )
        else:
            merged_regions.append(region)

    # Statistics
    stats = {
        'total_regions': len(merged_regions),
        'code_regions': sum(1 for r in merged_regions if r.region_type == 'code'),
        'data_regions': sum(1 for r in merged_regions if r.region_type == 'data'),
        'total_code_bytes': sum(r.size for r in merged_regions if r.region_type == 'code'),
        'total_data_bytes': sum(r.size for r in merged_regions if r.region_type == 'data'),
        'total_invalid_instructions': total_invalid,
        'text_vram_start': f"0x{text_vram_start:08X}",
        'text_vram_end': f"0x{text_vram_end:08X}",
        'text_file_end': text_file_end,
        'file_size': len(data),
    }

    return merged_regions, stats


def generate_splat_config(regions: List[Region], output_path: str, text_file_end: int, file_size: int):
    """Generate a splat configuration file from analyzed regions."""

    yaml_lines = [
        "# Auto-generated splat configuration with precise data region boundaries",
        "# Generated by analyze_embedded_data.py",
        "",
        "options:",
        "  platform: psx",
        "  basename: main",
        "  base_path: ..",
        "  build_path: build/jp",
        "  target_path: disks/SLPS_000.65",
        "  asm_path: asm/jp/main",
        "  asset_path: assets/main",
        "  src_path: src/main",
        "  ld_script_path: build/jp/main.ld",
        "  compiler: PSYQ",
        "  symbol_addrs_path:",
        "    - config/symbols.jp.txt",
        "    - config/symbols_all.jp.txt",
        "  find_file_boundaries: no",
        "  use_legacy_include_asm: no",
        "  disassemble_all: true",
        "  mips_abi_gpr: o32",
        "  string_encoding: ASCII",
        "  data_string_encoding: ASCII",
        "",
        "  # GP value from PS-X EXE header",
        "  gp_value: 0x800EB670",
        "",
        "  # Memory range",
        "  global_vram_start: 0x80041000",
        "  global_vram_end: 0x80200000",
        "",
        "  # Force disassembly even for unknown instructions",
        "  disasm_unknown: true",
        "",
        "  section_order:",
        '    - ".text"',
        '    - ".data"',
        '    - ".rodata"',
        '    - ".bss"',
        "",
        "# SHA1 checksum of SLPS_000.65",
        "sha1: 75760c51d18f0a19c02d11cc04f3397933c4f78b",
        "",
        "segments:",
        "  - [0x0, header]  # PS-X EXE header (2048 bytes = 0x800)",
        "  - name: main",
        "    type: code",
        "    start: 0x800",
        "    vram: 0x80041000",
        "    follows_vram: header",
        "    bss_size: 0x2000",
        "    subalign: 4",
        "    subsegments:",
    ]

    # Add all regions from the text section
    region_count = 0
    for region in regions:
        region_count += 1

        if region.region_type == 'code':
            name = f"code_{region.vram_start:08X}"
            seg_type = "asm"  # Use asm to avoid C compilation issues
        else:
            name = f"data_{region.vram_start:08X}"
            seg_type = "data"

        yaml_lines.append(f"      - [0x{region.start_offset:X}, {seg_type}, {name}]")

    # Add the data section after text section (this is pure data, no code)
    # From text_file_end to file_size
    data_section_vram = 0x80041000 + (text_file_end - 0x800)
    yaml_lines.append(f"      - [0x{text_file_end:X}, data, main_data]  # Data section: 0x{data_section_vram:08X}")

    # Add end marker
    yaml_lines.append(f"  - [0x{file_size:X}]  # End of file marker")
    yaml_lines.append("")

    with open(output_path, 'w') as f:
        f.write('\n'.join(yaml_lines))

    print(f"Generated splat config: {output_path}")
    print(f"Total regions in text section: {region_count}")
    print(f"Plus 1 data section from 0x{text_file_end:X} to 0x{file_size:X}")


def main():
    import argparse

    parser = argparse.ArgumentParser(description='Analyze embedded data regions in PS1 executable')
    parser.add_argument('--exe', default='disks/SLPS_000.65',
                       help='Path to executable')
    parser.add_argument('--output', default='config/splat.jp.main.yaml',
                       help='Output path for splat config')
    parser.add_argument('--verbose', '-v', action='store_true',
                       help='Verbose output')
    parser.add_argument('--json', help='Output regions as JSON file')
    parser.add_argument('--dry-run', action='store_true',
                       help='Analyze only, do not generate config')

    args = parser.parse_args()

    regions, stats = analyze_executable(args.exe, args.verbose)

    print("=== Analysis Results ===")
    print(f"Total regions: {stats['total_regions']}")
    print(f"Code regions: {stats['code_regions']} ({stats['total_code_bytes']} bytes)")
    print(f"Data regions: {stats['data_regions']} ({stats['total_data_bytes']} bytes)")
    print()

    # Show first few data regions
    data_regions = [r for r in regions if r.region_type == 'data']
    print(f"Data regions found ({len(data_regions)} total):")
    for i, r in enumerate(data_regions[:20]):
        print(f"  {i+1:3d}. 0x{r.vram_start:08X} - 0x{r.vram_end:08X} ({r.size:5d} bytes)")
    if len(data_regions) > 20:
        print(f"  ... and {len(data_regions) - 20} more")
    print()

    if args.json:
        json_data = {
            'stats': stats,
            'regions': [
                {
                    'start_offset': f"0x{r.start_offset:X}",
                    'end_offset': f"0x{r.end_offset:X}",
                    'vram_start': f"0x{r.vram_start:08X}",
                    'vram_end': f"0x{r.vram_end:08X}",
                    'size': r.size,
                    'type': r.region_type,
                }
                for r in regions
            ]
        }
        with open(args.json, 'w') as f:
            json.dump(json_data, f, indent=2)
        print(f"Saved regions to {args.json}")

    if not args.dry_run:
        generate_splat_config(regions, args.output, stats['text_file_end'], stats['file_size'])


if __name__ == '__main__':
    main()
