#!/usr/bin/env python3
"""
find_frame_only_mismatches.py - Find WIP functions that only differ in frame/offset values

This script analyzes WIP C files to find functions where the ONLY differences
from the target binary are:
1. addiu sp,sp,N instructions (frame size allocation/deallocation)
2. sw/lw ra/sX,offset(sp) instructions (callee-saved register offsets)

These functions can be auto-matched using frame patching tools like patch_frame_full.py.

Usage:
    python3 tools/analysis/find_frame_only_mismatches.py
    python3 tools/analysis/find_frame_only_mismatches.py --verbose
    python3 tools/analysis/find_frame_only_mismatches.py --output results.json
    python3 tools/analysis/find_frame_only_mismatches.py --test main
"""

import argparse
import json
import os
import re
import struct
import subprocess
import sys
import tempfile
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, List, Optional, Tuple, Set

# Project paths
ROOT_DIR = Path(__file__).parent.parent.parent
EXE_PATH = ROOT_DIR / "disks" / "SLPS_000.65"
WIP_DIR = ROOT_DIR / "src" / "main" / "wip"
CONFIG_PATH = ROOT_DIR / "config" / "decompiled_functions.json"
INCLUDE_DIR = ROOT_DIR / "include"

# IDO compiler path
IDO_CC = ROOT_DIR / "tools" / "ido-static-recomp" / "build" / "5.2" / "out" / "cc"
IDO_FLAGS = ["-EL", "-G0", "-mips1", "-O2", "-non_shared"]

# Constants
HEADER_SIZE = 2048  # PS1 EXE header size
BASE_ADDR = 0x80041000
CODE_END = 0x800C0000

# MIPS instruction encoding
OP_ADDIU = 0x09
OP_SW = 0x2B
OP_LW = 0x23
REG_SP = 29
REG_RA = 31
REG_S0 = 16  # s0-s7 are registers 16-23


@dataclass
class InstructionDiff:
    """Represents a difference between two instructions."""
    offset: int
    original: int
    compiled: int
    diff_type: str  # 'frame_prologue', 'frame_epilogue', 'ra_save', 'ra_restore', 's_save', 's_restore', 'other'
    details: str = ""


@dataclass
class FunctionAnalysis:
    """Analysis results for a single function."""
    name: str
    address: int
    size: int
    source_file: str
    compiled_successfully: bool = False
    compiled_size: int = 0
    total_instructions: int = 0
    matching_instructions: int = 0
    frame_only_diffs: List[InstructionDiff] = field(default_factory=list)
    relocation_diffs: List[InstructionDiff] = field(default_factory=list)
    other_diffs: List[InstructionDiff] = field(default_factory=list)
    original_frame_size: int = 0
    compiled_frame_size: int = 0
    is_frame_only_mismatch: bool = False
    is_frame_and_reloc_only: bool = False  # Frame + relocation diffs only (auto-patchable after linking)
    error_message: str = ""


def get_dest_addr() -> int:
    """Get the destination address from PS1 executable header."""
    with open(EXE_PATH, 'rb') as f:
        f.seek(0x18)
        return struct.unpack('<I', f.read(4))[0]


def extract_original_bytes(address: int, size: int) -> bytes:
    """Extract bytes from the original executable at the given address."""
    dest_addr = get_dest_addr()
    offset = address - dest_addr + HEADER_SIZE

    with open(EXE_PATH, 'rb') as f:
        f.seek(offset)
        return f.read(size)


def decode_mips_insn(word: int) -> Tuple[int, int, int, int, int, int]:
    """Decode a MIPS instruction into components."""
    op = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    rd = (word >> 11) & 0x1F
    imm = word & 0xFFFF

    # Sign-extend immediate
    if imm & 0x8000:
        imm_signed = imm - 0x10000
    else:
        imm_signed = imm

    return op, rs, rt, rd, imm, imm_signed


