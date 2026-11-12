#!/usr/bin/env python3
"""
Verify thunk placement for fallthrough mode.

Checks:
1. Thunk end address == body start address (continuity)
2. No gaps or overlaps between thunks
3. Thunk bytes in binary match expected pattern

Usage:
    python3 tools/build/verify_thunk_placement.py \
        --metadata build/thunk_stub_metadata.json \
        --exe disks/SLPS_000.65 \
        [--strict]
"""

import argparse
import json
import struct
import sys
from pathlib import Path
from typing import List, Tuple


def load_json(path: Path) -> dict:
    if not path.exists():
        raise FileNotFoundError(f"File not found: {path}")
    return json.loads(path.read_text())


def hex_to_int(addr: str) -> int:
    """Convert hex string to integer, handling both '0x' prefix and raw hex."""
    if isinstance(addr, int):
        return addr
    return int(addr, 16)


def verify_address_continuity(stubs: List[dict]) -> Tuple[List[dict], List[dict]]:
    """
    Verify thunk end address == body start address for fallthrough mode.

    Returns: (passed, failed) lists
    """
    passed = []
    failed = []

    for stub in stubs:
        mode = stub.get('mode', 'fallthrough')
        if mode != 'fallthrough':
            passed.append({**stub, 'check': 'skipped_jump_mode'})
            continue

        thunk_addr = hex_to_int(stub['thunk_address'])
        func_addr = hex_to_int(stub['function_address'])
        thunk_size = stub.get('thunk_size', 0)

        expected_func_addr = thunk_addr + thunk_size

        if expected_func_addr == func_addr:
            passed.append({
                **stub,
                'check': 'continuous',
                'thunk_end': f"0x{expected_func_addr:08X}",
            })
        else:
            gap = func_addr - expected_func_addr
            failed.append({
                **stub,
                'check': 'gap' if gap > 0 else 'overlap',
                'thunk_end': f"0x{expected_func_addr:08X}",
                'body_start': f"0x{func_addr:08X}",
                'gap_bytes': gap,
            })

    return passed, failed


def verify_no_overlaps(stubs: List[dict]) -> List[dict]:
    """Check for overlapping thunk regions."""
    overlaps = []

    # Sort by thunk address
    sorted_stubs = sorted(stubs, key=lambda s: hex_to_int(s['thunk_address']))

    for i in range(len(sorted_stubs) - 1):
        curr = sorted_stubs[i]
        next_stub = sorted_stubs[i + 1]

        curr_end = hex_to_int(curr['thunk_address']) + curr.get('thunk_size', 0)
        next_start = hex_to_int(next_stub['thunk_address'])

        if curr_end > next_start:
            overlaps.append({
                'thunk1': curr.get('thunk_name', curr['thunk_address']),
                'thunk2': next_stub.get('thunk_name', next_stub['thunk_address']),
                'overlap_bytes': curr_end - next_start,
                'thunk1_end': f"0x{curr_end:08X}",
                'thunk2_start': f"0x{next_start:08X}",
            })

    return overlaps


