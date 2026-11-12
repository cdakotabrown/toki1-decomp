#!/usr/bin/env python3
"""
insert_move_and_shift.py - Insert a move before a JAL + patch an in-place sllv

Some IDO 5.2 (ido-static-recomp) outputs collapse:
    move  v1, a0
    jal   callee
    sllv  a0, v1, t6    (delay slot)
into:
    jal   callee
    sllv  a0, a0, t6

The original binary can preserve the move, so to match we:
1) Insert an explicit move instruction at a requested .text offset
2) Patch the sllv operand register (rt) from the source reg to the move reg
3) Shift any .rel.text relocation r_offset >= insertion offset by +4

This tool performs the insertion *in place* by consuming 4 bytes of trailing
padding in the .text section. It will fail if .text has no room.

Typical use (_SpuIsInAllocateArea_ / func_80067490 style):
    python3 tools/ido/insert_move_and_shift.py in.o out.o \\
        --offset 0x10 --move-reg v1 --src-reg a0 --dst-reg a0
"""

import argparse
import struct
import sys
from pathlib import Path


REGS = {
    "zero": 0,
    "at": 1,
    "v0": 2,
    "v1": 3,
    "a0": 4,
    "a1": 5,
    "a2": 6,
    "a3": 7,
    "t0": 8,
    "t1": 9,
    "t2": 10,
    "t3": 11,
    "t4": 12,
    "t5": 13,
    "t6": 14,
    "t7": 15,
    "s0": 16,
    "s1": 17,
    "s2": 18,
    "s3": 19,
    "s4": 20,
    "s5": 21,
    "s6": 22,
    "s7": 23,
    "t8": 24,
    "t9": 25,
    "k0": 26,
    "k1": 27,
    "gp": 28,
    "sp": 29,
    "fp": 30,
    "ra": 31,
}


def parse_reg(s: str) -> int:
    if s in REGS:
        return REGS[s]
    return int(s, 0)


def decode_r(word: int):
    return {
        "op": (word >> 26) & 0x3F,
        "rs": (word >> 21) & 0x1F,
        "rt": (word >> 16) & 0x1F,
        "rd": (word >> 11) & 0x1F,
        "sa": (word >> 6) & 0x1F,
        "funct": word & 0x3F,
    }


def encode_addu_move(dst: int, src: int) -> int:
    # addu dst, src, zero
    op = 0
    rs = src
    rt = 0
    rd = dst
    sa = 0
    funct = 0x21
    return (op << 26) | (rs << 21) | (rt << 16) | (rd << 11) | (sa << 6) | funct


def find_section(data: bytes, name: str):
    """Return (section_header_offset, section_file_offset, section_size)."""
    if data[:4] != b"\x7fELF":
        raise ValueError("Not an ELF file")

    # ELF32 little-endian
    e_shoff = struct.unpack("<I", data[32:36])[0]
    e_shentsize = struct.unpack("<H", data[46:48])[0]
    e_shnum = struct.unpack("<H", data[48:50])[0]
    e_shstrndx = struct.unpack("<H", data[50:52])[0]

    shstr_hdr_off = e_shoff + (e_shstrndx * e_shentsize)
    shstr_off = struct.unpack("<I", data[shstr_hdr_off + 16 : shstr_hdr_off + 20])[0]

    for i in range(e_shnum):
        sh_off = e_shoff + (i * e_shentsize)
        sh_name_idx = struct.unpack("<I", data[sh_off : sh_off + 4])[0]
        sec_off = struct.unpack("<I", data[sh_off + 16 : sh_off + 20])[0]
        sec_size = struct.unpack("<I", data[sh_off + 20 : sh_off + 24])[0]

        n0 = shstr_off + sh_name_idx
        n1 = data.find(b"\x00", n0)
        sec_name = data[n0:n1].decode("ascii", errors="ignore")

        if sec_name == name:
            return sh_off, sec_off, sec_size

    return None, None, None


