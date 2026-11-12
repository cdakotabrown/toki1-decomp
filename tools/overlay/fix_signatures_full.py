#!/usr/bin/env python3
"""Fix ALL function signature inconsistencies in overlay source files.

This script updates extern declarations to match function definitions,
including return type and parameter mismatches.

Usage:
    python3 tools/overlay/fix_signatures_full.py OVERLAY_NAME [--dry-run]
    python3 tools/overlay/fix_signatures_full.py --all [--dry-run]
"""

import re
import sys
from pathlib import Path
from collections import defaultdict

BASE_DIR = Path(__file__).resolve().parent.parent.parent
OVERLAY_SRC = BASE_DIR / "src" / "overlay"

def find_definition(overlay_dir, func_name):
    """Find the canonical definition for a function."""
    def_file = overlay_dir / f"{func_name}.c"
    if not def_file.exists():
        return None

    content = def_file.read_text()

    # Look for function definition (not extern)
    # Match: return_type func_name(params) { or return_type func_name(params)\n{
    pattern = rf'^(\w+)\s+{func_name}\s*\(([^)]*)\)\s*(?:\{{|$)'
    for m in re.finditer(pattern, content, re.MULTILINE):
        ret_type, args = m.groups()
        if ret_type not in ('extern', 'static', 'typedef'):
            return (ret_type, args.strip())

    return None

def fix_overlay(overlay_name, dry_run=False):
    """Fix all signature inconsistencies in an overlay."""
    overlay_dir = OVERLAY_SRC / overlay_name
    if not overlay_dir.exists():
        print(f"Overlay not found: {overlay_name}")
        return 0

    # Collect all extern declarations and their locations
    externs = defaultdict(list)  # func_name -> [(file, line_num, full_line, ret_type, args)]

    for f in overlay_dir.glob("func_*.c"):
        content = f.read_text()
        lines = content.split('\n')

        for i, line in enumerate(lines):
            # Match extern declarations - handle multiline by looking at joined content
            m = re.search(r'extern\s+(\w+)\s+(func_[0-9A-Fa-f]+)\s*\(([^)]*)\)\s*;', line)
            if m:
                ret_type, name, args = m.groups()
                externs[name].append((f, i, line, ret_type, args.strip()))

    fixes = 0
    files_to_update = defaultdict(dict)  # file -> {line_num: new_line}

    for func_name, extern_list in externs.items():
        # Get the canonical definition
        definition = find_definition(overlay_dir, func_name)
        if not definition:
            continue

        def_ret, def_args = definition

        # Normalize args for comparison
        def_args_norm = def_args if def_args and def_args != 'void' else 'void'

        for f, line_num, line, ext_ret, ext_args in extern_list:
            ext_args_norm = ext_args if ext_args and ext_args != '' else 'void'

            # Check if mismatch
            if ext_ret != def_ret or ext_args_norm != def_args_norm:
                # Build corrected extern declaration
                if def_args == '' or def_args == 'void':
                    new_args = 'void'
                else:
                    new_args = def_args

                new_decl = f"extern {def_ret} {func_name}({new_args});"

                # Replace in the line
                old_pattern = rf'extern\s+\w+\s+{func_name}\s*\([^)]*\)\s*;'
                new_line = re.sub(old_pattern, new_decl, line)

                if new_line != line:
                    files_to_update[f][line_num] = new_line
                    fixes += 1
                    if not dry_run:
                        print(f"  {f.name}:{line_num+1}: {ext_ret} {func_name}({ext_args}) -> {def_ret} {func_name}({new_args})")

    # Apply fixes
    if not dry_run:
        for f, line_fixes in files_to_update.items():
            content = f.read_text()
            lines = content.split('\n')
            for line_num, new_line in line_fixes.items():
                lines[line_num] = new_line
            f.write_text('\n'.join(lines))

    return fixes

def main():
    dry_run = "--dry-run" in sys.argv
    args = [a for a in sys.argv[1:] if not a.startswith("-")]

    if "--all" in sys.argv or not args:
        overlays = sorted([d.name for d in OVERLAY_SRC.iterdir() if d.is_dir()])
    else:
        overlays = args

    total_fixes = 0
    for ovl in overlays:
        print(f"Processing {ovl}...")
        fixes = fix_overlay(ovl, dry_run)
        if fixes > 0:
            print(f"  {fixes} fixes" + (" (dry-run)" if dry_run else " applied"))
        total_fixes += fixes

    print(f"\nTotal: {total_fixes} fixes" + (" (dry-run)" if dry_run else " applied"))
    return 0

if __name__ == "__main__":
    sys.exit(main())
