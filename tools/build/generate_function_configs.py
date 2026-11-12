#!/usr/bin/env python3
"""
Generate config/decompiled_functions.json from src/level2/ C files.
Scans all .c files and extracts metadata from their headers.
"""

import os
import re
import json
import argparse
from pathlib import Path

def parse_function_file(filepath):
    """Extract metadata from a function C file header."""
    with open(filepath, 'r') as f:
        content = f.read(2000)  # Only need header

    # Extract function name from filename
    name = Path(filepath).stem

    # Extract address
    addr_match = re.search(r'Address:\s*(0x[0-9A-Fa-f]+)', content)
    address = int(addr_match.group(1), 16) if addr_match else 0

    # Extract size
    size_match = re.search(r'Size:\s*(\d+)\s*bytes', content)
    size = int(size_match.group(1)) if size_match else 0

    # Extract compiler type
    compiler_match = re.search(r'Compiler:\s*(\w+)', content)
    compiler = compiler_match.group(1).lower() if compiler_match else 'unknown'

    # Extract complexity
    complexity_match = re.search(r'Complexity:\s*(\w+)', content)
    complexity = complexity_match.group(1).lower() if complexity_match else 'unknown'

    # Extract leaf status
    leaf_match = re.search(r'Leaf:\s*(True|False)', content)
    is_leaf = leaf_match.group(1) == 'True' if leaf_match else False

    # Determine category from path
    if '/library/' in str(filepath):
        category = 'library'
    elif '/game/' in str(filepath):
        category = 'game'
    else:
        category = 'unknown'

    return {
        'name': name,
        'address': address,
        'address_hex': f'0x{address:08X}',
        'size': size,
        'compiler': compiler,
        'complexity': complexity,
        'is_leaf': is_leaf,
        'category': category,
        'source': str(filepath),
        'level': 2  # Level 2 = functionally correct
    }

def scan_directory(src_dir):
    """Scan directory for all C function files."""
    functions = []

    for root, dirs, files in os.walk(src_dir):
        for filename in files:
            if filename.endswith('.c') and filename.startswith('func_'):
                filepath = os.path.join(root, filename)
                try:
                    func_info = parse_function_file(filepath)
                    functions.append(func_info)
                except Exception as e:
                    print(f"Warning: Could not parse {filepath}: {e}")

    # Sort by address
    functions.sort(key=lambda x: x['address'])
    return functions

def main():
    parser = argparse.ArgumentParser(description='Generate function config JSONs')
    parser.add_argument('--src', default='src/level2', help='Source directory')
    parser.add_argument('--output', default='config/decompiled_functions.json', help='Output JSON file')
    parser.add_argument('--level3-output', default='config/level3_functions.json', help='Level 3 output JSON')
    args = parser.parse_args()

    # Find project root
    script_dir = Path(__file__).parent
    project_root = script_dir.parent.parent
    src_dir = project_root / args.src

    print(f"Scanning {src_dir}...")
    functions = scan_directory(src_dir)

    print(f"Found {len(functions)} decompiled functions")

    # Count by category
    game_count = sum(1 for f in functions if f['category'] == 'game')
    lib_count = sum(1 for f in functions if f['category'] == 'library')
    leaf_count = sum(1 for f in functions if f['is_leaf'])

    print(f"  Game functions: {game_count}")
    print(f"  Library functions: {lib_count}")
    print(f"  Leaf functions: {leaf_count}")

    # Create config directory if needed
    config_dir = project_root / 'config'
    config_dir.mkdir(exist_ok=True)

    # Write decompiled functions JSON
    output_path = project_root / args.output
    config = {
        'version': 'SLPS-00065',
        'description': 'Level 2 decompiled functions (functionally correct)',
        'total_count': len(functions),
        'game_count': game_count,
        'library_count': lib_count,
        'functions': functions
    }

    with open(output_path, 'w') as f:
        json.dump(config, f, indent=2)
    print(f"Wrote {output_path}")

    # Write empty level3 JSON (byte-perfect matches - none yet)
    level3_path = project_root / args.level3_output
    level3_config = {
        'version': 'SLPS-00065',
        'description': 'Level 3 byte-perfect matched functions',
        'total_count': 0,
        'functions': []
    }

    with open(level3_path, 'w') as f:
        json.dump(level3_config, f, indent=2)
    print(f"Wrote {level3_path}")

    # Summary
    print(f"\nSummary:")
    print(f"  Level 2 (functional): {len(functions)} functions")
    print(f"  Level 3 (byte-perfect): 0 functions")

    return 0

if __name__ == '__main__':
    exit(main())
