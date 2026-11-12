#!/usr/bin/env python3
"""
Detect data regions in the Tokimeki Memorial executable.

This script analyzes gaps between known functions to determine if they contain:
1. Valid MIPS code (missed functions)
2. Data (jump tables, strings, lookup tables)
3. Padding (NOPs or zeros)

Usage:
    python3 tools/detect_data_regions.py [--verbose]
"""

import argparse
import json
import struct
from pathlib import Path
from typing import List, Dict, Tuple, Set

# PS1 MIPS valid opcodes (6-bit primary opcode)
# Some opcodes are invalid or MIPS III/IV only (not valid on PS1 R3000)
VALID_PRIMARY_OPCODES = {
    0x00,  # SPECIAL (secondary opcode)
    0x01,  # REGIMM (branch instructions)
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
    0x12,  # COP2 (GTE)
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
    0x32,  # LWC2 (GTE)
    0x3A,  # SWC2 (GTE)
}

# Invalid primary opcodes for PS1
INVALID_PRIMARY_OPCODES = {
    0x11,  # COP1 (FPU - not on PS1!)
    0x13,  # COP3 (not on PS1)
    0x14,  # BEQL (MIPS II)
    0x15,  # BNEL (MIPS II)
    0x16,  # BLEZL (MIPS II)
    0x17,  # BGTZL (MIPS II)
    0x18,  # DADDI (MIPS III)
    0x19,  # DADDIU (MIPS III)
    0x1A,  # LDL (MIPS III)
    0x1B,  # LDR (MIPS III)
    0x1C,  # SPECIAL2 (MIPS32)
    0x27,  # LWU (MIPS III)
    0x30,  # LL (MIPS II)
    0x31,  # LWC1 (FPU)
    0x33,  # PREF (MIPS IV)
    0x34,  # LLD (MIPS III)
    0x35,  # LDC1 (FPU, MIPS II)
    0x37,  # LD (MIPS III)
    0x38,  # SC (MIPS II)
    0x39,  # SWC1 (FPU)
    0x3C,  # SCD (MIPS III)
    0x3D,  # SDC1 (FPU, MIPS II)
    0x3F,  # SD (MIPS III)
}


def is_valid_mips_instruction(word: int) -> bool:
    """Check if a word is a valid PS1 MIPS instruction."""
    if word == 0:  # NOP is valid
        return True

    opcode = (word >> 26) & 0x3F

    # Check if primary opcode is definitely invalid
    if opcode in INVALID_PRIMARY_OPCODES:
        return False

    # Check if primary opcode is valid
    if opcode in VALID_PRIMARY_OPCODES:
        return True

    # Opcodes not in either set are suspicious
    return False


def is_likely_data(data: bytes, verbose: bool = False) -> Tuple[bool, float, str]:
    """
    Determine if a block of data is likely data rather than code.

    Returns: (is_data, confidence, reason)
    """
    if len(data) < 4:
        return True, 1.0, "Too small"

    # Count invalid instructions
    invalid_count = 0
    nop_count = 0
    total_words = len(data) // 4

    for i in range(0, len(data) - 3, 4):
        word = struct.unpack('<I', data[i:i+4])[0]

        if word == 0:
            nop_count += 1
        elif not is_valid_mips_instruction(word):
            invalid_count += 1

    if total_words == 0:
        return True, 1.0, "Empty"

    invalid_ratio = invalid_count / total_words
    nop_ratio = nop_count / total_words

    # If more than 20% invalid instructions, it's likely data
    if invalid_ratio > 0.2:
        return True, invalid_ratio, f"High invalid ratio: {invalid_ratio:.1%}"

    # If almost all NOPs, it's padding
    if nop_ratio > 0.8:
        return True, nop_ratio, f"Mostly NOPs: {nop_ratio:.1%}"

    # Check for patterns suggesting data
    # - Repeated values
    # - Pointer-like values (0x800xxxxx)
    # - Small integers (likely table entries)

    pointer_count = 0
    small_int_count = 0

    for i in range(0, len(data) - 3, 4):
        word = struct.unpack('<I', data[i:i+4])[0]

        # Check for VRAM pointers
        if 0x80000000 <= word <= 0x80200000:
            pointer_count += 1

        # Check for small integers
        if word < 0x10000:
            small_int_count += 1

    pointer_ratio = pointer_count / total_words

    # High concentration of pointers suggests a jump table or pointer array
    if pointer_ratio > 0.5:
        return True, pointer_ratio, f"High pointer ratio: {pointer_ratio:.1%}"

    # Look for code patterns - function prologues
    has_prologue = False
    has_epilogue = False
    has_jal = False

    for i in range(0, len(data) - 3, 4):
        word = struct.unpack('<I', data[i:i+4])[0]

        # addiu sp, sp, -X (prologue)
        if (word & 0xFFFF0000) == 0x27BD0000:
            imm = word & 0xFFFF
            if imm >= 0xFF00:  # Negative value
                has_prologue = True

        # jr ra (epilogue)
        if word == 0x03E00008:
            has_epilogue = True

        # jal (function call)
        if (word >> 26) == 0x03:
            has_jal = True

    # If it has function patterns, it's probably code
    if has_prologue and (has_epilogue or has_jal):
        return False, 0.0, "Has function patterns"

    # Default: if low invalid count, assume it could be code
    if invalid_ratio < 0.05:
        return False, invalid_ratio, "Low invalid ratio"

    return True, invalid_ratio, f"Moderate invalid ratio: {invalid_ratio:.1%}"


