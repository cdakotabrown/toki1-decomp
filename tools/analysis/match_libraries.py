#!/usr/bin/env python3
"""
PSY-Q Library Function Matcher

Scans PSY-Q function signatures against the Tokimeki Memorial binary to identify
library functions. Outputs matches to console and updates symbol files.

This tool uses the psx_psyq_signatures repository (github.com/Xeeynamo/psx_psyq_signatures)
to match known PSY-Q SDK library functions against our binary. It identifies which
of our 842 functions are actually PSY-Q library code (graphics, sound, etc.) vs
game-specific code.

Requirements:
    - PSY-Q signatures must be in /tmp/psx_psyq_signatures/
    - Binary must be at disks/SLPS_000.65
    - Function map at build/function_analysis/function_map.json

Usage:
    # Default scan with 48-byte minimum (high confidence)
    python3 tools/match_libraries.py

    # More aggressive scan with 32-byte minimum (may have false positives)
    python3 tools/match_libraries.py --min-length 32

    # Scan specific PSY-Q versions
    python3 tools/match_libraries.py --version 470 --version 460

    # Verbose output showing each match
    python3 tools/match_libraries.py --verbose

    # Dry-run (don't update files)
    python3 tools/match_libraries.py --no-update

Outputs:
    - config/symbols_libraries.txt: Symbol definitions for library functions
    - Updates function_map.json with library metadata

Algorithm:
    1. Load PSY-Q signatures (hex byte patterns with wildcards)
    2. Scan binary on 4-byte boundaries (MIPS instruction alignment)
    3. Match patterns against our functions
    4. Filter short signatures (< min-length) to avoid false positives
    5. Deduplicate and filter suspicious patterns (same name appearing many times)
    6. Update symbol files and function map

Notes:
    - Very short signatures (< 32 bytes) often match generic prologues/epilogues
    - Default 48-byte minimum provides high-confidence matches
    - Some library functions may not match (if signature not in database)
    - Expected: ~5-20 matches with default settings
"""

import json
import sys
import os
import argparse
from pathlib import Path
from typing import List, Dict, Tuple, Set
from collections import defaultdict

# Project paths
PROJECT_ROOT = Path(__file__).parent.parent
BINARY_PATH = PROJECT_ROOT / "disks" / "SLPS_000.65"
FUNCTION_MAP_PATH = PROJECT_ROOT / "build" / "function_analysis" / "function_map.json"
SIGNATURES_BASE = Path("/tmp/psx_psyq_signatures")
OUTPUT_SYMBOLS = PROJECT_ROOT / "config" / "symbols_libraries.txt"

# Memory mapping from splat.jp.main.yaml
# File offset 0x800 maps to VRAM 0x80041000
FILE_OFFSET_BASE = 0x800
VRAM_BASE = 0x80041000


def parse_signature(sig_hex: str) -> bytes:
    """
    Convert hex signature string to bytes pattern.
    '??' represents wildcard bytes that match anything.

    Returns: (pattern_bytes, mask_bytes) where mask indicates wildcards
    """
    tokens = sig_hex.strip().split()
    pattern = bytearray()
    mask = bytearray()

    for token in tokens:
        if token == "??":
            pattern.append(0)
            mask.append(0)  # 0 = wildcard
        else:
            pattern.append(int(token, 16))
            mask.append(1)  # 1 = must match

    return bytes(pattern), bytes(mask)


def match_pattern(data: bytes, pattern: bytes, mask: bytes, offset: int) -> bool:
    """Check if pattern matches at given offset in data, respecting wildcards."""
    if offset + len(pattern) > len(data):
        return False

    for i, (pat_byte, mask_byte) in enumerate(zip(pattern, mask)):
        if mask_byte == 1:  # Must match
            if data[offset + i] != pat_byte:
                return False

    return True


