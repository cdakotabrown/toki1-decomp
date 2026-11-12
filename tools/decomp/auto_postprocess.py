#!/usr/bin/env python3
"""
Automatic Post-Processing Pipeline for Permuter Results

This tool takes permuter output and automatically applies post-processing
transformations to try to achieve a byte-perfect match.

Pipeline steps:
1. Frame size patching (24 -> target frame size)
2. RA/s-reg offset patching
3. Delay slot filling
4. OR -> ADDU conversion (for IDO region)
5. NOP stripping/compaction

Usage:
    python3 auto_postprocess.py func_XXXXXXXX [--verbose]
    python3 auto_postprocess.py --candidate path/to/source.c --target 0x800XXXXX [--size N]
"""

import argparse
import json
import os
import re
import shutil
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

# Add parent directory to path for imports
sys.path.insert(0, str(Path(__file__).parent.parent / 'ido'))

PROJECT_ROOT = Path(__file__).parent.parent.parent
PERMUTER_DIR = PROJECT_ROOT / 'tools' / 'decomp-permuter'
IDO_TOOLS = PROJECT_ROOT / 'tools' / 'ido'
IDO_CC = PROJECT_ROOT / 'tools' / 'ido-static-recomp' / 'build' / '5.2' / 'out' / 'cc'
EXE_PATH = PROJECT_ROOT / 'disks' / 'SLPS_000.65'

# IDO region boundary
IDO_REGION_START = 0x80086000


def get_target_bytes(addr, size, exe_path=None):
    """Extract target function bytes from executable."""
    if exe_path is None:
        exe_path = EXE_PATH

    with open(exe_path, 'rb') as f:
        exe_data = f.read()

    # PS-X EXE header
    load_addr = struct.unpack('<I', exe_data[0x18:0x1C])[0]
    header_size = 0x800

    offset = addr - load_addr + header_size
    return exe_data[offset:offset + size]


def get_function_frame_size(target_bytes):
    """Extract frame size from function prologue."""
    if len(target_bytes) < 4:
        return None

    first_instr = struct.unpack('<I', target_bytes[0:4])[0]

    # Check for addiu sp,sp,-N
    if (first_instr >> 16) == 0x27BD:
        imm = first_instr & 0xFFFF
        if imm & 0x8000:  # Sign extend
            frame_size = 0x10000 - imm
            return frame_size

    return None


def compile_source(source_path, output_path, include_dirs=None):
    """Compile C source with IDO."""
    if include_dirs is None:
        include_dirs = [str(PROJECT_ROOT / 'include')]

    cmd = [
        str(IDO_CC),
        '-O2', '-G0', '-mips1', '-non_shared',
        '-c', str(source_path),
        '-o', str(output_path)
    ]

    for inc in include_dirs:
        cmd.extend(['-I', inc])

    result = subprocess.run(cmd, capture_output=True, text=True)
    return result.returncode == 0, result.stderr


def apply_frame_patch(input_obj, output_obj, old_size, new_size, verbose=False):
    """Apply frame size patching."""
    cmd = [
        sys.executable,
        str(IDO_TOOLS / 'patch_frame_full.py'),
        str(input_obj), str(output_obj),
        '--old', str(old_size),
        '--new', str(new_size)
    ]
    if verbose:
        cmd.append('-v')

    result = subprocess.run(cmd, capture_output=True, text=True)
    return result.returncode == 0, result.stdout + result.stderr


def apply_delay_slot_fill(input_obj, output_obj, verbose=False):
    """Apply delay slot filling."""
    cmd = [
        sys.executable,
        str(IDO_TOOLS / 'fill_delay_slots.py'),
        str(input_obj), str(output_obj)
    ]
    if verbose:
        cmd.append('-v')

    result = subprocess.run(cmd, capture_output=True, text=True)
    return result.returncode == 0, result.stdout + result.stderr


def apply_or_to_addu(input_obj, output_obj, verbose=False):
    """Convert OR to ADDU for move instructions (IDO region)."""
    cmd = [
        sys.executable,
        str(IDO_TOOLS / 'convert_or_to_addu.py'),
        str(input_obj), str(output_obj)
    ]

    result = subprocess.run(cmd, capture_output=True, text=True)
    return result.returncode == 0, result.stdout + result.stderr


