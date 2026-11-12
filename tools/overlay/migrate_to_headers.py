#!/usr/bin/env python3
"""
migrate_to_headers.py - Migrate overlay C files to use centralized headers

This script analyzes overlay C files and:
1. Extracts all extern declarations
2. Categorizes them (main binary functions, main binary globals, overlay-local)
3. Updates C files to use include/overlay/common.h where possible
4. Generates a report of overlay-local symbols that need inline externs

Usage:
    python3 tools/overlay/migrate_to_headers.py --analyze          # Analyze without changes
    python3 tools/overlay/migrate_to_headers.py --migrate          # Migrate all files
    python3 tools/overlay/migrate_to_headers.py --migrate --overlay DATE  # Migrate one overlay
    python3 tools/overlay/migrate_to_headers.py --generate-headers # Generate header content from analysis

Output:
    - Updated C files with #include "overlay/common.h"
    - Report of symbols per overlay
    - Suggested additions to functions.h and globals.h
"""

import os
import re
import sys
import argparse
import json
from pathlib import Path
from typing import Dict, List, Set, Tuple, Optional
from collections import defaultdict
from dataclasses import dataclass, field

# Project root
ROOT = Path(__file__).parent.parent.parent
SRC_OVERLAY_DIR = ROOT / "src" / "overlay"
INCLUDE_OVERLAY_DIR = ROOT / "include" / "overlay"

# Address ranges
MAIN_BINARY_START = 0x80041000
MAIN_BINARY_END = 0x800BFFFF
OVERLAY_START = 0x80130000

@dataclass
class ExternDecl:
    """Represents an extern declaration."""
    raw_line: str
    symbol_name: str
    symbol_type: str  # 'function' or 'variable'
    return_type: str
    params: Optional[str]  # For functions
    address: Optional[int]

    @property
    def is_main_binary(self) -> bool:
        """Check if symbol is in main binary address range."""
        if self.address is None:
            return False
        return MAIN_BINARY_START <= self.address <= MAIN_BINARY_END

    @property
    def is_overlay_local(self) -> bool:
        """Check if symbol is in overlay address range."""
        if self.address is None:
            return False
        return self.address >= OVERLAY_START


@dataclass
class FileAnalysis:
    """Analysis results for a single C file."""
    path: Path
    overlay_name: str
    function_name: str
    extern_decls: List[ExternDecl] = field(default_factory=list)
    main_binary_functions: List[ExternDecl] = field(default_factory=list)
    main_binary_globals: List[ExternDecl] = field(default_factory=list)
    overlay_local_functions: List[ExternDecl] = field(default_factory=list)
    overlay_local_globals: List[ExternDecl] = field(default_factory=list)
    other_externs: List[ExternDecl] = field(default_factory=list)


def parse_extern_declaration(line: str) -> Optional[ExternDecl]:
    """Parse an extern declaration line."""
    line = line.strip()
    if not line.startswith('extern'):
        return None

    # Remove 'extern ' prefix
    decl = line[7:].strip()

    # Extract address from comment if present
    address = None
    addr_match = re.search(r'/\*.*?(0x[0-9A-Fa-f]+)', line)
    if addr_match:
        try:
            address = int(addr_match.group(1), 16)
        except ValueError:
            pass

    # Determine if function or variable
    # Functions have parentheses, variables don't (or have array brackets)
    if '(' in decl and ')' in decl:
        # Function declaration
        match = re.match(r'(\w+(?:\s*\*)?)\s+(\w+)\s*\(([^)]*)\)', decl)
        if match:
            return_type = match.group(1).strip()
            symbol_name = match.group(2).strip()
            params = match.group(3).strip()

            # Extract address from function name if it matches func_XXXXXXXX pattern
            if address is None:
                addr_match = re.search(r'func_([0-9A-Fa-f]{8})', symbol_name)
                if addr_match:
                    address = int(addr_match.group(1), 16)

            return ExternDecl(
                raw_line=line,
                symbol_name=symbol_name,
                symbol_type='function',
                return_type=return_type,
                params=params,
                address=address
            )
    else:
        # Variable declaration
        # Handle arrays: extern Type Name[size];
        # Handle pointers: extern Type *Name;
        match = re.match(r'([\w\s*]+?)\s+(\w+)(\[[^\]]*\])?\s*;', decl)
        if match:
            return_type = match.group(1).strip()
            symbol_name = match.group(2).strip()
            array_suffix = match.group(3) or ''

            # Extract address from variable name if it matches D_XXXXXXXX pattern
            if address is None:
                addr_match = re.search(r'D_([0-9A-Fa-f]{8})', symbol_name)
                if addr_match:
                    address = int(addr_match.group(1), 16)

            return ExternDecl(
                raw_line=line,
                symbol_name=symbol_name,
                symbol_type='variable',
                return_type=return_type + array_suffix,
                params=None,
                address=address
            )

    return None