def find_signature_in_binary(binary_data: bytes, sig_hex: str) -> List[int]:
    """
    Find all occurrences of a signature pattern in binary data.
    Returns list of file offsets where pattern was found.

    Optimized: Only search on 4-byte aligned boundaries (MIPS instructions are 4-byte aligned)
    """
    pattern, mask = parse_signature(sig_hex)
    matches = []

    # MIPS instructions are 4-byte aligned, so we can skip 3/4 of positions
    # Start at FILE_OFFSET_BASE (0x800) to skip PS-X EXE header
    # Ensure we start on 4-byte boundary
    start_offset = FILE_OFFSET_BASE
    if start_offset % 4 != 0:
        start_offset = (start_offset // 4 + 1) * 4

    # Scan on 4-byte boundaries
    for offset in range(start_offset, len(binary_data) - len(pattern) + 1, 4):
        if match_pattern(binary_data, pattern, mask, offset):
            matches.append(offset)

    return matches


def file_offset_to_vram(file_offset: int) -> int:
    """Convert file offset to VRAM address."""
    if file_offset < FILE_OFFSET_BASE:
        return None  # Header region, no VRAM mapping
    return VRAM_BASE + (file_offset - FILE_OFFSET_BASE)


def vram_to_file_offset(vram: int) -> int:
    """Convert VRAM address to file offset."""
    return FILE_OFFSET_BASE + (vram - VRAM_BASE)


def load_function_map() -> Dict:
    """Load function metadata from function_map.json."""
    with open(FUNCTION_MAP_PATH, 'r') as f:
        return json.load(f)


def load_psyq_signatures(versions: List[str]) -> Dict[str, List[Dict]]:
    """
    Load PSY-Q signatures from specified versions.
    Returns: {library_name: [signature_entries]}
    """
    all_signatures = {}

    for version in versions:
        version_dir = SIGNATURES_BASE / version
        if not version_dir.exists():
            print(f"Warning: PSY-Q version {version} not found at {version_dir}")
            continue

        print(f"Loading PSY-Q {version} signatures...")

        for json_file in sorted(version_dir.glob("*.json")):
            lib_name = json_file.stem  # e.g., "LIBC.LIB" from "LIBC.LIB.json"

            # Skip if we already loaded this library from a newer version
            if lib_name in all_signatures:
                continue

            try:
                with open(json_file, 'r') as f:
                    signatures = json.load(f)
                    if signatures:  # Only add if non-empty
                        all_signatures[lib_name] = signatures
            except Exception as e:
                print(f"Warning: Failed to load {json_file}: {e}")

    return all_signatures


def find_function_at_vram(functions: List[Dict], vram_addr: int) -> Dict:
    """Find which function contains the given VRAM address."""
    for func in functions:
        start = int(func['vram_start'], 16)
        end = int(func['vram_end'], 16)

        if start <= vram_addr < end:
            return func

    return None


def deduplicate_matches(matches: List[Tuple[str, str, int, str, Dict]]) -> List[Tuple[str, str, int, str, Dict]]:
    """
    Deduplicate matches - if same VRAM address matched by multiple signatures,
    keep the longest/most specific signature.

    Also filter out suspicious patterns where the same function name appears
    too many times (likely a false positive from a generic signature).

    Input: [(lib_name, obj_name, vram_addr, label_name, function)]
    """
    # Group by VRAM address
    by_vram = defaultdict(list)
    for match in matches:
        lib_name, obj_name, vram_addr, label_name, function = match
        by_vram[vram_addr].append(match)

    # For each address, keep the match with longest signature
    deduped = []
    for vram_addr, candidates in by_vram.items():
        # Keep all matches if they're from different functions
        # (shouldn't happen, but be conservative)
        if len(candidates) == 1:
            deduped.append(candidates[0])
        else:
            # Pick the most specific one (could use signature length, but for now just pick first)
            deduped.append(candidates[0])

    # Check for suspicious duplicate function names
    # Count how many times each function name appears
    name_counts = defaultdict(int)
    for lib_name, obj_name, vram_addr, label_name, function in deduped:
        name_counts[label_name] += 1

    # Filter out functions that appear more than 3 times (likely false positives)
    # Exception: SsUtReverbOn/Off are small stub functions that might legitimately appear multiple times
    suspicious_names = set()
    for name, count in name_counts.items():
        if count > 3 and not name.startswith('SsUtReverb'):
            suspicious_names.add(name)
            print(f"Warning: {name} appears {count} times - likely false positive, filtering out")

    filtered = [m for m in deduped if m[3] not in suspicious_names]

    return filtered


def match_libraries(binary_path: Path, signatures: Dict[str, List[Dict]],
                    function_map: Dict, verbose: bool = False,
                    min_sig_length: int = 48) -> List[Tuple[str, str, int, str, Dict]]:
    """
    Match PSY-Q signatures against binary and identify library functions.

    Args:
        min_sig_length: Minimum signature length in bytes to avoid false positives.
                       Default 48 bytes (12 MIPS instructions) filters out generic prologues.

    Returns: List of (lib_name, obj_name, vram_addr, label_name, function_metadata)
    """
    # Load binary
    with open(binary_path, 'rb') as f:
        binary_data = f.read()

    functions = function_map['functions']
    matches = []

    print(f"\nScanning {len(binary_data)} bytes against PSY-Q signatures...")
    print(f"Loaded {len(functions)} functions from function map")
    print(f"Minimum signature length: {min_sig_length} bytes")

    # Track statistics
    total_sigs = 0
    skipped_short = 0
    total_matches = 0

    # Scan each library
    for lib_name, lib_signatures in sorted(signatures.items()):
        lib_matches = 0

        for sig_entry in lib_signatures:
            obj_name = sig_entry['name']

            # Skip BSS entries (no code signature)
            if 'sig' not in sig_entry:
                continue

            sig_hex = sig_entry['sig']
            labels = sig_entry['labels']

            # Count signature length (each token is 1 byte)
            sig_tokens = sig_hex.strip().split()
            sig_length = len(sig_tokens)

            # Skip short signatures to avoid false positives
            if sig_length < min_sig_length:
                skipped_short += 1
                continue

            total_sigs += 1

            # Find all occurrences of this signature
            file_offsets = find_signature_in_binary(binary_data, sig_hex)

            for file_offset in file_offsets:
                vram_addr = file_offset_to_vram(file_offset)

                if vram_addr is None:
                    continue

                # Check each label in this signature
                for label in labels:
                    label_name = label['name']
                    label_offset = label['offset']
                    label_vram = vram_addr + label_offset

                    # Find which function contains this address
                    function = find_function_at_vram(functions, label_vram)

                    if function:
                        matches.append((lib_name, obj_name, label_vram, label_name, function))
                        lib_matches += 1
                        total_matches += 1

                        if verbose:
                            print(f"  Match: {label_name} @ 0x{label_vram:08X} in {function['name']} ({lib_name})")

        if lib_matches > 0:
            print(f"  {lib_name}: {lib_matches} matches")

    print(f"\nTotal: {total_matches} matches from {total_sigs} signatures")
    print(f"Skipped {skipped_short} short signatures (< {min_sig_length} bytes)")

    # Deduplicate
    matches = deduplicate_matches(matches)
    print(f"After deduplication: {len(matches)} unique matches")

    return matches


def generate_report(matches: List[Tuple[str, str, int, str, Dict]]) -> None:
    """Print detailed match report."""
    print("\n" + "="*80)
    print("LIBRARY FUNCTION MATCHES")
    print("="*80)

    # Group by library
    by_library = defaultdict(list)
    for lib_name, obj_name, vram_addr, label_name, function in matches:
        by_library[lib_name].append((obj_name, vram_addr, label_name, function))

    # Print by library
    for lib_name in sorted(by_library.keys()):
        lib_matches = by_library[lib_name]
        print(f"\n{lib_name} ({len(lib_matches)} functions):")
        print("-" * 80)

        for obj_name, vram_addr, label_name, function in sorted(lib_matches, key=lambda x: x[1]):
            func_name = function['name']
            func_size = function['size']
            print(f"  0x{vram_addr:08X}  {label_name:30s}  {func_name:20s}  ({func_size} bytes)")

    # Summary statistics
    print("\n" + "="*80)
    print("SUMMARY")
    print("="*80)
    print(f"Total library functions found: {len(matches)}")
    print(f"Libraries matched: {len(by_library)}")
    print(f"\nBreakdown by library:")
    for lib_name in sorted(by_library.keys(), key=lambda x: len(by_library[x]), reverse=True):
        print(f"  {lib_name:30s}: {len(by_library[lib_name]):3d} functions")


def generate_symbol_file(matches: List[Tuple[str, str, int, str, Dict]], output_path: Path) -> None:
    """Generate symbols_libraries.txt with library function symbols."""
    print(f"\nGenerating symbol file: {output_path}")

    lines = []
    lines.append("// PSY-Q Library Function Symbols")
    lines.append("// Auto-generated by tools/match_libraries.py")
    lines.append("//")
    lines.append("// Format: symbol_name = vram_address; // library source")
    lines.append("")

    # Group by library for organization
    by_library = defaultdict(list)
    for lib_name, obj_name, vram_addr, label_name, function in matches:
        by_library[lib_name].append((vram_addr, label_name, obj_name))

    # Write by library
    for lib_name in sorted(by_library.keys()):
        lines.append(f"// {lib_name}")

        for vram_addr, label_name, obj_name in sorted(by_library[lib_name], key=lambda x: x[0]):
            lines.append(f"{label_name} = 0x{vram_addr:08X}; // {lib_name} ({obj_name})")

        lines.append("")

    # Write to file
    with open(output_path, 'w') as f:
        f.write('\n'.join(lines))

    print(f"Wrote {len(matches)} symbols to {output_path}")


def update_function_map(matches: List[Tuple[str, str, int, str, Dict]],
                        function_map: Dict, output_path: Path) -> None:
    """Update function_map.json with library function tags."""
    print(f"\nUpdating function map: {output_path}")

    # Create mapping of VRAM -> (library, label)
    vram_to_lib = {}
    for lib_name, obj_name, vram_addr, label_name, function in matches:
        func_vram = int(function['vram_start'], 16)

        # If this function already tagged with a library, keep the first one
        if func_vram not in vram_to_lib:
            vram_to_lib[func_vram] = (lib_name, label_name, obj_name)

    # Update functions in map
    updated = 0
    for function in function_map['functions']:
        func_vram = int(function['vram_start'], 16)

        if func_vram in vram_to_lib:
            lib_name, label_name, obj_name = vram_to_lib[func_vram]
            function['library'] = lib_name
            function['library_name'] = label_name
            function['library_obj'] = obj_name
            function['category'] = 'library'  # Override category
            updated += 1

    # Update statistics
    if 'statistics' in function_map:
        if 'by_category' not in function_map['statistics']:
            function_map['statistics']['by_category'] = {}

        # Recount categories
        category_counts = defaultdict(int)
        for function in function_map['functions']:
            category = function.get('category', 'unknown')
            category_counts[category] += 1

        function_map['statistics']['by_category'] = dict(category_counts)

    # Write updated map
    with open(output_path, 'w') as f:
        json.dump(function_map, f, indent=2)

    print(f"Updated {updated} functions with library metadata")


def main():
    parser = argparse.ArgumentParser(
        description="Match PSY-Q library signatures against Tokimeki Memorial binary"
    )
    parser.add_argument(
        '--version',
        action='append',
        default=None,
        help='PSY-Q version to scan (can specify multiple times). Default: 470, 460, 450, 440'
    )
    parser.add_argument(
        '--verbose', '-v',
        action='store_true',
        help='Print each match as it is found'
    )
    parser.add_argument(
        '--no-update',
        action='store_true',
        help='Do not update symbol files or function map (report only)'
    )
    parser.add_argument(
        '--min-length',
        type=int,
        default=48,
        help='Minimum signature length in bytes to avoid false positives (default: 48)'
    )

    args = parser.parse_args()

    # Default versions if none specified
    versions = args.version if args.version else ['470', '460', '450', '440']

    # Verify files exist
    if not BINARY_PATH.exists():
        print(f"Error: Binary not found at {BINARY_PATH}")
        return 1

    if not FUNCTION_MAP_PATH.exists():
        print(f"Error: Function map not found at {FUNCTION_MAP_PATH}")
        return 1

    if not SIGNATURES_BASE.exists():
        print(f"Error: PSY-Q signatures not found at {SIGNATURES_BASE}")
        return 1

    print("="*80)
    print("PSY-Q LIBRARY FUNCTION MATCHER")
    print("="*80)
    print(f"Binary:       {BINARY_PATH}")
    print(f"Function map: {FUNCTION_MAP_PATH}")
    print(f"Signatures:   {SIGNATURES_BASE}")
    print(f"Versions:     {', '.join(versions)}")
    print("="*80)

    # Load data
    print("\nLoading function map...")
    function_map = load_function_map()

    print("\nLoading PSY-Q signatures...")
    signatures = load_psyq_signatures(versions)

    if not signatures:
        print("Error: No signatures loaded!")
        return 1

    print(f"Loaded {len(signatures)} libraries")

    # Match signatures
    matches = match_libraries(BINARY_PATH, signatures, function_map,
                            verbose=args.verbose, min_sig_length=args.min_length)

    # Generate report
    generate_report(matches)

    # Update files
    if not args.no_update and matches:
        generate_symbol_file(matches, OUTPUT_SYMBOLS)
        update_function_map(matches, function_map, FUNCTION_MAP_PATH)

        print("\n" + "="*80)
        print("SUCCESS: Library functions identified and files updated")
        print("="*80)
        print(f"\nNext steps:")
        print(f"  1. Review generated symbols: {OUTPUT_SYMBOLS}")
        print(f"  2. Updated function map: {FUNCTION_MAP_PATH}")
        print(f"  3. Consider adding symbols to config/symbols.jp.txt")
        print(f"  4. Re-run splat to incorporate new symbols: make extract")

    return 0


if __name__ == '__main__':
    sys.exit(main())