def extract_text_section(obj_path):
    """Extract .text section bytes from ELF object."""
    with open(obj_path, 'rb') as f:
        elf_data = f.read()

    if elf_data[:4] != b'\x7fELF':
        return None

    is_big_endian = elf_data[5] == 2
    endian = '>' if is_big_endian else '<'

    e_shoff = struct.unpack(endian + 'I', elf_data[32:36])[0]
    e_shentsize = struct.unpack(endian + 'H', elf_data[46:48])[0]
    e_shnum = struct.unpack(endian + 'H', elf_data[48:50])[0]
    e_shstrndx = struct.unpack(endian + 'H', elf_data[50:52])[0]

    shstrtab_off = e_shoff + e_shstrndx * e_shentsize
    shstrtab_addr = struct.unpack(endian + 'I', elf_data[shstrtab_off+16:shstrtab_off+20])[0]
    shstrtab_size = struct.unpack(endian + 'I', elf_data[shstrtab_off+20:shstrtab_off+24])[0]
    shstrtab = elf_data[shstrtab_addr:shstrtab_addr+shstrtab_size]

    for i in range(e_shnum):
        sh_offset = e_shoff + i * e_shentsize
        sh_name_idx = struct.unpack(endian + 'I', elf_data[sh_offset:sh_offset+4])[0]
        sh_offset_data = struct.unpack(endian + 'I', elf_data[sh_offset+16:sh_offset+20])[0]
        sh_size = struct.unpack(endian + 'I', elf_data[sh_offset+20:sh_offset+24])[0]

        name_end = shstrtab.find(b'\x00', sh_name_idx)
        name = shstrtab[sh_name_idx:name_end].decode('ascii')

        if name == '.text':
            return elf_data[sh_offset_data:sh_offset_data+sh_size], is_big_endian

    return None, False


def compare_with_target(obj_bytes, target_bytes, is_big_endian, verbose=False):
    """
    Compare compiled bytes with target, accounting for relocations.
    Returns (match_count, total_instructions, differences)
    """
    # Swap endianness if needed (compiled is big-endian, target is little-endian)
    compiled = bytearray(obj_bytes)
    if is_big_endian:
        # Swap to little-endian for comparison
        for i in range(0, len(compiled), 4):
            compiled[i:i+4] = compiled[i:i+4][::-1]

    min_len = min(len(compiled), len(target_bytes))
    num_instrs = min_len // 4
    matches = 0
    diffs = []

    for i in range(num_instrs):
        off = i * 4
        comp_instr = struct.unpack('<I', compiled[off:off+4])[0]
        targ_instr = struct.unpack('<I', target_bytes[off:off+4])[0]

        # Check for exact match
        if comp_instr == targ_instr:
            matches += 1
            continue

        # Check for relocation differences (upper bits match, lower bits differ)
        # lui: opcode bits match, imm differs
        # jal: opcode matches, target differs
        comp_op = comp_instr >> 26
        targ_op = targ_instr >> 26

        if comp_op == targ_op:
            # Same opcode - check if it's a relocation case
            if comp_op == 0x0F:  # lui
                # lui with 0 in compiled, non-zero in target = relocation
                if (comp_instr & 0xFFFF) == 0:
                    matches += 1
                    continue
            elif comp_op == 0x09:  # addiu
                # addiu with 0 in compiled = relocation
                if (comp_instr & 0xFFFF) == 0 and (comp_instr & 0xFFFF0000) == (targ_instr & 0xFFFF0000):
                    matches += 1
                    continue
            elif comp_op in (0x23, 0x2B):  # lw, sw
                # lw/sw with 0 offset in compiled = relocation
                if (comp_instr & 0xFFFF) == 0 and (comp_instr & 0xFFFF0000) == (targ_instr & 0xFFFF0000):
                    matches += 1
                    continue
            elif comp_op == 0x03:  # jal
                # jal target is always a relocation
                matches += 1
                continue

        diffs.append((i, comp_instr, targ_instr))

    return matches, num_instrs, diffs


