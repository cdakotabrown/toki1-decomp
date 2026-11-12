#!/usr/bin/env python3
"""
decompile.py - Automated Decompilation Helper for Tokimeki Memorial

Automates the workflow for decompiling a function:
1. Find function in function_map.json (or by address)
2. Generate context with m2ctx.py
3. Write disassembly to temp file
4. Run m2c to generate initial C code
5. Print helpful next steps

Usage:
    python3 tools/decompile.py func_800ACBC0
    python3 tools/decompile.py 0x800ACBC0
    python3 tools/decompile.py func_800ACBC0 --output-file src/main/new_func.c

Based on Silent Hill decomp project's decompile.py
Adapted for Tokimeki Memorial's objdump-based workflow
"""

import argparse
import json
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Optional, Dict, List

# Project paths
SCRIPT_DIR = Path(__file__).parent
ROOT_DIR = SCRIPT_DIR.parent.parent  # tools/decomp -> tools -> root
FUNCTION_MAP_PATH = ROOT_DIR / "build" / "function_analysis" / "function_map.json"
SRC_DIR = ROOT_DIR / "src"
M2C_DIR = ROOT_DIR / "tools" / "m2c"
M2CTX_PATH = ROOT_DIR / "tools" / "m2ctx.py"
DEFAULT_SRC_FILE = SRC_DIR / "main" / "functions.c"


def load_function_map() -> Dict:
    """Load the function map JSON."""
    if not FUNCTION_MAP_PATH.exists():
        print(f"Error: Function map not found at {FUNCTION_MAP_PATH}")
        print("Run: python3 tools/extract_all_functions.py")
        sys.exit(1)

    with open(FUNCTION_MAP_PATH, 'r') as f:
        return json.load(f)


def find_function(func_map: Dict, name_or_addr: str) -> Optional[Dict]:
    """
    Find a function by name or address.

    Args:
        func_map: The loaded function map
        name_or_addr: Function name (func_800XXXXX) or address (0x800XXXXX)

    Returns:
        Function dict or None if not found
    """
    # Normalize input
    name_or_addr = name_or_addr.strip()

    # Handle address format (0x800XXXXX -> func_800XXXXX)
    if name_or_addr.startswith("0x"):
        addr = int(name_or_addr, 16)
        name_or_addr = f"func_{addr:08X}"

    # Search for function
    for func in func_map['functions']:
        if func['name'].lower() == name_or_addr.lower():
            return func

    return None


def write_disassembly_file(func: Dict, output_path: Path) -> None:
    """
    Write function disassembly to a file in GNU AS format.

    This creates a .s file suitable for m2c input.
    Converts objdump format to proper assembly with labels for branches.
    """
    disasm_lines = func.get('disassembly', [])
    if not disasm_lines:
        print(f"Error: No disassembly data for function {func['name']}")
        sys.exit(1)

    # Get function address
    vram_start = int(func['vram_start'], 16)
    func_name = func['name']

    # First pass: collect all branch targets
    branch_targets = set()

    # Pattern for branches: bXX reg,reg,target or bXX reg,target
    branch_pattern_2reg = re.compile(r'(beq|bne)\s+(\$?\w+),(\$?\w+),(0x[0-9a-fA-F]+|\d+)$')
    branch_pattern_1reg = re.compile(r'(blez|bgtz|bltz|bgez|beqz|bnez)\s+(\$?\w+),(0x[0-9a-fA-F]+|\d+)$')
    branch_pattern_uncond = re.compile(r'\bb\s+(0x[0-9a-fA-F]+|\d+)$')

    for line in disasm_lines:
        parts = line.split('\t')
        if len(parts) >= 3:
            instruction = '\t'.join(parts[2:])
            # Try all patterns
            for pattern in [branch_pattern_2reg, branch_pattern_1reg, branch_pattern_uncond]:
                match = pattern.search(instruction)
                if match:
                    target = match.groups()[-1]  # Last group is always target
                    try:
                        if target.startswith('0x'):
                            branch_targets.add(int(target, 16))
                        else:
                            branch_targets.add(int(target))
                    except ValueError:
                        pass
                    break

    lines = []
    lines.append(f".set noat")
    lines.append(f".set noreorder")
    lines.append(f".section .text")
    lines.append(f"")
    lines.append(f".global {func_name}")
    lines.append(f".type {func_name}, @function")
    lines.append(f"{func_name}:")

    # Convert objdump disassembly to proper MIPS assembly
    for i, line in enumerate(disasm_lines):
        # objdump format: "offset: bytes  instruction"
        # Example: "   0:\td0ffbd27 \taddiu\tsp,sp,-48"
        parts = line.split('\t')
        if len(parts) >= 2:
            # Get offset
            offset_str = parts[0].rstrip(':').strip()
            try:
                offset = int(offset_str, 16)
            except ValueError:
                offset = -1

            # Add label if this is a branch target
            if offset in branch_targets:
                lines.append(f".L{func_name}_{offset:X}:")

            if len(parts) >= 3:
                instruction = '\t'.join(parts[2:])
            else:
                instruction = parts[-1]

            # Replace branch targets with labels
            def replace_target(match, pattern_type):
                groups = match.groups()
                target = groups[-1]
                try:
                    if target.startswith('0x'):
                        target_offset = int(target, 16)
                    else:
                        target_offset = int(target)
                    label = f".L{func_name}_{target_offset:X}"

                    if pattern_type == '2reg':
                        return f"{groups[0]} {groups[1]}, {groups[2]}, {label}"
                    elif pattern_type == '1reg':
                        return f"{groups[0]} {groups[1]}, {label}"
                    else:  # uncond
                        return f"b {label}"
                except ValueError:
                    return match.group(0)

            # Try each pattern
            match = branch_pattern_2reg.search(instruction)
            if match:
                instruction = branch_pattern_2reg.sub(lambda m: replace_target(m, '2reg'), instruction)
            else:
                match = branch_pattern_1reg.search(instruction)
                if match:
                    instruction = branch_pattern_1reg.sub(lambda m: replace_target(m, '1reg'), instruction)
                else:
                    match = branch_pattern_uncond.search(instruction)
                    if match:
                        instruction = branch_pattern_uncond.sub(lambda m: replace_target(m, 'uncond'), instruction)

            lines.append(f"    {instruction}")

    with open(output_path, 'w') as f:
        f.write('\n'.join(lines))


