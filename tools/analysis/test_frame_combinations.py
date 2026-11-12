#!/usr/bin/env python3
"""
test_frame_combinations.py - Test different frame size patch combinations on WIP functions

This script systematically tests different frame size patches to find exact matches:
- 24 -> 40 (most common GCC region pattern)
- 32 -> 48
- 40 -> 56
- 24 -> 32
- 32 -> 40
- 16 -> 24
- 16 -> 32

For each WIP function:
1. Compile with IDO 5.2 -O2
2. Extract target binary's actual frame size
3. Try patching compiled output to match target frame size
4. Check if result is byte-perfect match (excluding relocations)

Usage:
    python3 tools/analysis/test_frame_combinations.py
    python3 tools/analysis/test_frame_combinations.py --limit 20
    python3 tools/analysis/test_frame_combinations.py --verbose
    python3 tools/analysis/test_frame_combinations.py --test graph_init
"""

import argparse
import json
import os
import re
import struct
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, List, Optional, Tuple

# Project paths
ROOT_DIR = Path(__file__).parent.parent.parent
EXE_PATH = ROOT_DIR / "disks" / "SLPS_000.65"
WIP_DIR = ROOT_DIR / "src" / "main" / "wip"
CONFIG_PATH = ROOT_DIR / "config" / "decompiled_functions.json"
INCLUDE_DIR = ROOT_DIR / "include"

# IDO compiler and tools
IDO_CC = ROOT_DIR / "tools" / "ido-static-recomp" / "build" / "5.2" / "out" / "cc"
IDO_FLAGS = ["-EL", "-G0", "-mips1", "-O2", "-non_shared"]
PATCH_FRAME_TOOL = ROOT_DIR / "tools" / "ido" / "patch_frame_size.py"

# Constants
HEADER_SIZE = 2048
BASE_ADDR = 0x80041000

# Frame size combinations to test (compiled_size -> target_size)
FRAME_COMBINATIONS = [
    (24, 40),  # Most common GCC region pattern
    (24, 32),
    (24, 48),
    (32, 40),
    (32, 48),
    (40, 56),
    (16, 24),
    (16, 32),
    (16, 40),
]


@dataclass
class MatchResult:
    """Result of testing a frame patch combination."""
    function_name: str
    address: int
    size: int
    source_file: str
    compiled_frame_size: int
    target_frame_size: int
    patch_applied: Optional[Tuple[int, int]]  # (old, new) if patched
    is_perfect_match: bool
    matching_instructions: int
    total_instructions: int
    relocation_count: int
    other_diff_count: int
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


def get_frame_size(data: bytes) -> int:
    """Extract frame size from function prologue."""
    if len(data) < 4:
        return 0

    word = struct.unpack('<I', data[0:4])[0]
    op, rs, rt, rd, imm, imm_signed = decode_mips_insn(word)

    # addiu sp,sp,-N
    if op == 0x09 and rs == 29 and rt == 29 and imm_signed < 0:
        return -imm_signed

    return 0


def is_relocation_diff(orig_word: int, comp_word: int) -> bool:
    """Check if the difference is likely due to unresolved relocations."""
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
        if comp_target == 0 or comp_target != orig_target:
            return True

    # j (0x02) - jump target relocation
    if orig_op == 0x02:
        return True

    # addiu/ori/etc with non-sp base (potential relocation)
    if orig_op in (0x09, 0x0D, 0x0C, 0x0E, 0x0A, 0x0B):
        orig_rs = (orig_word >> 21) & 0x1F
        comp_rs = (comp_word >> 21) & 0x1F
        orig_rt = (orig_word >> 16) & 0x1F
        comp_rt = (comp_word >> 16) & 0x1F

        if orig_rs == comp_rs and orig_rt == comp_rt and orig_rs != 29:
            if comp_imm == 0 or orig_imm != comp_imm:
                return True

    # lw/sw with non-sp base (potential relocation)
    if 0x20 <= orig_op <= 0x2F:
        orig_rs = (orig_word >> 21) & 0x1F
        comp_rs = (comp_word >> 21) & 0x1F
        orig_rt = (orig_word >> 16) & 0x1F
        comp_rt = (comp_word >> 16) & 0x1F

        if orig_rs == comp_rs and orig_rt == comp_rt and orig_rs != 29:
            if comp_imm == 0 or orig_imm != comp_imm:
                return True

    return False


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