def analyze_file(c_file: Path) -> FileAnalysis:
    """Analyze a single C file for extern declarations."""
    overlay_name = c_file.parent.name
    function_name = c_file.stem

    analysis = FileAnalysis(
        path=c_file,
        overlay_name=overlay_name,
        function_name=function_name
    )

    with open(c_file, 'r') as f:
        content = f.read()

    # Find all extern declarations
    for line in content.split('\n'):
        line_stripped = line.strip()
        if line_stripped.startswith('extern'):
            decl = parse_extern_declaration(line_stripped)
            if decl:
                analysis.extern_decls.append(decl)

                # Categorize
                if decl.is_main_binary:
                    if decl.symbol_type == 'function':
                        analysis.main_binary_functions.append(decl)
                    else:
                        analysis.main_binary_globals.append(decl)
                elif decl.is_overlay_local:
                    if decl.symbol_type == 'function':
                        analysis.overlay_local_functions.append(decl)
                    else:
                        analysis.overlay_local_globals.append(decl)
                else:
                    analysis.other_externs.append(decl)

    return analysis


def analyze_all_overlays() -> Dict[str, List[FileAnalysis]]:
    """Analyze all overlay C files."""
    results = defaultdict(list)

    for overlay_dir in sorted(SRC_OVERLAY_DIR.iterdir()):
        if not overlay_dir.is_dir():
            continue
        if overlay_dir.name.startswith('.') or overlay_dir.name == 'README.md':
            continue

        for c_file in sorted(overlay_dir.glob('*.c')):
            analysis = analyze_file(c_file)
            results[overlay_dir.name].append(analysis)

    return results


def collect_unique_symbols(analyses: Dict[str, List[FileAnalysis]]) -> Tuple[Dict, Dict]:
    """Collect unique main binary symbols across all overlays."""
    functions = {}  # symbol_name -> list of declarations
    globals_ = {}   # symbol_name -> list of declarations

    for overlay_name, file_analyses in analyses.items():
        for analysis in file_analyses:
            for decl in analysis.main_binary_functions:
                if decl.symbol_name not in functions:
                    functions[decl.symbol_name] = []
                functions[decl.symbol_name].append(decl)

            for decl in analysis.main_binary_globals:
                if decl.symbol_name not in globals_:
                    globals_[decl.symbol_name] = []
                globals_[decl.symbol_name].append(decl)

    return functions, globals_


def generate_functions_header(functions: Dict[str, List[ExternDecl]]) -> str:
    """Generate content for functions.h based on analysis."""
    lines = [
        "/**",
        " * include/overlay/functions.h",
        " *",
        " * Function prototypes for main binary functions called from overlays.",
        " * Auto-generated from overlay source analysis.",
        " */",
        "",
        "#ifndef OVERLAY_FUNCTIONS_H",
        "#define OVERLAY_FUNCTIONS_H",
        "",
        "#include \"types.h\"",
        "",
    ]

    # Sort by address
    sorted_funcs = sorted(functions.items(), key=lambda x: x[1][0].address or 0)

    current_prefix = None
    for func_name, decls in sorted_funcs:
        # Add section headers
        addr = decls[0].address
        if addr:
            prefix = (addr & 0xFFFF0000) >> 16
            if prefix != current_prefix:
                current_prefix = prefix
                lines.append(f"")
                lines.append(f"/* 0x{prefix:04X}xxxx functions */")

        # Pick the most detailed declaration
        best_decl = max(decls, key=lambda d: len(d.params or ''))

        # Format the declaration
        params = best_decl.params if best_decl.params else 'void'
        ret = best_decl.return_type
        lines.append(f"extern {ret} {func_name}({params});")

    lines.extend([
        "",
        "#endif /* OVERLAY_FUNCTIONS_H */",
    ])

    return '\n'.join(lines)


