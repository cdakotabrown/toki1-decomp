#!/usr/bin/env python3
"""
find_size_matches.py - Find WIP functions where compiled size matches target size

These are the best candidates for frame patching since the instruction count is correct.
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
    if not IDO_CC.exists():
        return None
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
    if not PATCH_TOOL.exists():
        return None
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


def compare_bytes(original: bytes, compiled: bytes) -> Tuple[int, int, int]:
    """Returns (matching, relocations, other_diffs)"""
    if len(original) != len(compiled):
        return (0, 0, 999)

    matching = 0
    relocations = 0
    other = 0

    for i in range(0, len(original), 4):
        orig = struct.unpack('<I', original[i:i+4])[0]
        comp = struct.unpack('<I', compiled[i:i+4])[0]

        if orig == comp:
            matching += 1
        else:
            # Check if relocation
            orig_op = (orig >> 26) & 0x3F
            comp_op = (comp >> 26) & 0x3F

            if orig_op == comp_op:
                # Same opcode - likely relocation or frame diff
                orig_imm = orig & 0xFFFF
                comp_imm = comp & 0xFFFF

                # lui with 0 immediate = relocation
                if orig_op == 0x0F and comp_imm == 0:
                    relocations += 1
                    continue

                # jal/j with different target = relocation
                if orig_op in (0x02, 0x03):
                    relocations += 1
                    continue

                # Frame-related or other stack operations
                rs = (orig >> 21) & 0x1F
                if rs == 29:  # sp-relative
                    # Could be frame or relocation
                    relocations += 1
                    continue

            other += 1

    return (matching, relocations, other)


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

    print("Finding WIP functions with matching compiled size...")
    print()

    candidates = []

    for source_file in wip_files:
        func_name = source_file.stem
        func_key = func_name.lower()

        if func_key not in func_config:
            continue

        address, size = func_config[func_key]
        if size == 0:
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

        # Check if size matches
        if len(original) == len(compiled):
            # Compare bytes
            matching, relocs, other = compare_bytes(original, compiled)
            total = len(original) // 4

            # Try frame patch if needed
            best_match = matching
            best_relocs = relocs
            best_other = other
            patch_needed = None

            if compiled_frame != target_frame and compiled_frame > 0 and target_frame > 0:
                patched_obj = patch_frame_size(obj_path, compiled_frame, target_frame)
                if patched_obj:
                    patched_bytes = extract_text_section(patched_obj)
                    if patched_bytes:
                        p_match, p_reloc, p_other = compare_bytes(original, patched_bytes)
                        if p_other < best_other or (p_other == best_other and p_match > best_match):
                            best_match = p_match
                            best_relocs = p_reloc
                            best_other = p_other
                            patch_needed = (compiled_frame, target_frame)
                    os.unlink(patched_obj)

            candidates.append({
                'name': func_name,
                'address': f"0x{address:08X}",
                'size': size,
                'matching': best_match,
                'total': total,
                'relocations': best_relocs,
                'other_diffs': best_other,
                'compiled_frame': compiled_frame,
                'target_frame': target_frame,
                'patch_needed': patch_needed
            })

        os.unlink(obj_path)

    # Sort by other_diffs (best first)
    candidates.sort(key=lambda x: (x['other_diffs'], -x['matching']))

    print(f"Found {len(candidates)} functions with matching size")
    print()

    perfect = [c for c in candidates if c['other_diffs'] == 0]
    near_perfect = [c for c in candidates if 0 < c['other_diffs'] <= 5]

    if perfect:
        print("=" * 70)
        print(f"PERFECT MATCHES: {len(perfect)}")
        print("=" * 70)
        for c in perfect:
            patch_str = ""
            if c['patch_needed']:
                patch_str = f" (patch {c['patch_needed'][0]}->{c['patch_needed'][1]})"
            print(f"{c['name']}: {c['matching']}/{c['total']} instructions, {c['relocations']} relocs{patch_str}")
        print()

    if near_perfect:
        print("=" * 70)
        print(f"NEAR-PERFECT (1-5 diffs): {len(near_perfect)}")
        print("=" * 70)
        for c in near_perfect:
            patch_str = ""
            if c['patch_needed']:
                patch_str = f" (patch {c['patch_needed'][0]}->{c['patch_needed'][1]})"
            print(f"{c['name']}: {c['matching']}/{c['total']} ({c['other_diffs']} diffs, {c['relocations']} relocs){patch_str}")
        print()

    if candidates:
        print("=" * 70)
        print(f"TOP 20 CANDIDATES (by fewest diffs):")
        print("=" * 70)
        for c in candidates[:20]:
            patch_str = ""
            if c['patch_needed']:
                patch_str = f" patch:{c['patch_needed'][0]}->{c['patch_needed'][1]}"
            print(f"{c['name']:18s}: {c['matching']:3d}/{c['total']:3d} ({c['other_diffs']:3d} diffs, {c['relocations']:3d} relocs){patch_str}")


if __name__ == '__main__':
    main()
