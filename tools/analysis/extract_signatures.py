#!/usr/bin/env python3
"""
extract_signatures.py - Extract function signatures from matched C files

Parses all matched C files in src/main/matched/ and generates a header file
with proper function prototypes sorted by address.

Usage:
    python3 tools/analysis/extract_signatures.py
    python3 tools/analysis/extract_signatures.py --output include/functions.h
    python3 tools/analysis/extract_signatures.py --verbose
    python3 tools/analysis/extract_signatures.py --dry-run

Output format:
    /* Auto-generated function prototypes from matched C */
    #ifndef FUNCTIONS_H
    #define FUNCTIONS_H
    #include "types.h"

    /* 0x80041000 - 0x80050000 */
    extern s32 func_80041234(s32 a0, s32 a1);
    extern void func_80041300(void);
    ...
    #endif
"""

import argparse
import os
import re
import sys
from pathlib import Path
from typing import Dict, List, Optional, Tuple

# Project paths
SCRIPT_DIR = Path(__file__).parent
ROOT_DIR = SCRIPT_DIR.parent.parent
MATCHED_DIR = ROOT_DIR / "src/main/matched"
DEFAULT_OUTPUT = ROOT_DIR / "include/functions.h"


def extract_address_from_filename(filename: str) -> Optional[int]:
    """
    Extract address from filename like 'func_80041234.c' or named files.
    Returns None if no address can be extracted.
    """
    # Pattern: func_XXXXXXXX.c
    match = re.match(r'func_([0-9A-Fa-f]{8})\.c$', filename)
    if match:
        return int(match.group(1), 16)
    return None


def extract_address_from_content(content: str) -> Optional[int]:
    """
    Extract address from file content (comments like 'Address: 0x80041234').
    """
    # Pattern: Address: 0xXXXXXXXX
    match = re.search(r'Address:\s*0x([0-9A-Fa-f]+)', content)
    if match:
        return int(match.group(1), 16)
    return None


def extract_function_name_from_filename(filename: str) -> str:
    """Extract function name from filename."""
    return filename.replace('.c', '')


def normalize_type(type_str: str) -> str:
    """
    Normalize type string by removing extra whitespace and handling
    pointer/const qualifiers properly.
    """
    # Remove multiple spaces
    type_str = re.sub(r'\s+', ' ', type_str.strip())
    # Ensure pointer is attached to type
    type_str = re.sub(r'\s+\*', ' *', type_str)
    type_str = re.sub(r'\*\s+', '*', type_str)
    return type_str


# Known types from types.h and common.h
KNOWN_TYPES = {
    'void', 'int', 'char', 'short', 'long', 'float', 'double',
    'u8', 's8', 'u16', 's16', 'u32', 's32', 'u64', 's64',
    'bool', 'RECT', 'unsigned', 'signed', 'const', 'volatile',
    'size_t', 'ptrdiff_t',
}

# Types that should be replaced with void* (file-local typedefs)
REPLACE_WITH_VOID_PTR = {
    'func_ptr_t', 'schar', 'BlurState', 'SelectStruct', 'Block', 'Block8',
    'NodeStruct', 'AutoCtrl', 'MATRIX', 'SndObj', 'SndCtrl', 'vtable_t',
    'Rect', 'Pos', 'SoundStruct',
}


