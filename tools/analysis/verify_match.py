#!/usr/bin/env python3
"""
verify_match.py - Verify byte-perfect function matches for Level 3 decompilation

Compares compiled C function assembly against the original binary to verify
exact byte-level matches. Essential for achieving Level 3 (byte-perfect) matches.

Features:
- Extract specific function from original SLPM_86.053 by address
- Extract compiled function from object file
- Byte-by-byte comparison with detailed diff output
- Percentage similarity for non-matching functions
- Integration with asm-differ for visual comparison

Usage:
    python3 tools/verify_match.py func_8004B338
    python3 tools/verify_match.py func_8004B338 --verbose
    python3 tools/verify_match.py func_8004B338 --asm-differ
    python3 tools/verify_match.py --all  # Verify all decompiled functions
"""

import argparse
import json
import subprocess
import sys
import os
import struct
from pathlib import Path
from typing import Dict, List, Tuple, Optional

# Project paths
ROOT_DIR = Path(__file__).parent.parent
ORIGINAL_EXE = ROOT_DIR / "disks" / "SLPM_86.053"
FUNCTION_MAP = ROOT_DIR / "build" / "function_analysis" / "function_map.json"
DECOMP_CONFIG = ROOT_DIR / "config" / "decompiled_functions.json"
BUILD_DIR = ROOT_DIR / "build" / "jp"
ASM_DIR = ROOT_DIR / "build" / "function_analysis" / "asm"

# Constants
TEXT_SECTION_START = 0x80041000  # VRAM start of .text
FILE_OFFSET_BASE = 0x800  # Header size (first 0x800 bytes are PSX-EXE header)


def load_function_map() -> Dict:
    """Load the function map containing all function metadata."""
    if not FUNCTION_MAP.exists():
        print(f"Error: Function map not found at {FUNCTION_MAP}")
        print("Run: make analyze")
        sys.exit(1)

    with open(FUNCTION_MAP, 'r') as f:
        return json.load(f)


def load_decomp_config() -> Dict:
    """Load the decompiled functions configuration."""
    if not DECOMP_CONFIG.exists():
        print(f"Error: Decompiled functions config not found at {DECOMP_CONFIG}")
        sys.exit(1)

    with open(DECOMP_CONFIG, 'r') as f:
        return json.load(f)


def get_function_info(func_name: str, function_map: Dict) -> Optional[Dict]:
    """Get function metadata from the function map."""
    for func in function_map.get('functions', []):
        if func['name'] == func_name:
            return func
    return None


def vram_to_file_offset(vram_addr: int) -> int:
    """Convert VRAM address to file offset."""
    return (vram_addr - TEXT_SECTION_START) + FILE_OFFSET_BASE


def extract_original_bytes(vram_start: int, size: int) -> bytes:
    """Extract function bytes from the original executable."""
    if not ORIGINAL_EXE.exists():
        print(f"Error: Original executable not found at {ORIGINAL_EXE}")
        sys.exit(1)

    file_offset = vram_to_file_offset(vram_start)

    with open(ORIGINAL_EXE, 'rb') as f:
        f.seek(file_offset)
        return f.read(size)


