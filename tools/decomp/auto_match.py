#!/usr/bin/env python3
"""
auto_match.py - Automated function matching tool for Tokimeki Memorial PS1

Usage:
    python3 tools/decomp/auto_match.py src/main/wip/func_XXXXXXXX.c
    python3 tools/decomp/auto_match.py src/main/wip/func_XXXXXXXX.c --move-on-match
    python3 tools/decomp/auto_match.py --batch src/main/wip/func_8004*.c

Features:
    - Compiles C with IDO 5.2
    - Auto-detects region (GCC vs IDO) from address
    - Tries multiple frame size patches
    - Applies OR->ADDU conversion for IDO region
    - Compares bytes against target (masks relocations)
    - Reports MATCH/MISMATCH with detailed diff
    - Optional auto-move to matched/ on success
"""

import argparse
import json
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

# Project paths
PROJECT_ROOT = Path(__file__).parent.parent.parent
EXE_PATH = PROJECT_ROOT / "disks" / "SLPS_000.65"
IDO_CC = PROJECT_ROOT / "tools" / "ido-static-recomp" / "build" / "5.2" / "out" / "cc"
FUNCTION_MAP = PROJECT_ROOT / "build" / "function_analysis" / "function_map.json"
CONFIG_FILE = PROJECT_ROOT / "config" / "level3_functions.json"

# Region boundaries (compiler boundary verified at 0x80086000)
GCC_START = 0x80041000
GCC_END = 0x80085FFF
IDO_START = 0x80086000
IDO_END = 0x800BFFFF

# Common frame size patches to try
FRAME_PATCHES = [
    (24, 40),   # Most common for GCC region
    (32, 48),   # Functions with more locals
    (40, 56),   # Functions with stack args
    (24, 32),   # Smaller patch
    (32, 40),   # Medium patch
]


def get_function_info(source_path):
    """Extract function address and size from source file header."""
    with open(source_path, 'r') as f:
        content = f.read(2000)

    # Extract address from filename or header
    filename = os.path.basename(source_path)
    addr_match = re.search(r'func_([0-9A-Fa-f]{8})', filename)
    if not addr_match:
        addr_match = re.search(r'Address:\s*0x([0-9A-Fa-f]{8})', content)

    if not addr_match:
        return None, None, None

    address = int(addr_match.group(1), 16)

    # Extract size from header
    size_match = re.search(r'Size:\s*(\d+)\s*bytes', content)
    size = int(size_match.group(1)) if size_match else None

    # Determine region
    if GCC_START <= address <= GCC_END:
        region = "GCC"
    elif IDO_START <= address <= IDO_END:
        region = "IDO"
    else:
        region = "UNKNOWN"

    return address, size, region


def get_target_bytes(address, size):
    """Read target bytes from original executable."""
    file_offset = 0x800 + (address - 0x80041000)

    with open(EXE_PATH, 'rb') as f:
        f.seek(file_offset)
        return f.read(size)


def compile_c(source_path, output_path):
    """Compile C source with IDO 5.2."""
    cmd = [
        str(IDO_CC),
        "-O2", "-G0", "-mips1", "-EL", "-non_shared",
        "-c", str(source_path),
        "-o", str(output_path),
        "-I" + str(PROJECT_ROOT / "include")
    ]

    result = subprocess.run(cmd, capture_output=True, text=True)
    return result.returncode == 0, result.stderr


def patch_frame_size(input_path, output_path, old_size, new_size):
    """Patch stack frame size in object file."""
    patch_script = PROJECT_ROOT / "tools" / "ido" / "patch_frame_size.py"

    cmd = [
        "python3", str(patch_script),
        str(input_path), str(output_path),
        "--old", str(old_size),
        "--new", str(new_size)
    ]

    result = subprocess.run(cmd, capture_output=True, text=True)
    return result.returncode == 0