def normalize_parameters(params: str) -> str:
    """
    Normalize parameter types, replacing unknown struct types with void*.
    """
    if params == 'void' or not params:
        return params

    # Split parameters by comma
    param_list = []
    depth = 0
    current = ""

    for c in params:
        if c == '(':
            depth += 1
            current += c
        elif c == ')':
            depth -= 1
            current += c
        elif c == ',' and depth == 0:
            param_list.append(current.strip())
            current = ""
        else:
            current += c

    if current.strip():
        param_list.append(current.strip())

    # Process each parameter
    normalized = []
    for param in param_list:
        param = param.strip()

        # Skip variadic
        if param == '...':
            normalized.append(param)
            continue

        # Handle function pointer params (leave as-is)
        if '(*)' in param or '(*' in param:
            normalized.append(param)
            continue

        # Check if param contains an unknown type
        # Split into type and name parts
        words = param.split()
        if not words:
            normalized.append(param)
            continue

        # Find the type portion (everything except the last word if it's a name)
        # Handle cases like "SomeStruct *ptr" or "s32 arg0"
        has_pointer = '*' in param

        # Remove pointer and get base words
        param_no_ptr = param.replace('*', ' ').strip()
        words_no_ptr = param_no_ptr.split()

        # Determine if last word is a variable name (starts with lowercase or is typical arg name)
        if len(words_no_ptr) >= 2:
            potential_name = words_no_ptr[-1]
            potential_types = words_no_ptr[:-1]

            # Check for types that should explicitly be replaced with void*
            for t in potential_types:
                if t in REPLACE_WITH_VOID_PTR:
                    # func_ptr_t is already a pointer type, replace with void*
                    # Other struct types need void* if they have * in param
                    if t == 'func_ptr_t':
                        normalized.append(f"void *{potential_name}")
                    elif has_pointer:
                        normalized.append(f"void *{potential_name}")
                    else:
                        normalized.append(param)
                    break
            else:
                # Check if potential_types contain unknown types
                unknown_found = False
                for t in potential_types:
                    if t not in KNOWN_TYPES and not t.startswith('func_'):
                        # Unknown type - check if it's a struct pointer
                        if has_pointer:
                            unknown_found = True
                            break

                if unknown_found:
                    # Replace with void *name
                    normalized.append(f"void *{potential_name}")
                else:
                    normalized.append(param)
            continue

        normalized.append(param)

    return ', '.join(normalized)