def is_relocation_diff(orig_word: int, comp_word: int) -> bool:
    """
    Check if the difference is likely due to unresolved relocations.

    Relocation patterns:
    1. lui: opcode 0x0F, immediate differs (compiled often has 0x0000)
    2. addiu/ori/etc with rs!=0: lower immediate differs
    3. jal: target address in lower 26 bits differs
    4. lw/sw/lb/etc with base!=sp: offset differs
    """
    orig_op = (orig_word >> 26) & 0x3F
    comp_op = (comp_word >> 26) & 0x3F

    if orig_op != comp_op:
        return False

    orig_imm = orig_word & 0xFFFF
    comp_imm = comp_word & 0xFFFF

    # lui (0x0F) - if compiled immediate is 0, likely a relocation
    if orig_op == 0x0F:
        if comp_imm == 0 or (orig_word >> 16) == (comp_word >> 16):
            return True

    # jal (0x03) - target address relocation
    if orig_op == 0x03:
        orig_target = orig_word & 0x03FFFFFF
        comp_target = comp_word & 0x03FFFFFF
        # If compiled target is 0 or different, likely relocation
        if comp_target == 0 or comp_target != orig_target:
            return True

    # j (0x02) - jump target relocation
    if orig_op == 0x02:
        return True

    # addiu (0x09), ori (0x0D), andi (0x0C), etc. with non-sp base
    # If the upper bits match but immediate differs, could be relocation
    if orig_op in (0x09, 0x0D, 0x0C, 0x0E, 0x0A, 0x0B):  # addiu, ori, andi, xori, slti, sltiu
        orig_rs = (orig_word >> 21) & 0x1F
        comp_rs = (comp_word >> 21) & 0x1F
        orig_rt = (orig_word >> 16) & 0x1F
        comp_rt = (comp_word >> 16) & 0x1F

        if orig_rs == comp_rs and orig_rt == comp_rt and orig_rs != REG_SP:
            if comp_imm == 0 or orig_imm != comp_imm:
                return True

    # lw/sw/lb/etc (0x20-0x2F range) with non-sp base
    if 0x20 <= orig_op <= 0x2F:
        orig_rs = (orig_word >> 21) & 0x1F
        comp_rs = (comp_word >> 21) & 0x1F
        orig_rt = (orig_word >> 16) & 0x1F
        comp_rt = (comp_word >> 16) & 0x1F

        if orig_rs == comp_rs and orig_rt == comp_rt and orig_rs != REG_SP:
            if comp_imm == 0 or orig_imm != comp_imm:
                return True

    return False


def classify_instruction_diff(orig_word: int, comp_word: int, offset: int) -> InstructionDiff:
    """Classify the difference between two MIPS instructions."""
    orig_op, orig_rs, orig_rt, orig_rd, orig_imm, orig_imm_signed = decode_mips_insn(orig_word)
    comp_op, comp_rs, comp_rt, comp_rd, comp_imm, comp_imm_signed = decode_mips_insn(comp_word)

    diff = InstructionDiff(offset=offset, original=orig_word, compiled=comp_word, diff_type='other')

    # Check for relocation-related differences first
    if is_relocation_diff(orig_word, comp_word):
        diff.diff_type = 'relocation'
        diff.details = f"Relocation: 0x{orig_word:08X} vs 0x{comp_word:08X}"
        return diff

    # Check if both are the same opcode type
    if orig_op != comp_op:
        diff.details = f"Different opcodes: {orig_op} vs {comp_op}"
        return diff

    # Check for addiu sp,sp,N (frame allocation/deallocation)
    if orig_op == OP_ADDIU and orig_rs == REG_SP and orig_rt == REG_SP:
        if comp_rs == REG_SP and comp_rt == REG_SP:
            # Both are addiu sp,sp,N - likely frame size difference
            if orig_imm_signed < 0 and comp_imm_signed < 0:
                diff.diff_type = 'frame_prologue'
                diff.details = f"Prologue: {-orig_imm_signed} vs {-comp_imm_signed}"
            elif orig_imm_signed > 0 and comp_imm_signed > 0:
                diff.diff_type = 'frame_epilogue'
                diff.details = f"Epilogue: {orig_imm_signed} vs {comp_imm_signed}"
            return diff

    # Check for sw/lw with sp base (callee-saved register operations)
    if orig_op in (OP_SW, OP_LW) and orig_rs == REG_SP:
        if comp_op == orig_op and comp_rs == REG_SP and comp_rt == orig_rt:
            # Same operation, same register, different offset
            is_ra = (orig_rt == REG_RA)
            is_s_reg = (REG_S0 <= orig_rt <= REG_S0 + 7)

            if is_ra or is_s_reg:
                reg_name = 'ra' if is_ra else f's{orig_rt - REG_S0}'
                op_name = 'sw' if orig_op == OP_SW else 'lw'

                if orig_op == OP_SW:
                    diff.diff_type = 'ra_save' if is_ra else 's_save'
                else:
                    diff.diff_type = 'ra_restore' if is_ra else 's_restore'

                diff.details = f"{op_name} {reg_name}: offset {orig_imm_signed} vs {comp_imm_signed}"
                return diff

    # Not a frame-related difference
    diff.details = f"0x{orig_word:08X} vs 0x{comp_word:08X}"
    return diff