def patch_frame_size(obj_path: Path, old_size: int, new_size: int) -> Optional[Path]:
    """Patch frame size in object file."""
    if not PATCH_FRAME_TOOL.exists():
        return None

    # Create temp output file
    patched_fd, patched_path = tempfile.mkstemp(suffix='.o')
    os.close(patched_fd)

    cmd = [
        "python3", str(PATCH_FRAME_TOOL),
        str(obj_path), patched_path,
        "--old", str(old_size),
        "--new", str(new_size)
    ]

    result = subprocess.run(cmd, capture_output=True, text=True)

    if result.returncode != 0:
        os.unlink(patched_path)
        return None

    return Path(patched_path)


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


def compare_bytes(original: bytes, compiled: bytes) -> Tuple[int, int, int]:
    """
    Compare two byte sequences and count differences.

    Returns: (matching_instructions, relocation_diffs, other_diffs)
    """
    if len(original) != len(compiled):
        # Size mismatch - count as other diff
        return (0, 0, 1)

    matching = 0
    reloc_diffs = 0
    other_diffs = 0

    for i in range(0, len(original), 4):
        orig_word = struct.unpack('<I', original[i:i+4])[0]
        comp_word = struct.unpack('<I', compiled[i:i+4])[0]

        if orig_word == comp_word:
            matching += 1
        elif is_relocation_diff(orig_word, comp_word):
            reloc_diffs += 1
        else:
            other_diffs += 1

    return (matching, reloc_diffs, other_diffs)


def test_function(name: str, address: int, size: int, source_file: Path) -> MatchResult:
    """Test a single function with all frame size combinations."""
    # Extract original bytes
    try:
        original = extract_original_bytes(address, size)
    except Exception as e:
        return MatchResult(
            function_name=name,
            address=address,
            size=size,
            source_file=str(source_file),
            compiled_frame_size=0,
            target_frame_size=0,
            patch_applied=None,
            is_perfect_match=False,
            matching_instructions=0,
            total_instructions=size // 4,
            relocation_count=0,
            other_diff_count=0,
            error_message=f"Failed to extract original bytes: {e}"
        )

    target_frame_size = get_frame_size(original)

    # Compile function
    obj_path = compile_function(source_file)
    if obj_path is None:
        return MatchResult(
            function_name=name,
            address=address,
            size=size,
            source_file=str(source_file),
            compiled_frame_size=0,
            target_frame_size=target_frame_size,
            patch_applied=None,
            is_perfect_match=False,
            matching_instructions=0,
            total_instructions=size // 4,
            relocation_count=0,
            other_diff_count=0,
            error_message="Compilation failed"
        )

    # Extract compiled bytes
    compiled = extract_text_section(obj_path)
    if compiled is None:
        os.unlink(obj_path)
        return MatchResult(
            function_name=name,
            address=address,
            size=size,
            source_file=str(source_file),
            compiled_frame_size=0,
            target_frame_size=target_frame_size,
            patch_applied=None,
            is_perfect_match=False,
            matching_instructions=0,
            total_instructions=size // 4,
            relocation_count=0,
            other_diff_count=0,
            error_message="Failed to extract .text section"
        )

    compiled_frame_size = get_frame_size(compiled)

    # First, try without patching
    matching, reloc_diffs, other_diffs = compare_bytes(original, compiled)
    total_instructions = len(original) // 4

    best_result = MatchResult(
        function_name=name,
        address=address,
        size=size,
        source_file=str(source_file),
        compiled_frame_size=compiled_frame_size,
        target_frame_size=target_frame_size,
        patch_applied=None,
        is_perfect_match=(other_diffs == 0),
        matching_instructions=matching,
        total_instructions=total_instructions,
        relocation_count=reloc_diffs,
        other_diff_count=other_diffs
    )

    # If already perfect (excluding relocations), we're done
    if other_diffs == 0:
        os.unlink(obj_path)
        return best_result

    # Try frame size patches if compiled and target sizes differ
    if compiled_frame_size > 0 and target_frame_size > 0 and compiled_frame_size != target_frame_size:
        # Try patching to target size
        patched_obj = patch_frame_size(obj_path, compiled_frame_size, target_frame_size)
        if patched_obj is not None:
            patched_bytes = extract_text_section(patched_obj)
            if patched_bytes is not None:
                match_p, reloc_p, other_p = compare_bytes(original, patched_bytes)

                # If this patch gives better results, use it
                if other_p < best_result.other_diff_count or (other_p == best_result.other_diff_count and match_p > best_result.matching_instructions):
                    best_result = MatchResult(
                        function_name=name,
                        address=address,
                        size=size,
                        source_file=str(source_file),
                        compiled_frame_size=compiled_frame_size,
                        target_frame_size=target_frame_size,
                        patch_applied=(compiled_frame_size, target_frame_size),
                        is_perfect_match=(other_p == 0),
                        matching_instructions=match_p,
                        total_instructions=total_instructions,
                        relocation_count=reloc_p,
                        other_diff_count=other_p
                    )

            os.unlink(patched_obj)

    # Also try common frame size combinations
    for old_frame, new_frame in FRAME_COMBINATIONS:
        # Skip if we already tried this combination
        if old_frame == compiled_frame_size and new_frame == target_frame_size:
            continue

        # Only try if compiled frame matches the "old" size
        if compiled_frame_size != old_frame:
            continue

        patched_obj = patch_frame_size(obj_path, old_frame, new_frame)
        if patched_obj is not None:
            patched_bytes = extract_text_section(patched_obj)
            if patched_bytes is not None:
                match_p, reloc_p, other_p = compare_bytes(original, patched_bytes)

                # If this patch gives better results, use it
                if other_p < best_result.other_diff_count or (other_p == best_result.other_diff_count and match_p > best_result.matching_instructions):
                    best_result = MatchResult(
                        function_name=name,
                        address=address,
                        size=size,
                        source_file=str(source_file),
                        compiled_frame_size=compiled_frame_size,
                        target_frame_size=target_frame_size,
                        patch_applied=(old_frame, new_frame),
                        is_perfect_match=(other_p == 0),
                        matching_instructions=match_p,
                        total_instructions=total_instructions,
                        relocation_count=reloc_p,
                        other_diff_count=other_p
                    )

            os.unlink(patched_obj)

    os.unlink(obj_path)
    return best_result