def generate_context(src_file: Path) -> str:
    """Generate m2c context from source file."""
    if not src_file.exists():
        print(f"Warning: Source file {src_file} not found")
        print("Generating minimal context...")
        return ""

    # Use m2ctx.py to generate context
    try:
        import m2ctx
        return m2ctx.import_c_file(str(src_file))
    except ImportError:
        # Fall back to subprocess
        result = subprocess.run(
            [sys.executable, str(M2CTX_PATH), str(src_file), "-o", "/dev/stdout"],
            capture_output=True,
            text=True,
            cwd=ROOT_DIR
        )
        if result.returncode != 0:
            print(f"Warning: m2ctx failed: {result.stderr}")
            return ""
        return result.stdout


def run_m2c(asm_file: Path, ctx_file: Optional[Path] = None, verbose: bool = False) -> str:
    """
    Run m2c on the assembly file.

    Returns the generated C code.
    """
    m2c_script = M2C_DIR / "m2c.py"
    if not m2c_script.exists():
        # Try alternate location
        m2c_script = M2C_DIR / "m2c" / "main.py"
        if not m2c_script.exists():
            print(f"Error: m2c not found at {M2C_DIR}")
            print("Please install m2c to build/m2c/")
            sys.exit(1)

    cmd = [
        sys.executable, str(m2c_script),
        "-P", "4",                           # 4-space indent
        "--pointer-style", "left",           # int* x style
        "--comment-style", "oneline",        # Use // comments
        "--target", "mipsel-gcc-c",          # PSX target
    ]

    if ctx_file and ctx_file.exists():
        cmd.extend(["--context", str(ctx_file)])

    cmd.append(str(asm_file))

    if verbose:
        print(f"Running: {' '.join(cmd)}")

    result = subprocess.run(
        cmd,
        capture_output=True,
        text=True,
        cwd=ROOT_DIR
    )

    if result.returncode != 0:
        print(f"m2c error: {result.stderr}")
        # Still return any output we got
        return result.stdout if result.stdout else ""

    return result.stdout


def print_function_info(func: Dict) -> None:
    """Print information about a function."""
    print(f"Function: {func['name']}")
    print(f"  Address:     {func['vram_start']} - {func['vram_end']}")
    print(f"  Size:        {func['size']} bytes")
    print(f"  Instructions: {func['num_instructions']}")
    print(f"  Complexity:  {func['complexity']}")
    print(f"  Category:    {func['category']}")
    print(f"  JAL calls:   {func['num_jal_calls']}")
    print(f"  Branches:    {func['num_branches']}")

    # Show library info if present
    if 'library' in func:
        print(f"  Library:     {func['library']} ({func.get('library_name', 'unknown')})")

    # Show undeclared data refs if present
    undeclared = func.get('undeclared_data_refs', [])
    if undeclared:
        print(f"  Undeclared data refs: {len(undeclared)}")
        for ref in undeclared[:5]:
            print(f"    - {ref}")
        if len(undeclared) > 5:
            print(f"    ... and {len(undeclared) - 5} more")


