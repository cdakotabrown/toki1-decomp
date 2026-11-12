#!/usr/bin/env python3
"""
Batch decompilation helper for Level 2 matching.
Generates C stub files for all functions.
"""

import json
import os
import sys
import subprocess
import re

# Paths
FUNCTION_MAP = "build/function_analysis/function_map.json"
IDO_CC = "tools/ido-static-recomp/build/5.3/out/cc"
OUTPUT_DIR = "src/level2"
BUILD_DIR = "build/level2_test"

# The 15 functions that actually match PSY-Q GCC 2.6
# Everything else uses IDO (even if we don't have exact matching version)
GCC_MATCHING_FUNCTIONS = {
    "func_80086850", "func_80086944", "func_80086964", "func_80086984",
    "func_800878B8", "func_80087954", "func_80087974", "CdRead",
    "func_80087E4C", "SsQuit", "func_8009AD30", "func_800ADBB0",
    "func_8008B8E4", "func_8008BE14", "func_8008FEB0"
}

def load_functions():
    """Load all functions from the function map."""
    with open(FUNCTION_MAP) as f:
        data = json.load(f)
    return data['functions']

def classify_function(func):
    """Determine if function is GCC (15 specific) or IDO (everything else)."""
    name = func['name']
    if name in GCC_MATCHING_FUNCTIONS:
        return "gcc"  # Only these 15 match PSY-Q GCC
    return "ido"  # Everything else uses IDO patterns

def analyze_disassembly(func):
    """Analyze disassembly to determine function signature and behavior."""
    disasm = func.get('disassembly', [])

    # Check for return value
    has_return = any('li\tv0,' in line or 'move\tv0,' in line or
                     'lw\tv0,' in line or 'lh\tv0,' in line for line in disasm)

    # Count arguments used (a0-a3)
    args_used = set()
    for line in disasm:
        if 'a0' in line and ('lw' in line or 'sw' in line or 'move' in line or 'addiu' in line):
            args_used.add('a0')
        if 'a1' in line:
            args_used.add('a1')
        if 'a2' in line:
            args_used.add('a2')
        if 'a3' in line:
            args_used.add('a3')

    # Check for global accesses (lui pattern)
    global_refs = []
    for i, line in enumerate(disasm):
        if 'lui' in line and '0x80' in line:
            match = re.search(r'lui\s+\w+,0x([0-9a-f]+)', line)
            if match:
                hi = int(match.group(1), 16) << 16
                # Look for following load/store
                if i + 1 < len(disasm):
                    next_line = disasm[i + 1]
                    offset_match = re.search(r'(-?\d+)\(', next_line)
                    if offset_match:
                        offset = int(offset_match.group(1))
                        addr = (hi + offset) & 0xFFFFFFFF
                        global_refs.append(hex(addr))

    return {
        'has_return': has_return,
        'num_args': len(args_used),
        'global_refs': list(set(global_refs))[:5],  # Limit to 5
        'is_leaf': func['num_jal_calls'] == 0,
        'complexity': func['complexity']
    }

