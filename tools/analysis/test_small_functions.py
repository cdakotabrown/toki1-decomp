#!/usr/bin/env python3
"""
test_small_functions.py - Test small WIP functions for potential matches

Focuses on small functions (< 100 bytes) as they're most likely to match with simple frame patches.
"""

import json
import os
import struct
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Dict, List, Optional, Tuple

ROOT_DIR = Path(__file__).parent.parent.parent
EXE_PATH = ROOT_DIR / "disks" / "SLPS_000.65"
WIP_DIR = ROOT_DIR / "src" / "main" / "wip"
CONFIG_PATH = ROOT_DIR / "config" / "decompiled_functions.json"
INCLUDE_DIR = ROOT_DIR / "include"
IDO_CC = ROOT_DIR / "tools" / "ido-static-recomp" / "build" / "5.2" / "out" / "cc"
IDO_FLAGS = ["-EL", "-G0", "-mips1", "-O2", "-non_shared"]
PATCH_TOOL = ROOT_DIR / "tools" / "ido" / "patch_frame_size.py"

HEADER_SIZE = 2048


def get_dest_addr() -> int:
    with open(EXE_PATH, 'rb') as f:
        f.seek(0x18)
        return struct.unpack('<I', f.read(4))[0]


def extract_original_bytes(address: int, size: int) -> bytes:
    dest_addr = get_dest_addr()
    offset = address - dest_addr + HEADER_SIZE
    with open(EXE_PATH, 'rb') as f:
        f.seek(offset)
        return f.read(size)


def get_frame_size(data: bytes) -> int:
    if len(data) < 4:
        return 0
    word = struct.unpack('<I', data[0:4])[0]
    op = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    imm = word & 0xFFFF
    if imm & 0x8000:
        imm_signed = imm - 0x10000
    else:
        imm_signed = imm
    if op == 0x09 and rs == 29 and rt == 29 and imm_signed < 0:
        return -imm_signed
    return 0


def compile_function(source_path: Path) -> Optional[Path]:
    obj_fd, obj_path = tempfile.mkstemp(suffix='.o')
    os.close(obj_fd)
    cmd = [str(IDO_CC)] + IDO_FLAGS + [
        "-c", str(source_path), "-o", obj_path,
        "-I", str(INCLUDE_DIR)
    ]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        os.unlink(obj_path)
        return None
    return Path(obj_path)


def extract_text_section(obj_path: Path) -> Optional[bytes]:
    text_fd, text_path = tempfile.mkstemp(suffix='.bin')
    os.close(text_fd)
    cmd = ["mipsel-linux-gnu-objcopy", "-O", "binary", "-j", ".text",
           str(obj_path), text_path]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        os.unlink(text_path)
        return None
    with open(text_path, 'rb') as f:
        data = f.read()
    os.unlink(text_path)
    return data


def patch_frame_size(obj_path: Path, old_size: int, new_size: int) -> Optional[Path]:
    patched_fd, patched_path = tempfile.mkstemp(suffix='.o')
    os.close(patched_fd)
    cmd = [
        "python3", str(PATCH_TOOL),
        str(obj_path), patched_path,
        "--old", str(old_size),
        "--new", str(new_size)
    ]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        os.unlink(patched_path)
        return None
    return Path(patched_path)


def is_relocation(orig: int, comp: int) -> bool:
    """Check if difference is a relocation."""
    orig_op = (orig >> 26) & 0x3F
    comp_op = (comp >> 26) & 0x3F

    if orig_op != comp_op:
        return False

    # lui with 0 immediate
    if orig_op == 0x0F and (comp & 0xFFFF) == 0:
        return True

    # jal/j
    if orig_op in (0x02, 0x03):
        return True

    return False


def compare_bytes(original: bytes, compiled: bytes, verbose=False) -> Tuple[int, int, int, List]:
    """Returns (matching, relocations, other_diffs, diff_details)"""
    if len(original) != len(compiled):
        return (0, 0, 999, [f"Size: {len(original)} vs {len(compiled)}"])

    matching = 0
    relocations = 0
    other = 0
    details = []

    for i in range(0, len(original), 4):
        orig = struct.unpack('<I', original[i:i+4])[0]
        comp = struct.unpack('<I', compiled[i:i+4])[0]

        if orig == comp:
            matching += 1
        elif is_relocation(orig, comp):
            relocations += 1
            if verbose:
                details.append(f"+0x{i:02X}: RELOC 0x{orig:08X} vs 0x{comp:08X}")
        else:
            other += 1
            if verbose:
                details.append(f"+0x{i:02X}: DIFF  0x{orig:08X} vs 0x{comp:08X}")

    return (matching, relocations, other, details)


