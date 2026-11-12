#!/usr/bin/env python3
"""
test_specific_functions.py - Test specific WIP functions with all frame combinations
"""

import os
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT_DIR = Path(__file__).parent.parent.parent
EXE_PATH = ROOT_DIR / "disks" / "SLPS_000.65"
INCLUDE_DIR = ROOT_DIR / "include"
IDO_CC = ROOT_DIR / "tools" / "ido-static-recomp" / "build" / "5.2" / "out" / "cc"
IDO_FLAGS = ["-EL", "-G0", "-mips1", "-O2", "-non_shared"]
PATCH_TOOL = ROOT_DIR / "tools" / "ido" / "patch_frame_size.py"

HEADER_SIZE = 2048

# Test these specific functions
TEST_FUNCTIONS = [
    (0x80075110, 40,  "src/main/wip/vacation_day_bustup_init.c"),
    (0x80051198, 92,  "src/main/wip/x_taku_string_set.c"),
    (0x80061348, 124, "src/main/wip/pre_syogatu_init1.c"),
    (0x80059688, 132, "src/main/wip/frame_buf_clear.c"),
    (0x80057420, 164, "src/main/wip/song_base_set.c"),
    (0x80041000, 172, "src/main/wip/graph_init.c"),
    (0x80049144, 172, "src/main/wip/class_clear_all.c"),
    (0x80052480, 172, "src/main/wip/xa_set_m.c"),
    (0x80054114, 180, "src/main/wip/card_event_step.c"),
]

FRAME_COMBOS = [
    (24, 40), (32, 48), (40, 56), (24, 32),
    (48, 64), (16, 24), (16, 32), (32, 40),
    (40, 48), (24, 48), (56, 72), (64, 80),
    (72, 88), (80, 96), (88, 104), (96, 112)
]


def get_dest_addr():
    with open(EXE_PATH, 'rb') as f:
        f.seek(0x18)
        return struct.unpack('<I', f.read(4))[0]


def extract_original_bytes(address, size):
    dest_addr = get_dest_addr()
    offset = address - dest_addr + HEADER_SIZE
    with open(EXE_PATH, 'rb') as f:
        f.seek(offset)
        return f.read(size)


def get_frame_size(data):
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


def compile_function(source_path):
    obj_fd, obj_path = tempfile.mkstemp(suffix='.o')
    os.close(obj_fd)
    cmd = [str(IDO_CC)] + IDO_FLAGS + [
        "-c", str(source_path), "-o", obj_path,
        "-I", str(INCLUDE_DIR)
    ]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"Compilation error: {result.stderr}")
        os.unlink(obj_path)
        return None
    return Path(obj_path)


def extract_text_section(obj_path):
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


def patch_frame_size(obj_path, old_size, new_size):
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


def compare_bytes_detailed(original, compiled):
    """Compare and show detailed differences."""
    if len(original) != len(compiled):
        return None

    diffs = []
    for i in range(0, len(original), 4):
        orig = struct.unpack('<I', original[i:i+4])[0]
        comp = struct.unpack('<I', compiled[i:i+4])[0]

        if orig != comp:
            # Check if relocation
            orig_op = (orig >> 26) & 0x3F
            comp_op = (comp >> 26) & 0x3F

            is_reloc = False
            if orig_op == comp_op:
                if orig_op == 0x0F and (comp & 0xFFFF) == 0:  # lui
                    is_reloc = True
                elif orig_op in (0x02, 0x03):  # j/jal
                    is_reloc = True

            diff_type = "RELOC" if is_reloc else "DIFF"
            diffs.append((i, orig, comp, diff_type))

    return diffs


def main():
    print("Testing specific WIP functions with frame size combinations")
    print("=" * 70)
    print()

    for address, size, source_path in TEST_FUNCTIONS:
        func_name = Path(source_path).stem
        print(f"\n{'=' * 70}")
        print(f"Testing {func_name} @ 0x{address:08X} ({size} bytes)")
        print(f"{'=' * 70}")

        if not Path(source_path).exists():
            print(f"  SKIP: Source file not found")
            continue

        # Get original
        try:
            original = extract_original_bytes(address, size)
        except Exception as e:
            print(f"  ERROR: Failed to extract original bytes: {e}")
            continue

        target_frame = get_frame_size(original)
        print(f"  Target frame size: {target_frame} bytes")

        # Compile
        obj_path = compile_function(Path(source_path))
        if obj_path is None:
            print(f"  FAIL: Compilation failed")
            continue

        compiled = extract_text_section(obj_path)
        if compiled is None:
            os.unlink(obj_path)
            print(f"  ERROR: Failed to extract .text section")
            continue

        compiled_frame = get_frame_size(compiled)
        print(f"  Compiled frame size: {compiled_frame} bytes")
        print(f"  Compiled size: {len(compiled)} bytes (target: {len(original)} bytes)")

        if len(compiled) != len(original):
            print(f"  SKIP: Size mismatch ({len(compiled)} vs {len(original)})")
            os.unlink(obj_path)
            continue

        # Test without patch
        diffs = compare_bytes_detailed(original, compiled)
        relocs = [d for d in diffs if d[3] == "RELOC"]
        others = [d for d in diffs if d[3] == "DIFF"]

        print(f"\n  Without patch:")
        print(f"    Matching: {(len(original)//4) - len(diffs)}/{len(original)//4} instructions")
        print(f"    Relocations: {len(relocs)}")
        print(f"    Other diffs: {len(others)}")

        if others:
            print(f"    First differences:")
            for offset, orig, comp, dtype in others[:5]:
                print(f"      +0x{offset:03X}: 0x{orig:08X} vs 0x{comp:08X}")

        best_other = len(others)
        best_patch = None

        # Try frame patches
        for old_frame, new_frame in FRAME_COMBOS:
            if compiled_frame != old_frame:
                continue

            patched_obj = patch_frame_size(obj_path, old_frame, new_frame)
            if patched_obj:
                patched_bytes = extract_text_section(patched_obj)
                if patched_bytes and len(patched_bytes) == len(original):
                    p_diffs = compare_bytes_detailed(original, patched_bytes)
                    p_relocs = [d for d in p_diffs if d[3] == "RELOC"]
                    p_others = [d for d in p_diffs if d[3] == "DIFF"]

                    if len(p_others) < best_other:
                        best_other = len(p_others)
                        best_patch = (old_frame, new_frame)

                        print(f"\n  With patch {old_frame}->{new_frame}:")
                        print(f"    Matching: {(len(original)//4) - len(p_diffs)}/{len(original)//4} instructions")
                        print(f"    Relocations: {len(p_relocs)}")
                        print(f"    Other diffs: {len(p_others)}")

                        if len(p_others) == 0:
                            print(f"\n  *** PERFECT MATCH with patch {old_frame}->{new_frame}! ***")
                        elif len(p_others) <= 3:
                            print(f"\n  *** NEAR MATCH ({len(p_others)} diffs) with patch {old_frame}->{new_frame}! ***")
                            print(f"    Remaining differences:")
                            for offset, orig, comp, dtype in p_others:
                                print(f"      +0x{offset:03X}: 0x{orig:08X} vs 0x{comp:08X}")

                os.unlink(patched_obj)

        os.unlink(obj_path)

    print("\n" + "=" * 70)
    print("Testing complete")
    print("=" * 70)


if __name__ == '__main__':
    main()