def generate_c_stub(func, analysis, compiler_type):
    """Generate a C stub file for the function."""
    name = func['name']
    addr = func['vram_start']
    size = func['size']

    # Generate argument list
    args = []
    arg_names = ['arg0', 'arg1', 'arg2', 'arg3']
    for i in range(min(analysis['num_args'], 4)):
        args.append(f"u32 {arg_names[i]}")
    args_str = ', '.join(args) if args else 'void'

    # Return type
    ret_type = 'int' if analysis['has_return'] else 'void'

    # Generate global declarations
    global_decls = []
    for ref in analysis['global_refs']:
        global_decls.append(f"extern u32 D_{ref[2:].upper()};  /* {ref} */")

    # Generate called function declarations
    call_decls = []
    disasm = func.get('disassembly', [])
    for line in disasm:
        if 'jal\t0x' in line:
            match = re.search(r'jal\t0x([0-9a-f]+)', line)
            if match:
                target = int(match.group(1), 16)
                # Convert to actual address
                actual = 0x80000000 | target
                call_decls.append(f"extern void func_{actual:08X}(void);  /* TODO: determine signature */")

    # Disassembly comments
    disasm_comments = []
    for line in func.get('disassembly', [])[:30]:  # First 30 lines
        # Clean up the line
        clean = line.strip()
        if clean:
            disasm_comments.append(f" * {clean}")
    if len(func.get('disassembly', [])) > 30:
        disasm_comments.append(f" * ... ({len(func['disassembly']) - 30} more instructions)")

    # Build the C file
    lines = [
        f"/* {name} - Level 2 Decompilation Stub",
        f" * Address: {addr}",
        f" * Size: {size} bytes",
        f" * Compiler: {'IDO' if compiler_type == 'game' else 'GCC (library)'}",
        f" * Complexity: {analysis['complexity']}",
        f" * Leaf: {analysis['is_leaf']}",
        f" *",
        f" * TODO: Implement function logic",
        f" *",
        f" * Original disassembly:",
    ]
    lines.extend(disasm_comments)
    lines.append(" */")
    lines.append("")
    lines.append("typedef unsigned int u32;")
    lines.append("typedef signed int s32;")
    lines.append("typedef unsigned short u16;")
    lines.append("typedef signed short s16;")
    lines.append("typedef unsigned char u8;")
    lines.append("typedef signed char s8;")
    lines.append("")

    if global_decls:
        lines.append("/* Global references */")
        lines.extend(list(set(global_decls)))
        lines.append("")

    if call_decls:
        lines.append("/* Called functions */")
        lines.extend(list(set(call_decls))[:10])  # Limit declarations
        lines.append("")

    # Function stub
    lines.append(f"/* TODO: Level 2 - Implement to match functionality */")
    lines.append(f"{ret_type} {name}({args_str}) {{")
    lines.append(f"    /* TODO: Decompile {size} bytes at {addr} */")
    if analysis['has_return']:
        lines.append("    return 0;  /* TODO: Implement */")
    lines.append("}")
    lines.append("")

    return '\n'.join(lines)

def process_function(func, output_base):
    """Process a single function and write its stub."""
    name = func['name']
    compiler_type = classify_function(func)
    analysis = analyze_disassembly(func)

    # Generate C code
    c_code = generate_c_stub(func, analysis, compiler_type)

    # Write to file
    subdir = "game" if compiler_type == "game" else "library"
    output_path = os.path.join(output_base, subdir, f"{name}.c")

    with open(output_path, 'w') as f:
        f.write(c_code)

    return {
        'name': name,
        'path': output_path,
        'type': compiler_type,
        'size': func['size'],
        'complexity': analysis['complexity']
    }

def main():
    """Main entry point."""
    import argparse
    parser = argparse.ArgumentParser(description='Batch decompile functions')
    parser.add_argument('--start', type=int, default=0, help='Start index')
    parser.add_argument('--count', type=int, default=0, help='Number of functions (0=all)')
    parser.add_argument('--output', default=OUTPUT_DIR, help='Output directory')
    args = parser.parse_args()

    # Load functions
    functions = load_functions()
    print(f"Loaded {len(functions)} functions")

    # Apply range
    if args.count > 0:
        functions = functions[args.start:args.start + args.count]
    elif args.start > 0:
        functions = functions[args.start:]

    print(f"Processing {len(functions)} functions (start={args.start})")

    # Ensure output directories exist
    os.makedirs(os.path.join(args.output, 'game'), exist_ok=True)
    os.makedirs(os.path.join(args.output, 'library'), exist_ok=True)

    # Process each function
    results = {'game': 0, 'library': 0, 'errors': []}
    for i, func in enumerate(functions):
        try:
            result = process_function(func, args.output)
            results[result['type']] += 1
            if (i + 1) % 50 == 0:
                print(f"  Processed {i + 1}/{len(functions)}: {result['name']}")
        except Exception as e:
            results['errors'].append({'name': func['name'], 'error': str(e)})
            print(f"  Error processing {func['name']}: {e}")

    print(f"\nComplete: {results['game']} game, {results['library']} library, {len(results['errors'])} errors")
    return results

if __name__ == '__main__':
    main()