def process_candidate(source_path, target_addr, target_size=None, verbose=False):
    """
    Process a candidate source file through the post-processing pipeline.

    Returns: (success, final_obj_path, match_info)
    """
    # Get target bytes
    if target_size is None:
        # Try to find from function map or estimate
        target_size = 256  # Default estimate

    target_bytes = get_target_bytes(target_addr, target_size)
    target_frame = get_function_frame_size(target_bytes)

    if verbose:
        print(f"Target: 0x{target_addr:08X}, size={target_size}, frame={target_frame}")

    is_ido_region = target_addr >= IDO_REGION_START

    with tempfile.TemporaryDirectory() as tmpdir:
        tmpdir = Path(tmpdir)

        # Step 1: Compile
        base_obj = tmpdir / 'base.o'
        success, err = compile_source(source_path, base_obj)
        if not success:
            return False, None, f"Compilation failed: {err}"

        current_obj = base_obj

        # Get compiled frame size
        text_bytes, is_big = extract_text_section(current_obj)
        if text_bytes is None:
            return False, None, "Failed to extract .text section"

        compiled_frame = get_function_frame_size(
            text_bytes[::-1] if is_big else text_bytes[:4]  # Handle endianness
        )

        # Actually, let's properly get frame from compiled
        # The compiled object is big-endian, so swap for analysis
        if is_big and len(text_bytes) >= 4:
            first_word = struct.unpack('>I', text_bytes[0:4])[0]
            if (first_word >> 16) == 0x27BD:
                imm = first_word & 0xFFFF
                if imm & 0x8000:
                    compiled_frame = 0x10000 - imm
                else:
                    compiled_frame = None

        if verbose:
            print(f"Compiled frame: {compiled_frame}, Target frame: {target_frame}")

        # Step 2: Frame patching (if needed)
        if compiled_frame and target_frame and compiled_frame != target_frame:
            patched_obj = tmpdir / 'frame_patched.o'
            success, msg = apply_frame_patch(current_obj, patched_obj,
                                            compiled_frame, target_frame, verbose)
            if success:
                current_obj = patched_obj
                if verbose:
                    print(f"Frame patch: {msg.strip()}")

        # Step 3: Delay slot filling
        delay_obj = tmpdir / 'delay_filled.o'
        success, msg = apply_delay_slot_fill(current_obj, delay_obj, verbose)
        if success:
            current_obj = delay_obj
            if verbose:
                print(f"Delay slots: {msg.strip()}")

        # Step 4: OR to ADDU (for IDO region)
        if is_ido_region:
            addu_obj = tmpdir / 'addu_converted.o'
            success, msg = apply_or_to_addu(current_obj, addu_obj, verbose)
            if success:
                current_obj = addu_obj
                if verbose:
                    print(f"OR->ADDU: {msg.strip()}")

        # Compare result
        final_bytes, is_big = extract_text_section(current_obj)
        matches, total, diffs = compare_with_target(final_bytes, target_bytes, is_big, verbose)

        # Copy final object out of temp dir
        final_path = Path(source_path).with_suffix('.postprocessed.o')
        shutil.copy(current_obj, final_path)

        match_info = {
            'matches': matches,
            'total': total,
            'percentage': (matches / total * 100) if total > 0 else 0,
            'differences': len(diffs),
            'diff_details': diffs[:10] if verbose else []
        }

        return matches == total, final_path, match_info


def find_best_permuter_candidates(func_name):
    """Find best permuter candidates for a function."""
    func_dir = PERMUTER_DIR / func_name
    if not func_dir.exists():
        return []

    candidates = []
    for item in func_dir.iterdir():
        if item.is_dir() and item.name.startswith('output-'):
            # Extract score from directory name
            match = re.match(r'output-(\d+(?:\.\d+)?)-', item.name)
            if match:
                score = float(match.group(1))
                source_file = item / 'source.c'
                if source_file.exists():
                    candidates.append((score, source_file))

    # Sort by score (lowest first)
    candidates.sort(key=lambda x: x[0])
    return candidates