def insert_and_patch_text(
    text: bytes,
    insert_off: int,
    move_dst: int,
    move_src: int,
    sllv_dst: int,
) -> tuple[bytes, int]:
    """Return (new_text, patched_sllv_count). Length is unchanged."""
    if insert_off % 4 != 0:
        raise ValueError(f"insert offset must be 4-byte aligned: 0x{insert_off:x}")
    if insert_off < 0 or insert_off + 4 > len(text):
        raise ValueError(f"insert offset out of range: 0x{insert_off:x} (text size {len(text)})")

    # Keep section size constant by consuming 4 bytes from the end.
    if len(text) < 4:
        raise ValueError("text section too small")

    tail = text[-4:]
    if tail != b"\x00\x00\x00\x00":
        raise ValueError("no trailing NOP padding available in .text (last 4 bytes are non-zero)")

    move_word = encode_addu_move(move_dst, move_src)
    move_bytes = struct.pack("<I", move_word)

    # Insert move and drop last 4 bytes to keep length constant.
    new_text = text[:insert_off] + move_bytes + text[insert_off:-4]
    assert len(new_text) == len(text)

    # Patch sllv a0,a0,rs -> sllv a0,move_reg,rs (i.e., change rt only)
    patched = bytearray(new_text)
    patched_sllv = 0
    for i in range(0, len(patched), 4):
        word = struct.unpack("<I", patched[i : i + 4])[0]
        d = decode_r(word)
        if d["op"] != 0 or d["funct"] != 0x04 or d["sa"] != 0:
            continue
        # sllv dst, rt, rs (rd=dst)
        if d["rd"] != sllv_dst:
            continue
        if d["rt"] != move_src:
            continue

        # Change rt to move_dst (value-to-shift reg)
        word = (word & ~(0x1F << 16)) | (move_dst << 16)
        patched[i : i + 4] = struct.pack("<I", word)
        patched_sllv += 1

    return bytes(patched), patched_sllv


def patch_rel_text(data: bytearray, rel_off: int, rel_size: int, insert_off: int):
    """Patch Elf32_Rel entries (little-endian) in-place."""
    if rel_size % 8 != 0:
        raise ValueError(f".rel.text size not multiple of 8: {rel_size}")
    n = rel_size // 8
    patched = 0
    for i in range(n):
        ent_off = rel_off + i * 8
        r_offset = struct.unpack("<I", data[ent_off : ent_off + 4])[0]
        if r_offset >= insert_off:
            struct.pack_into("<I", data, ent_off, r_offset + 4)
            patched += 1
    return patched


def main():
    ap = argparse.ArgumentParser(description="Insert move + patch sllv + fix .rel.text offsets")
    ap.add_argument("input", type=Path, help="Input ELF .o")
    ap.add_argument("output", type=Path, help="Output ELF .o")
    ap.add_argument("--offset", required=True, type=lambda s: int(s, 0), help="Insert offset in .text (bytes)")
    ap.add_argument("--move-reg", required=True, help="Destination register for inserted move (e.g. v1)")
    ap.add_argument("--src-reg", required=True, help="Source register for inserted move (e.g. a0)")
    ap.add_argument("--dst-reg", default=None, help="sllv destination register to patch (default: src-reg)")
    ap.add_argument("-v", "--verbose", action="store_true", help="Verbose output")
    args = ap.parse_args()

    move_dst = parse_reg(args.move_reg)
    move_src = parse_reg(args.src_reg)
    sllv_dst = parse_reg(args.dst_reg) if args.dst_reg is not None else move_src

    data = bytearray(args.input.read_bytes())

    text_sh_off, text_off, text_size = find_section(data, ".text")
    if text_off is None:
        print("Error: .text section not found", file=sys.stderr)
        return 1

    if args.verbose:
        print(f".text: file_off=0x{text_off:x} size=0x{text_size:x}")

    text = bytes(data[text_off : text_off + text_size])
    new_text, patched_sllv = insert_and_patch_text(
        text,
        insert_off=args.offset,
        move_dst=move_dst,
        move_src=move_src,
        sllv_dst=sllv_dst,
    )

    if patched_sllv != 1:
        print(f"Error: expected to patch exactly 1 sllv, patched {patched_sllv}", file=sys.stderr)
        return 1

    data[text_off : text_off + text_size] = new_text

    # Patch relocations if present.
    rel_sh_off, rel_off, rel_size = find_section(data, ".rel.text")
    if rel_off is not None:
        rel_patched = patch_rel_text(data, rel_off, rel_size, args.offset)
        if args.verbose:
            print(f".rel.text: patched {rel_patched} relocation offsets (+4) at r_offset>=0x{args.offset:x}")
    else:
        if args.verbose:
            print("Note: no .rel.text section found, skipping relocation patch")

    args.output.write_bytes(data)
    if args.verbose:
        print(f"Wrote {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