def compile_function(func_name: str, source_file: str) -> Optional[bytes]:
    """
    Compile a C source file and extract the function bytes.

    Returns the compiled bytes for the function, or None if compilation fails.
    """
    source_path = ROOT_DIR / source_file
    if not source_path.exists():
        print(f"Error: Source file not found: {source_path}")
        return None

    # Create temp build directory
    temp_dir = BUILD_DIR / "verify_temp"
    temp_dir.mkdir(parents=True, exist_ok=True)

    # Compile C to assembly
    asm_file = temp_dir / f"{func_name}.s"
    obj_file = temp_dir / f"{func_name}.o"

    # Run preprocessor + cc1-psx-26
    cpp_cmd = [
        "cpp",
        "-I", str(ROOT_DIR / "include"),
        "-Iinclude",
        "-undef", "-Wall", "-lang-c", "-fno-builtin",
        str(source_path)
    ]

    cc1_cmd = [
        str(ROOT_DIR / "tools" / "psyq-gcc-260" / "cc1-psx-26"),
        "-G0", "-O2", "-funsigned-char", "-fpeephole",
        "-ffunction-cse", "-fpcc-struct-return", "-fcommon",
        "-fverbose-asm", "-msoft-float",
        "-o", str(asm_file)
    ]

    try:
        # Run cpp | cc1
        cpp_proc = subprocess.Popen(cpp_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        cc1_proc = subprocess.Popen(cc1_cmd, stdin=cpp_proc.stdout,
                                     stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        cpp_proc.stdout.close()
        _, cc1_err = cc1_proc.communicate()

        if cc1_proc.returncode != 0:
            print(f"Compilation error: {cc1_err.decode()}")
            return None

        # Assemble (must run from project root for include paths)
        as_cmd = [
            "mipsel-linux-gnu-as",
            "-march=r3000", "-O1", "-G0",
            "-I", str(ROOT_DIR / "include"),
            "-o", str(obj_file),
            str(asm_file)
        ]

        result = subprocess.run(as_cmd, capture_output=True, cwd=str(ROOT_DIR))
        if result.returncode != 0:
            print(f"Assembly error: {result.stderr.decode()}")
            return None

        # Extract .text section from object file
        objcopy_cmd = [
            "mipsel-linux-gnu-objcopy",
            "-O", "binary",
            "-j", ".text",
            str(obj_file),
            str(temp_dir / f"{func_name}.bin")
        ]

        result = subprocess.run(objcopy_cmd, capture_output=True)
        if result.returncode != 0:
            print(f"objcopy error: {result.stderr.decode()}")
            return None

        # Read the binary
        bin_file = temp_dir / f"{func_name}.bin"
        if bin_file.exists():
            with open(bin_file, 'rb') as f:
                return f.read()

        return None

    except Exception as e:
        print(f"Compilation failed: {e}")
        return None


def disassemble_bytes(data: bytes, base_addr: int = 0) -> List[str]:
    """Disassemble raw bytes using objdump."""
    import tempfile

    with tempfile.NamedTemporaryFile(suffix='.bin', delete=False) as tmp:
        tmp.write(data)
        tmp_path = tmp.name

    try:
        cmd = [
            "mipsel-linux-gnu-objdump",
            "-D", "-b", "binary", "-m", "mips:3000",
            "--adjust-vma", hex(base_addr),
            tmp_path
        ]
        result = subprocess.run(cmd, capture_output=True, text=True)

        # Parse output
        lines = []
        for line in result.stdout.split('\n'):
            if ':' in line and '\t' in line:
                # Extract just the instruction part
                parts = line.split('\t')
                if len(parts) >= 2:
                    lines.append(line.strip())

        return lines
    finally:
        os.unlink(tmp_path)


def compare_bytes(original: bytes, compiled: bytes) -> Tuple[bool, float, List[Tuple[int, int, int]]]:
    """
    Compare two byte sequences.

    Returns:
        - match: True if bytes are identical
        - similarity: Percentage of matching bytes (0.0 to 100.0)
        - differences: List of (offset, orig_byte, compiled_byte) tuples
    """
    if original == compiled:
        return True, 100.0, []

    # Find differences
    differences = []
    min_len = min(len(original), len(compiled))
    max_len = max(len(original), len(compiled))

    for i in range(min_len):
        if original[i] != compiled[i]:
            differences.append((i, original[i], compiled[i]))

    # Handle length differences
    if len(original) > len(compiled):
        for i in range(len(compiled), len(original)):
            differences.append((i, original[i], None))
    elif len(compiled) > len(original):
        for i in range(len(original), len(compiled)):
            differences.append((i, None, compiled[i]))

    # Calculate similarity
    matching_bytes = max_len - len(differences)
    similarity = (matching_bytes / max_len) * 100.0 if max_len > 0 else 0.0

    return False, similarity, differences


def run_asm_differ(func_name: str, vram_start: int, original_bytes: bytes, compiled_bytes: bytes):
    """Run asm-differ for visual comparison."""
    # Check if asm-differ is available
    asm_differ = ROOT_DIR / "tools" / "asm-differ" / "diff.py"
    if not asm_differ.exists():
        print("asm-differ not found. Install with: pip install asm-differ")
        return

    # Create temp files with disassembly
    import tempfile

    orig_disasm = disassemble_bytes(original_bytes, vram_start)
    comp_disasm = disassemble_bytes(compiled_bytes, vram_start)

    print("\n" + "=" * 80)
    print("ASSEMBLY COMPARISON (asm-differ style)")
    print("=" * 80)
    print(f"{'Original':^40} | {'Compiled':^40}")
    print("-" * 80)

    max_lines = max(len(orig_disasm), len(comp_disasm))
    for i in range(max_lines):
        orig_line = orig_disasm[i] if i < len(orig_disasm) else ""
        comp_line = comp_disasm[i] if i < len(comp_disasm) else ""

        # Simple diff coloring
        if orig_line == comp_line:
            prefix = "  "
        else:
            prefix = "! "

        # Truncate lines for display
        orig_display = orig_line[:38] if len(orig_line) > 38 else orig_line
        comp_display = comp_line[:38] if len(comp_line) > 38 else comp_line

        print(f"{prefix}{orig_display:<38} | {comp_display:<38}")


def verify_function(func_name: str, verbose: bool = False, use_asm_differ: bool = False) -> bool:
    """
    Verify that a compiled function matches the original bytes.

    Returns True if it's a perfect match.
    """
    print("=" * 80)
    print(f"VERIFYING: {func_name}")
    print("=" * 80)
    print()

    # Load configurations
    function_map = load_function_map()
    decomp_config = load_decomp_config()

    # Get function info from map
    func_info = get_function_info(func_name, function_map)
    if not func_info:
        print(f"Error: Function '{func_name}' not found in function map")
        return False

    # Get decompiled function info
    decomp_func = None
    for func in decomp_config.get('functions', []):
        if func['name'] == func_name:
            decomp_func = func
            break

    if not decomp_func:
        print(f"Error: Function '{func_name}' not in decompiled_functions.json")
        print("Add it to config/decompiled_functions.json first.")
        return False

    # Extract info
    vram_start = int(func_info['vram_start'], 16)
    size = func_info['size']
    source_file = decomp_func['source_file']
    status = decomp_func.get('status', 'unknown')

    print(f"Function:    {func_name}")
    print(f"VRAM:        0x{vram_start:08X}")
    print(f"Size:        {size} bytes ({size // 4} instructions)")
    print(f"Source:      {source_file}")
    print(f"Status:      {status}")
    print()

    # Extract original bytes
    print("Extracting original bytes...")
    original_bytes = extract_original_bytes(vram_start, size)

    # Compile and extract compiled bytes
    print("Compiling C source...")
    compiled_bytes = compile_function(func_name, source_file)

    if compiled_bytes is None:
        print("\nCompilation FAILED")
        return False

    print(f"\nOriginal size:  {len(original_bytes)} bytes")
    print(f"Compiled size:  {len(compiled_bytes)} bytes")
    print()

    # Compare
    match, similarity, differences = compare_bytes(original_bytes, compiled_bytes)

    if match:
        print("=" * 40)
        print("  PERFECT MATCH! (Level 3)")
        print("=" * 40)
        print()
        print("This function is byte-perfect!")
        return True

    # Not a perfect match
    print("=" * 40)
    print(f"  NOT MATCHING ({similarity:.1f}% similar)")
    print("=" * 40)
    print()

    if len(original_bytes) != len(compiled_bytes):
        print(f"SIZE MISMATCH: {len(original_bytes)} vs {len(compiled_bytes)} bytes")
        size_diff = len(compiled_bytes) - len(original_bytes)
        print(f"Compiled is {size_diff:+d} bytes different")
        print()

    # Show differences
    print(f"Found {len(differences)} differing bytes:")
    print()

    display_count = min(len(differences), 20 if not verbose else 100)

    print("  Offset   Original   Compiled")
    print("  " + "-" * 35)

    for i in range(display_count):
        offset, orig, comp = differences[i]
        orig_str = f"0x{orig:02X}" if orig is not None else "----"
        comp_str = f"0x{comp:02X}" if comp is not None else "----"
        print(f"  0x{offset:04X}    {orig_str}       {comp_str}")

    if len(differences) > display_count:
        print(f"  ... and {len(differences) - display_count} more differences")

    # Show assembly comparison if requested
    if use_asm_differ or verbose:
        run_asm_differ(func_name, vram_start, original_bytes, compiled_bytes)

    # Analysis hints
    print()
    print("ANALYSIS HINTS:")

    if len(original_bytes) > len(compiled_bytes):
        print("- Original is larger: may have extra stack frame or nops")
        print("- Try different optimization flags (-O1 vs -O2)")
    elif len(compiled_bytes) > len(original_bytes):
        print("- Compiled is larger: too much code generated")
        print("- Look for unnecessary operations")

    if len(differences) <= 8:
        print("- Close match! Try decomp-permuter for instruction reordering")
        print(f"  Run: python3 tools/permute_function.py {func_name}")

    return False


def verify_all_functions(verbose: bool = False) -> Tuple[int, int, int]:
    """
    Verify all decompiled functions.

    Returns: (total, matching, non_matching)
    """
    decomp_config = load_decomp_config()
    functions = decomp_config.get('functions', [])

    if not functions:
        print("No decompiled functions found in config/decompiled_functions.json")
        return 0, 0, 0

    print("=" * 80)
    print(f"VERIFYING ALL DECOMPILED FUNCTIONS ({len(functions)} total)")
    print("=" * 80)
    print()

    matching = 0
    non_matching = 0
    results = []

    for func in functions:
        func_name = func['name']
        is_match = verify_function(func_name, verbose=False)
        results.append((func_name, is_match, func.get('status', 'unknown')))

        if is_match:
            matching += 1
        else:
            non_matching += 1

        print()

    # Summary
    print("=" * 80)
    print("SUMMARY")
    print("=" * 80)
    print()
    print(f"Total functions:    {len(functions)}")
    print(f"Perfect matches:    {matching} (Level 3)")
    print(f"Non-matching:       {non_matching}")
    print()

    print("Function Status:")
    print("-" * 50)
    for name, is_match, status in results:
        status_str = "MATCH (L3)" if is_match else f"Non-match ({status})"
        print(f"  {name}: {status_str}")

    return len(functions), matching, non_matching


def main():
    parser = argparse.ArgumentParser(
        description="Verify byte-perfect function matches for Level 3 decompilation",
        epilog="""
Examples:
    python3 tools/verify_match.py func_8004B338
    python3 tools/verify_match.py func_8004B338 --verbose
    python3 tools/verify_match.py func_8004B338 --asm-differ
    python3 tools/verify_match.py --all
        """,
        formatter_class=argparse.RawDescriptionHelpFormatter
    )

    parser.add_argument(
        'function',
        nargs='?',
        help='Function name to verify (e.g., func_8004B338)'
    )
    parser.add_argument(
        '-v', '--verbose',
        action='store_true',
        help='Show detailed comparison output'
    )
    parser.add_argument(
        '-a', '--asm-differ',
        action='store_true',
        help='Show side-by-side assembly comparison'
    )
    parser.add_argument(
        '--all',
        action='store_true',
        help='Verify all decompiled functions'
    )

    args = parser.parse_args()

    if args.all:
        total, matching, non_matching = verify_all_functions(args.verbose)
        sys.exit(0 if non_matching == 0 else 1)
    elif args.function:
        is_match = verify_function(args.function, args.verbose, args.asm_differ)
        sys.exit(0 if is_match else 1)
    else:
        parser.print_help()
        sys.exit(1)


if __name__ == "__main__":
    main()
