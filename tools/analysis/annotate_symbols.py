#!/usr/bin/env python3
"""
Annotate all matched source files with debug symbol information.

For ASM files: Adds debug symbol name to comment header.
For C files: Adds debug symbol name to comment header.
Generates comprehensive README and unmatched function tracking.

Uses three tiers of confidence:
  - EXACT: Address matches perfectly (offset 0)
  - HIGH: Address within 16 bytes
  - LIKELY: Address within 256 bytes (same ordering region)
  - GUESS: Nearest symbol in sequence (>256 bytes)
"""

import json
import os
import re
from pathlib import Path
from bisect import bisect_left
from collections import defaultdict

BASE_DIR = Path(__file__).resolve().parents[2]
SYMBOL_MAP = BASE_DIR / "build/slps00065/symbols/symbol_map.json"
SYMBOL_TXT = BASE_DIR / "build/slps00065/symbols/tokimeki_symbols.txt"
ASM_DIR = BASE_DIR / "asm/main/matched"
SRC_DIR = BASE_DIR / "src/main/matched"
OUTPUT_DIR = BASE_DIR / "build/symbol_audit"


def load_debug_symbols():
    """Load all debug symbols sorted by address."""
    with open(SYMBOL_MAP) as f:
        data = json.load(f)

    symbols = []
    for sym in data["symbols"]:
        addr = int(sym["addr"], 16)
        symbols.append((addr, sym["name"]))

    return sorted(symbols, key=lambda x: x[0])


def load_all_symbols_txt():
    """Load all symbol names from the text file for name lookups."""
    syms = {}
    with open(SYMBOL_TXT) as f:
        for line in f:
            line = line.strip()
            if line.startswith('#') or not line:
                continue
            parts = line.split(None, 1)
            if len(parts) == 2:
                addr = int(parts[0], 16)
                name = parts[1]
                syms[addr] = name
    return syms


def extract_address_from_filename(filename):
    """Extract hex address from filename."""
    match = re.search(r'(?:func_|FUN_)([0-9A-Fa-f]{8})', filename)
    if match:
        return int(match.group(1), 16)
    return None


def get_all_functions():
    """Get all function files with addresses."""
    functions = {}

    for f in ASM_DIR.glob("*.s"):
        addr = extract_address_from_filename(f.name)
        if addr:
            functions[addr] = {"file": f.name, "type": "asm", "path": f}
        else:
            # Already renamed file - extract address from content
            try:
                content = f.read_text()
                m = re.search(r'VRAM:\s*0x([0-9A-Fa-f]+)', content)
                if not m:
                    m = re.search(r'Address:\s*0x([0-9A-Fa-f]+)', content)
                if not m:
                    m = re.search(r'\.globl\s+func_([0-9A-Fa-f]{8})', content)
                if m:
                    addr = int(m.group(1), 16)
                    functions[addr] = {"file": f.name, "type": "asm", "path": f}
            except:
                pass

    for f in SRC_DIR.glob("*.c"):
        addr = extract_address_from_filename(f.name)
        if addr:
            functions[addr] = {"file": f.name, "type": "c", "path": f}
        else:
            # Already renamed file - extract address from content
            try:
                content = f.read_text()
                m = re.search(r'Address:\s*0x([0-9A-Fa-f]+)', content)
                if not m:
                    m = re.search(r'func_([0-9A-Fa-f]{8})', content)
                if m:
                    addr = int(m.group(1), 16)
                    functions[addr] = {"file": f.name, "type": "c", "path": f}
            except:
                pass

    return functions