def generate_globals_header(globals_: Dict[str, List[ExternDecl]]) -> str:
    """Generate content for globals.h based on analysis."""
    lines = [
        "/**",
        " * include/overlay/globals.h",
        " *",
        " * Global variable declarations for main binary variables accessed from overlays.",
        " * Auto-generated from overlay source analysis.",
        " */",
        "",
        "#ifndef OVERLAY_GLOBALS_H",
        "#define OVERLAY_GLOBALS_H",
        "",
        "#include \"types.h\"",
        "",
    ]

    # Sort by address
    sorted_vars = sorted(globals_.items(), key=lambda x: x[1][0].address or 0)

    current_prefix = None
    for var_name, decls in sorted_vars:
        # Add section headers
        addr = decls[0].address
        if addr:
            prefix = (addr & 0xFFFF0000) >> 16
            if prefix != current_prefix:
                current_prefix = prefix
                lines.append(f"")
                lines.append(f"/* 0x{prefix:04X}xxxx globals */")

        # Pick the most detailed declaration
        best_decl = max(decls, key=lambda d: len(d.return_type or ''))

        # Format the declaration
        var_type = best_decl.return_type
        lines.append(f"extern {var_type} {var_name};")

    lines.extend([
        "",
        "#endif /* OVERLAY_GLOBALS_H */",
    ])

    return '\n'.join(lines)


def migrate_file(analysis: FileAnalysis, dry_run: bool = False) -> bool:
    """
    Migrate a single C file to use overlay/common.h.

    Returns True if the file was modified (or would be in dry_run mode).
    """
    with open(analysis.path, 'r') as f:
        content = f.read()

    lines = content.split('\n')
    new_lines = []
    modified = False
    include_added = False
    in_extern_block = False
    extern_lines_to_keep = []

    # Collect overlay-local extern lines that need to stay
    overlay_local_symbols = set()
    for decl in analysis.overlay_local_functions + analysis.overlay_local_globals:
        overlay_local_symbols.add(decl.symbol_name)

    for i, line in enumerate(lines):
        stripped = line.strip()

        # Check if this is an #include "types.h" line
        if stripped == '#include "types.h"':
            # Replace with overlay/common.h if we have main binary externs
            if analysis.main_binary_functions or analysis.main_binary_globals:
                new_lines.append('#include "overlay/common.h"')
                include_added = True
                modified = True
            else:
                new_lines.append(line)
            continue

        # Check if this is an extern declaration
        if stripped.startswith('extern'):
            decl = parse_extern_declaration(stripped)
            if decl:
                # Keep overlay-local externs, remove main binary externs
                if decl.is_overlay_local:
                    new_lines.append(line)
                elif decl.is_main_binary:
                    # Skip this line - it's covered by the header
                    if include_added:
                        modified = True
                        continue
                    else:
                        # Keep for now if no include was added yet
                        new_lines.append(line)
                else:
                    # Unknown extern - keep it
                    new_lines.append(line)
                continue

        new_lines.append(line)

    if modified:
        new_content = '\n'.join(new_lines)

        if dry_run:
            print(f"Would modify: {analysis.path}")
            return True
        else:
            with open(analysis.path, 'w') as f:
                f.write(new_content)
            print(f"Modified: {analysis.path}")
            return True

    return False