def parse_function_signature(content: str, expected_name: str = None) -> Optional[Tuple[str, str, str]]:
    """
    Parse function signature from C file content.

    Returns: (return_type, function_name, parameters) or None if not found.

    If expected_name is None, finds any function definition (not extern/prototype).

    Handles:
    - Standard functions: s32 func_80041234(s32 a0, s32 a1)
    - void parameters: void func_80041234(void)
    - No parameters: void func_80041234()
    - Pointer returns: u8 *func_80041234(void)
    - Struct parameters: void func_80041234(RECT *rect)
    - Variadic: s32 printf(const char *fmt, ...)
    """
    # Remove single-line comments
    content_no_comments = re.sub(r'//.*$', '', content, flags=re.MULTILINE)

    # Remove multi-line comments (non-greedy)
    content_no_comments = re.sub(r'/\*.*?\*/', '', content_no_comments, flags=re.DOTALL)

    # Remove preprocessor directives
    content_no_comments = re.sub(r'^\s*#.*$', '', content_no_comments, flags=re.MULTILINE)

    # Remove typedef, struct, enum definitions (they can confuse the parser)
    content_no_comments = re.sub(r'typedef\s+[^;]+;', '', content_no_comments)
    content_no_comments = re.sub(r'typedef\s+struct\s*\{[^}]*\}\s*\w+\s*;', '', content_no_comments, flags=re.DOTALL)

    # Remove extern declarations (we want definitions, not prototypes)
    content_no_comments = re.sub(r'extern\s+[^;]+;', '', content_no_comments)

    # Remove function prototypes (declarations without body)
    # Pattern: return_type function_name(params);
    content_no_comments = re.sub(r'\n\s*\w+[\s\*]+\w+\s*\([^)]*\)\s*;', '\n', content_no_comments)

    if expected_name:
        # Pattern for specific function definition
        pattern = r'''
            (?:^|\n)\s*                           # Start of line/string
            (                                      # Return type group
                (?:static\s+|const\s+)*            # Optional qualifiers (not extern)
                (?:unsigned\s+|signed\s+)?         # Optional sign
                \w+                                 # Base type (s32, void, etc.)
                (?:\s*\*+)?                         # Optional pointer(s)
            )
            \s+
            (\*?\s*''' + re.escape(expected_name) + r''')  # Function name (maybe with *)
            \s*
            \(                                     # Opening paren
            ([^{]*)                                # Parameters (allow nested parens for func ptrs)
            \)                                     # Closing paren
            \s*                                    # Optional whitespace
            \{                                     # Function body start (not semicolon)
        '''

        match = re.search(pattern, content_no_comments, re.VERBOSE | re.MULTILINE)

        if not match:
            # Try simpler pattern for edge cases
            simple_pattern = r'(\w+(?:\s*\*)?)\s+(\*?\s*' + re.escape(expected_name) + r')\s*\(([^{]*)\)\s*\{'
            match = re.search(simple_pattern, content_no_comments)
    else:
        # Pattern for ANY function definition (not prototype)
        # This finds definitions with a { body
        pattern = r'''
            (?:^|\n)\s*                           # Start of line/string
            (                                      # Return type group
                (?:static\s+|const\s+)*            # Optional qualifiers (not extern)
                (?:unsigned\s+|signed\s+)?         # Optional sign
                \w+                                 # Base type (s32, void, etc.)
                (?:\s*\*+)?                         # Optional pointer(s)
            )
            \s+
            (\*?\s*\w+)                            # Function name (maybe with *)
            \s*
            \(                                     # Opening paren
            ([^{]*)                                # Parameters (allow nested parens for func ptrs)
            \)                                     # Closing paren
            \s*                                    # Optional whitespace
            \{                                     # Function body start (requires {)
        '''
        match = re.search(pattern, content_no_comments, re.VERBOSE | re.MULTILINE)

    if match:
        return_type = normalize_type(match.group(1))
        func_name = match.group(2).strip()
        params = match.group(3).strip()

        # Handle pointer attached to function name
        if func_name.startswith('*'):
            return_type = return_type + ' *'
            func_name = func_name.lstrip('* ')

        # Normalize empty params to void
        if not params or params.isspace():
            params = 'void'

        # Clean up parameter whitespace
        params = re.sub(r'\s+', ' ', params)
        params = re.sub(r'\s*,\s*', ', ', params)

        # Normalize parameters (replace unknown types with void*)
        params = normalize_parameters(params)

        return (return_type.strip(), func_name.strip(), params.strip())

    return None


def parse_c_file(filepath: Path, verbose: bool = False) -> Optional[Dict]:
    """
    Parse a C file and extract function signature information.

    Returns dict with: address, name, return_type, parameters, full_signature
    """
    filename = filepath.name
    expected_name = extract_function_name_from_filename(filename)

    try:
        with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
            content = f.read()
    except Exception as e:
        if verbose:
            print(f"Warning: Could not read {filepath}: {e}", file=sys.stderr)
        return None

    # Get address from content first (more reliable)
    address = extract_address_from_content(content)
    if address is None:
        address = extract_address_from_filename(filename)

    # Try to get function signature
    # First try with expected name from filename
    sig = parse_function_signature(content, expected_name)

    # If that fails, try to find ANY function definition in the file
    if sig is None:
        sig = parse_function_signature(content, None)

    if sig is None:
        if verbose:
            print(f"Warning: Could not parse function in {filename}", file=sys.stderr)
        return None

    return_type, func_name, params = sig

    return {
        'address': address,
        'name': func_name,
        'return_type': return_type,
        'parameters': params,
        'full_signature': f"{return_type} {func_name}({params})"
    }