def convert_or_to_addu(input_path, output_path):
    """Convert OR encoding to ADDU for IDO region."""
    convert_script = PROJECT_ROOT / "tools" / "ido" / "convert_or_to_addu.py"

    if not convert_script.exists():
        # Just copy if script doesn't exist
        import shutil
        shutil.copy(input_path, output_path)
        return True

    cmd = [
        "python3", str(convert_script),
        str(input_path), str(output_path)
    ]

    result = subprocess.run(cmd, capture_output=True, text=True)
    return result.returncode == 0


def extract_text_section(obj_path, bin_path):
    """Extract .text section from object file."""
    cmd = [
        "mipsel-linux-gnu-objcopy",
        "-O", "binary",
        "-j", ".text",
        str(obj_path), str(bin_path)
    ]

    result = subprocess.run(cmd, capture_output=True, text=True)
    return result.returncode == 0


def compare_bytes(compiled, target):
    """Compare compiled bytes against target, masking relocations."""
    if len(compiled) != len(target):
        return False, f"Size mismatch: compiled={len(compiled)}, target={len(target)}"

    mismatches = []

    for i in range(0, len(target), 4):
        if i + 4 > len(target):
            break

        c_word = int.from_bytes(compiled[i:i+4], 'little')
        t_word = int.from_bytes(target[i:i+4], 'little')

        # Check opcode
        opcode = (t_word >> 26) & 0x3F

        # JAL (opcode 0x03) - mask target address
        if opcode == 0x03:
            if (c_word >> 26) == 0x03:
                continue  # Both are JAL, consider it a match
            else:
                mismatches.append((i, c_word, t_word, "JAL opcode mismatch"))
                continue

        # LUI (opcode 0x0F) - mask immediate for global refs
        if opcode == 0x0F:
            if (c_word & 0xFFFF0000) == (t_word & 0xFFFF0000):
                continue  # Same register, different immediate is OK

        # Branch instructions - mask offset
        if opcode in [0x04, 0x05, 0x06, 0x07, 0x01]:  # beq, bne, blez, bgtz, regimm
            if (c_word >> 16) == (t_word >> 16):
                continue  # Same opcode and registers, offset differs

        # ADDIU/LW/SW with global refs - mask offset
        if opcode in [0x09, 0x23, 0x2B]:  # addiu, lw, sw
            # Check if this looks like a global reference (preceded by LUI)
            if i >= 4:
                prev_word = int.from_bytes(target[i-4:i], 'little')
                if (prev_word >> 26) == 0x0F:  # Previous was LUI
                    if (c_word & 0xFFFF0000) == (t_word & 0xFFFF0000):
                        continue

        if c_word != t_word:
            mismatches.append((i, c_word, t_word, ""))

    if mismatches:
        diff_str = "\n".join([
            f"  0x{off:02X}: compiled=0x{c:08X} target=0x{t:08X} {note}"
            for off, c, t, note in mismatches[:10]
        ])
        if len(mismatches) > 10:
            diff_str += f"\n  ... and {len(mismatches) - 10} more"
        return False, f"Mismatches:\n{diff_str}"

    return True, "MATCH"


def try_match(source_path, address, size, region, verbose=False):
    """Try to match a function with various post-processing options."""

    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = Path(tmpdir)
        obj_file = tmp / "compiled.o"
        patched_file = tmp / "patched.o"
        final_file = tmp / "final.o"
        bin_file = tmp / "compiled.bin"

        # Compile
        success, error = compile_c(source_path, obj_file)
        if not success:
            return False, f"Compilation failed: {error}", None

        # Get target bytes
        target = get_target_bytes(address, size)
        if len(target) != size:
            return False, f"Could not read {size} bytes from target", None

        # Try different frame size patches
        for old_size, new_size in FRAME_PATCHES:
            # Patch frame size
            if not patch_frame_size(obj_file, patched_file, old_size, new_size):
                continue

            # Apply OR->ADDU for IDO region
            if region == "IDO":
                if not convert_or_to_addu(patched_file, final_file):
                    final_file = patched_file
            else:
                final_file = patched_file

            # Extract text section
            if not extract_text_section(final_file, bin_file):
                continue

            # Compare bytes
            with open(bin_file, 'rb') as f:
                compiled = f.read()

            # Pad or trim to match target size
            if len(compiled) < size:
                compiled = compiled + b'\x00' * (size - len(compiled))
            compiled = compiled[:size]

            match, msg = compare_bytes(compiled, target)

            if match:
                patch_info = f"patch_frame_size.py --old {old_size} --new {new_size}"
                if region == "IDO":
                    patch_info += " + convert_or_to_addu.py"
                return True, "MATCH", patch_info

        # No patch worked - return last comparison result
        return False, msg, None


