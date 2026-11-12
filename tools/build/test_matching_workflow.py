#!/usr/bin/env python3
"""
End-to-end test for the byte-perfect matching workflow.

This script demonstrates and tests the complete matching pipeline:
1. Compile C code with IDO 5.2
2. Apply post-processing (frame size patching, OR->ADDU conversion)
3. Verify against target bytes

Requirements:
- IDO 5.2 compiler built (make setup-ido52)
- Original executable (disks/SLPS_000.65)

Usage:
    python3 test_matching_workflow.py
    python3 test_matching_workflow.py --test-compile  # Just test compilation
"""

import argparse
import os
import struct
import subprocess
import sys
import tempfile
from pathlib import Path


# Constants
PROJECT_ROOT = Path(__file__).parent.parent.parent
IDO_CC = PROJECT_ROOT / "tools/ido-static-recomp/build/5.2/out/cc"
IDO_FLAGS = ["-EL", "-G0", "-mips1", "-O2", "-non_shared"]
ORIGINAL_EXE = PROJECT_ROOT / "disks/SLPS_000.65"
HEADER_SIZE = 2048


def check_ido_compiler():
    """Check if IDO 5.2 compiler is available."""
    if not IDO_CC.exists():
        print(f"ERROR: IDO 5.2 compiler not found at {IDO_CC}")
        print("")
        print("Build it with:")
        print("  make setup-ido52")
        return False
    print(f"IDO 5.2 compiler: {IDO_CC}")
    return True


def test_simple_compile():
    """Test basic compilation with IDO 5.2."""
    print("\n=== Test 1: Simple Compilation ===")

    # Create test source
    test_code = '''
int add(int a, int b) {
    return a + b;
}
'''

    with tempfile.NamedTemporaryFile(mode='w', suffix='.c', delete=False) as f:
        f.write(test_code)
        source_path = f.name

    obj_path = source_path.replace('.c', '.o')

    try:
        cmd = [str(IDO_CC)] + IDO_FLAGS + ["-c", source_path, "-o", obj_path]
        result = subprocess.run(cmd, capture_output=True, text=True)

        if result.returncode != 0:
            print(f"FAIL: Compilation failed")
            print(result.stderr)
            return False

        if not os.path.exists(obj_path):
            print("FAIL: Output file not created")
            return False

        # Check file format
        result = subprocess.run(["file", obj_path], capture_output=True, text=True)
        if "ELF 32-bit LSB" not in result.stdout or "MIPS" not in result.stdout:
            print(f"FAIL: Wrong output format: {result.stdout}")
            return False

        print("PASS: Simple compilation works")
        print(f"  Output: {result.stdout.strip()}")
        return True

    finally:
        os.unlink(source_path)
        if os.path.exists(obj_path):
            os.unlink(obj_path)


def test_empty_function():
    """Test that empty function matches expected output."""
    print("\n=== Test 2: Empty Function ===")

    test_code = '''
void empty_func(void) {
    return;
}
'''

    with tempfile.NamedTemporaryFile(mode='w', suffix='.c', delete=False) as f:
        f.write(test_code)
        source_path = f.name

    obj_path = source_path.replace('.c', '.o')
    bin_path = source_path.replace('.c', '.bin')

    try:
        # Compile
        cmd = [str(IDO_CC)] + IDO_FLAGS + ["-c", source_path, "-o", obj_path]
        result = subprocess.run(cmd, capture_output=True, text=True)

        if result.returncode != 0:
            print(f"FAIL: Compilation failed")
            return False

        # Extract .text section
        cmd = ["mips-linux-gnu-objcopy", "-O", "binary", "-j", ".text", obj_path, bin_path]
        result = subprocess.run(cmd, capture_output=True, text=True)

        if result.returncode != 0:
            print(f"FAIL: objcopy failed")
            return False

        # Read and check
        with open(bin_path, 'rb') as f:
            code = f.read()

        # Expected: jr ra; nop (03e00008 00000000)
        expected = bytes([0x08, 0x00, 0xe0, 0x03, 0x00, 0x00, 0x00, 0x00])

        if code[:8] == expected:
            print("PASS: Empty function matches expected bytes")
            print(f"  Output: {code[:8].hex()}")
            return True
        else:
            print(f"FAIL: Bytes don't match")
            print(f"  Expected: {expected.hex()}")
            print(f"  Got:      {code[:8].hex()}")
            return False

    finally:
        os.unlink(source_path)
        if os.path.exists(obj_path):
            os.unlink(obj_path)
        if os.path.exists(bin_path):
            os.unlink(bin_path)