def analyze_gap(data: bytes, gap_start: int, gap_end: int, verbose: bool = False) -> Dict:
    """
    Analyze a gap between functions to classify its contents.

    Returns a dict with gap analysis.
    """
    size = gap_end - gap_start

    # Get gap data
    file_start = gap_start - 0x80041000 + 0x800  # VRAM to file offset
    gap_data = data[file_start:file_start + size]

    is_data, confidence, reason = is_likely_data(gap_data, verbose)

    # Check for all zeros (pure padding)
    if all(b == 0 for b in gap_data):
        return {
            'start_vram': gap_start,
            'end_vram': gap_end,
            'size': size,
            'type': 'padding',
            'confidence': 1.0,
            'reason': 'All zeros'
        }

    # Check for alignment padding (small gaps of NOPs)
    if size <= 16:
        # Small gaps are usually alignment
        return {
            'start_vram': gap_start,
            'end_vram': gap_end,
            'size': size,
            'type': 'padding',
            'confidence': 0.9,
            'reason': f'Small gap ({size} bytes)'
        }

    return {
        'start_vram': gap_start,
        'end_vram': gap_end,
        'size': size,
        'type': 'data' if is_data else 'code',
        'confidence': confidence,
        'reason': reason
    }


def find_all_regions(exe_path: str, func_map_path: str, verbose: bool = False) -> Tuple[List[Dict], List[Dict]]:
    """
    Find all code and data regions in the executable.

    Returns: (functions, data_regions)
    """
    # Load executable
    with open(exe_path, 'rb') as f:
        exe_data = f.read()

    # Load function map
    with open(func_map_path, 'r') as f:
        func_map = json.load(f)

    # Parse functions
    functions = []
    for f in func_map['functions']:
        functions.append({
            'name': f['name'],
            'vram_start': int(f['vram_start'], 16),
            'vram_end': int(f['vram_end'], 16),
            'file_offset': int(f['file_offset'], 16),
            'size': f['size'],
            'type': 'code'
        })

    # Sort by vram_start
    functions.sort(key=lambda x: x['vram_start'])

    # Define text section boundaries
    text_vram_start = int(func_map['metadata']['text_section']['vram_start'], 16)
    text_vram_end = int(func_map['metadata']['text_section']['vram_end'], 16)

    # But we know code starts at 0x80041130 (first function) and the
    # data before that is initial_data
    code_vram_start = functions[0]['vram_start']
    code_vram_end = functions[-1]['vram_end']

    if verbose:
        print(f"Text section: 0x{text_vram_start:08X} - 0x{text_vram_end:08X}")
        print(f"Code area: 0x{code_vram_start:08X} - 0x{code_vram_end:08X}")
        print(f"Found {len(functions)} functions")

    # Find all gaps
    data_regions = []

    # Data region before first function
    if text_vram_start < code_vram_start:
        data_regions.append({
            'name': 'initial_data',
            'vram_start': text_vram_start,
            'vram_end': code_vram_start,
            'size': code_vram_start - text_vram_start,
            'type': 'data',
            'confidence': 1.0,
            'reason': 'Before first function'
        })

    # Find gaps between functions
    for i in range(len(functions) - 1):
        current = functions[i]
        next_func = functions[i + 1]

        if current['vram_end'] < next_func['vram_start']:
            gap_analysis = analyze_gap(
                exe_data,
                current['vram_end'],
                next_func['vram_start'],
                verbose
            )

            # Generate name for the region
            if gap_analysis['type'] == 'data':
                gap_analysis['name'] = f"data_{gap_analysis['start_vram']:08X}"
            elif gap_analysis['type'] == 'padding':
                gap_analysis['name'] = f"pad_{gap_analysis['start_vram']:08X}"
            else:
                gap_analysis['name'] = f"code_{gap_analysis['start_vram']:08X}"

            data_regions.append(gap_analysis)

    # Data region after last function (main_data section)
    if code_vram_end < text_vram_end:
        data_regions.append({
            'name': 'main_data',
            'vram_start': code_vram_end,
            'vram_end': text_vram_end,
            'size': text_vram_end - code_vram_end,
            'type': 'data',
            'confidence': 1.0,
            'reason': 'After last function'
        })

    if verbose:
        print(f"Found {len(data_regions)} gaps/data regions")

        # Summarize by type
        type_counts = {}
        type_sizes = {}
        for r in data_regions:
            t = r['type']
            type_counts[t] = type_counts.get(t, 0) + 1
            type_sizes[t] = type_sizes.get(t, 0) + r['size']

        print("\nRegion summary:")
        for t in sorted(type_counts.keys()):
            print(f"  {t}: {type_counts[t]} regions, {type_sizes[t]} bytes total")

    return functions, data_regions


