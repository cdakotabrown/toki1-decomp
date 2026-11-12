#!/usr/bin/env python3
"""
Test an extracted IDO compiler by compiling test code and analyzing output.

This script:
1. Locates compiler binaries in extracted IDO directory
2. Attempts to compile test code using ido-static-recomp
3. Analyzes the output for key patterns (frame size, move encoding, etc.)

Usage:
    python3 test_ido_compiler.py /path/to/extracted/ido
"""

import sys
import os
import subprocess
import shutil
import tempfile
from pathlib import Path

# Test C code - simple function to check compiler behavior
TEST_CODE = """
/* Test function to analyze IDO code generation */
int test_simple(int arg) {
    return arg + 1;
}

int test_with_call(unsigned short arg) {
    extern int other_func(int);
    return other_func(arg);
}

int test_multiple_args(int a, int b, int c, int d) {
    return a + b + c + d;
}

/* Vararg function to test argument homing */
#include <stdarg.h>
int test_vararg(int count, ...) {
    va_list ap;
    int sum = 0;
    va_start(ap, count);
    while (count-- > 0) {
        sum += va_arg(ap, int);
    }
    va_end(ap);
    return sum;
}
"""

# Simpler test without varargs
SIMPLE_TEST_CODE = """
int test_simple(int arg) {
    return arg + 1;
}

int test_with_call(unsigned short arg) {
    extern int other_func(int);
    return other_func(arg);
}
"""

def find_ido_binaries(ido_dir):
    """Find IDO compiler binaries in extracted directory."""
    binaries = {}
    needed = ['cc', 'cfe', 'ugen', 'uopt', 'as0', 'as1']

    for root, dirs, files in os.walk(ido_dir):
        for name in needed:
            if name in files and name not in binaries:
                full_path = os.path.join(root, name)
                # Check if it's actually executable/binary
                if os.path.getsize(full_path) > 1000:  # Reasonable size for binary
                    binaries[name] = full_path

    return binaries

def get_ido_version_string(binary_path):
    """Extract version string from IDO binary."""
    try:
        result = subprocess.run(
            ['strings', binary_path],
            capture_output=True, text=True
        )
        for line in result.stdout.split('\n'):
            if 'version' in line.lower() or 'Version' in line:
                return line.strip()
            # Also look for ECOFF version
            if line.startswith('3.') and len(line) < 10:
                return f"ECOFF/C version: {line.strip()}"
    except:
        pass
    return "Unknown"

def check_recomp_available():
    """Check if ido-static-recomp is available."""
    recomp_paths = [
        './tools/ido-static-recomp',
        os.path.expanduser('~/ido-static-recomp'),
        '/opt/ido-static-recomp'
    ]

    for path in recomp_paths:
        if os.path.isdir(path):
            # Check for recompiled binaries
            build_dir = os.path.join(path, 'build')
            if os.path.isdir(build_dir):
                return path
    return None

def setup_ido_for_recomp(ido_dir, recomp_dir, version='5.1'):
    """
    Set up extracted IDO binaries for use with ido-static-recomp.
    This creates the necessary directory structure.
    """
    target_dir = os.path.join(recomp_dir, 'ido', version)
    os.makedirs(target_dir, exist_ok=True)

    # Find and copy binaries
    binaries = find_ido_binaries(ido_dir)

    print(f"Setting up IDO {version} in {target_dir}")
    for name, src_path in binaries.items():
        dst_path = os.path.join(target_dir, name)
        print(f"  Copying {name}...")
        shutil.copy2(src_path, dst_path)

    return target_dir

def analyze_assembly(asm_text):
    """Analyze assembly output for key patterns."""
    results = {
        'frame_sizes': [],
        'move_encoding': None,
        'temp_register': None,
        'arg_saves': 0,
        'delay_slot_filled': False
    }

    lines = asm_text.split('\n')

    for i, line in enumerate(lines):
        line = line.strip()

        # Frame size (addiu sp,sp,X)
        if 'addiu' in line and 'sp' in line and 'sp' in line:
            parts = line.replace(',', ' ').split()
            for p in parts:
                if p.startswith('-') or p.isdigit():
                    try:
                        size = abs(int(p))
                        if size > 0 and size not in results['frame_sizes']:
                            results['frame_sizes'].append(size)
                    except:
                        pass

        # Move encoding (addu vs or)
        if 'move' in line or ('$zero' in line and ('addu' in line or 'or' in line)):
            if 'addu' in line:
                results['move_encoding'] = 'addu (IDO style)'
            elif 'or' in line:
                results['move_encoding'] = 'or (GCC style)'

        # Temp register usage
        if '$t6' in line or '$14' in line:
            results['temp_register'] = '$t6'
        elif '$t0' in line or '$8' in line:
            if results['temp_register'] is None:
                results['temp_register'] = '$t0'

        # Argument saves (sw $a0-$a3 to stack)
        if line.startswith('sw') and ('$a0' in line or '$a1' in line or '$a2' in line or '$a3' in line):
            if '($sp)' in line or ',sp)' in line:
                results['arg_saves'] += 1

        # Delay slot (instruction after jal/jr that's not nop)
        if i > 0 and ('jal' in lines[i-1] or 'jr' in lines[i-1]):
            if 'nop' not in line and line:
                results['delay_slot_filled'] = True

    return results