def load_function_config() -> Dict[str, Tuple[int, int]]:
    """Load function addresses and sizes from config."""
    if not CONFIG_PATH.exists():
        return {}

    with open(CONFIG_PATH) as f:
        config = json.load(f)

    result = {}
    for func in config.get('functions', []):
        addr_str = func.get('address', '')
        if addr_str.startswith('0x'):
            addr = int(addr_str, 16)
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
            content = f.read(2000)
    except:
        return None, None

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


def main():
    parser = argparse.ArgumentParser(
        description='Test frame size patch combinations on WIP functions',
        formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument('-v', '--verbose', action='store_true',
                        help='Show detailed analysis for each function')
    parser.add_argument('--limit', type=int, default=0,
                        help='Limit number of functions to analyze (0=all)')
    parser.add_argument('--test', type=str,
                        help='Test a specific function (e.g., graph_init)')
    parser.add_argument('--output', type=str,
                        help='Save results to JSON file')
    parser.add_argument('--matches-only', action='store_true',
                        help='Only show perfect matches')

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

    if not PATCH_FRAME_TOOL.exists():
        print(f"ERROR: Frame patch tool not found at {PATCH_FRAME_TOOL}")
        sys.exit(1)

    # Load function config
    func_config = load_function_config()

    # Find WIP files to analyze
    if args.test:
        test_name = args.test
        if not test_name.startswith('func_'):
            test_name = f"func_{test_name}"

        source_file = WIP_DIR / f"{test_name}.c"
        if not source_file.exists():
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

    print(f"Testing {len(wip_files)} WIP functions with frame size combinations...")
    print()

    results: List[MatchResult] = []
    perfect_matches = []
    improved_matches = []
    compile_failures = 0

    for i, source_file in enumerate(wip_files, 1):
        func_name = source_file.stem

        # Get function info
        func_key = func_name.lower()
        if func_key in func_config:
            address, size = func_config[func_key]
        else:
            address, size = parse_function_header(source_file)

        if address is None or size is None or size == 0:
            if args.verbose:
                print(f"[{i}/{len(wip_files)}] SKIP: {func_name} - Could not determine address/size")
            continue

        # Test function
        if args.verbose and not args.matches_only:
            print(f"[{i}/{len(wip_files)}] Testing {func_name}...", end='', flush=True)

        result = test_function(func_name, address, size, source_file)
        results.append(result)

        if result.error_message:
            compile_failures += 1
            if args.verbose and not args.matches_only:
                print(f" FAIL: {result.error_message}")
        elif result.is_perfect_match:
            perfect_matches.append(result)
            print(f"\n{'=' * 70}")
            print(f"PERFECT MATCH: {func_name}")
            print(f"{'=' * 70}")
            print(f"  Address: 0x{address:08X}, Size: {size} bytes")
            print(f"  Frame: {result.compiled_frame_size} -> {result.target_frame_size}")
            if result.patch_applied:
                print(f"  Patch applied: {result.patch_applied[0]} -> {result.patch_applied[1]}")
            else:
                print(f"  No patch needed!")
            print(f"  Relocations: {result.relocation_count}")
            print()
        elif result.other_diff_count < 5 and result.patch_applied:
            improved_matches.append(result)
            if not args.matches_only:
                print(f"\n{'=' * 70}")
                print(f"IMPROVED: {func_name}")
                print(f"  Address: 0x{address:08X}, Size: {size} bytes")
                print(f"  Patch applied: {result.patch_applied[0]} -> {result.patch_applied[1]}")
                print(f"  Matching: {result.matching_instructions}/{result.total_instructions} instructions")
                print(f"  Relocations: {result.relocation_count}, Other diffs: {result.other_diff_count}")
                print()
        elif args.verbose and not args.matches_only:
            print(f" {result.matching_instructions}/{result.total_instructions} instructions, {result.other_diff_count} other diffs")

    # Summary
    print("\n" + "=" * 70)
    print("SUMMARY")
    print("=" * 70)
    print(f"Total tested:         {len(results)}")
    print(f"Compilation failures: {compile_failures}")
    print(f"Perfect matches:      {len(perfect_matches)}")
    print(f"Improved (< 5 diffs): {len(improved_matches)}")
    print()

    if perfect_matches:
        print("PERFECT MATCHES (ready for promotion to matched/):")
        print("-" * 70)
        for r in perfect_matches:
            patch_str = f"patch {r.patch_applied[0]}->{r.patch_applied[1]}" if r.patch_applied else "no patch"
            print(f"  {r.function_name}: frame {r.compiled_frame_size}->{r.target_frame_size}, {patch_str}, {r.relocation_count} relocs")
        print()

    if improved_matches:
        print("IMPROVED MATCHES (< 5 other diffs after patching):")
        print("-" * 70)
        for r in improved_matches:
            print(f"  {r.function_name}: patch {r.patch_applied[0]}->{r.patch_applied[1]}, {r.other_diff_count} other diffs")
        print()

    # Save JSON results
    if args.output:
        output_data = {
            'summary': {
                'total_tested': len(results),
                'compilation_failures': compile_failures,
                'perfect_matches': len(perfect_matches),
                'improved_matches': len(improved_matches)
            },
            'perfect_matches': [
                {
                    'name': r.function_name,
                    'address': f"0x{r.address:08X}",
                    'size': r.size,
                    'source_file': r.source_file,
                    'compiled_frame_size': r.compiled_frame_size,
                    'target_frame_size': r.target_frame_size,
                    'patch_applied': r.patch_applied,
                    'relocation_count': r.relocation_count
                }
                for r in perfect_matches
            ],
            'improved_matches': [
                {
                    'name': r.function_name,
                    'address': f"0x{r.address:08X}",
                    'size': r.size,
                    'source_file': r.source_file,
                    'compiled_frame_size': r.compiled_frame_size,
                    'target_frame_size': r.target_frame_size,
                    'patch_applied': r.patch_applied,
                    'matching_instructions': r.matching_instructions,
                    'total_instructions': r.total_instructions,
                    'relocation_count': r.relocation_count,
                    'other_diff_count': r.other_diff_count
                }
                for r in improved_matches
            ]
        }

        with open(args.output, 'w') as f:
            json.dump(output_data, f, indent=2)

        print(f"Results saved to {args.output}")


if __name__ == '__main__':
    main()