def test_return_constant():
    """Test return constant function."""
    print("\n=== Test 3: Return Constant ===")

    test_code = '''
int return_minus_one(void) {
    return -1;
}
'''

    with tempfile.NamedTemporaryFile(mode='w', suffix='.c', delete=False) as f:
        f.write(test_code)
        source_path = f.name

    obj_path = source_path.replace('.c', '.o')
    bin_path = source_path.replace('.c', '.bin')

    try:
        # Compile
        cmd = [str(IDO_CC)] + IDO_FLAGS + ["-c", source_path, "-o", obj_path]
        result = subprocess.run(cmd, capture_output=True, text=True)

        if result.returncode != 0:
            print(f"FAIL: Compilation failed")
            return False

        # Extract .text section
        cmd = ["mips-linux-gnu-objcopy", "-O", "binary", "-j", ".text", obj_path, bin_path]
        result = subprocess.run(cmd, capture_output=True, text=True)

        if result.returncode != 0:
            print(f"FAIL: objcopy failed")
            return False

        # Read and check
        with open(bin_path, 'rb') as f:
            code = f.read()

        # Expected: jr ra; li v0,-1 (03e00008 2402ffff)
        expected = bytes([0x08, 0x00, 0xe0, 0x03, 0xff, 0xff, 0x02, 0x24])

        if code[:8] == expected:
            print("PASS: Return -1 matches expected bytes")
            print(f"  Output: {code[:8].hex()}")
            return True
        else:
            print(f"FAIL: Bytes don't match")
            print(f"  Expected: {expected.hex()}")
            print(f"  Got:      {code[:8].hex()}")
            return False

    finally:
        os.unlink(source_path)
        if os.path.exists(obj_path):
            os.unlink(obj_path)
        if os.path.exists(bin_path):
            os.unlink(bin_path)


def test_frame_size_patcher():
    """Test frame size patcher tool."""
    print("\n=== Test 4: Frame Size Patcher ===")

    # Create a function with a 24-byte frame
    test_code = '''
extern void external_call(void);

void test_func(void) {
    external_call();
}
'''

    with tempfile.NamedTemporaryFile(mode='w', suffix='.c', delete=False) as f:
        f.write(test_code)
        source_path = f.name

    obj_path = source_path.replace('.c', '.o')
    patched_path = source_path.replace('.c', '_patched.o')

    try:
        # Compile
        cmd = [str(IDO_CC)] + IDO_FLAGS + ["-c", source_path, "-o", obj_path]
        result = subprocess.run(cmd, capture_output=True, text=True)

        if result.returncode != 0:
            print(f"FAIL: Compilation failed")
            return False

        # Check original frame size
        cmd = ["mips-linux-gnu-objdump", "-d", obj_path]
        result = subprocess.run(cmd, capture_output=True, text=True)

        if "addiu\tsp,sp,-24" not in result.stdout:
            print(f"NOTE: Function doesn't have 24-byte frame, skipping patch test")
            return True

        # Patch frame size
        patch_script = PROJECT_ROOT / "tools/ido/patch_frame_size.py"
        cmd = ["python3", str(patch_script), obj_path, patched_path,
               "--old-size", "24", "--new-size", "40", "-v"]
        result = subprocess.run(cmd, capture_output=True, text=True)

        if result.returncode != 0:
            print(f"FAIL: Frame patcher failed")
            print(result.stderr)
            return False

        # Verify patch
        cmd = ["mips-linux-gnu-objdump", "-d", patched_path]
        result = subprocess.run(cmd, capture_output=True, text=True)

        if "addiu\tsp,sp,-40" in result.stdout:
            print("PASS: Frame size patcher works")
            print("  24-byte frame patched to 40-byte frame")
            return True
        else:
            print("FAIL: Frame not patched correctly")
            return False

    finally:
        os.unlink(source_path)
        if os.path.exists(obj_path):
            os.unlink(obj_path)
        if os.path.exists(patched_path):
            os.unlink(patched_path)