def run_native_test(ido_dir):
    """
    Try to run IDO natively (will fail on x86, but useful for MIPS systems).
    """
    binaries = find_ido_binaries(ido_dir)

    if 'cc' not in binaries:
        print("  cc not found in IDO directory")
        return None

    cc_path = binaries['cc']

    # Try to get version
    try:
        result = subprocess.run(
            [cc_path, '-version'],
            capture_output=True, text=True, timeout=5
        )
        return result.stdout + result.stderr
    except Exception as e:
        return f"Cannot run natively (expected on x86): {e}"

def main():
    if len(sys.argv) < 2:
        print("Usage: test_ido_compiler.py <extracted_ido_dir>")
        print()
        print("This script analyzes extracted IDO compiler binaries")
        print("and attempts to test compilation if ido-static-recomp is available.")
        sys.exit(1)

    ido_dir = Path(sys.argv[1]).resolve()

    if not ido_dir.exists():
        print(f"Error: Directory not found: {ido_dir}")
        sys.exit(1)

    print("=" * 60)
    print("IDO Compiler Analysis")
    print("=" * 60)
    print(f"Directory: {ido_dir}")
    print()

    # Find binaries
    print("=== Searching for compiler binaries ===")
    binaries = find_ido_binaries(ido_dir)

    if not binaries:
        print("No compiler binaries found!")
        print()
        print("The extracted directory may need further processing.")
        print("SGI software often comes in 'inst' packages that need extraction.")
        sys.exit(1)

    print(f"Found {len(binaries)} binaries:")
    for name, path in sorted(binaries.items()):
        size = os.path.getsize(path)
        version = get_ido_version_string(path)
        print(f"  {name}: {path}")
        print(f"    Size: {size:,} bytes")
        print(f"    Version: {version}")
        print()

    # Check specifically for cfe (C frontend) version
    if 'cfe' in binaries:
        print("=== C Frontend (cfe) Analysis ===")
        cfe_path = binaries['cfe']

        # Look for ECOFF version in cfe
        result = subprocess.run(['strings', cfe_path], capture_output=True, text=True)
        ecoff_versions = []
        for line in result.stdout.split('\n'):
            # ECOFF versions are like 3.10, 3.15, 3.18, etc.
            if line.strip().startswith('3.') and len(line.strip()) < 6:
                ecoff_versions.append(line.strip())

        if ecoff_versions:
            print(f"  ECOFF/C compiler versions found: {ecoff_versions}")
            if '3.18' in ecoff_versions:
                print("  *** FOUND C 3.18 - This might be the IDO version we need! ***")
            elif '3.19' in ecoff_versions:
                print("  *** Found C 3.19 - This is newer than target ***")
        else:
            print("  No ECOFF version strings found")
        print()

    # Check for ido-static-recomp
    print("=== ido-static-recomp Check ===")
    recomp_dir = check_recomp_available()

    if recomp_dir:
        print(f"  Found at: {recomp_dir}")
        print()
        print("  To use this IDO version with ido-static-recomp:")
        print(f"    1. Copy binaries to {recomp_dir}/ido/5.1/")
        print("    2. Run: cd {recomp_dir} && ./build.sh 5.1")
        print("    3. Test compilation with the recompiled binaries")
    else:
        print("  Not found. Clone from: https://github.com/decompals/ido-static-recomp")

    # Try native execution (will fail on x86)
    print()
    print("=== Native Execution Test ===")
    native_result = run_native_test(ido_dir)
    print(f"  {native_result}")

    print()
    print("=" * 60)
    print("Summary")
    print("=" * 60)
    print(f"IDO binaries found: {list(binaries.keys())}")

    # Key question: Does cfe have version 3.18?
    if 'cfe' in binaries:
        result = subprocess.run(['strings', binaries['cfe']], capture_output=True, text=True)
        if '3.18' in result.stdout:
            print()
            print("*** IMPORTANT: This IDO version has C compiler 3.18! ***")
            print("This matches the ECOFF version in Tokimeki Memorial's debug symbols.")
            print("This could be the correct compiler version!")
        elif '3.19' in result.stdout:
            print()
            print("This IDO has C compiler 3.19 (IDO 5.3 era)")
        elif '3.15' in result.stdout or '3.10' in result.stdout:
            print()
            print("This IDO has an older C compiler version")

if __name__ == "__main__":
    main()