def find_best_match(func_addr, symbols, max_distance=512):
    """Find the best symbol match using proximity + sequential ordering."""
    addrs = [s[0] for s in symbols]
    idx = bisect_left(addrs, func_addr)

    candidates = []

    # Check nearby symbols
    for i in range(max(0, idx - 5), min(len(symbols), idx + 5)):
        sym_addr, sym_name = symbols[i]
        dist = abs(func_addr - sym_addr)
        if dist <= max_distance:
            candidates.append((dist, sym_addr, sym_name))

    if not candidates:
        return None

    best = min(candidates, key=lambda x: x[0])
    dist, sym_addr, sym_name = best

    if dist == 0:
        confidence = "EXACT"
    elif dist <= 16:
        confidence = "HIGH"
    elif dist <= 128:
        confidence = "LIKELY"
    elif dist <= 256:
        confidence = "PROBABLE"
    else:
        confidence = "GUESS"

    return {
        "sym_name": sym_name,
        "sym_addr": sym_addr,
        "distance": dist,
        "confidence": confidence
    }


def categorize_region(addr):
    if 0x80041000 <= addr < 0x80086000:
        return "GCC"
    elif 0x80086000 <= addr < 0x800C0000:
        return "IDO"
    else:
        return "OTHER"


def annotate_asm_file(path, match_info):
    """Add debug symbol annotation to ASM file header comment."""
    content = path.read_text()

    # Check if already annotated
    if "Debug Symbol:" in content or "Likely Symbol:" in content:
        return False

    sym_name = match_info["sym_name"]
    confidence = match_info["confidence"]
    distance = match_info["distance"]
    sym_addr = match_info["sym_addr"]

    if confidence in ("EXACT", "HIGH"):
        tag = "Debug Symbol"
        note = f"(from o.bin, {confidence.lower()} match)"
    elif confidence in ("LIKELY", "PROBABLE"):
        tag = "Likely Symbol"
        note = f"(from o.bin, offset {distance:+d} bytes from 0x{sym_addr:08X})"
    else:
        tag = "Possible Symbol"
        note = f"(from o.bin, offset {distance:+d} bytes, low confidence)"

    annotation = f"# {tag}: {sym_name} {note}\n"

    # Insert after the first comment block header line
    lines = content.split('\n')
    insert_idx = 0
    for i, line in enumerate(lines):
        if line.startswith('# Function:') or line.startswith('# VRAM:'):
            insert_idx = i + 1
            break
        elif line.startswith('.set') or line.startswith('.text') or line.startswith('.globl'):
            insert_idx = i
            break

    if insert_idx == 0:
        # No header found, insert at top
        lines.insert(0, annotation.rstrip())
    else:
        lines.insert(insert_idx, annotation.rstrip())

    path.write_text('\n'.join(lines))
    return True


def annotate_c_file(path, match_info):
    """Add debug symbol annotation to C file header comment."""
    content = path.read_text()

    if "Debug Symbol:" in content or "Likely Symbol:" in content:
        return False

    sym_name = match_info["sym_name"]
    confidence = match_info["confidence"]
    distance = match_info["distance"]
    sym_addr = match_info["sym_addr"]

    if confidence in ("EXACT", "HIGH"):
        tag = "Debug Symbol"
        note = f"(from o.bin, {confidence.lower()} match)"
    elif confidence in ("LIKELY", "PROBABLE"):
        tag = "Likely Symbol"
        note = f"(from o.bin, offset {distance:+d} bytes from 0x{sym_addr:08X})"
    else:
        tag = "Possible Symbol"
        note = f"(from o.bin, offset {distance:+d} bytes, low confidence)"

    annotation = f" * {tag}: {sym_name} {note}\n"

    # Insert into existing comment block or add one
    if content.startswith('/*'):
        # Find the first line of the comment block
        lines = content.split('\n')
        for i, line in enumerate(lines):
            if line.strip().startswith('* Address:') or line.strip().startswith('* Size:'):
                lines.insert(i, annotation.rstrip())
                break
            elif line.strip().startswith('*/'):
                lines.insert(i, annotation.rstrip())
                break
            elif i == 0:
                continue
            elif line.strip().startswith('*') and ('func_' in line or 'FUN_' in line):
                # Insert after the function name line
                lines.insert(i + 1, annotation.rstrip())
                break
        else:
            # Just insert after first line
            lines.insert(1, annotation.rstrip())

        path.write_text('\n'.join(lines))
    else:
        # No comment block, add one at top
        header = f"/* {tag}: {sym_name} {note} */\n"
        path.write_text(header + content)

    return True


