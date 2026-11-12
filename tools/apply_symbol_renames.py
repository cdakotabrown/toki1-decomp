#!/usr/bin/env python3
"""Apply HIGH confidence symbol renames from symbol_match_results.json.

For each HIGH confidence assignment where the file is still named func_XXXXXXXX:
- Renames the source file (asm .s or src .c)
- Updates config/level3_functions.json (source, name fields)
- Updates config/function_classification.json (name, filepath fields)
- Updates config/asm_conversion_status.json (key rename)
- Adds "Address: 0xXXXXXXXX" header comment if missing
"""

import json
import os
import re
import sys

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ASM_DIR = os.path.join(BASE_DIR, "asm", "main", "matched")
SRC_DIR = os.path.join(BASE_DIR, "src", "main", "matched")
CONFIG_DIR = os.path.join(BASE_DIR, "config")

RESULTS_FILE = os.path.join(BASE_DIR, "build", "symbol_match", "symbol_match_results.json")
L3_FILE = os.path.join(CONFIG_DIR, "level3_functions.json")
FC_FILE = os.path.join(CONFIG_DIR, "function_classification.json")
ACS_FILE = os.path.join(CONFIG_DIR, "asm_conversion_status.json")


def load_json(path):
    with open(path, "r") as f:
        return json.load(f)


def save_json(path, data):
    with open(path, "w") as f:
        json.dump(data, f, indent=2, ensure_ascii=False)
        f.write("\n")


def get_high_assignments(results):
    """Get HIGH confidence assignments where retail_name starts with func_."""
    assignments = results.get("assignments", [])
    high = []
    for a in assignments:
        if a["confidence"] == "HIGH":
            addr = a["retail_addr"]
            addr_hex = addr[2:]  # strip 0x
            func_name = f"func_{addr_hex}"
            # Only include if current name is func_XXXXXXXX
            if a["retail_name"] == func_name:
                high.append(a)
    return high


def find_source_file(func_name):
    """Find the source file for a function (asm .s or src .c)."""
    asm_path = os.path.join(ASM_DIR, f"{func_name}.s")
    c_path = os.path.join(SRC_DIR, f"{func_name}.c")
    if os.path.exists(asm_path):
        return asm_path, "asm"
    elif os.path.exists(c_path):
        return c_path, "src"
    return None, None


def ensure_address_header(filepath, address, sym_name, file_type):
    """Add or update header comment with Address and symbol name info."""
    with open(filepath, "r") as f:
        content = f.read()

    has_address = bool(re.search(r"Address:\s*0x[0-9A-Fa-f]+", content))

    if file_type == "asm":
        # ASM files use # comments
        if has_address:
            # Check if Debug Symbol line exists, add if not
            if "Debug Symbol:" not in content:
                # Insert Debug Symbol line after the first line that has Address
                content = re.sub(
                    r"(#.*Address:\s*0x[0-9A-Fa-f]+.*\n)",
                    rf"\1# Debug Symbol: {sym_name} (from o.bin, high match)\n",
                    content,
                    count=1,
                )
            return content
        else:
            # Prepend address header
            header = f"# Debug Symbol: {sym_name} (from o.bin, high match)\n# Address: {address}\n"
            return header + content
    else:
        # C files use /* */ comments
        if has_address:
            if "Debug Symbol:" not in content:
                content = re.sub(
                    r"(\*.*Address:\s*0x[0-9A-Fa-f]+.*\n)",
                    rf"\1 * Debug Symbol: {sym_name} (from o.bin, high match)\n",
                    content,
                    count=1,
                )
            return content
        else:
            # Check if file starts with a /* comment block
            if content.startswith("/*"):
                # Insert after the first line
                first_newline = content.index("\n")
                header_line = f" * Debug Symbol: {sym_name} (from o.bin, high match)\n * Address: {address}\n"
                return content[:first_newline + 1] + header_line + content[first_newline + 1:]
            else:
                header = f"/* Debug Symbol: {sym_name} (from o.bin, high match)\n * Address: {address}\n */\n"
                return header + content
    return content


def main():
    dry_run = "--dry-run" in sys.argv

    # Load all data
    results = load_json(RESULTS_FILE)
    l3_data = load_json(L3_FILE)
    fc_data = load_json(FC_FILE)
    acs_data = load_json(ACS_FILE)

    high_assignments = get_high_assignments(results)
    print(f"Found {len(high_assignments)} HIGH confidence assignments to apply")

    if not high_assignments:
        print("Nothing to do.")
        return 0

    renamed_count = 0
    skipped_count = 0
    errors = []

    for assignment in high_assignments:
        addr = assignment["retail_addr"]
        addr_hex = addr[2:]
        func_name = f"func_{addr_hex}"
        sym_name = assignment["sym_name"]

        # Find source file
        src_path, src_type = find_source_file(func_name)
        if src_path is None:
            # Check if already renamed to sym_name
            asm_check = os.path.join(ASM_DIR, f"{sym_name}.s")
            c_check = os.path.join(SRC_DIR, f"{sym_name}.c")
            if os.path.exists(asm_check) or os.path.exists(c_check):
                print(f"  SKIP {func_name} -> {sym_name} (already renamed)")
                skipped_count += 1
            else:
                msg = f"  ERROR {func_name}: source file not found"
                print(msg)
                errors.append(msg)
            continue

        ext = ".s" if src_type == "asm" else ".c"
        dir_path = ASM_DIR if src_type == "asm" else SRC_DIR
        old_rel = f"{src_type}/main/matched/{func_name}{ext}"
        new_rel = f"{src_type}/main/matched/{sym_name}{ext}"
        new_path = os.path.join(dir_path, f"{sym_name}{ext}")

        # Check for name collision
        if os.path.exists(new_path):
            msg = f"  ERROR {func_name} -> {sym_name}: target file already exists!"
            print(msg)
            errors.append(msg)
            continue

        print(f"  RENAME {func_name}{ext} -> {sym_name}{ext} ({src_type})")

        if dry_run:
            renamed_count += 1
            continue

        # 1. Update file header
        new_content = ensure_address_header(src_path, addr, sym_name, src_type)
        with open(src_path, "w") as f:
            f.write(new_content)

        # 2. Rename the file
        os.rename(src_path, new_path)

        # 3. Update level3_functions.json
        for func in l3_data["functions"]:
            if func["address"] == addr and func["name"] == func_name:
                func["name"] = sym_name
                func["source"] = new_rel
                break

        # 4. Update function_classification.json
        for func in fc_data["functions"]:
            if func["address"] == addr and func["name"] == func_name:
                func["name"] = sym_name
                func["filepath"] = new_rel
                break

        # 5. Update asm_conversion_status.json
        if func_name in acs_data["functions"]:
            acs_data["functions"][sym_name] = acs_data["functions"].pop(func_name)

        renamed_count += 1

    if not dry_run:
        # Save all config files
        save_json(L3_FILE, l3_data)
        save_json(FC_FILE, fc_data)
        save_json(ACS_FILE, acs_data)

    print(f"\nSummary:")
    print(f"  Renamed: {renamed_count}")
    print(f"  Skipped: {skipped_count}")
    print(f"  Errors:  {len(errors)}")

    if errors:
        print("\nErrors:")
        for e in errors:
            print(f"  {e}")
        return 1

    return 0


if __name__ == "__main__":
    sys.exit(main())
