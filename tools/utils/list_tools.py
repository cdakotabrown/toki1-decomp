#!/usr/bin/env python3
"""
list_tools.py - List all available decompilation tools

Shows all tools with descriptions and usage examples.

Usage:
    python3 tools/list_tools.py
"""

from pathlib import Path

# Tool catalog
TOOLS = [
    {
        'name': 'extract_all_functions.py',
        'category': 'Analysis',
        'description': 'Extract and analyze all 842 functions from binary',
        'usage': 'python3 tools/extract_all_functions.py --verbose',
        'output': 'build/function_analysis/function_map.json, TOP_50_CANDIDATES.md',
    },
    {
        'name': 'match_libraries.py',
        'category': 'Analysis',
        'description': 'Identify PSY-Q library functions via signature matching',
        'usage': 'python3 tools/match_libraries.py --min-length 36',
        'output': 'config/symbols_libraries.txt, updated function_map.json',
    },
    {
        'name': 'progress.py',
        'category': 'Analysis',
        'description': 'Track decompilation progress and statistics',
        'usage': 'python3 tools/progress.py --verbose',
        'output': 'Console output with progress percentage',
    },
    {
        'name': 'm2ctx.py',
        'category': 'Decompilation',
        'description': 'Generate context file for m2c/decomp.me',
        'usage': 'python3 build/m2ctx.py src/main/functions.c',
        'output': 'ctx.c (context for decompilation)',
    },
    {
        'name': 'decompile.py',
        'category': 'Decompilation',
        'description': 'Automated decompilation workflow helper',
        'usage': 'python3 tools/decompile.py func_800ACBC0',
        'output': 'Generated C code and assembly',
    },
    {
        'name': 'diff_binary.py',
        'category': 'Verification',
        'description': 'Quick binary comparison tool',
        'usage': 'python3 tools/diff_binary.py --verbose',
        'output': 'Byte-level differences between original and rebuilt',
    },
    {
        'name': 'm2c',
        'category': 'Decompilation',
        'description': 'MIPS assembly to C decompiler',
        'usage': 'python3 build/m2c/m2c.py function.s --context ctx.c',
        'output': 'C code from assembly',
    },
    {
        'name': 'asm-differ',
        'category': 'Verification',
        'description': 'Compare assembly output side-by-side',
        'usage': 'python3 tools/asm-differ/diff.py -o overlay func_name',
        'output': 'Visual diff of assembly',
    },
    {
        'name': 'decomp-permuter',
        'category': 'Matching',
        'description': 'Find matching code variants',
        'usage': 'python3 build/decomp-permuter/permuter.py function.c',
        'output': 'Alternate C implementations that match',
    },
    {
        'name': 'maspsx',
        'category': 'Build',
        'description': 'PSX assembler',
        'usage': 'python3 tools/maspsx/maspsx.py file.s',
        'output': 'Assembled object file',
    },
]

def print_tools():
    """Print tool listing"""
    print("=" * 100)
    print("TOKIMEKI MEMORIAL DECOMPILATION TOOLS")
    print("=" * 100)
    print()

    # Group by category
    categories = {}
    for tool in TOOLS:
        cat = tool['category']
        if cat not in categories:
            categories[cat] = []
        categories[cat].append(tool)

    # Print by category
    for category in ['Analysis', 'Decompilation', 'Verification', 'Matching', 'Build']:
        if category not in categories:
            continue

        print(f"## {category} Tools")
        print()

        for tool in categories[category]:
            print(f"### {tool['name']}")
            print(f"    {tool['description']}")
            print()
            print(f"    Usage:")
            print(f"        {tool['usage']}")
            print()
            if tool.get('output'):
                print(f"    Output:")
                print(f"        {tool['output']}")
                print()

    print("=" * 100)
    print()
    print("Quick Start Workflow:")
    print("  1. Analyze:      python3 tools/extract_all_functions.py")
    print("  2. Find libs:    python3 tools/match_libraries.py")
    print("  3. Check progress: python3 tools/progress.py")
    print("  4. Decompile:    python3 tools/decompile.py func_800ACBC0")
    print("  5. Build:        make")
    print("  6. Verify:       python3 tools/diff_binary.py")
    print()

if __name__ == "__main__":
    print_tools()
