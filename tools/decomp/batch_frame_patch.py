#!/usr/bin/env python3
"""
batch_frame_patch.py - Batch frame size patching for easy candidates

Automatically detects frame size mismatches and patches compiled objects.
"""

import argparse
import json
import os
import struct
import subprocess
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).parent.parent.parent
EXE_PATH = PROJECT_ROOT / "disks" / "SLPS_000.65"
IDO_CC = PROJECT_ROOT / "tools" / "ido-static-recomp" / "build" / "5.2" / "out" / "cc"
PATCH_SCRIPT = PROJECT_ROOT / "tools" / "ido" / "patch_frame_size.py"

# Easy candidates with known frame issues
EASY_CANDIDATES = [
    ("func_8008499C", 0x8008499C, 68),   # 17 instr
    ("safe_env", 0x80049468, 108),  # 27 instr
    ("dtd_on", 0x80049574, 128),  # 32 instr
    ("menu2_init", 0x800833DC, 180),  # 45 instr
    ("_spu_read", 0x80095CD8, 68),   # 17 instr
]


def extract_frame_size(data: bytes) -> int:
    """Extract frame size from addiu sp,sp,-N instruction"""
    if len(data) < 4:
        return 0
    word = struct.unpack("<I", data[:4])[0]
    # addiu sp,sp,-N: 27BDXXXX where XXXX is negative offset
    if (word >> 16) == 0x27BD:
        offset = word & 0xFFFF
        if offset >= 0x8000:  # Negative
            return 0x10000 - offset
    return 0


def get_target_bytes(addr: int, size: int) -> bytes:
    """Get target bytes from original executable"""
    # PSX-EXE: load address is 0x80041000, header is 0x800 bytes
    file_offset = addr - 0x80041000 + 0x800
    with open(EXE_PATH, "rb") as f:
        f.seek(file_offset)
        return f.read(size)


def compile_function(func_name: str) -> Path:
    """Compile WIP C file and return object path"""
    wip_path = PROJECT_ROOT / "src" / "main" / "wip" / f"{func_name}.c"
    if not wip_path.exists():
        print(f"  No WIP file: {wip_path}")
        return None

    out_path = Path(f"/tmp/{func_name}.o")
    result = subprocess.run(
        [str(IDO_CC), "-O2", "-G0", "-mips1", "-EL", "-non_shared",
         f"-I{PROJECT_ROOT}/include", "-c", str(wip_path), "-o", str(out_path)],
        capture_output=True, text=True
    )

    if result.returncode != 0:
        print(f"  Compile failed: {result.stderr[:200]}")
        return None

    return out_path


def get_text_section(obj_path: Path) -> bytes:
    """Extract .text section from object file"""
    result = subprocess.run(
        ["mipsel-linux-gnu-objcopy", "-O", "binary", "-j", ".text",
         str(obj_path), "/tmp/text.bin"],
        capture_output=True
    )
    if result.returncode != 0:
        return b""
    with open("/tmp/text.bin", "rb") as f:
        return f.read()


def patch_and_verify(func_name: str, obj_path: Path, target_bytes: bytes) -> bool:
    """Patch frame size and verify match"""
    compiled_bytes = get_text_section(obj_path)
    if not compiled_bytes:
        return False

    compiled_frame = extract_frame_size(compiled_bytes)
    target_frame = extract_frame_size(target_bytes)

    print(f"  Frame: compiled={compiled_frame}, target={target_frame}")

    if compiled_frame == target_frame:
        # Check if already matches
        if compiled_bytes[:len(target_bytes)] == target_bytes:
            print(f"  Already matches!")
            return True
        print(f"  Frame matches but bytes differ")
        return False

    # Patch frame size
    patched_path = Path(f"/tmp/{func_name}_patched.o")
    result = subprocess.run(
        ["python3", str(PATCH_SCRIPT), str(obj_path), str(patched_path),
         "--old", str(compiled_frame), "--new", str(target_frame)],
        capture_output=True, text=True
    )

    if result.returncode != 0:
        print(f"  Patch failed: {result.stderr[:200]}")
        return False

    # Verify patched bytes
    patched_bytes = get_text_section(patched_path)
    if patched_bytes[:len(target_bytes)] == target_bytes:
        print(f"  MATCH after patching!")
        return True

    print(f"  Still differs after patching")
    return False


def main():
    parser = argparse.ArgumentParser(description="Batch frame size patching")
    parser.add_argument("--verbose", "-v", action="store_true")
    args = parser.parse_args()

    print("=== Batch Frame Size Patching ===\n")

    matched = []
    failed = []

    for func_name, addr, size in EASY_CANDIDATES:
        print(f"{func_name} (0x{addr:08X}, {size}B):")

        target_bytes = get_target_bytes(addr, size)
        obj_path = compile_function(func_name)

        if obj_path and patch_and_verify(func_name, obj_path, target_bytes):
            matched.append(func_name)
        else:
            failed.append(func_name)
        print()

    print("=== Summary ===")
    print(f"Matched: {len(matched)} - {matched}")
    print(f"Failed:  {len(failed)} - {failed}")


if __name__ == "__main__":
    main()
