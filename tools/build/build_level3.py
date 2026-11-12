#!/usr/bin/env python3
"""
Build game binary with Level 3 (byte-perfect) matched functions.

This script:
1. Reads the original executable
2. Loads Level 3 function definitions from config
3. Assembles Level 3 functions and patches them into the binary
4. Verifies the output matches the original SHA1

Usage:
    python3 tools/build_level3.py [--verbose]
"""

import json
import struct
import hashlib
import argparse
import subprocess
import tempfile
import os
import sys
from pathlib import Path

# Constants
EXE_HEADER_SIZE = 0x800
TEXT_LOAD_ADDR = 0x80041000
EXPECTED_SHA1 = "e823bd844a8f8fa4d05483b59c66bc54b8393b26"


def addr_to_file_offset(vram_addr):
    """Convert VRAM address to file offset."""
    return EXE_HEADER_SIZE + (vram_addr - TEXT_LOAD_ADDR)


def assemble_function(asm_path, func_name, func_addr, verbose=False):
    """Assemble a single function and return its bytes."""
    if not os.path.exists(asm_path):
        raise FileNotFoundError(f"Assembly file not found: {asm_path}")

    with tempfile.TemporaryDirectory() as tmpdir:
        obj_path = os.path.join(tmpdir, "func.o")
        bin_path = os.path.join(tmpdir, "func.bin")

        # Assemble
        cmd = [
            "mipsel-linux-gnu-as",
            "-march=r3000",
            "-I.", "-Iinclude",
            "-o", obj_path,
            asm_path
        ]
        if verbose:
            print(f"  Assembling: {' '.join(cmd)}")

        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.returncode != 0:
            raise RuntimeError(f"Assembly failed for {func_name}:\n{result.stderr}")

        # Extract .text section
        cmd = [
            "mipsel-linux-gnu-objcopy",
            "-O", "binary",
            "-j", ".text",
            obj_path,
            bin_path
        ]
        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.returncode != 0:
            raise RuntimeError(f"objcopy failed for {func_name}:\n{result.stderr}")

        # Read binary
        with open(bin_path, "rb") as f:
            func_bytes = f.read()

        return func_bytes


def fixup_jal_targets(func_bytes, func_addr, exe_data, file_offset):
    """Fix JAL and J instruction targets based on original binary."""
    result = bytearray(func_bytes)

    for i in range(0, len(func_bytes), 4):
        word = struct.unpack('<I', func_bytes[i:i+4])[0]
        op = (word >> 26) & 0x3F

        if op == 3:  # JAL instruction
            # Get the original JAL target from the exe
            orig_word = struct.unpack('<I', exe_data[file_offset + i:file_offset + i + 4])[0]
            orig_op = (orig_word >> 26) & 0x3F

            if orig_op == 3:  # Original is also JAL
                # Copy the target from original
                result[i:i+4] = exe_data[file_offset + i:file_offset + i + 4]

        elif op == 2:  # J instruction
            # Get the original J target from the exe
            orig_word = struct.unpack('<I', exe_data[file_offset + i:file_offset + i + 4])[0]
            orig_op = (orig_word >> 26) & 0x3F

            if orig_op == 2:  # Original is also J
                # Copy the target from original
                result[i:i+4] = exe_data[file_offset + i:file_offset + i + 4]

    return bytes(result)


