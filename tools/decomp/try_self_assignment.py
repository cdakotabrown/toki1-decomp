#!/usr/bin/env python3
"""
try_self_assignment.py - Batch test self-assignment pattern on WIP files

This script:
1. Finds WIP files with global variable access patterns
2. Transforms `temp = D_GLOBAL;` to `temp = (D_GLOBAL = D_GLOBAL);`
3. Compiles and compares against target binary
4. Reports improvements

Usage:
    python3 tools/decomp/try_self_assignment.py [--dry-run] [--limit N] [--verbose]
    python3 tools/decomp/try_self_assignment.py --file src/main/wip/func_XXXXX.c
"""

import argparse
import os
import re
import subprocess
import sys
import tempfile
import shutil
from pathlib import Path

# Project paths
PROJECT_ROOT = Path(__file__).parent.parent.parent
WIP_DIR = PROJECT_ROOT / "src/main/wip"
EXE_PATH = PROJECT_ROOT / "disks/SLPS_000.65"
IDO_CC = PROJECT_ROOT / "tools/ido-static-recomp/build/5.2/out/cc"
PATCH_FRAME = PROJECT_ROOT / "tools/ido/patch_frame_size.py"
INCLUDE_DIR = PROJECT_ROOT / "include"

# Pattern to find global variable reads
GLOBAL_READ_PATTERN = re.compile(
    r'(\w+)\s*=\s*(D_80[0-9A-Fa-f]{5,6})\s*;'
)

# Pattern to find already self-assigned
SELF_ASSIGN_PATTERN = re.compile(
    r'\(\s*(D_80[0-9A-Fa-f]+)\s*=\s*\1\s*\)'
)

def extract_address_from_filename(filename):
    """Extract function address from filename like Fade_In_Main.c"""
    match = re.search(r'func_(80[0-9A-Fa-f]{6})', filename)
    if match:
        return int(match.group(1), 16)
    return None

def get_function_info(address):
    """Get function size from function map or analysis"""
    func_map = PROJECT_ROOT / "build/function_analysis/function_map.json"
    if func_map.exists():
        import json
        with open(func_map) as f:
            data = json.load(f)
            for func in data.get('functions', []):
                if func.get('address') == f"0x{address:08X}" or func.get('address') == f"0x{address:08x}":
                    return func.get('size', 0)
    return 0

def get_target_bytes(address, size):
    """Extract target bytes from original executable"""
    if not EXE_PATH.exists():
        return None

    # Calculate file offset (address - load_address + header_offset)
    load_address = 0x80041000
    header_offset = 0x800  # PS-X EXE header
    file_offset = (address - load_address) + header_offset

    with open(EXE_PATH, 'rb') as f:
        f.seek(file_offset)
        return f.read(size)

def compile_and_extract(source_code, temp_dir):
    """Compile source code and extract .text section bytes"""
    src_file = Path(temp_dir) / "test.c"
    obj_file = Path(temp_dir) / "test.o"
    patched_file = Path(temp_dir) / "test_patched.o"
    bin_file = Path(temp_dir) / "test.bin"

    # Write source
    with open(src_file, 'w') as f:
        f.write(source_code)

    # Compile with IDO
    try:
        result = subprocess.run(
            [str(IDO_CC), "-O2", "-G0", "-mips1", "-EL", "-non_shared",
             "-c", str(src_file), "-o", str(obj_file), f"-I{INCLUDE_DIR}"],
            capture_output=True,
            timeout=30
        )
        if result.returncode != 0:
            return None, f"Compile error: {result.stderr.decode()}"
    except Exception as e:
        return None, f"Compile exception: {e}"

    # Try frame size patches (24->40 is most common)
    for old_size, new_size in [(24, 40), (32, 48), (24, 32), (40, 56)]:
        try:
            subprocess.run(
                [sys.executable, str(PATCH_FRAME), str(obj_file), str(patched_file),
                 "--old", str(old_size), "--new", str(new_size)],
                capture_output=True,
                timeout=10
            )
            if patched_file.exists():
                break
        except:
            continue

    # Use patched if available, otherwise original
    final_obj = patched_file if patched_file.exists() else obj_file

    # Extract .text section
    try:
        subprocess.run(
            ["mipsel-linux-gnu-objcopy", "-O", "binary", "-j", ".text",
             str(final_obj), str(bin_file)],
            capture_output=True,
            timeout=10
        )
        if bin_file.exists():
            with open(bin_file, 'rb') as f:
                return f.read(), None
    except Exception as e:
        return None, f"Extract error: {e}"

    return None, "Failed to extract binary"