def generate_asm_readme(functions, symbols, all_matches):
    """Generate comprehensive README for ASM directory."""
    readme_path = ASM_DIR / "README.md"

    # Sort functions by address
    sorted_funcs = sorted(all_matches.items(), key=lambda x: x[0])

    # Count by confidence
    counts = defaultdict(int)
    for addr, info in sorted_funcs:
        if info.get("match"):
            counts[info["match"]["confidence"]] += 1
        else:
            counts["UNMATCHED"] += 1

    with open(readme_path, 'w') as f:
        f.write("# ASM Matched Functions\n\n")
        f.write("## Debug Symbol Coverage\n\n")
        f.write(f"Total ASM functions: {sum(1 for a,i in sorted_funcs if i['type'] == 'asm')}\n\n")
        f.write("| Confidence | Count | Description |\n")
        f.write("|------------|-------|-------------|\n")
        f.write(f"| EXACT | {counts.get('EXACT', 0)} | Address matches perfectly |\n")
        f.write(f"| HIGH | {counts.get('HIGH', 0)} | Within 16 bytes |\n")
        f.write(f"| LIKELY | {counts.get('LIKELY', 0)} | Within 128 bytes |\n")
        f.write(f"| PROBABLE | {counts.get('PROBABLE', 0)} | Within 256 bytes |\n")
        f.write(f"| GUESS | {counts.get('GUESS', 0)} | Within 512 bytes |\n")
        f.write(f"| UNMATCHED | {counts.get('UNMATCHED', 0)} | No nearby symbol |\n")
        f.write("\n")

        # GCC Region
        f.write("## GCC Region (0x80041000 - 0x80085FFF)\n\n")
        f.write("| Address | File | Debug Symbol | Confidence | Offset |\n")
        f.write("|---------|------|--------------|------------|--------|\n")
        for addr, info in sorted_funcs:
            if info["type"] != "asm":
                continue
            if not (0x80041000 <= addr < 0x80086000):
                continue
            match = info.get("match")
            if match:
                f.write(f"| 0x{addr:08X} | {info['file']} | "
                        f"{match['sym_name']} | {match['confidence']} | "
                        f"{match['distance']:+d} |\n")
            else:
                f.write(f"| 0x{addr:08X} | {info['file']} | - | UNMATCHED | - |\n")

        f.write("\n")

        # IDO Region
        f.write("## IDO Region (0x80086000 - 0x800BFFFF)\n\n")
        f.write("| Address | File | Debug Symbol | Confidence | Offset |\n")
        f.write("|---------|------|--------------|------------|--------|\n")
        for addr, info in sorted_funcs:
            if info["type"] != "asm":
                continue
            if not (0x80086000 <= addr < 0x800C0000):
                continue
            match = info.get("match")
            if match:
                f.write(f"| 0x{addr:08X} | {info['file']} | "
                        f"{match['sym_name']} | {match['confidence']} | "
                        f"{match['distance']:+d} |\n")
            else:
                f.write(f"| 0x{addr:08X} | {info['file']} | - | UNMATCHED | - |\n")

    print(f"  Wrote: {readme_path}")