def generate_splat_config(functions: List[Dict], data_regions: List[Dict], output_path: str, force_asm: bool = False):
    """
    Generate a new splat configuration file.

    Args:
        functions: List of function dicts
        data_regions: List of gap/data region dicts
        output_path: Where to write the config
        force_asm: If True, use 'asm' type for all code to avoid C compilation issues
    """
    # Combine and sort all regions
    all_regions = []

    # Code type: 'c' for C compilation, 'asm' to keep as assembly
    code_type = 'asm' if force_asm else 'c'

    for f in functions:
        all_regions.append({
            'vram': f['vram_start'],
            'type': code_type,
            'name': f['name'],
            'original': f
        })

    for d in data_regions:
        # Get vram from either key name
        vram = d.get('vram_start') or d.get('start_vram')
        # ALL gaps are treated as data for splat purposes
        # This prevents splat from trying to disassemble data as code
        all_regions.append({
            'vram': vram,
            'type': 'data',
            'name': d['name'],
            'original': d
        })

    # Sort by vram address
    all_regions.sort(key=lambda x: x['vram'])

    # Generate YAML content
    yaml_lines = [
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

    # Add all subsegments
    for region in all_regions:
        # Convert VRAM to file offset
        file_offset = region['vram'] - 0x80041000 + 0x800
        yaml_lines.append(f"      - [0x{file_offset:X}, {region['type']}, {region['name']}]")

    # Add end of file marker
    yaml_lines.append("  - [0xa6ee0]  # End of file marker")
    yaml_lines.append("")

    with open(output_path, 'w') as f:
        f.write('\n'.join(yaml_lines))

    print(f"Generated splat config: {output_path}")
    print(f"Total regions: {len(all_regions)}")


def main():
    parser = argparse.ArgumentParser(description='Detect data regions in executable')
    parser.add_argument('--exe', default='disks/SLPS_000.65',
                       help='Path to executable')
    parser.add_argument('--func-map', default='build/function_analysis/function_map.json',
                       help='Path to function map JSON')
    parser.add_argument('--output', default='config/splat.jp.main.yaml',
                       help='Output path for splat config')
    parser.add_argument('--verbose', '-v', action='store_true',
                       help='Verbose output')
    parser.add_argument('--dry-run', action='store_true',
                       help='Analyze only, do not generate config')
    parser.add_argument('--force-asm', action='store_true',
                       help='Use asm type for code segments (avoids C compilation)')

    args = parser.parse_args()

    print(f"Analyzing {args.exe}")
    print(f"Using function map: {args.func_map}")
    print()

    functions, data_regions = find_all_regions(args.exe, args.func_map, args.verbose)

    print()
    print("=== Analysis Results ===")
    print(f"Functions: {len(functions)}")
    print(f"Data/gap regions: {len(data_regions)}")

    # Show data regions
    data_only = [r for r in data_regions if r['type'] == 'data']
    padding_only = [r for r in data_regions if r['type'] == 'padding']
    code_gaps = [r for r in data_regions if r['type'] == 'code']

    print(f"  - Data regions: {len(data_only)}")
    print(f"  - Padding regions: {len(padding_only)}")
    print(f"  - Possible missed code: {len(code_gaps)}")

    if args.verbose:
        print()
        print("Data regions (>32 bytes):")
        for r in data_regions:
            if r['type'] == 'data' and r['size'] > 32:
                start = r.get('vram_start', r.get('start_vram'))
                end = r.get('vram_end', r.get('end_vram'))
                print(f"  0x{start:08X} - 0x{end:08X} ({r['size']:5d} bytes): {r['reason']}")

        if code_gaps:
            print()
            print("Possible missed code regions:")
            for r in code_gaps:
                start = r.get('vram_start', r.get('start_vram'))
                end = r.get('vram_end', r.get('end_vram'))
                print(f"  0x{start:08X} - 0x{end:08X} ({r['size']:5d} bytes): {r['reason']}")

    if not args.dry_run:
        print()
        generate_splat_config(functions, data_regions, args.output, args.force_asm)


if __name__ == '__main__':
    main()
