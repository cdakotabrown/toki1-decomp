#!/usr/bin/env python3
"""
Apply debug symbol renames to matched functions.

This tool renames files and updates all references based on the
high-confidence matches from the symbol audit.

Usage:
    python3 apply_symbol_renames.py --dry-run        # Preview changes
    python3 apply_symbol_renames.py --apply          # Apply changes
    python3 apply_symbol_renames.py --apply --exact  # Only exact matches
"""

import argparse
import json
import os
import re
import shutil
from pathlib import Path

BASE_DIR = Path(__file__).resolve().parents[2]
RENAMES_FILE = BASE_DIR / "build/symbol_audit/high_confidence_renames.json"
ASM_DIR = BASE_DIR / "asm/main/matched"
SRC_DIR = BASE_DIR / "src/main/matched"
CONFIG_DIR = BASE_DIR / "config"

# Files to update references in
CONFIG_FILES = [
    CONFIG_DIR / "function_classification.json",
    CONFIG_DIR / "function_classification.csv",
    CONFIG_DIR / "decompiled_functions.json",
    CONFIG_DIR / "level3_functions.json",
    CONFIG_DIR / "asm_conversion_status.json",
]


def load_renames(exact_only=False):
    """Load rename mappings from audit results."""
    with open(RENAMES_FILE) as f:
        renames = json.load(f)

    if exact_only:
        renames = [r for r in renames if r["confidence"] == "exact"]

    return renames


def rename_file(old_path, new_path, dry_run=True):
    """Rename a file."""
    if dry_run:
        print(f"  [DRY-RUN] Would rename: {old_path.name} -> {new_path.name}")
        return True

    if old_path.exists():
        # Backup the original
        backup = old_path.with_suffix(old_path.suffix + ".bak")
        if backup.exists():
            backup.unlink()

        try:
            shutil.move(str(old_path), str(new_path))
            print(f"  [RENAMED] {old_path.name} -> {new_path.name}")
            return True
        except Exception as e:
            print(f"  [ERROR] Failed to rename {old_path}: {e}")
            return False
    else:
        print(f"  [SKIP] File not found: {old_path}")
        return False


def update_json_file(file_path, old_name, new_name, dry_run=True):
    """Update references in a JSON file."""
    if not file_path.exists():
        return 0

    try:
        content = file_path.read_text()
        original = content

        # Replace all variations of the name
        old_stem = Path(old_name).stem  # disp_mode_set2
        new_stem = new_name  # srn_vram_set

        # Replace function name references
        content = content.replace(f'"{old_stem}"', f'"{new_stem}"')
        content = content.replace(f"'{old_stem}'", f"'{new_stem}'")

        # Replace file references
        for ext in ['.s', '.c', '.o']:
            content = content.replace(f'"{old_stem}{ext}"', f'"{new_stem}{ext}"')
            content = content.replace(f"'{old_stem}{ext}'", f"'{new_stem}{ext}'")

        # Replace path references
        content = content.replace(f'/{old_stem}.', f'/{new_stem}.')

        if content != original:
            if dry_run:
                print(f"    [DRY-RUN] Would update {file_path.name}")
                return 1
            else:
                file_path.write_text(content)
                print(f"    [UPDATED] {file_path.name}")
                return 1

        return 0

    except Exception as e:
        print(f"    [ERROR] Failed to update {file_path}: {e}")
        return 0


def update_csv_file(file_path, old_name, new_name, dry_run=True):
    """Update references in a CSV file."""
    if not file_path.exists():
        return 0

    try:
        content = file_path.read_text()
        original = content

        old_stem = Path(old_name).stem
        new_stem = new_name

        # Replace in CSV (comma-separated)
        content = content.replace(f',{old_stem},', f',{new_stem},')
        content = content.replace(f',{old_stem}\n', f',{new_stem}\n')

        if content != original:
            if dry_run:
                print(f"    [DRY-RUN] Would update {file_path.name}")
                return 1
            else:
                file_path.write_text(content)
                print(f"    [UPDATED] {file_path.name}")
                return 1

        return 0

    except Exception as e:
        print(f"    [ERROR] Failed to update {file_path}: {e}")
        return 0