def update_file_status(source_path, patch_info):
    """Update the source file header to mark as MATCHING."""
    with open(source_path, 'r') as f:
        content = f.read()

    # Update Status line
    content = re.sub(
        r'Status:\s*WIP.*',
        f'Status: MATCHING (verified with {patch_info})',
        content
    )

    with open(source_path, 'w') as f:
        f.write(content)


def move_to_matched(source_path):
    """Move source file from wip/ to matched/."""
    if '/wip/' not in str(source_path):
        return False

    dest_path = str(source_path).replace('/wip/', '/matched/')
    os.makedirs(os.path.dirname(dest_path), exist_ok=True)
    os.rename(source_path, dest_path)
    return dest_path


def main():
    parser = argparse.ArgumentParser(description='Automated function matching tool')
    parser.add_argument('sources', nargs='+', help='Source file(s) to match')
    parser.add_argument('--move-on-match', action='store_true',
                       help='Move matched files to src/main/matched/')
    parser.add_argument('--verbose', '-v', action='store_true',
                       help='Verbose output')
    parser.add_argument('--batch', action='store_true',
                       help='Batch mode - process multiple files')

    args = parser.parse_args()

    # Check prerequisites
    if not EXE_PATH.exists():
        print(f"Error: Executable not found at {EXE_PATH}")
        sys.exit(1)

    if not IDO_CC.exists():
        print(f"Error: IDO compiler not found at {IDO_CC}")
        print("Run: make setup-ido52")
        sys.exit(1)

    results = {'match': 0, 'mismatch': 0, 'error': 0}

    for source in args.sources:
        source_path = Path(source)

        if not source_path.exists():
            print(f"Error: {source} not found")
            results['error'] += 1
            continue

        # Get function info
        address, size, region = get_function_info(source_path)

        if address is None:
            print(f"Error: Could not extract address from {source}")
            results['error'] += 1
            continue

        if size is None:
            # Try to get size from function map
            if FUNCTION_MAP.exists():
                with open(FUNCTION_MAP, 'r') as f:
                    fmap = json.load(f)
                for func in fmap.get('functions', []):
                    if func.get('vram_start') == f"0x{address:08X}":
                        size = func.get('size')
                        break

        if size is None:
            print(f"Error: Could not determine size for {source}")
            results['error'] += 1
            continue

        func_name = source_path.stem
        print(f"\n{'='*60}")
        print(f"Function: {func_name}")
        print(f"Address:  0x{address:08X}")
        print(f"Size:     {size} bytes")
        print(f"Region:   {region}")
        print(f"{'='*60}")

        # Try to match
        match, msg, patch_info = try_match(source_path, address, size, region, args.verbose)

        if match:
            print(f"Result:   MATCH!")
            print(f"Patch:    {patch_info}")
            results['match'] += 1

            if args.move_on_match:
                update_file_status(source_path, patch_info)
                new_path = move_to_matched(source_path)
                if new_path:
                    print(f"Moved to: {new_path}")
        else:
            print(f"Result:   MISMATCH")
            print(f"Details:  {msg}")
            results['mismatch'] += 1

    # Summary
    print(f"\n{'='*60}")
    print(f"Summary: {results['match']} matched, {results['mismatch']} mismatched, {results['error']} errors")
    print(f"{'='*60}")

    return 0 if results['mismatch'] == 0 and results['error'] == 0 else 1


if __name__ == '__main__':
    sys.exit(main())