def generate_src_readme(functions, symbols, all_matches):
    """Generate comprehensive README for C source directory."""
    readme_path = SRC_DIR / "README.md"

    sorted_funcs = sorted(
        ((a, i) for a, i in all_matches.items() if i["type"] == "c"),
        key=lambda x: x[0]
    )

    counts = defaultdict(int)
    for addr, info in sorted_funcs:
        if info.get("match"):
            counts[info["match"]["confidence"]] += 1
        else:
            counts["UNMATCHED"] += 1

    with open(readme_path, 'w') as f:
        f.write("# C Matched Functions\n\n")
        f.write("## Debug Symbol Coverage\n\n")
        f.write(f"Total C functions: {len(sorted_funcs)}\n\n")
        f.write("| Confidence | Count |\n")
        f.write("|------------|-------|\n")
        for conf in ["EXACT", "HIGH", "LIKELY", "PROBABLE", "GUESS", "UNMATCHED"]:
            if counts.get(conf, 0) > 0:
                f.write(f"| {conf} | {counts[conf]} |\n")
        f.write("\n")

        f.write("## Function List\n\n")
        f.write("| Address | File | Debug Symbol | Confidence | Region |\n")
        f.write("|---------|------|--------------|------------|--------|\n")
        for addr, info in sorted_funcs:
            region = categorize_region(addr)
            match = info.get("match")
            if match:
                f.write(f"| 0x{addr:08X} | {info['file']} | "
                        f"{match['sym_name']} | {match['confidence']} | {region} |\n")
            else:
                f.write(f"| 0x{addr:08X} | {info['file']} | - | UNMATCHED | {region} |\n")

    print(f"  Wrote: {readme_path}")


def generate_unmatched_tracking(all_matches):
    """Generate tracking file for all unmatched functions."""
    unmatched = []
    low_confidence = []

    for addr, info in sorted(all_matches.items()):
        match = info.get("match")
        if not match:
            unmatched.append({
                "address": f"0x{addr:08X}",
                "file": info["file"],
                "type": info["type"],
                "region": categorize_region(addr)
            })
        elif match["confidence"] in ("GUESS", "PROBABLE"):
            low_confidence.append({
                "address": f"0x{addr:08X}",
                "file": info["file"],
                "type": info["type"],
                "region": categorize_region(addr),
                "best_guess": match["sym_name"],
                "offset": match["distance"],
                "confidence": match["confidence"]
            })

    report = {
        "summary": {
            "total_unmatched": len(unmatched),
            "total_low_confidence": len(low_confidence),
            "gcc_unmatched": sum(1 for u in unmatched if u["region"] == "GCC"),
            "ido_unmatched": sum(1 for u in unmatched if u["region"] == "IDO"),
        },
        "unmatched_functions": unmatched,
        "low_confidence_matches": low_confidence
    }

    path = OUTPUT_DIR / "unmatched_functions.json"
    with open(path, 'w') as f:
        json.dump(report, f, indent=2)
    print(f"  Wrote: {path}")

    # Also write human-readable list
    txt_path = OUTPUT_DIR / "UNMATCHED_FUNCTIONS.md"
    with open(txt_path, 'w') as f:
        f.write("# Unmatched Functions - Work Queue\n\n")
        f.write("Functions without confident debug symbol matches.\n")
        f.write("These need manual investigation.\n\n")
        f.write(f"## Summary\n\n")
        f.write(f"- **Completely unmatched**: {len(unmatched)}\n")
        f.write(f"- **Low confidence guesses**: {len(low_confidence)}\n\n")

        f.write("## Completely Unmatched\n\n")
        f.write("These have no debug symbol within 512 bytes.\n\n")

        # Group by region
        f.write("### GCC Region (0x80041000-0x80085FFF)\n\n")
        f.write("| Address | File | Type |\n")
        f.write("|---------|------|------|\n")
        for u in unmatched:
            if u["region"] == "GCC":
                f.write(f"| {u['address']} | {u['file']} | {u['type']} |\n")

        f.write("\n### IDO Region (0x80086000-0x800BFFFF)\n\n")
        f.write("| Address | File | Type |\n")
        f.write("|---------|------|------|\n")
        for u in unmatched:
            if u["region"] == "IDO":
                f.write(f"| {u['address']} | {u['file']} | {u['type']} |\n")

        f.write("\n## Low-Confidence Guesses\n\n")
        f.write("These have a nearby symbol but the offset is large.\n")
        f.write("Needs manual verification.\n\n")
        f.write("| Address | File | Best Guess | Offset | Confidence |\n")
        f.write("|---------|------|------------|--------|------------|\n")
        for lc in low_confidence:
            f.write(f"| {lc['address']} | {lc['file']} | "
                    f"{lc['best_guess']} | {lc['offset']:+d} | {lc['confidence']} |\n")

    print(f"  Wrote: {txt_path}")

    return report