def get_frame_size(data: bytes) -> int:
    """Extract frame size from function prologue."""
    if len(data) < 4:
        return 0

    word = struct.unpack('<I', data[0:4])[0]
    op, rs, rt, rd, imm, imm_signed = decode_mips_insn(word)

    if op == OP_ADDIU and rs == REG_SP and rt == REG_SP and imm_signed < 0:
        return -imm_signed

    return 0


def compile_function(source_path: Path) -> Optional[Path]:
    """Compile a C source file with IDO 5.2 and return the object file path."""
    if not IDO_CC.exists():
        return None

    # Create temp object file
    obj_fd, obj_path = tempfile.mkstemp(suffix='.o')
    os.close(obj_fd)

    cmd = [str(IDO_CC)] + IDO_FLAGS + [
        "-c", str(source_path), "-o", obj_path,
        "-I", str(INCLUDE_DIR)
    ]

    result = subprocess.run(cmd, capture_output=True, text=True)

    if result.returncode != 0:
        os.unlink(obj_path)
        return None

    return Path(obj_path)


def extract_text_section(obj_path: Path) -> Optional[bytes]:
    """Extract the .text section from an ELF object file."""
    text_fd, text_path = tempfile.mkstemp(suffix='.bin')
    os.close(text_fd)

    cmd = ["mipsel-linux-gnu-objcopy", "-O", "binary", "-j", ".text",
           str(obj_path), text_path]

    result = subprocess.run(cmd, capture_output=True, text=True)

    if result.returncode != 0:
        os.unlink(text_path)
        return None

    with open(text_path, 'rb') as f:
        data = f.read()

    os.unlink(text_path)
    return data


def analyze_function(name: str, address: int, size: int, source_file: Path) -> FunctionAnalysis:
    """Analyze a single function for frame-only mismatches."""
    analysis = FunctionAnalysis(
        name=name,
        address=address,
        size=size,
        source_file=str(source_file)
    )

    # Extract original bytes
    try:
        original = extract_original_bytes(address, size)
    except Exception as e:
        analysis.error_message = f"Failed to extract original bytes: {e}"
        return analysis

    analysis.original_frame_size = get_frame_size(original)

    # Compile function
    obj_path = compile_function(source_file)
    if obj_path is None:
        analysis.error_message = "Compilation failed"
        return analysis

    # Extract compiled bytes
    compiled = extract_text_section(obj_path)
    os.unlink(obj_path)

    if compiled is None:
        analysis.error_message = "Failed to extract .text section"
        return analysis

    analysis.compiled_successfully = True
    analysis.compiled_size = len(compiled)
    analysis.compiled_frame_size = get_frame_size(compiled)

    # Compare instruction-by-instruction
    # Trim to smaller size for comparison
    min_size = min(len(original), len(compiled))
    analysis.total_instructions = min_size // 4

    # Check for size mismatch (instruction count difference)
    if len(original) != len(compiled):
        # Size mismatch is always "other" type difference
        analysis.other_diffs.append(InstructionDiff(
            offset=-1,
            original=len(original),
            compiled=len(compiled),
            diff_type='size_mismatch',
            details=f"Size mismatch: {len(original)} vs {len(compiled)} bytes"
        ))

    # Compare each instruction
    for i in range(0, min_size, 4):
        orig_word = struct.unpack('<I', original[i:i+4])[0]
        comp_word = struct.unpack('<I', compiled[i:i+4])[0]

        if orig_word == comp_word:
            analysis.matching_instructions += 1
        else:
            diff = classify_instruction_diff(orig_word, comp_word, i)

            if diff.diff_type in ('frame_prologue', 'frame_epilogue',
                                   'ra_save', 'ra_restore', 's_save', 's_restore'):
                analysis.frame_only_diffs.append(diff)
            elif diff.diff_type == 'relocation':
                analysis.relocation_diffs.append(diff)
            else:
                analysis.other_diffs.append(diff)

    # Determine if this is a frame-only mismatch
    analysis.is_frame_only_mismatch = (
        analysis.compiled_successfully and
        len(original) == len(compiled) and  # Same size
        len(analysis.other_diffs) == 0 and  # No other differences
        len(analysis.relocation_diffs) == 0 and  # No relocation differences
        len(analysis.frame_only_diffs) > 0  # Has frame-related diffs
    )

    # Frame + relocation only (can be auto-patched after proper linking)
    analysis.is_frame_and_reloc_only = (
        analysis.compiled_successfully and
        len(original) == len(compiled) and  # Same size
        len(analysis.other_diffs) == 0 and  # No other differences
        (len(analysis.frame_only_diffs) > 0 or len(analysis.relocation_diffs) > 0)
    )

    return analysis


