#!/usr/bin/env python3
"""
fix_c89_decls.py - Fix C89 declaration-after-statement issues

C89 requires all declarations at the start of a block.
This script finds declarations after statements and moves them up.

Usage:
    python3 tools/overlay/fix_c89_decls.py --all
    python3 tools/overlay/fix_c89_decls.py src/overlay/TT/
"""

import sys
import re
from pathlib import Path

ROOT = Path(__file__).parent.parent.parent
SRC_OVERLAY_DIR = ROOT / "src" / "overlay"

# Pattern to match variable declarations
DECL_PATTERN = re.compile(
    r'^(\s+)(u8|s8|u16|s16|u32|s32|void|char|short|int|long|unsigned)\s+(\w+)'
)

# Pattern to match statements (assignments, function calls, etc.)
STMT_PATTERN = re.compile(
    r'^(\s+)(\w+\s*=|if\s*\(|while\s*\(|for\s*\(|switch\s*\(|return\s|goto\s|\*|/\*)'
)


def fix_declarations(content: str) -> tuple[str, int]:
    """
    Find and fix declaration-after-statement issues.
    Returns (fixed_content, count_of_fixes).
    """
    lines = content.split('\n')
    fixes = 0

    # Process each function body
    i = 0
    while i < len(lines):
        line = lines[i]

        # Look for function body start
        if line.strip() == '{' or (line.rstrip().endswith('{') and '(' in line):
            # Find the declaration section
            brace_count = 1
            func_start = i
            first_stmt = None
            decls_to_move = []

            j = i + 1
            while j < len(lines) and brace_count > 0:
                curr = lines[j]
                stripped = curr.strip()

                # Track brace depth
                brace_count += curr.count('{') - curr.count('}')

                # Skip empty lines and comments
                if not stripped or stripped.startswith('/*') or stripped.startswith('*') or stripped.startswith('//'):
                    j += 1
                    continue

                # Check if it's a declaration
                decl_match = DECL_PATTERN.match(curr)
                stmt_match = STMT_PATTERN.match(curr)

                if decl_match and first_stmt is not None:
                    # Declaration after statement - need to move it
                    indent = decl_match.group(1)
                    type_name = decl_match.group(2)
                    var_name = decl_match.group(3)

                    # Extract the full declaration line
                    decl_line = curr.rstrip()

                    # Check if it has initialization
                    if '=' in decl_line:
                        # Split into declaration and assignment
                        # e.g., "    s32 at = 0;" -> "    s32 at;" and "    at = 0;"
                        parts = decl_line.split('=', 1)
                        decl_part = parts[0].rstrip() + ';'
                        # Get the var name(s) and create assignment
                        var_match = re.search(r'(\w+)\s*$', parts[0])
                        if var_match:
                            var_n = var_match.group(1)
                            assign_part = f"{indent}{var_n} ={parts[1]}"
                            decls_to_move.append((j, decl_part, assign_part))
                    else:
                        # Just a declaration, move it up
                        decls_to_move.append((j, decl_line, None))
                elif stmt_match or (stripped and not stripped.startswith('}') and not DECL_PATTERN.match(curr)):
                    if first_stmt is None:
                        first_stmt = j

                j += 1

            # Apply fixes in reverse order to maintain line indices
            if decls_to_move:
                for line_idx, decl, assign in reversed(decls_to_move):
                    # Remove the original line
                    lines[line_idx] = assign if assign else ''

                    # Insert declaration after function open brace
                    insert_pos = func_start + 1
                    lines.insert(insert_pos, decl)
                    fixes += 1

            i = j
        else:
            i += 1

    return '\n'.join(lines), fixes


def process_file(path: Path) -> tuple[bool, int]:
    """Process a single C file. Returns (changed, fix_count)."""
    try:
        content = path.read_text()
        original = content

        fixed, count = fix_declarations(content)

        if fixed != original:
            path.write_text(fixed)
            return True, count
        return False, 0
    except Exception as e:
        print(f"[ERROR] {path}: {e}")
        return False, 0


def process_directory(dir_path: Path) -> tuple[int, int]:
    """Process all C files in directory. Returns (files_changed, total_fixes)."""
    files_changed = 0
    total_fixes = 0

    for c_file in sorted(dir_path.glob("*.c")):
        changed, fixes = process_file(c_file)
        if changed:
            files_changed += 1
            total_fixes += fixes

    return files_changed, total_fixes


def main():
    if len(sys.argv) < 2:
        print("Usage: python3 tools/overlay/fix_c89_decls.py <directory>")
        print("       python3 tools/overlay/fix_c89_decls.py --all")
        sys.exit(1)

    if sys.argv[1] == "--all":
        total_changed = 0
        total_fixes = 0

        for overlay_dir in sorted(SRC_OVERLAY_DIR.iterdir()):
            if overlay_dir.is_dir() and not overlay_dir.name.startswith('.'):
                changed, fixes = process_directory(overlay_dir)
                if changed > 0:
                    print(f"[{overlay_dir.name}] Fixed {fixes} decls in {changed} files")
                total_changed += changed
                total_fixes += fixes

        print(f"\n[DONE] Fixed {total_fixes} declaration issues in {total_changed} files")
    else:
        dir_path = Path(sys.argv[1])
        if not dir_path.is_dir():
            print(f"[ERROR] Not a directory: {dir_path}")
            sys.exit(1)

        changed, fixes = process_directory(dir_path)
        print(f"[DONE] Fixed {fixes} declaration issues in {changed} files")


if __name__ == "__main__":
    main()
