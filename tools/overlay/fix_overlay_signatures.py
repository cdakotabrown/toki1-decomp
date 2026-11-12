#!/usr/bin/env python3
"""
Fix function signature mismatches in overlay C files.

This script parses each C file and fixes only the declarations that
actually cause compile errors.
"""

import os
import re
import sys
import subprocess
from pathlib import Path

IDO_CC = Path(__file__).parent.parent / "ido-static-recomp/build/5.2/out/cc"
INCLUDE_DIR = Path(__file__).parent.parent.parent / "include"

def compile_test(filepath):
    """Try to compile a file and return error info."""
    cmd = [
        str(IDO_CC), "-O2", "-G0", "-mips1", "-EL", "-non_shared",
        f"-I{INCLUDE_DIR}", "-c", str(filepath), "-o", "/tmp/test.o"
    ]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode == 0:
        return None
    return result.stderr

def parse_error(stderr):
    """Parse the compiler error to extract function name and issue."""
    # Look for common error patterns
    patterns = [
        # Type of argument incompatible
        r'line (\d+):.*Type.*incompatible.*prototype',
        # Number of arguments doesn't agree
        r'line (\d+):.*number of arguments doesn\'t agree',
        # Reference of void type
        r'line (\d+):.*Reference of an expression of void type',
    ]

    for pattern in patterns:
        match = re.search(pattern, stderr)
        if match:
            return int(match.group(1))
    return None

def get_line_content(filepath, lineno):
    """Get the content of a specific line."""
    with open(filepath, 'r') as f:
        lines = f.readlines()
    if lineno <= len(lines):
        return lines[lineno - 1]
    return None

def find_function_in_line(line):
    """Find function call in a line."""
    match = re.search(r'(func_[0-9A-Fa-f]{8})\s*\(', line)
    if match:
        return match.group(1)
    return None

def count_args_in_call(line, func_name):
    """Count arguments in a function call."""
    # Find the call
    pattern = re.escape(func_name) + r'\s*\(([^)]*)\)'
    match = re.search(pattern, line)
    if match:
        args = match.group(1).strip()
        if args == '':
            return 0
        # Count commas + 1
        depth = 0
        comma_count = 0
        for c in args:
            if c == '(':
                depth += 1
            elif c == ')':
                depth -= 1
            elif c == ',' and depth == 0:
                comma_count += 1
        return comma_count + 1
    return -1

def fix_declaration(content, func_name, needs_return, needs_args):
    """Fix a function declaration."""
    # Find the extern declaration
    pattern = r'(extern\s+)(void|s32|u32|int)\s+' + re.escape(func_name) + r'\s*\(([^)]*)\)\s*;'
    match = re.search(pattern, content)
    if not match:
        return content

    old_decl = match.group(0)
    old_return = match.group(2)
    old_params = match.group(3).strip()

    new_return = old_return
    new_params = old_params

    if needs_return and old_return == 'void':
        new_return = 's32'

    if needs_args > 0:
        if old_params == 'void' or old_params == '':
            # No params declared but args passed
            if needs_args == 1:
                new_params = 's32 a0'
            elif needs_args == 2:
                new_params = 's32 a0, s32 a1'
            elif needs_args == 3:
                new_params = 's32 a0, s32 a1, s32 a2'
            elif needs_args == 4:
                new_params = 's32 a0, s32 a1, s32 a2, s32 a3'
            else:
                new_params = ', '.join([f's32 a{i}' for i in range(needs_args)])
    elif needs_args == 0:
        # Called with no args but declared with args
        if old_params != 'void' and old_params != '':
            new_params = 'void'

    if new_return == old_return and new_params == old_params:
        return content

    new_decl = f'extern {new_return} {func_name}({new_params});'
    print(f"  Fix: {old_decl} -> {new_decl}")
    return content.replace(old_decl, new_decl)

def fix_file(filepath):
    """Fix a single file iteratively until it compiles."""
    max_iterations = 20

    for iteration in range(max_iterations):
        error = compile_test(filepath)
        if error is None:
            return True

        line_no = parse_error(error)
        if line_no is None:
            print(f"  Cannot parse error: {error[:200]}")
            return False

        line = get_line_content(filepath, line_no)
        if line is None:
            print(f"  Cannot read line {line_no}")
            return False

        func_name = find_function_in_line(line)
        if func_name is None:
            print(f"  Cannot find function in line: {line.strip()}")
            return False

        # Determine what kind of fix is needed
        needs_return = 'Reference of an expression of void type' in error
        needs_args = 0

        if 'number of arguments' in error or 'incompatible' in error:
            needs_args = count_args_in_call(line, func_name)

        # Read and fix the file
        with open(filepath, 'r') as f:
            content = f.read()

        new_content = fix_declaration(content, func_name, needs_return, needs_args)

        if new_content == content:
            print(f"  No change made for {func_name} at line {line_no}")
            return False

        with open(filepath, 'w') as f:
            f.write(new_content)

    return False

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 fix_overlay_signatures.py <overlay_dir>")
        sys.exit(1)

    overlay_dir = Path(sys.argv[1])
    if not overlay_dir.exists():
        print(f"Directory not found: {overlay_dir}")
        sys.exit(1)

    if not IDO_CC.exists():
        print(f"IDO compiler not found: {IDO_CC}")
        sys.exit(1)

    fixed = 0
    failed = 0

    for c_file in sorted(overlay_dir.glob("func_*.c")):
        # First check if it compiles
        if compile_test(c_file) is None:
            continue

        print(f"\nFixing {c_file.name}...")
        if fix_file(c_file):
            fixed += 1
        else:
            failed += 1

    print(f"\nSummary: {fixed} fixed, {failed} still failing")

if __name__ == '__main__':
    main()
