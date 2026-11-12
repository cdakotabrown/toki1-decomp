#!/usr/bin/env python3
"""Fix inconsistent function signatures in overlay source files.

Strategy:
1. Find the DEFINITION of each function (the actual implementation)
2. Update all EXTERN declarations to match the definition
3. If no definition exists in this overlay, use the most common signature

Usage:
    python3 tools/overlay/fix_signatures.py OVERLAY_NAME [OVERLAY_NAME2 ...]
    python3 tools/overlay/fix_signatures.py --all
"""

import os
import re
import sys
from pathlib import Path
from collections import defaultdict, Counter

BASE_DIR = Path(__file__).resolve().parent.parent.parent
OVERLAY_SRC = BASE_DIR / "src" / "overlay"

def parse_signature(sig_str):
    """Parse 'ret_type(args)' into components."""
    m = re.match(r'(\w+)\(([^)]*)\)', sig_str)
    if m:
        return m.group(1), m.group(2).strip()
    return None, None

def normalize_args(args):
    """Normalize argument list for comparison."""
    if not args or args == 'void':
        return 'void'
    # Remove parameter names, keep only types
    parts = []
    for arg in args.split(','):
        arg = arg.strip()
        # Extract just the type (last word before any *)
        tokens = arg.split()
        if tokens:
            # Handle pointer types
            type_part = tokens[0]
            if len(tokens) > 1 and '*' in tokens[-1]:
                type_part += ' *'
            elif '*' in arg:
                type_part += ' *'
            parts.append(type_part)
    return ', '.join(parts) if parts else 'void'

def get_canonical_signature(overlay_dir, func_name):
    """Get the canonical signature for a function.

    Priority:
    1. Definition in this overlay (func_XXXX.c file)
    2. Most common extern declaration
    """
    signatures = []
    definition = None

    # Check if there's a definition file for this function
    def_file = overlay_dir / f"{func_name}.c"
    if def_file.exists():
        content = def_file.read_text()
        # Look for the function definition
        for m in re.finditer(rf'^(\w+)\s+{func_name}\s*\(([^)]*)\)\s*(?:\{{|$)', content, re.MULTILINE):
            ret_type, args = m.groups()
            if ret_type not in ('extern', 'static', 'typedef'):
                definition = (ret_type, args.strip())
                break

    if definition:
        return definition

    # No definition found, collect all extern declarations
    for f in overlay_dir.glob("func_*.c"):
        content = f.read_text()
        for m in re.finditer(rf'extern\s+(\w+)\s+{func_name}\s*\(([^)]*)\)\s*;', content):
            ret_type, args = m.groups()
            signatures.append((ret_type, args.strip()))

    if signatures:
        # Return most common signature
        counter = Counter(signatures)
        return counter.most_common(1)[0][0]

    return None

def fix_overlay(overlay_name, dry_run=False):
    """Fix all signature inconsistencies in an overlay."""
    overlay_dir = OVERLAY_SRC / overlay_name
    if not overlay_dir.exists():
        print(f"Overlay not found: {overlay_name}")
        return 0

    # First pass: collect all function references
    func_refs = defaultdict(list)  # func_name -> [(file, line_content, is_extern)]

    for f in overlay_dir.glob("func_*.c"):
        content = f.read_text()
        lines = content.split('\n')

        for i, line in enumerate(lines):
            # Find extern declarations
            for m in re.finditer(r'extern\s+(\w+)\s+(func_[0-9A-Fa-f]+)\s*\(([^)]*)\)\s*;', line):
                ret_type, name, args = m.groups()
                func_refs[name].append((f, i, line, True, ret_type, args.strip()))

            # Find definitions (not extern)
            for m in re.finditer(r'^(\w+)\s+(func_[0-9A-Fa-f]+)\s*\(([^)]*)\)\s*(?:\{|$)', line):
                ret_type, name, args = m.groups()
                if ret_type not in ('extern', 'static', 'typedef'):
                    func_refs[name].append((f, i, line, False, ret_type, args.strip()))

    # Second pass: fix inconsistencies
    fixes = 0
    files_to_update = defaultdict(dict)  # file -> {line_num: new_line}

    for func_name, refs in func_refs.items():
        # Check if there are inconsistencies (include raw args for () vs (void) detection)
        sigs = set((r[4], r[5]) for r in refs)  # Use raw args, not normalized
        if len(sigs) <= 1:
            continue

        # Get canonical signature
        canonical = get_canonical_signature(overlay_dir, func_name)
        if not canonical:
            continue

        canon_ret, canon_args = canonical

        # Fix all extern declarations that don't match
        for f, line_num, line, is_extern, ret_type, args in refs:
            if not is_extern:
                continue

            norm_args = normalize_args(args)
            canon_norm = normalize_args(canon_args)

            if ret_type != canon_ret or norm_args != canon_norm or args != canon_args:
                # Build new extern declaration
                new_decl = f"extern {canon_ret} {func_name}({canon_args});"
                # Replace in line
                old_pattern = rf'extern\s+\w+\s+{func_name}\s*\([^)]*\)\s*;'
                new_line = re.sub(old_pattern, new_decl, line)

                if new_line != line:
                    files_to_update[f][line_num] = new_line
                    fixes += 1

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
        fixes = fix_overlay(ovl, dry_run)
        if fixes > 0:
            print(f"{ovl}: {fixes} fixes" + (" (dry-run)" if dry_run else ""))
        total_fixes += fixes

    print(f"\nTotal: {total_fixes} fixes" + (" (dry-run)" if dry_run else " applied"))
    return 0

if __name__ == "__main__":
    sys.exit(main())