def load_function_config() -> Dict[str, Tuple[int, int]]:
    """Load function addresses and sizes from config."""
    if not CONFIG_PATH.exists():
        return {}

    with open(CONFIG_PATH) as f:
        config = json.load(f)

    result = {}
    for func in config.get('functions', []):
        addr_val = func.get('address', '')
        # Handle both integer and string addresses
        if isinstance(addr_val, int):
            addr = addr_val
        elif isinstance(addr_val, str) and addr_val.startswith('0x'):
            addr = int(addr_val, 16)
        else:
            continue

        size = func.get('size', 0)
        name = f"func_{addr:08X}"
        result[name.lower()] = (addr, size)

    return result


def parse_function_header(source_file: Path) -> Tuple[Optional[int], Optional[int]]:
    """Parse function address and size from C file header comments."""
    try:
        with open(source_file) as f:
            content = f.read(2000)  # Just read header
    except:
        return None, None

    # Look for patterns like:
    # Address: 0x800422A0
    # Size: 100 bytes
    addr_match = re.search(r'Address:\s*0x([0-9a-fA-F]+)', content)
    size_match = re.search(r'Size:\s*(\d+)\s*bytes?', content)

    address = int(addr_match.group(1), 16) if addr_match else None
    size = int(size_match.group(1)) if size_match else None

    return address, size


def find_wip_files() -> List[Path]:
    """Find all WIP C source files."""
    if not WIP_DIR.exists():
        return []

    return sorted(WIP_DIR.glob("func_*.c"))


def format_diff(diff: InstructionDiff) -> str:
    """Format a single instruction difference for display."""
    if diff.offset == -1:
        return f"  {diff.details}"
    return f"  +0x{diff.offset:04X}: {diff.diff_type}: {diff.details}"