def generate_symbol_header(renames, output_path, dry_run=True):
    """Generate a header file with function declarations."""
    content = """/* Auto-generated from o.bin debug symbols */
/* Do not edit manually */

#ifndef TOKIMEKI_SYMBOLS_H
#define TOKIMEKI_SYMBOLS_H

/* Function declarations from debug symbols */

"""

    by_category = {
        "game": [],
        "sdk": [],
        "sound": [],
        "graphics": [],
        "other": []
    }

    for r in renames:
        name = r["suggested_name"]

        # Categorize by prefix
        if name.startswith(("Cd", "Dec")):
            by_category["sdk"].append(r)
        elif name.startswith(("Ss", "Spu", "Snd_", "SD_")):
            by_category["sound"].append(r)
        elif name.startswith(("Gs", "Gt")):
            by_category["graphics"].append(r)
        else:
            by_category["game"].append(r)

    for category, funcs in by_category.items():
        if funcs:
            content += f"/* {category.upper()} functions */\n"
            for r in funcs:
                content += f"extern void {r['suggested_name']}(void);  /* {r['address']} */\n"
            content += "\n"

    content += "#endif /* TOKIMEKI_SYMBOLS_H */\n"

    if dry_run:
        print(f"\n[DRY-RUN] Would generate: {output_path}")
        print(f"  Contains {len(renames)} function declarations")
    else:
        output_path.parent.mkdir(parents=True, exist_ok=True)
        output_path.write_text(content)
        print(f"\n[GENERATED] {output_path}")
        print(f"  Contains {len(renames)} function declarations")


def main():
    parser = argparse.ArgumentParser(description="Apply symbol renames")
    parser.add_argument("--dry-run", action="store_true",
                        help="Preview changes without applying")
    parser.add_argument("--apply", action="store_true",
                        help="Actually apply the changes")
    parser.add_argument("--exact", action="store_true",
                        help="Only use exact matches (most conservative)")
    parser.add_argument("--header-only", action="store_true",
                        help="Only generate header file, don't rename")
    args = parser.parse_args()

    if not args.dry_run and not args.apply:
        print("Error: Must specify --dry-run or --apply")
        print("Use --dry-run to preview changes first")
        return 1

    dry_run = not args.apply

    print("=" * 60)
    print("Symbol Rename Tool")
    print("=" * 60)

    if not RENAMES_FILE.exists():
        print(f"Error: Rename file not found: {RENAMES_FILE}")
        print("Run audit_symbol_coverage.py and match_symbols_proximity.py first")
        return 1

    renames = load_renames(exact_only=args.exact)
    print(f"\nLoaded {len(renames)} renames ({'exact only' if args.exact else 'exact + close'})")

    if args.header_only:
        header_path = BASE_DIR / "include/tokimeki_symbols.h"
        generate_symbol_header(renames, header_path, dry_run)
        return 0

    # Process each rename
    files_renamed = 0
    configs_updated = 0

    print("\n" + "-" * 60)
    print("File Renames")
    print("-" * 60)

    for r in renames:
        addr = r["address"]
        old_file = r["current_file"]
        new_name = r["suggested_name"]
        ftype = r["type"]

        # Determine directories
        if ftype == "asm":
            old_path = ASM_DIR / old_file
            new_path = ASM_DIR / f"{new_name}.s"
        else:
            old_path = SRC_DIR / old_file
            new_path = SRC_DIR / f"{new_name}.c"

        print(f"\n{addr}: {old_file} -> {new_name}.{ftype}")

        # Skip if new file already exists
        if new_path.exists() and not dry_run:
            print(f"  [SKIP] Target already exists: {new_path.name}")
            continue

        if rename_file(old_path, new_path, dry_run):
            files_renamed += 1

            # Update config files
            for config_file in CONFIG_FILES:
                if config_file.suffix == ".csv":
                    configs_updated += update_csv_file(
                        config_file, old_file, new_name, dry_run
                    )
                else:
                    configs_updated += update_json_file(
                        config_file, old_file, new_name, dry_run
                    )

    # Generate header
    header_path = BASE_DIR / "include/tokimeki_symbols.h"
    generate_symbol_header(renames, header_path, dry_run)

    # Summary
    print("\n" + "=" * 60)
    print("Summary")
    print("=" * 60)
    print(f"Files renamed: {files_renamed}")
    print(f"Config updates: {configs_updated}")
    print(f"Header generated: {header_path}")

    if dry_run:
        print("\n[DRY-RUN] No changes were made. Use --apply to apply changes.")
    else:
        print("\n[COMPLETE] Changes applied successfully.")

    return 0


if __name__ == "__main__":
    exit(main())