def create_include_asm_stub(func: Dict) -> str:
    """Generate INCLUDE_ASM stub for the function."""
    return f'INCLUDE_ASM("{func["name"]}");\n'


def main():
    parser = argparse.ArgumentParser(
        description="Automated decompilation helper for Tokimeki Memorial",
        epilog="""
Examples:
    python3 tools/decompile.py func_800ACBC0
    python3 tools/decompile.py 0x800ACBC0
    python3 tools/decompile.py func_800ACBC0 --info-only
    python3 tools/decompile.py func_800ACBC0 --src src/main/game.c
        """
    )
    parser.add_argument(
        "function",
        help="Function name (func_800XXXXX) or address (0x800XXXXX)"
    )
    parser.add_argument(
        "--src", "-s",
        default=str(DEFAULT_SRC_FILE),
        help=f"Source file for context generation (default: {DEFAULT_SRC_FILE})"
    )
    parser.add_argument(
        "--info-only", "-i",
        action="store_true",
        help="Only print function info, don't run m2c"
    )
    parser.add_argument(
        "--output-file", "-o",
        help="Write m2c output to this file instead of stdout"
    )
    parser.add_argument(
        "--no-context",
        action="store_true",
        help="Skip context generation (faster, but may produce worse output)"
    )
    parser.add_argument(
        "--verbose", "-v",
        action="store_true",
        help="Verbose output"
    )

    args = parser.parse_args()

    # Load function map
    print("Loading function map...")
    func_map = load_function_map()
    print(f"Loaded {len(func_map['functions'])} functions")

    # Find the function
    func = find_function(func_map, args.function)
    if not func:
        print(f"Error: Function '{args.function}' not found")
        print("\nHint: Try one of these functions:")
        # Show some similar names
        pattern = args.function.lower().replace("func_", "").replace("0x", "")
        matches = [f['name'] for f in func_map['functions']
                   if pattern in f['name'].lower()][:5]
        for m in matches:
            print(f"  {m}")
        sys.exit(1)

    # Print function info
    print()
    print_function_info(func)
    print()

    if args.info_only:
        sys.exit(0)

    # Create temp files for m2c
    with tempfile.TemporaryDirectory() as tmpdir:
        tmpdir = Path(tmpdir)

        # Write disassembly
        asm_file = tmpdir / f"{func['name']}.s"
        print(f"Writing disassembly to {asm_file}...")
        write_disassembly_file(func, asm_file)

        # Generate context
        ctx_file = None
        if not args.no_context:
            src_file = Path(args.src)
            if src_file.exists():
                print(f"Generating context from {src_file}...")
                ctx_content = generate_context(src_file)
                if ctx_content:
                    ctx_file = tmpdir / "ctx.c"
                    with open(ctx_file, 'w') as f:
                        f.write(ctx_content)
                    if args.verbose:
                        print(f"Context: {len(ctx_content)} bytes")

        # Run m2c
        print("Running m2c...")
        c_code = run_m2c(asm_file, ctx_file, verbose=args.verbose)

        if not c_code:
            print("Error: m2c produced no output")
            print("\nManual steps:")
            print(f"  1. Check disassembly at {asm_file}")
            print(f"  2. Run m2c manually: python3 build/m2c/m2c.py {asm_file}")
            sys.exit(1)

        # Output result
        if args.output_file:
            output_path = Path(args.output_file)
            with open(output_path, 'w') as f:
                f.write(f"// Decompiled from {func['name']}\n")
                f.write(f"// Address: {func['vram_start']}\n")
                f.write(f"// Size: {func['size']} bytes\n\n")
                f.write(c_code)
            print(f"Output written to: {output_path}")
        else:
            print("\n" + "=" * 60)
            print("DECOMPILED C CODE")
            print("=" * 60)
            print(c_code)
            print("=" * 60)

    # Print next steps
    print("\nNext steps:")
    print(f"  1. Review the generated C code above")
    print(f"  2. Add to source file: {args.src}")
    print(f"  3. Test build: make build")
    print(f"  4. Compare: Use asm-differ to verify")
    print()
    print("INCLUDE_ASM stub (if needed):")
    print(f'  {create_include_asm_stub(func)}')


if __name__ == "__main__":
    main()
