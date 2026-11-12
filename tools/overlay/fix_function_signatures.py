#!/usr/bin/env python3
"""
Fix function signature mismatches in overlay C files.

This script parses each C file, finds function calls that don't match
the declared signature, and updates the extern declarations.
"""

import os
import re
import sys
from pathlib import Path

def find_function_calls(content):
    """Find all function calls and their argument counts."""
    calls = {}
    # Match func_XXXXXXXX(...)
    for match in re.finditer(r'(func_[0-9A-Fa-f]{8})\s*\(([^)]*)\)', content):
        func_name = match.group(1)
        args_str = match.group(2).strip()
        # Count arguments (rough estimate)
        if args_str == '':
            arg_count = 0
        else:
            # Count commas + 1, but handle nested parens
            depth = 0
            comma_count = 0
            for c in args_str:
                if c == '(':
                    depth += 1
                elif c == ')':
                    depth -= 1
                elif c == ',' and depth == 0:
                    comma_count += 1
            arg_count = comma_count + 1

        if func_name not in calls:
            calls[func_name] = set()
        calls[func_name].add(arg_count)
    return calls

def find_extern_declarations(content):
    """Find all extern function declarations."""
    decls = {}
    # Match extern void/s32/u32 func_XXXXXXXX(...)
    for match in re.finditer(r'extern\s+(void|s32|u32|s16|u16|u8|s8|int)\s+(func_[0-9A-Fa-f]{8})\s*\(([^)]*)\)\s*;', content):
        return_type = match.group(1)
        func_name = match.group(2)
        params = match.group(3).strip()
        if params == 'void' or params == '':
            param_count = 0
        else:
            # Count parameters
            depth = 0
            comma_count = 0
            for c in params:
                if c == '(':
                    depth += 1
                elif c == ')':
                    depth -= 1
                elif c == ',' and depth == 0:
                    comma_count += 1
            param_count = comma_count + 1
        decls[func_name] = {
            'return_type': return_type,
            'params': params,
            'param_count': param_count,
            'full_match': match.group(0)
        }
    return decls

def is_used_as_return_value(content, func_name):
    """Check if function result is used (assigned to variable)."""
    # Check for patterns like: v0 = func_XXX(); or result = func_XXX();
    pattern = r'\w+\s*=\s*' + re.escape(func_name) + r'\s*\('
    return bool(re.search(pattern, content))

def fix_file(filepath):
    """Fix function signature mismatches in a file."""
    with open(filepath, 'r') as f:
        content = f.read()

    calls = find_function_calls(content)
    decls = find_extern_declarations(content)

    changes = []
    new_content = content

    for func_name, arg_counts in calls.items():
        if func_name not in decls:
            continue

        decl = decls[func_name]
        max_args = max(arg_counts)

        needs_fix = False
        new_return = decl['return_type']
        new_params = decl['params']

        # Check if called with more args than declared
        if max_args > decl['param_count']:
            needs_fix = True
            # Generate param list
            if max_args == 1:
                new_params = 's32 a0'
            elif max_args == 2:
                new_params = 's32 a0, s32 a1'
            elif max_args == 3:
                new_params = 's32 a0, s32 a1, s32 a2'
            elif max_args == 4:
                new_params = 's32 a0, s32 a1, s32 a2, s32 a3'
            else:
                new_params = ', '.join([f's32 a{i}' for i in range(max_args)])

        # Check if return value is used but declared as void
        if decl['return_type'] == 'void' and is_used_as_return_value(content, func_name):
            needs_fix = True
            new_return = 's32'

        if needs_fix:
            old_decl = decl['full_match']
            new_decl = f'extern {new_return} {func_name}({new_params});'
            if old_decl != new_decl:
                new_content = new_content.replace(old_decl, new_decl)
                changes.append(f'{func_name}: {old_decl} -> {new_decl}')

    if changes:
        with open(filepath, 'w') as f:
            f.write(new_content)
        return changes
    return []

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 fix_function_signatures.py <overlay_dir>")
        sys.exit(1)

    overlay_dir = Path(sys.argv[1])
    if not overlay_dir.exists():
        print(f"Directory not found: {overlay_dir}")
        sys.exit(1)

    total_changes = 0
    for c_file in sorted(overlay_dir.glob("func_*.c")):
        changes = fix_file(c_file)
        if changes:
            print(f"\n{c_file.name}:")
            for change in changes:
                print(f"  {change}")
            total_changes += len(changes)

    print(f"\nTotal changes: {total_changes}")

if __name__ == '__main__':
    main()