def load_function_config() -> Dict[str, Tuple[int, int]]:
    if not CONFIG_PATH.exists():
        return {}
    with open(CONFIG_PATH) as f:
        config = json.load(f)
    result = {}
    for func in config.get('functions', []):
        addr_str = func.get('address', '')
        if addr_str.startswith('0x'):
            addr = int(addr_str, 16)
        else:
            continue
        size = func.get('size', 0)
        name = f"func_{addr:08X}"
        result[name.lower()] = (addr, size)
    return result


def main():
    if not EXE_PATH.exists():
        print(f"ERROR: {EXE_PATH} not found")
        sys.exit(1)
    if not IDO_CC.exists():
        print(f"ERROR: {IDO_CC} not found")
        sys.exit(1)

    func_config = load_function_config()
    wip_files = sorted(WIP_DIR.glob("func_*.c"))

    print("Testing small WIP functions (< 100 bytes)...")
    print()

    # Test combinations
    frame_combos = [
        (24, 40), (32, 48), (40, 56), (24, 32),
        (48, 64), (16, 24), (16, 32), (32, 40),
        (40, 48), (24, 48), (56, 72), (64, 80)
    ]

    perfect_matches = []
    near_matches = []

    for source_file in wip_files:
        func_name = source_file.stem
        func_key = func_name.lower()

        if func_key not in func_config:
            continue

        address, size = func_config[func_key]

        # Only test small functions
        if size == 0 or size > 100:
            continue

        # Get original
        try:
            original = extract_original_bytes(address, size)
        except:
            continue

        target_frame = get_frame_size(original)

        # Compile
        obj_path = compile_function(source_file)
        if obj_path is None:
            continue

        compiled = extract_text_section(obj_path)
        if compiled is None:
            os.unlink(obj_path)
            continue

        compiled_frame = get_frame_size(compiled)

        # Test without patch
        best_match, best_reloc, best_other, best_details = compare_bytes(original, compiled, verbose=True)
        best_patch = None

        # Try frame patches
        for old_frame, new_frame in frame_combos:
            if compiled_frame != old_frame:
                continue

            patched_obj = patch_frame_size(obj_path, old_frame, new_frame)
            if patched_obj:
                patched_bytes = extract_text_section(patched_obj)
                if patched_bytes and len(patched_bytes) == len(original):
                    p_match, p_reloc, p_other, p_details = compare_bytes(original, patched_bytes, verbose=True)
                    if p_other < best_other or (p_other == best_other and p_match > best_match):
                        best_match = p_match
                        best_reloc = p_reloc
                        best_other = p_other
                        best_details = p_details
                        best_patch = (old_frame, new_frame)
                os.unlink(patched_obj)

        os.unlink(obj_path)

        total = len(original) // 4

        # Report results
        if best_other == 0:
            perfect_matches.append({
                'name': func_name,
                'address': f"0x{address:08X}",
                'size': size,
                'match': best_match,
                'total': total,
                'relocs': best_reloc,
                'patch': best_patch,
                'details': best_details
            })
        elif best_other <= 3:
            near_matches.append({
                'name': func_name,
                'address': f"0x{address:08X}",
                'size': size,
                'match': best_match,
                'total': total,
                'other': best_other,
                'relocs': best_reloc,
                'patch': best_patch,
                'details': best_details
            })

    # Report
    print("=" * 70)
    print(f"PERFECT MATCHES: {len(perfect_matches)}")
    print("=" * 70)
    for m in perfect_matches:
        patch_str = f" patch:{m['patch'][0]}->{m['patch'][1]}" if m['patch'] else ""
        print(f"\n{m['name']} @ {m['address']} ({m['size']} bytes){patch_str}")
        print(f"  {m['match']}/{m['total']} matching, {m['relocs']} relocations")
        if m['details']:
            for detail in m['details'][:5]:
                print(f"  {detail}")

    print()
    print("=" * 70)
    print(f"NEAR MATCHES (1-3 diffs): {len(near_matches)}")
    print("=" * 70)
    for m in near_matches:
        patch_str = f" patch:{m['patch'][0]}->{m['patch'][1]}" if m['patch'] else ""
        print(f"\n{m['name']} @ {m['address']} ({m['size']} bytes){patch_str}")
        print(f"  {m['match']}/{m['total']} matching, {m['other']} other diffs, {m['relocs']} relocations")
        if m['details']:
            for detail in m['details'][:10]:
                print(f"  {detail}")


if __name__ == '__main__':
    main()