def verify_binary_bytes(stubs: List[dict], exe_path: Path) -> Tuple[List[dict], List[dict]]:
    """
    Verify thunk instructions in binary are valid setup patterns.

    This checks that the bytes at each thunk address look like valid
    thunk instructions (lui, addiu, lw, sll, etc.) and not random data.
    """
    if not exe_path.exists():
        return [], [{'error': f'Executable not found: {exe_path}'}]

    exe_data = exe_path.read_bytes()
    load_addr = 0x80041000
    header_offset = 0x800

    passed = []
    warnings = []

    # Common thunk instruction patterns (first word)
    THUNK_OPCODES = {
        0x0F: 'lui',      # lui rt, imm
        0x09: 'addiu',    # addiu rt, rs, imm
        0x00: 'r-type',   # sll, sra, addu, etc.
        0x23: 'lw',       # lw rt, offset(rs)
        0x24: 'lbu',      # lbu rt, offset(rs)
    }

    for stub in stubs:
        thunk_addr = hex_to_int(stub['thunk_address'])
        thunk_size = stub.get('thunk_size', 0)

        # Calculate file offset
        file_offset = thunk_addr - load_addr + header_offset

        if file_offset < 0 or file_offset + 4 > len(exe_data):
            warnings.append({
                **stub,
                'check': 'out_of_bounds',
                'file_offset': file_offset,
            })
            continue

        # Read first instruction
        first_word = struct.unpack('<I', exe_data[file_offset:file_offset+4])[0]
        opcode = (first_word >> 26) & 0x3F

        if opcode in THUNK_OPCODES:
            passed.append({
                **stub,
                'check': 'valid_opcode',
                'first_opcode': THUNK_OPCODES[opcode],
                'first_word': f"0x{first_word:08X}",
            })
        else:
            warnings.append({
                **stub,
                'check': 'unexpected_opcode',
                'opcode': opcode,
                'first_word': f"0x{first_word:08X}",
            })

    return passed, warnings


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Verify thunk placement for byte-identical builds"
    )
    parser.add_argument(
        '--metadata',
        default='build/thunk_stub_metadata.json',
        help='Thunk stub metadata JSON'
    )
    parser.add_argument(
        '--exe',
        default='disks/SLPS_000.65',
        help='Target executable for binary verification'
    )
    parser.add_argument(
        '--strict',
        action='store_true',
        help='Exit with error on any failure'
    )
    parser.add_argument(
        '--verbose', '-v',
        action='store_true',
        help='Show detailed output'
    )
    args = parser.parse_args()

    metadata_path = Path(args.metadata)
    exe_path = Path(args.exe)

    if not metadata_path.exists():
        print(f"Error: Metadata file not found: {metadata_path}")
        print("Run 'make thunk-stubs' first to generate thunk metadata.")
        return 1

    metadata = load_json(metadata_path)
    stubs = metadata.get('stubs', [])

    if not stubs:
        print("No thunk stubs found in metadata.")
        return 0

    print(f"Verifying {len(stubs)} thunk stubs...")
    print(f"Mode: {metadata.get('metadata', {}).get('mode', 'unknown')}")
    print()

    errors = 0
    warnings_count = 0

    # Check 1: Address continuity
    print("=== Address Continuity Check ===")
    passed, failed = verify_address_continuity(stubs)

    if failed:
        print(f"❌ {len(failed)} thunks have address gaps/overlaps:")
        for f in failed:
            print(f"  {f.get('thunk_name', f['thunk_address'])}: "
                  f"{f['check']} of {f['gap_bytes']} bytes")
            print(f"    Thunk ends at {f['thunk_end']}, body at {f['body_start']}")
        errors += len(failed)
    else:
        print(f"✅ All {len(passed)} thunks are address-continuous")
    print()

    # Check 2: No overlapping thunks
    print("=== Overlap Check ===")
    overlaps = verify_no_overlaps(stubs)

    if overlaps:
        print(f"❌ {len(overlaps)} thunk overlaps detected:")
        for o in overlaps:
            print(f"  {o['thunk1']} overlaps {o['thunk2']} by {o['overlap_bytes']} bytes")
        errors += len(overlaps)
    else:
        print("✅ No overlapping thunks")
    print()

    # Check 3: Binary verification
    if exe_path.exists():
        print("=== Binary Instruction Check ===")
        bin_passed, bin_warnings = verify_binary_bytes(stubs, exe_path)

        if bin_warnings:
            print(f"⚠️  {len(bin_warnings)} thunks with unexpected opcodes:")
            for w in bin_warnings[:5]:  # Show first 5
                print(f"  {w.get('thunk_name', w['thunk_address'])}: "
                      f"{w.get('check', 'unknown')} - {w.get('first_word', 'N/A')}")
            if len(bin_warnings) > 5:
                print(f"  ... and {len(bin_warnings) - 5} more")
            warnings_count += len(bin_warnings)
        else:
            print(f"✅ All {len(bin_passed)} thunks have valid instruction opcodes")
    else:
        print(f"⚠️  Skipping binary check: {exe_path} not found")
        warnings_count += 1

    # Summary
    print()
    print("=== Summary ===")
    print(f"Total thunks: {len(stubs)}")
    print(f"Errors: {errors}")
    print(f"Warnings: {warnings_count}")

    if errors > 0:
        print()
        print("❌ VERIFICATION FAILED")
        print("Fix address continuity issues before using fallthrough mode.")
        return 1 if args.strict else 0

    if warnings_count > 0 and args.verbose:
        print()
        print("⚠️  Warnings present - review before proceeding")

    print()
    print("✅ VERIFICATION PASSED")
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
