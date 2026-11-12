#!/usr/bin/env python3
"""
Generate linker symbol definitions from function_map.json.

This creates --defsym flags for all functions so C code can call
them and the linker will resolve to the correct addresses.
"""

import json
import sys
from pathlib import Path


def main():
    # Look for function map in common locations
    possible_paths = [
        Path('build/function_analysis/function_map.json'),
        Path('config/function_map.json'),
        Path('build/jp/function_map.json'),
    ]

    function_map = None
    for path in possible_paths:
        if path.exists():
            with open(path) as f:
                function_map = json.load(f)
            break

    if function_map is None:
        # No function map found, output nothing
        sys.exit(0)

    # Generate --defsym flags
    symbols = []
    functions = function_map.get('functions', [])

    for func in functions:
        name = func.get('name', '')
        addr = func.get('address', 0)
        if name and addr:
            symbols.append(f"--defsym={name}=0x{addr:08X}")

    # Print all symbols (to be captured by shell)
    print(' '.join(symbols))


if __name__ == '__main__':
    main()
