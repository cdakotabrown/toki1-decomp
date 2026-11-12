#!/usr/bin/env python3
"""
decompile_asm.py - Decompile ASM file to C using m2c

Handles our ASM format automatically by preprocessing before m2c.

Usage:
    python3 decompile_asm.py get_weekly_bg_sector
    python3 decompile_asm.py asm/main/matched/get_weekly_bg_sector.s
    python3 decompile_asm.py 0x80073A94
"""

import argparse
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

PROJECT_ROOT = Path(__file__).parent.parent.parent
ASM_DIR = PROJECT_ROOT / 'asm/main/matched'
M2C_PATH = PROJECT_ROOT / 'build/m2c/m2c.py'
PREPROCESS_PATH = PROJECT_ROOT / 'tools/decomp/asm_to_m2c.py'
CONTEXT_PATH = PROJECT_ROOT / 'include/context.h'


def find_asm_file(name: str) -> Path:
    """Find ASM file by name, address, or path"""
    # Direct path
    if os.path.exists(name):
        return Path(name)

    # Function name
    if name.startswith('func_'):
        path = ASM_DIR / f'{name}.s'
        if path.exists():
            return path

    # Hex address
    if name.startswith('0x') or name.startswith('0X'):
        addr = name[2:].lower()
        path = ASM_DIR / f'func_{addr}.s'
        if path.exists():
            return path
        # Try uppercase
        path = ASM_DIR / f'func_{addr.upper()}.s'
        if path.exists():
            return path

    # Try as raw hex
    if re.match(r'^[0-9a-fA-F]+$', name):
        path = ASM_DIR / f'func_{name.lower()}.s'
        if path.exists():
            return path

    return None


def preprocess_asm(asm_path: Path) -> str:
    """Preprocess ASM file for m2c"""
    result = subprocess.run(
        ['python3', str(PREPROCESS_PATH), str(asm_path)],
        capture_output=True,
        text=True
    )
    if result.returncode != 0:
        print(f"Preprocessing failed: {result.stderr}", file=sys.stderr)
        sys.exit(1)
    return result.stdout


def decompile(asm_content: str, target: str = 'mips-ido-c', context: Path = None) -> str:
    """Run m2c on preprocessed ASM"""

    # Try with context first, fall back to no context if it fails
    for use_context in [True, False] if context and context.exists() else [False]:
        cmd = ['python3', str(M2C_PATH), '--target', target]

        if use_context:
            cmd.extend(['--context', str(context)])

        cmd.append('-')  # Read from stdin

        result = subprocess.run(
            cmd,
            input=asm_content,
            capture_output=True,
            text=True
        )

        # Check if context caused an error
        if 'Syntax error when parsing C context' in result.stdout:
            if use_context:
                continue  # Retry without context
            # If we already tried without context, return the error

        # m2c outputs warnings to stderr but still produces output
        output = result.stdout
        if result.stderr and 'warning' in result.stderr.lower():
            # Include warnings as comments
            warnings = '\n'.join(f'// m2c warning: {line}'
                                for line in result.stderr.strip().split('\n')
                                if line.strip())
            if warnings:
                output = warnings + '\n\n' + output

        return output

    return "// Decompilation failed"


def main():
    parser = argparse.ArgumentParser(description='Decompile ASM to C')
    parser.add_argument('function', help='Function name, address, or ASM file path')
    parser.add_argument('--target', default='mips-ido-c',
                       choices=['mips-ido-c', 'mips-gcc-c'],
                       help='m2c target (default: mips-ido-c)')
    parser.add_argument('--context', type=Path, help='Context file for type info')
    parser.add_argument('-o', '--output', help='Output file (default: stdout)')
    args = parser.parse_args()

    # Find ASM file
    asm_path = find_asm_file(args.function)
    if not asm_path:
        print(f"Error: Could not find ASM file for '{args.function}'", file=sys.stderr)
        print(f"Searched in: {ASM_DIR}", file=sys.stderr)
        sys.exit(1)

    print(f"Decompiling: {asm_path}", file=sys.stderr)

    # Preprocess
    preprocessed = preprocess_asm(asm_path)

    # Decompile
    context = args.context or CONTEXT_PATH
    c_code = decompile(preprocessed, args.target, context if context.exists() else None)

    # Output
    if args.output:
        Path(args.output).write_text(c_code)
        print(f"Written to: {args.output}", file=sys.stderr)
    else:
        print(c_code)


if __name__ == '__main__':
    main()