def main():
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)

    print("=" * 70)
    print("Comprehensive Symbol Annotation")
    print("=" * 70)

    # Load data
    print("\nLoading debug symbols...")
    symbols = load_debug_symbols()
    print(f"  Loaded {len(symbols)} debug symbols")

    print("\nLoading function files...")
    functions = get_all_functions()
    print(f"  Found {len(functions)} functions")

    # Match ALL functions with best available symbol
    print("\nMatching symbols (extended range: 512 bytes)...")
    all_matches = {}
    for addr, func_info in sorted(functions.items()):
        match = find_best_match(addr, symbols, max_distance=512)
        all_matches[addr] = {
            "file": func_info["file"],
            "type": func_info["type"],
            "path": func_info["path"],
            "region": categorize_region(addr),
            "match": match
        }

    # Count results
    confidence_counts = defaultdict(int)
    for addr, info in all_matches.items():
        if info["match"]:
            confidence_counts[info["match"]["confidence"]] += 1
        else:
            confidence_counts["UNMATCHED"] += 1

    print("\nMatch Results:")
    for conf in ["EXACT", "HIGH", "LIKELY", "PROBABLE", "GUESS", "UNMATCHED"]:
        print(f"  {conf:12}: {confidence_counts.get(conf, 0)}")

    # Annotate files
    print("\n" + "=" * 70)
    print("Annotating Source Files")
    print("=" * 70)

    asm_annotated = 0
    c_annotated = 0
    skipped = 0

    for addr, info in sorted(all_matches.items()):
        match = info.get("match")
        if not match:
            continue

        path = info["path"]

        # Only annotate EXACT, HIGH, LIKELY, PROBABLE
        if match["confidence"] == "GUESS":
            continue

        try:
            if info["type"] == "asm":
                if annotate_asm_file(path, match):
                    asm_annotated += 1
                else:
                    skipped += 1
            elif info["type"] == "c":
                if annotate_c_file(path, match):
                    c_annotated += 1
                else:
                    skipped += 1
        except Exception as e:
            print(f"  [ERROR] {path.name}: {e}")

    print(f"\n  ASM files annotated: {asm_annotated}")
    print(f"  C files annotated: {c_annotated}")
    print(f"  Already annotated (skipped): {skipped}")

    # Generate READMEs
    print("\n" + "=" * 70)
    print("Generating READMEs and Tracking Files")
    print("=" * 70)

    generate_asm_readme(functions, symbols, all_matches)
    generate_src_readme(functions, symbols, all_matches)
    unmatched_report = generate_unmatched_tracking(all_matches)

    # Generate master symbol map (all known mappings)
    master_map = []
    for addr, info in sorted(all_matches.items()):
        entry = {
            "address": f"0x{addr:08X}",
            "file": info["file"],
            "type": info["type"],
            "region": info["region"]
        }
        if info["match"]:
            entry["debug_name"] = info["match"]["sym_name"]
            entry["confidence"] = info["match"]["confidence"]
            entry["offset"] = info["match"]["distance"]
        else:
            entry["debug_name"] = None
            entry["confidence"] = "UNMATCHED"
            entry["offset"] = None
        master_map.append(entry)

    master_path = OUTPUT_DIR / "master_symbol_map.json"
    with open(master_path, 'w') as f:
        json.dump(master_map, f, indent=2)
    print(f"  Wrote: {master_path}")

    # Summary
    print("\n" + "=" * 70)
    print("SUMMARY")
    print("=" * 70)
    total = len(all_matches)
    matched = total - confidence_counts.get("UNMATCHED", 0)
    print(f"  Total functions: {total}")
    print(f"  With symbol info: {matched} ({matched/total*100:.1f}%)")
    print(f"  Without symbol info: {confidence_counts.get('UNMATCHED', 0)}")
    print(f"  Files annotated: {asm_annotated + c_annotated}")
    print(f"  Unmatched for later: {unmatched_report['summary']['total_unmatched']}")


if __name__ == "__main__":
    main()