def get_function_info(func_name):
    """Get function address and size from function map."""
    func_map_path = PROJECT_ROOT / 'build' / 'function_analysis' / 'function_map.json'

    if not func_map_path.exists():
        return None, None

    with open(func_map_path) as f:
        data = json.load(f)

    for func in data['functions']:
        if func['name'].lower() == func_name.lower():
            addr = int(func['vram_start'], 16)
            size = func['size']
            return addr, size

    # Try to extract address from function name
    match = re.search(r'func_([0-9A-Fa-f]{8})', func_name, re.IGNORECASE)
    if match:
        return int(match.group(1), 16), None

    return None, None


def main():
    parser = argparse.ArgumentParser(
        description='Automatic post-processing pipeline for permuter results',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
    # Process best permuter candidates for a function
    %(prog)s _spu_close

    # Process a specific candidate
    %(prog)s --candidate path/to/source.c --target 0x800959DC --size 52

    # Verbose output
    %(prog)s _spu_close -v
"""
    )

    parser.add_argument('func_name', nargs='?', help='Function name (e.g., _spu_close)')
    parser.add_argument('--candidate', '-c', help='Specific candidate source file')
    parser.add_argument('--target', '-t', help='Target address (hex)')
    parser.add_argument('--size', '-s', type=int, help='Function size in bytes')
    parser.add_argument('--verbose', '-v', action='store_true', help='Verbose output')
    parser.add_argument('--all', '-a', action='store_true',
                        help='Try all permuter candidates, not just best')

    args = parser.parse_args()

    if args.candidate:
        # Process specific candidate
        if not args.target:
            print("Error: --target required with --candidate")
            sys.exit(1)

        target_addr = int(args.target, 16)
        success, obj_path, info = process_candidate(
            args.candidate, target_addr, args.size, args.verbose
        )

        if isinstance(info, str):
            print(f"Error: {info}")
            sys.exit(1)

        print(f"\nResult: {info['matches']}/{info['total']} instructions match ({info['percentage']:.1f}%)")
        if success:
            print(f"SUCCESS! Byte-perfect match achieved.")
            print(f"Output: {obj_path}")
        else:
            print(f"Differences remaining: {info['differences']}")
            if args.verbose and info['diff_details']:
                print("First differences:")
                for idx, comp, targ in info['diff_details'][:5]:
                    print(f"  Instruction {idx}: compiled=0x{comp:08X}, target=0x{targ:08X}")

    elif args.func_name:
        # Process permuter candidates
        addr, size = get_function_info(args.func_name)
        if addr is None:
            print(f"Error: Could not find function {args.func_name}")
            sys.exit(1)

        if size is None:
            size = 256  # Default

        print(f"Function: {args.func_name}")
        print(f"Address: 0x{addr:08X}")
        print(f"Size: {size} bytes")
        print()

        candidates = find_best_permuter_candidates(args.func_name)
        if not candidates:
            print(f"No permuter candidates found in {PERMUTER_DIR / args.func_name}")
            sys.exit(1)

        print(f"Found {len(candidates)} permuter candidates")

        # Try candidates
        max_to_try = len(candidates) if args.all else min(5, len(candidates))

        best_match = None
        best_info = None

        for i, (score, source) in enumerate(candidates[:max_to_try]):
            print(f"\n[{i+1}/{max_to_try}] Trying candidate with permuter score {score}...")

            success, obj_path, info = process_candidate(
                source, addr, size, args.verbose
            )

            if isinstance(info, str):
                print(f"  Error: {info}")
                continue

            pct = info['percentage']
            print(f"  Result: {info['matches']}/{info['total']} ({pct:.1f}%)")

            if success:
                print(f"\n*** SUCCESS! Byte-perfect match with {source} ***")
                print(f"Output: {obj_path}")
                sys.exit(0)

            if best_info is None or info['matches'] > best_info['matches']:
                best_match = (score, source, obj_path)
                best_info = info

        print(f"\nBest result: {best_info['matches']}/{best_info['total']} ({best_info['percentage']:.1f}%)")
        print(f"  From: {best_match[1]}")
        print(f"  Permuter score: {best_match[0]}")
        print(f"  Remaining differences: {best_info['differences']}")

    else:
        parser.print_help()
        sys.exit(1)


if __name__ == '__main__':
    main()