def compare_bytes_masked(compiled, target):
    """Compare bytes, masking relocation fields (addresses)"""
    if len(compiled) != len(target):
        return len(compiled) * 100 + abs(len(compiled) - len(target)) * 50

    score = 0
    for i in range(0, len(compiled), 4):
        if i + 4 > len(compiled):
            break

        c_word = int.from_bytes(compiled[i:i+4], 'little')
        t_word = int.from_bytes(target[i:i+4], 'little')

        # Mask relocation fields (lui, lw, sw, etc. immediate fields)
        c_opcode = (c_word >> 26) & 0x3F
        t_opcode = (t_word >> 26) & 0x3F

        # For load/store and lui instructions, mask the immediate
        if c_opcode in [0x0F, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2B]:  # lui, lw, lbu, lhu, sb, sh, sw
            c_word &= 0xFFFF0000
            t_word &= 0xFFFF0000

        # For jal, mask the target
        if c_opcode == 0x03:  # jal
            c_word &= 0xFC000000
            t_word &= 0xFC000000

        if c_word != t_word:
            score += 100

    return score

def apply_self_assignment(source_code):
    """Transform global reads to self-assignment pattern"""
    # Skip if already has self-assignment
    if SELF_ASSIGN_PATTERN.search(source_code):
        return source_code, False

    modified = False

    def replace_global_read(match):
        nonlocal modified
        var_name = match.group(1)
        global_name = match.group(2)
        modified = True
        return f"{var_name} = ({global_name} = {global_name});"

    new_code = GLOBAL_READ_PATTERN.sub(replace_global_read, source_code)
    return new_code, modified

def process_file(filepath, dry_run=False, verbose=False):
    """Process a single WIP file"""
    filepath = Path(filepath)

    if not filepath.exists():
        return None, "File not found"

    address = extract_address_from_filename(filepath.name)
    if not address:
        return None, "Cannot extract address from filename"

    # Determine region (compiler boundary at 0x80086000)
    is_ido_region = address >= 0x80086000

    # Read source
    with open(filepath) as f:
        original_code = f.read()

    # Apply transformation
    transformed_code, was_modified = apply_self_assignment(original_code)

    if not was_modified:
        return None, "No applicable patterns found"

    if dry_run:
        if verbose:
            print(f"Would transform {filepath.name}")
            # Show diff
            for i, (orig_line, new_line) in enumerate(zip(original_code.split('\n'), transformed_code.split('\n'))):
                if orig_line != new_line:
                    print(f"  - {orig_line}")
                    print(f"  + {new_line}")
        return None, "Dry run - no changes made"

    # Get function size
    size = get_function_info(address)
    if size == 0:
        # Try to estimate from file analysis
        size = 256  # Default estimate

    # Get target bytes
    target_bytes = get_target_bytes(address, size)
    if not target_bytes:
        return None, "Cannot get target bytes"

    with tempfile.TemporaryDirectory() as temp_dir:
        # Compile original
        orig_bytes, orig_error = compile_and_extract(original_code, temp_dir)
        if orig_bytes is None:
            return None, f"Original compile failed: {orig_error}"

        orig_score = compare_bytes_masked(orig_bytes, target_bytes[:len(orig_bytes)])

        # Compile transformed
        trans_bytes, trans_error = compile_and_extract(transformed_code, temp_dir)
        if trans_bytes is None:
            return None, f"Transformed compile failed: {trans_error}"

        trans_score = compare_bytes_masked(trans_bytes, target_bytes[:len(trans_bytes)])

        improvement = orig_score - trans_score

        result = {
            'file': filepath.name,
            'address': f"0x{address:08X}",
            'orig_score': orig_score,
            'trans_score': trans_score,
            'improvement': improvement,
            'is_match': trans_score == 0,
            'is_ido_region': is_ido_region
        }

        return result, None

