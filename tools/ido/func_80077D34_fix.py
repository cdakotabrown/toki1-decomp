#!/usr/bin/env python3
"""
Custom post-processing script for magazine_slot_get.

Transforms IDO dispatch code from 13-instruction layout to 14-instruction target layout:
1. Strip handler code (8 trailing instructions)
2. Change delay slot of 1st beq from `li at,96` to `move v0,v1`
3. Insert standalone `li at,96` before 2nd beq
4. Change delay slot of 4th beq from `li v0,4` to `nop`
5. Change delay slot of fallthrough jr ra from `move v0,v1` to `nop`
"""

import struct
import sys


def find_text_section(data):
    """Find and return (offset, size) of .text section."""
    if data[:4] != b"\x7fELF":
        raise ValueError("Not an ELF file")
    e_shoff = struct.unpack_from("<I", data, 32)[0]
    e_shentsize = struct.unpack_from("<H", data, 46)[0]
    e_shnum = struct.unpack_from("<H", data, 48)[0]
    e_shstrndx = struct.unpack_from("<H", data, 50)[0]

    shstr_hdr_off = e_shoff + (e_shstrndx * e_shentsize)
    shstr_off = struct.unpack_from("<I", data, shstr_hdr_off + 16)[0]

    for i in range(e_shnum):
        sh_off = e_shoff + (i * e_shentsize)
        sh_name_idx = struct.unpack_from("<I", data, sh_off)[0]
        sec_off = struct.unpack_from("<I", data, sh_off + 16)[0]
        sec_size = struct.unpack_from("<I", data, sh_off + 20)[0]

        n0 = shstr_off + sh_name_idx
        n1 = data.find(b"\x00", n0)
        sec_name = data[n0:n1].decode("ascii", errors="ignore")

        if sec_name == ".text":
            return sh_off, sec_off, sec_size

    raise ValueError("No .text section found")


def patch_word(data, offset, new_word):
    """Patch a 4-byte word at given offset."""
    data = bytearray(data)
    struct.pack_into("<I", data, offset, new_word)
    return bytes(data)


def insert_word(data, text_off, text_size, insert_at_local, new_word, sh_off):
    """
    Insert a word at `insert_at_local` (relative to .text start),
    shifting subsequent instructions. Updates .text section size.
    """
    data = bytearray(data)

    # Insert the instruction into .text
    insert_abs = text_off + insert_at_local
    new_text = (
        data[text_off:text_off + insert_at_local]
        + struct.pack("<I", new_word)
        + data[text_off + insert_at_local:text_off + text_size]
    )
    data[text_off:text_off + text_size + 4] = new_text

    # Update section size
    old_size = struct.unpack_from("<I", data, sh_off + 20)[0]
    struct.pack_into("<I", data, sh_off + 20, old_size + 4)

    return bytes(data)


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} input.o output.o", file=sys.stderr)
        sys.exit(1)

    input_path = sys.argv[1]
    output_path = sys.argv[2]

    with open(input_path, "rb") as f:
        data = f.read()

    sh_off, text_off, text_size = find_text_section(data)

    # Read current instructions
    def get_instr(local_offset):
        return struct.unpack_from("<I", data, text_off + local_offset)[0]

    # Verify expected starting layout (13 instructions = 52 bytes):
    # 0x00: lui v1
    # 0x04: lbu v1
    # 0x08: li at,95
    # 0x0c: beq v1,at [delay: 0x10 = li at,96]
    # 0x10: li at,96
    # 0x14: beq v1,at [delay: 0x18 = li at,97]
    # 0x18: li at,97
    # 0x1c: beq v1,at [delay: 0x20 = li at,98]
    # 0x20: li at,98
    # 0x24: beq v1,at [delay: 0x28 = li v0,4]
    # 0x28: li v0,4
    # 0x2c: jr ra [delay: 0x30 = move v0,v1]
    # 0x30: move v0,v1

    LI_AT_96 = 0x24010060
    LI_AT_97 = 0x24010061
    LI_AT_98 = 0x24010062
    MOVE_V0_V1 = 0x00601025
    JR_RA = 0x03e00008
    NOP = 0x00000000

    assert get_instr(0x10) == LI_AT_96, f"Expected li at,96 at 0x10, got 0x{get_instr(0x10):08x}"
    assert get_instr(0x18) == LI_AT_97, f"Expected li at,97 at 0x18, got 0x{get_instr(0x18):08x}"
    assert get_instr(0x20) == LI_AT_98, f"Expected li at,98 at 0x20, got 0x{get_instr(0x20):08x}"
    assert get_instr(0x2c) == JR_RA, f"Expected jr ra at 0x2c, got 0x{get_instr(0x2c):08x}"
    assert get_instr(0x30) == MOVE_V0_V1, f"Expected move v0,v1 at 0x30, got 0x{get_instr(0x30):08x}"

    data = bytearray(data)

    # Step 1: Change 0x10 from `li at,96` to `move v0,v1`
    struct.pack_into("<I", data, text_off + 0x10, MOVE_V0_V1)

    # Step 2: Change 0x28 (4th beq delay) from `li v0,4` to `nop`
    struct.pack_into("<I", data, text_off + 0x28, NOP)

    # Step 3: Change 0x30 (jr ra delay) from `move v0,v1` to `nop`
    struct.pack_into("<I", data, text_off + 0x30, NOP)

    # Step 4: Insert `li at,96` at 0x14, shifting instructions at 0x14+
    # First, expand the .text section
    insert_at = text_off + 0x14
    new_text_bytes = (
        bytes(data[text_off:insert_at])
        + struct.pack("<I", LI_AT_96)
        + bytes(data[insert_at:text_off + text_size])
    )
    data[text_off:text_off + text_size + 4] = new_text_bytes

    # Update .text section size
    old_size = struct.unpack_from("<I", data, sh_off + 20)[0]
    struct.pack_into("<I", data, sh_off + 20, old_size + 4)

    # Step 5: Strip trailing instructions - we only want 14 instructions = 56 bytes
    # Find new text section info
    new_text_size = struct.unpack_from("<I", data, sh_off + 20)[0]

    # We want exactly 56 bytes (14 instructions)
    target_size = 56

    # Truncate to target size
    struct.pack_into("<I", data, sh_off + 20, target_size)
    # Zero out the bytes beyond target_size (they become padding)
    for i in range(target_size, new_text_size):
        data[text_off + i] = 0

    with open(output_path, "wb") as f:
        f.write(bytes(data))

    print(f"Transformed magazine_slot_get: 13-instr dispatch -> 14-instr target layout")


if __name__ == "__main__":
    main()
