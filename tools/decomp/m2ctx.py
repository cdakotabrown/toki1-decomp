#!/usr/bin/env python3
"""
m2ctx.py - Context Generator for m2c and decomp.me

Generates a ctx.c file containing all type definitions, function declarations,
and preprocessor macros needed for decompiling a function with m2c or decomp.me.

Usage:
    python3 build/m2ctx.py src/main/functions.c

Output:
    ctx.c - Contains preprocessed context ready for m2c/decomp.me

Based on Silent Hill decomp project's m2ctx.py
Adapted for Tokimeki Memorial: Forever with You
"""

import argparse
import os
import sys
import subprocess
import tempfile

script_dir = os.path.dirname(os.path.realpath(__file__))
root_dir = os.path.abspath(os.path.join(script_dir, ".."))
src_dir = os.path.join(root_dir, "src")

# Project-specific preprocessor flags for Tokimeki Memorial
CPP_FLAGS = [
    "-Iinclude",
    "-Isrc",
    "-D_LANGUAGE_C",
    "-D_MIPS_SZLONG=32",
    "-D__attribute__(...)=",
    "-D__asm__(...)=",
    "-ffreestanding",
    "-DM2CTX",
]


def import_c_file(in_file: str) -> str:
    """
    Preprocess a C file and extract context for m2c/decomp.me.

    Args:
        in_file: Path to C source file

    Returns:
        Preprocessed content with all type definitions and declarations
    """
    in_file = os.path.relpath(in_file, root_dir)

    # Determine platform-specific GCC command
    cpp = "gcc"

    cpp_command = [cpp, "-E", "-P", "-dD", *CPP_FLAGS, in_file]

    # Get stock macros (predefined by compiler)
    with tempfile.NamedTemporaryFile(suffix=".c", delete=True) as tmp:
        try:
            stock_macros = subprocess.check_output(
                [cpp, "-E", "-P", "-dM", tmp.name],
                cwd=root_dir,
                encoding="utf-8",
                stderr=subprocess.DEVNULL
            )
        except subprocess.CalledProcessError as e:
            print(f"Warning: Failed to get stock macros: {e}", file=sys.stderr)
            stock_macros = ""

    # Preprocess the input file
    try:
        out_text = subprocess.check_output(
            cpp_command,
            cwd=root_dir,
            encoding="utf-8",
            stderr=subprocess.PIPE
        )
    except subprocess.CalledProcessError as e:
        print(
            "Failed to preprocess input file, when running command:\n"
            + " ".join(cpp_command),
            file=sys.stderr,
        )
        if e.stderr:
            print(f"Error output:\n{e.stderr}", file=sys.stderr)
        sys.exit(1)

    if not out_text:
        print("Error: Preprocessor output is empty", file=sys.stderr)
        sys.exit(1)

    # Parse preprocessor output to separate defines from source
    defines = {}
    source_lines = []

    for line in out_text.splitlines(keepends=True):
        if line.startswith("#define"):
            # Extract macro name
            parts = line.split()
            if len(parts) >= 2:
                sym = parts[1].split("(")[0]
                defines[sym] = line
        elif line.startswith("#undef"):
            # Remove undefined macros
            parts = line.split()
            if len(parts) >= 2:
                sym = parts[1]
                if sym in defines:
                    del defines[sym]
        else:
            # Skip inline assembly (not needed for m2c)
            if line.strip().startswith("__asm__"):
                continue
            source_lines.append(line)

    # Remove stock compiler macros from our defines
    for line in stock_macros.strip().splitlines():
        parts = line.split()
        if len(parts) >= 2:
            sym = parts[1].split("(")[0]
            if sym in defines:
                del defines[sym]

    # Combine defines and source
    return "".join(defines.values()) + "".join(source_lines)


def main():
    parser = argparse.ArgumentParser(
        description="Create a context file which can be used for m2c / decomp.me",
        epilog="""
Example usage:
    python3 build/m2ctx.py src/main/functions.c

This will create ctx.c in the project root, which can be pasted into
decomp.me or used with m2c for decompilation context.
        """
    )
    parser.add_argument(
        "c_file",
        help="C source file to generate context from",
    )
    parser.add_argument(
        "-o", "--output",
        default="ctx.c",
        help="Output file path (default: ctx.c)",
    )
    parser.add_argument(
        "-v", "--verbose",
        action="store_true",
        help="Print verbose output",
    )

    args = parser.parse_args()

    # Validate input file
    if not os.path.exists(args.c_file):
        print(f"Error: Input file '{args.c_file}' not found", file=sys.stderr)
        sys.exit(1)

    if args.verbose:
        print(f"Generating context from: {args.c_file}")
        print(f"Output file: {args.output}")
        print(f"Preprocessor flags: {' '.join(CPP_FLAGS)}")

    # Generate context
    try:
        output = import_c_file(args.c_file)
    except Exception as e:
        print(f"Error generating context: {e}", file=sys.stderr)
        import traceback
        traceback.print_exc()
        sys.exit(1)

    # Write output file
    output_path = os.path.join(root_dir, args.output)
    try:
        with open(output_path, "w", encoding="UTF-8") as f:
            f.write(output)
    except Exception as e:
        print(f"Error writing output file: {e}", file=sys.stderr)
        sys.exit(1)

    if args.verbose:
        print(f"✓ Context generated successfully: {output_path}")
        print(f"  Total size: {len(output)} bytes")
        print(f"  Lines: {len(output.splitlines())}")
    else:
        print(f"Context written to: {output_path}")


if __name__ == "__main__":
    main()