def main():
    parser = argparse.ArgumentParser(description='Batch test self-assignment pattern')
    parser.add_argument('--dry-run', action='store_true', help='Show what would be changed')
    parser.add_argument('--limit', type=int, default=50, help='Max files to process')
    parser.add_argument('--file', type=str, help='Process single file')
    parser.add_argument('--verbose', '-v', action='store_true', help='Verbose output')
    parser.add_argument('--ido-only', action='store_true', help='Only process IDO region files')
    args = parser.parse_args()

    if args.file:
        result, error = process_file(args.file, args.dry_run, args.verbose)
        if error:
            print(f"Error: {error}")
        elif result:
            print(f"File: {result['file']}")
            print(f"Original score: {result['orig_score']}")
            print(f"Transformed score: {result['trans_score']}")
            print(f"Improvement: {result['improvement']}")
            if result['is_match']:
                print("*** BYTE-PERFECT MATCH! ***")
        return

    # Find all WIP files with global patterns
    print("Scanning WIP files for self-assignment candidates...")

    candidates = []
    for wip_file in sorted(WIP_DIR.glob("func_*.c")):
        with open(wip_file) as f:
            content = f.read()

        # Check if has global read pattern and not already self-assigned
        if GLOBAL_READ_PATTERN.search(content) and not SELF_ASSIGN_PATTERN.search(content):
            address = extract_address_from_filename(wip_file.name)
            if address:
                is_ido = address >= 0x80086000  # Compiler boundary
                if args.ido_only and not is_ido:
                    continue
                candidates.append((wip_file, is_ido))

    print(f"Found {len(candidates)} candidates")

    if args.dry_run:
        print("\nDry run - showing candidates:")
        for wip_file, is_ido in candidates[:args.limit]:
            region = "IDO" if is_ido else "GCC"
            print(f"  {wip_file.name} ({region})")
        return

    # Process files
    results = []
    improvements = []
    matches = []

    for i, (wip_file, is_ido) in enumerate(candidates[:args.limit]):
        if args.verbose:
            print(f"[{i+1}/{min(len(candidates), args.limit)}] Processing {wip_file.name}...")

        result, error = process_file(wip_file, dry_run=False, verbose=args.verbose)

        if result:
            results.append(result)
            if result['improvement'] > 0:
                improvements.append(result)
            if result['is_match']:
                matches.append(result)
                print(f"  *** MATCH: {result['file']} ***")
        elif args.verbose and error:
            print(f"  Skip: {error}")

    # Summary
    print(f"\n{'='*60}")
    print("RESULTS SUMMARY")
    print(f"{'='*60}")
    print(f"Files processed: {len(results)}")
    print(f"Improvements found: {len(improvements)}")
    print(f"Byte-perfect matches: {len(matches)}")

    if improvements:
        print(f"\nTop improvements:")
        for r in sorted(improvements, key=lambda x: x['improvement'], reverse=True)[:10]:
            region = "IDO" if r['is_ido_region'] else "GCC"
            print(f"  {r['file']} ({region}): {r['orig_score']} -> {r['trans_score']} (improved by {r['improvement']})")

    if matches:
        print(f"\n*** BYTE-PERFECT MATCHES ***")
        for r in matches:
            print(f"  {r['file']} - READY TO PROMOTE!")

if __name__ == "__main__":
    main()