def main():
    parser = argparse.ArgumentParser(
        description='Find WIP functions with frame-only mismatches',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
    # Analyze all WIP files
    %(prog)s

    # Verbose output with details
    %(prog)s --verbose

    # Test specific function
    %(prog)s --test main

    # Save results to JSON
    %(prog)s --output results.json

    # Only show frame-only mismatches
    %(prog)s --frame-only
"""
    )
    parser.add_argument('-v', '--verbose', action='store_true',
                        help='Show detailed analysis for each function')
    parser.add_argument('-o', '--output', type=str,
                        help='Save results to JSON file')
    parser.add_argument('--test', type=str,
                        help='Test a specific function (e.g., main)')
    parser.add_argument('--frame-only', action='store_true',
                        help='Only show functions with frame-only mismatches')
    parser.add_argument('--limit', type=int, default=0,
                        help='Limit number of functions to analyze (0=all)')

    args = parser.parse_args()

    # Check dependencies
    if not EXE_PATH.exists():
        print(f"ERROR: Original executable not found at {EXE_PATH}")
        print("Run: make extract-exe")
        sys.exit(1)

    if not IDO_CC.exists():
        print(f"ERROR: IDO 5.2 compiler not found at {IDO_CC}")
        print("Run: make setup-ido52")
        sys.exit(1)

    # Load function config for sizes
    func_config = load_function_config()

    # Find WIP files to analyze
    if args.test:
        # Test specific function - try to find matching file
        test_name = args.test
        if not test_name.startswith('func_'):
            test_name = f"func_{test_name}"

        # Try exact match first, then case-insensitive search
        source_file = WIP_DIR / f"{test_name}.c"
        if not source_file.exists():
            # Search case-insensitively
            found = None
            for f in WIP_DIR.glob("func_*.c"):
                if f.stem.lower() == test_name.lower():
                    found = f
                    break
            if found:
                source_file = found
            else:
                print(f"ERROR: Source file not found: {source_file}")
                sys.exit(1)

        wip_files = [source_file]
    else:
        wip_files = find_wip_files()

    if args.limit > 0:
        wip_files = wip_files[:args.limit]

    print(f"Analyzing {len(wip_files)} WIP functions...")
    print()

    results: List[FunctionAnalysis] = []
    frame_only_count = 0
    frame_reloc_only_count = 0
    compile_fail_count = 0
    other_diff_count = 0
    perfect_match_count = 0
    reloc_only_count = 0

    for source_file in wip_files:
        func_name = source_file.stem

        # Get function info from config or header
        func_key = func_name.lower()
        if func_key in func_config:
            address, size = func_config[func_key]
        else:
            address, size = parse_function_header(source_file)

        if address is None or size is None or size == 0:
            if args.verbose:
                print(f"SKIP: {func_name} - Could not determine address/size")
            continue

        # Analyze function
        analysis = analyze_function(func_name, address, size, source_file)
        results.append(analysis)

        # Categorize result
        if not analysis.compiled_successfully:
            compile_fail_count += 1
            if args.verbose:
                print(f"FAIL: {func_name} - {analysis.error_message}")
        elif analysis.is_frame_only_mismatch:
            frame_only_count += 1
            print(f"FRAME-ONLY: {func_name}")
            print(f"  Address: 0x{address:08X}, Size: {size} bytes")
            print(f"  Frame size: {analysis.original_frame_size} (target) vs {analysis.compiled_frame_size} (compiled)")
            print(f"  Matching: {analysis.matching_instructions}/{analysis.total_instructions} instructions")
            if args.verbose:
                for diff in analysis.frame_only_diffs:
                    print(format_diff(diff))
            print()
        elif len(analysis.other_diffs) == 0 and len(analysis.frame_only_diffs) == 0 and len(analysis.relocation_diffs) == 0:
            perfect_match_count += 1
            if not args.frame_only:
                print(f"MATCH: {func_name} - PERFECT MATCH (consider promoting to matched/)")
        elif len(analysis.other_diffs) == 0 and len(analysis.relocation_diffs) > 0 and len(analysis.frame_only_diffs) == 0:
            reloc_only_count += 1
            if not args.frame_only:
                print(f"RELOC-ONLY: {func_name}")
                print(f"  Address: 0x{address:08X}, Size: {size} bytes")
                print(f"  Relocations: {len(analysis.relocation_diffs)}, Matching: {analysis.matching_instructions}/{analysis.total_instructions}")
                if args.verbose:
                    for diff in analysis.relocation_diffs[:5]:
                        print(format_diff(diff))
                    if len(analysis.relocation_diffs) > 5:
                        print(f"  ... and {len(analysis.relocation_diffs) - 5} more relocations")
                print()
        elif analysis.is_frame_and_reloc_only and not analysis.is_frame_only_mismatch:
            frame_reloc_only_count += 1
            if not args.frame_only:
                print(f"FRAME+RELOC: {func_name}")
                print(f"  Address: 0x{address:08X}, Size: {size} bytes")
                print(f"  Frame size: {analysis.original_frame_size} (target) vs {analysis.compiled_frame_size} (compiled)")
                print(f"  Frame diffs: {len(analysis.frame_only_diffs)}, Relocations: {len(analysis.relocation_diffs)}")
                print(f"  Matching: {analysis.matching_instructions}/{analysis.total_instructions} instructions")
                if args.verbose:
                    for diff in analysis.frame_only_diffs:
                        print(format_diff(diff))
                    for diff in analysis.relocation_diffs[:3]:
                        print(format_diff(diff))
                print()
        else:
            other_diff_count += 1
            if not args.frame_only and args.verbose:
                print(f"OTHER: {func_name}")
                print(f"  Matching: {analysis.matching_instructions}/{analysis.total_instructions} instructions")
                print(f"  Frame diffs: {len(analysis.frame_only_diffs)}, Reloc diffs: {len(analysis.relocation_diffs)}, Other diffs: {len(analysis.other_diffs)}")
                for diff in analysis.other_diffs[:3]:  # Show first 3 other diffs
                    print(format_diff(diff))
                if len(analysis.other_diffs) > 3:
                    print(f"  ... and {len(analysis.other_diffs) - 3} more other diffs")
                print()

    # Summary
    print("=" * 60)
    print("SUMMARY")
    print("=" * 60)
    print(f"Total analyzed:         {len(results)}")
    print(f"Compilation failures:   {compile_fail_count}")
    print(f"Perfect matches:        {perfect_match_count}")
    print(f"Frame-only mismatches:  {frame_only_count}")
    print(f"Relocation-only diffs:  {reloc_only_count}")
    print(f"Frame+reloc only diffs: {frame_reloc_only_count}")
    print(f"Other differences:      {other_diff_count}")
    print()

    auto_patchable = frame_only_count + reloc_only_count + frame_reloc_only_count
    if auto_patchable > 0:
        print(f"AUTO-PATCHABLE FUNCTIONS: {auto_patchable}")
        print("(These can be matched after proper linking and frame patching)")
        print()

    if frame_only_count > 0:
        print("FRAME-ONLY MISMATCH FUNCTIONS (can be auto-patched):")
        print("-" * 60)
        for analysis in results:
            if analysis.is_frame_only_mismatch:
                frame_delta = analysis.original_frame_size - analysis.compiled_frame_size
                print(f"  {analysis.name}: frame {analysis.compiled_frame_size} -> {analysis.original_frame_size} (delta={frame_delta})")
        print()
        print("To patch these functions, use:")
        print("  python3 tools/ido/patch_frame_full.py input.o output.o --old <compiled> --new <target>")
        print()

    if reloc_only_count > 0:
        print("RELOCATION-ONLY FUNCTIONS (will match after linking):")
        print("-" * 60)
        for analysis in results:
            if (len(analysis.other_diffs) == 0 and
                len(analysis.relocation_diffs) > 0 and
                len(analysis.frame_only_diffs) == 0 and
                analysis.compiled_successfully):
                print(f"  {analysis.name}: {len(analysis.relocation_diffs)} relocations")
        print()

    if frame_reloc_only_count > 0:
        print("FRAME+RELOCATION FUNCTIONS (patch frame after linking):")
        print("-" * 60)
        for analysis in results:
            if (analysis.is_frame_and_reloc_only and
                not analysis.is_frame_only_mismatch and
                len(analysis.frame_only_diffs) > 0):
                frame_delta = analysis.original_frame_size - analysis.compiled_frame_size
                print(f"  {analysis.name}: frame {analysis.compiled_frame_size} -> {analysis.original_frame_size}, {len(analysis.relocation_diffs)} relocs")
        print()

    # Save JSON results if requested
    if args.output:
        output_data = {
            'summary': {
                'total_analyzed': len(results),
                'compilation_failures': compile_fail_count,
                'perfect_matches': perfect_match_count,
                'frame_only_mismatches': frame_only_count,
                'relocation_only': reloc_only_count,
                'frame_and_reloc_only': frame_reloc_only_count,
                'other_differences': other_diff_count,
                'auto_patchable': auto_patchable
            },
            'frame_only_functions': [
                {
                    'name': a.name,
                    'address': f"0x{a.address:08X}",
                    'size': a.size,
                    'source_file': a.source_file,
                    'original_frame_size': a.original_frame_size,
                    'compiled_frame_size': a.compiled_frame_size,
                    'matching_instructions': a.matching_instructions,
                    'total_instructions': a.total_instructions,
                    'frame_diffs': [
                        {'offset': d.offset, 'type': d.diff_type, 'details': d.details}
                        for d in a.frame_only_diffs
                    ]
                }
                for a in results if a.is_frame_only_mismatch
            ],
            'relocation_only_functions': [
                {
                    'name': a.name,
                    'address': f"0x{a.address:08X}",
                    'size': a.size,
                    'source_file': a.source_file,
                    'relocation_count': len(a.relocation_diffs)
                }
                for a in results
                if (a.compiled_successfully and
                    len(a.other_diffs) == 0 and
                    len(a.relocation_diffs) > 0 and
                    len(a.frame_only_diffs) == 0)
            ],
            'frame_and_reloc_functions': [
                {
                    'name': a.name,
                    'address': f"0x{a.address:08X}",
                    'size': a.size,
                    'source_file': a.source_file,
                    'original_frame_size': a.original_frame_size,
                    'compiled_frame_size': a.compiled_frame_size,
                    'relocation_count': len(a.relocation_diffs)
                }
                for a in results
                if (a.is_frame_and_reloc_only and
                    not a.is_frame_only_mismatch and
                    len(a.frame_only_diffs) > 0)
            ],
            'perfect_matches': [
                {
                    'name': a.name,
                    'address': f"0x{a.address:08X}",
                    'size': a.size,
                    'source_file': a.source_file
                }
                for a in results
                if (a.compiled_successfully and
                    len(a.other_diffs) == 0 and
                    len(a.frame_only_diffs) == 0 and
                    len(a.relocation_diffs) == 0)
            ]
        }

        with open(args.output, 'w') as f:
            json.dump(output_data, f, indent=2)

        print(f"\nResults saved to {args.output}")


if __name__ == '__main__':
    main()