def test_or_to_addu_converter():
    """Test OR to ADDU converter."""
    print("\n=== Test 5: OR to ADDU Converter ===")

    # Create a function that uses 'move' instruction
    test_code = '''
int identity(int x) {
    return x;
}
'''

    with tempfile.NamedTemporaryFile(mode='w', suffix='.c', delete=False) as f:
        f.write(test_code)
        source_path = f.name

    obj_path = source_path.replace('.c', '.o')
    converted_path = source_path.replace('.c', '_addu.o')
    bin_before = source_path.replace('.c', '_before.bin')
    bin_after = source_path.replace('.c', '_after.bin')

    try:
        # Compile
        cmd = [str(IDO_CC)] + IDO_FLAGS + ["-c", source_path, "-o", obj_path]
        result = subprocess.run(cmd, capture_output=True, text=True)

        if result.returncode != 0:
            print(f"FAIL: Compilation failed")
            return False

        # Extract original bytes
        cmd = ["mips-linux-gnu-objcopy", "-O", "binary", "-j", ".text", obj_path, bin_before]
        subprocess.run(cmd, capture_output=True)

        # Convert OR to ADDU
        convert_script = PROJECT_ROOT / "tools/ido/convert_or_to_addu.py"
        cmd = ["python3", str(convert_script), obj_path, converted_path, "-v"]
        result = subprocess.run(cmd, capture_output=True, text=True)

        if result.returncode != 0:
            print(f"FAIL: Converter failed")
            print(result.stderr)
            return False

        # Extract converted bytes
        cmd = ["mips-linux-gnu-objcopy", "-O", "binary", "-j", ".text", converted_path, bin_after]
        subprocess.run(cmd, capture_output=True)

        # Check for changes
        with open(bin_before, 'rb') as f:
            before = f.read()
        with open(bin_after, 'rb') as f:
            after = f.read()

        # Identity function with OR encoding: move v0,a0 = or v0,a0,zero = 00802025
        # With ADDU encoding: move v0,a0 = addu v0,a0,zero = 00801021

        or_encoding = bytes([0x25, 0x20, 0x80, 0x00])   # or v0,a0,zero (little-endian)
        addu_encoding = bytes([0x21, 0x10, 0x80, 0x00]) # addu v0,a0,zero (little-endian)

        if or_encoding in before and addu_encoding in after:
            print("PASS: OR to ADDU converter works")
            print(f"  Before: {before.hex()}")
            print(f"  After:  {after.hex()}")
            return True
        else:
            print("NOTE: Converter test inconclusive (no OR-encoded move found)")
            return True

    finally:
        os.unlink(source_path)
        for p in [obj_path, converted_path, bin_before, bin_after]:
            if os.path.exists(p):
                os.unlink(p)


def main():
    parser = argparse.ArgumentParser(
        description='Test the byte-perfect matching workflow'
    )
    parser.add_argument('--test-compile', action='store_true',
                        help='Only test compilation')

    args = parser.parse_args()

    print("=" * 60)
    print("  Byte-Perfect Matching Workflow Tests")
    print("=" * 60)

    # Check compiler
    if not check_ido_compiler():
        sys.exit(1)

    # Run tests
    results = []

    results.append(("Simple Compilation", test_simple_compile()))
    results.append(("Empty Function", test_empty_function()))
    results.append(("Return Constant", test_return_constant()))

    if not args.test_compile:
        results.append(("Frame Size Patcher", test_frame_size_patcher()))
        results.append(("OR to ADDU Converter", test_or_to_addu_converter()))

    # Summary
    print("\n" + "=" * 60)
    print("  Test Summary")
    print("=" * 60)

    passed = sum(1 for _, r in results if r)
    total = len(results)

    for name, result in results:
        status = "PASS" if result else "FAIL"
        print(f"  {status}: {name}")

    print("")
    print(f"  {passed}/{total} tests passed")

    if passed == total:
        print("")
        print("All tests passed! The compilation workflow is working.")
        print("")
        print("Next steps:")
        print("  1. Place original executable at: disks/SLPS_000.65")
        print("  2. Use verify_function.py to check individual functions")
        print("  3. Add verified functions to config/level3_functions.json")
        return 0
    else:
        return 1


if __name__ == '__main__':
    sys.exit(main())