def print_analysis_report(analyses: Dict[str, List[FileAnalysis]]):
    """Print a summary report of the analysis."""
    total_files = 0
    total_externs = 0
    main_binary_func_count = 0
    main_binary_glob_count = 0
    overlay_local_func_count = 0
    overlay_local_glob_count = 0

    # Collect unique symbols
    unique_main_funcs = set()
    unique_main_globs = set()

    for overlay_name, file_analyses in sorted(analyses.items()):
        overlay_externs = 0
        for analysis in file_analyses:
            total_files += 1
            total_externs += len(analysis.extern_decls)
            main_binary_func_count += len(analysis.main_binary_functions)
            main_binary_glob_count += len(analysis.main_binary_globals)
            overlay_local_func_count += len(analysis.overlay_local_functions)
            overlay_local_glob_count += len(analysis.overlay_local_globals)
            overlay_externs += len(analysis.extern_decls)

            for decl in analysis.main_binary_functions:
                unique_main_funcs.add(decl.symbol_name)
            for decl in analysis.main_binary_globals:
                unique_main_globs.add(decl.symbol_name)

    print("=" * 60)
    print("OVERLAY EXTERN DECLARATION ANALYSIS")
    print("=" * 60)
    print(f"Total overlay C files:        {total_files}")
    print(f"Total extern declarations:    {total_externs}")
    print()
    print("BREAKDOWN BY CATEGORY:")
    print(f"  Main binary functions:      {main_binary_func_count} ({len(unique_main_funcs)} unique)")
    print(f"  Main binary globals:        {main_binary_glob_count} ({len(unique_main_globs)} unique)")
    print(f"  Overlay-local functions:    {overlay_local_func_count}")
    print(f"  Overlay-local globals:      {overlay_local_glob_count}")
    print()
    print("POTENTIAL HEADER CONSOLIDATION:")
    print(f"  Functions for functions.h:  {len(unique_main_funcs)}")
    print(f"  Globals for globals.h:      {len(unique_main_globs)}")
    print()

    # Per-overlay breakdown
    print("PER-OVERLAY SUMMARY:")
    for overlay_name, file_analyses in sorted(analyses.items()):
        files = len(file_analyses)
        externs = sum(len(a.extern_decls) for a in file_analyses)
        main_funcs = sum(len(a.main_binary_functions) for a in file_analyses)
        main_globs = sum(len(a.main_binary_globals) for a in file_analyses)
        print(f"  {overlay_name:12s}: {files:4d} files, {externs:5d} externs "
              f"({main_funcs:4d} funcs, {main_globs:4d} globs)")


def main():
    parser = argparse.ArgumentParser(description='Migrate overlay C files to use centralized headers')
    parser.add_argument('--analyze', action='store_true', help='Analyze without making changes')
    parser.add_argument('--migrate', action='store_true', help='Migrate files to use headers')
    parser.add_argument('--overlay', type=str, help='Only process specific overlay')
    parser.add_argument('--generate-headers', action='store_true', help='Generate header content')
    parser.add_argument('--dry-run', action='store_true', help='Show what would be changed without changing')
    parser.add_argument('--json', action='store_true', help='Output analysis as JSON')

    args = parser.parse_args()

    if not any([args.analyze, args.migrate, args.generate_headers]):
        args.analyze = True  # Default to analyze

    # Run analysis
    print("Analyzing overlay source files...")
    analyses = analyze_all_overlays()

    if args.overlay:
        if args.overlay not in analyses:
            print(f"Error: Overlay '{args.overlay}' not found")
            sys.exit(1)
        analyses = {args.overlay: analyses[args.overlay]}

    if args.json:
        # Output as JSON
        output = {}
        for overlay_name, file_analyses in analyses.items():
            output[overlay_name] = []
            for analysis in file_analyses:
                output[overlay_name].append({
                    'file': str(analysis.path),
                    'function': analysis.function_name,
                    'main_binary_functions': [d.symbol_name for d in analysis.main_binary_functions],
                    'main_binary_globals': [d.symbol_name for d in analysis.main_binary_globals],
                    'overlay_local_functions': [d.symbol_name for d in analysis.overlay_local_functions],
                    'overlay_local_globals': [d.symbol_name for d in analysis.overlay_local_globals],
                })
        print(json.dumps(output, indent=2))
        return

    if args.analyze:
        print_analysis_report(analyses)

    if args.generate_headers:
        functions, globals_ = collect_unique_symbols(analyses)

        print("\n" + "=" * 60)
        print("GENERATED functions.h CONTENT:")
        print("=" * 60)
        print(generate_functions_header(functions))

        print("\n" + "=" * 60)
        print("GENERATED globals.h CONTENT:")
        print("=" * 60)
        print(generate_globals_header(globals_))

    if args.migrate:
        print("\n" + "=" * 60)
        print("MIGRATING FILES...")
        print("=" * 60)

        modified_count = 0
        for overlay_name, file_analyses in sorted(analyses.items()):
            for analysis in file_analyses:
                if migrate_file(analysis, dry_run=args.dry_run):
                    modified_count += 1

        print(f"\nModified {modified_count} files")


if __name__ == '__main__':
    main()
