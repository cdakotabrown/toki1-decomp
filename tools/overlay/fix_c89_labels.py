#!/usr/bin/env python3
"""
fix_c89_labels.py - Fix C89 label-at-end-of-block issues

C89 requires a statement after every label. C99 relaxed this.
IDO 5.2 is C89 only, so we need to add empty statements after
labels that are followed by closing braces.

Usage:
    python3 tools/overlay/fix_c89_labels.py --all
    python3 tools/overlay/fix_c89_labels.py src/overlay/TACO/
"""

import sys
import re
from pathlib import Path

ROOT = Path(__file__).parent.parent.parent
SRC_OVERLAY_DIR = ROOT / "src" / "overlay"


def fix_label_at_end(content: str) -> tuple[str, int]:
    """
    Fix labels followed directly by } (C89 violation).
    Returns (fixed_content, count_of_fixes).
    """
    # Pattern: label followed by optional whitespace then }
    # We need to insert a semicolon (empty statement) after the label
    pattern = r'(\n[ \t]*\w+:)([ \t]*\n[ \t]*\})'

    count = 0
    def replacer(m):
        nonlocal count
        count += 1
        # Insert newline + indent + semicolon after the label
        return m.group(1) + '\n    ;' + m.group(2)

    fixed = re.sub(pattern, replacer, content)
    return fixed, count


def ensure_trailing_newline(content: str) -> str:
    """Ensure file ends with a newline."""
    if content and not content.endswith('\n'):
        return content + '\n'
    return content


def process_file(path: Path) -> tuple[bool, int]:
    """Process a single C file. Returns (changed, fix_count)."""
    try:
        content = path.read_text()
        original = content

        # Fix label-at-end issues
        content, label_fixes = fix_label_at_end(content)

        # Ensure trailing newline
        content = ensure_trailing_newline(content)

        if content != original:
            path.write_text(content)
            return True, label_fixes
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
        print("Usage: python3 tools/overlay/fix_c89_labels.py <directory>")
        print("       python3 tools/overlay/fix_c89_labels.py --all")
        sys.exit(1)

    if sys.argv[1] == "--all":
        total_changed = 0
        total_fixes = 0

        for overlay_dir in sorted(SRC_OVERLAY_DIR.iterdir()):
            if overlay_dir.is_dir() and not overlay_dir.name.startswith('.'):
                changed, fixes = process_directory(overlay_dir)
                if changed > 0:
                    print(f"[{overlay_dir.name}] Fixed {fixes} labels in {changed} files")
                total_changed += changed
                total_fixes += fixes

        print(f"\n[DONE] Fixed {total_fixes} label issues in {total_changed} files")
    else:
        dir_path = Path(sys.argv[1])
        if not dir_path.is_dir():
            print(f"[ERROR] Not a directory: {dir_path}")
            sys.exit(1)

        changed, fixes = process_directory(dir_path)
        print(f"[DONE] Fixed {fixes} label issues in {changed} files")


if __name__ == "__main__":
    main()