def main():
    parser = argparse.ArgumentParser(description='Build with Level 3 matched functions')
    parser.add_argument('--verbose', '-v', action='store_true', help='Verbose output')
    parser.add_argument('--exe', type=str, default='disks/SLPM_86.053', help='Original executable')
    parser.add_argument('--config', type=str, default='config/level3_functions.json', help='Level 3 config')
    parser.add_argument('--output', type=str, default='build/jp/level3/SLPM_86.053', help='Output path')
    args = parser.parse_args()

    print("=" * 70)
    print("LEVEL 3 BUILD - Byte-Perfect Matched Functions")
    print("=" * 70)

    # Load original executable
    print(f"\nLoading original: {args.exe}")
    with open(args.exe, 'rb') as f:
        exe_data = bytearray(f.read())

    orig_sha1 = hashlib.sha1(exe_data).hexdigest()
    print(f"Original SHA1: {orig_sha1}")

    if orig_sha1 != EXPECTED_SHA1:
        print(f"WARNING: Original SHA1 doesn't match expected!")
        print(f"Expected: {EXPECTED_SHA1}")

    # Load Level 3 config
    print(f"\nLoading Level 3 config: {args.config}")
    with open(args.config, 'r') as f:
        config = json.load(f)

    functions = config.get('functions', [])
    print(f"Level 3 functions: {len(functions)}")

    if not functions:
        print("\nNo Level 3 functions configured. Output will match original.")

    # Process each Level 3 function
    patched_count = 0
    for func in functions:
        name = func['name']
        addr = int(func['address'], 16)
        size = func['size']
        source = func['source']
        expected_sha1 = func.get('sha1', '')

        file_offset = addr_to_file_offset(addr)
        print(f"\n  Processing {name}:")
        print(f"    Address: 0x{addr:08X}")
        print(f"    File offset: 0x{file_offset:X}")
        print(f"    Size: {size} bytes")
        print(f"    Source: {source}")

        # Get original bytes for comparison
        orig_bytes = bytes(exe_data[file_offset:file_offset + size])
        orig_func_sha1 = hashlib.sha1(orig_bytes).hexdigest()

        if expected_sha1 and orig_func_sha1 != expected_sha1:
            print(f"    WARNING: Original function SHA1 mismatch!")
            print(f"      Expected: {expected_sha1}")
            print(f"      Got: {orig_func_sha1}")

        # Assemble the function
        try:
            func_bytes = assemble_function(source, name, addr, args.verbose)
        except Exception as e:
            print(f"    ERROR: {e}")
            continue

        if len(func_bytes) < size:
            print(f"    ERROR: Size too small! Expected {size}, got {len(func_bytes)}")
            continue
        elif len(func_bytes) > size:
            # Assembler may add alignment padding - truncate to expected size
            if args.verbose:
                print(f"    Note: Truncating from {len(func_bytes)} to {size} bytes (alignment padding)")
            func_bytes = func_bytes[:size]

        # Fix up JAL targets
        func_bytes = fixup_jal_targets(func_bytes, addr, exe_data, file_offset)

        # Compare with original
        if func_bytes == orig_bytes:
            print(f"    ✓ MATCH! Bytes are identical")
            patched_count += 1
        else:
            # Show differences
            print(f"    ✗ MISMATCH! Comparing bytes:")
            for i in range(0, size, 4):
                orig_word = struct.unpack('<I', orig_bytes[i:i+4])[0]
                new_word = struct.unpack('<I', func_bytes[i:i+4])[0]
                if orig_word != new_word:
                    print(f"      0x{i:04X}: orig={orig_word:08x} new={new_word:08x}")

        # Patch the executable (even if mismatch, for testing)
        exe_data[file_offset:file_offset + size] = func_bytes

    # Write output
    output_path = Path(args.output)
    output_path.parent.mkdir(parents=True, exist_ok=True)

    print(f"\nWriting output: {args.output}")
    with open(args.output, 'wb') as f:
        f.write(exe_data)

    # Verify final SHA1
    final_sha1 = hashlib.sha1(exe_data).hexdigest()
    print(f"\nFinal SHA1: {final_sha1}")

    print("\n" + "=" * 70)
    print("BUILD SUMMARY")
    print("=" * 70)
    print(f"Level 3 functions processed: {len(functions)}")
    print(f"Successfully matched: {patched_count}")
    print(f"Original SHA1:        {orig_sha1}")
    print(f"Output SHA1:          {final_sha1}")

    if final_sha1 == EXPECTED_SHA1:
        print("\n✓ SUCCESS! Output matches original SHA1!")
        return 0
    else:
        print("\n✗ Output SHA1 does not match original")
        print("  This is expected if Level 3 functions have byte differences")
        return 1


if __name__ == '__main__':
    sys.exit(main())