def generate_header(functions: List[Dict], output_path: Path) -> str:
    """
    Generate header file content with function prototypes.
    """
    lines = []

    # Header guard and includes
    lines.append("/* Auto-generated function prototypes from matched C files */")
    lines.append("/* Generated by tools/analysis/extract_signatures.py */")
    lines.append("/* DO NOT EDIT - regenerate with: python3 tools/analysis/extract_signatures.py */")
    lines.append("")
    lines.append("#ifndef FUNCTIONS_H")
    lines.append("#define FUNCTIONS_H")
    lines.append("")
    lines.append("#include \"types.h\"")
    lines.append("")

    # Sort by address (functions without address go at end)
    sorted_funcs = sorted(functions, key=lambda f: f['address'] if f['address'] else 0xFFFFFFFF)

    # Deduplicate by function name (keep first occurrence)
    seen_names = set()
    unique_funcs = []
    for func in sorted_funcs:
        if func['name'] not in seen_names:
            seen_names.add(func['name'])
            unique_funcs.append(func)

    # Group by address ranges (every 0x10000)
    current_range_start = None

    for func in unique_funcs:
        addr = func['address']

        if addr is not None:
            range_start = (addr // 0x10000) * 0x10000
            range_end = range_start + 0xFFFF

            if range_start != current_range_start:
                if current_range_start is not None:
                    lines.append("")
                lines.append(f"/* 0x{range_start:08X} - 0x{range_end:08X} */")
                current_range_start = range_start
        else:
            if current_range_start is not None:
                lines.append("")
                lines.append("/* Functions without address */")
                current_range_start = None

        # Generate extern declaration
        proto = f"extern {func['return_type']} {func['name']}({func['parameters']});"
        lines.append(proto)

    lines.append("")
    lines.append("#endif /* FUNCTIONS_H */")
    lines.append("")

    return "\n".join(lines)


def main():
    parser = argparse.ArgumentParser(
        description="Extract function signatures from matched C files"
    )
    parser.add_argument(
        '--output', '-o',
        type=Path,
        default=DEFAULT_OUTPUT,
        help=f"Output header file path (default: {DEFAULT_OUTPUT})"
    )
    parser.add_argument(
        '--verbose', '-v',
        action='store_true',
        help="Print verbose output"
    )
    parser.add_argument(
        '--dry-run', '-n',
        action='store_true',
        help="Parse files but don't write output"
    )
    parser.add_argument(
        '--stats',
        action='store_true',
        help="Print statistics only"
    )
    args = parser.parse_args()

    if not MATCHED_DIR.exists():
        print(f"Error: Matched directory not found: {MATCHED_DIR}", file=sys.stderr)
        sys.exit(1)

    # Find all .c files
    c_files = sorted(MATCHED_DIR.glob("*.c"))

    if args.verbose:
        print(f"Found {len(c_files)} C files in {MATCHED_DIR}")

    # Parse all files
    functions = []
    failed = []

    for filepath in c_files:
        result = parse_c_file(filepath, verbose=args.verbose)
        if result:
            functions.append(result)
        else:
            failed.append(filepath.name)

    # Print statistics
    total = len(c_files)
    parsed = len(functions)
    with_addr = sum(1 for f in functions if f['address'] is not None)

    print(f"Parsed {parsed}/{total} files successfully")
    print(f"  - With address: {with_addr}")
    print(f"  - Without address: {parsed - with_addr}")

    if failed and args.verbose:
        print(f"\nFailed to parse ({len(failed)} files):")
        for f in failed[:10]:
            print(f"  - {f}")
        if len(failed) > 10:
            print(f"  ... and {len(failed) - 10} more")

    if args.stats:
        return

    # Generate header
    header_content = generate_header(functions, args.output)

    if args.dry_run:
        print(f"\nDry run - would write {len(header_content)} bytes to {args.output}")
        if args.verbose:
            print("\n--- Preview (first 50 lines) ---")
            preview_lines = header_content.split('\n')[:50]
            for line in preview_lines:
                print(line)
            if len(header_content.split('\n')) > 50:
                print("...")
    else:
        # Write output
        args.output.parent.mkdir(parents=True, exist_ok=True)
        with open(args.output, 'w', encoding='utf-8') as f:
            f.write(header_content)
        print(f"\nWrote {len(functions)} prototypes to {args.output}")


if __name__ == "__main__":
    main()
