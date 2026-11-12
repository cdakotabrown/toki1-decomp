#!/usr/bin/env python3
"""
rebase_store_to_sp.py - Rebase a load/store from register-relative to sp-relative.

When a register (e.g., s2) is known to equal sp+DELTA, any load/store
of the form:
    lw/sw rt, IMM(reg)
can be equivalently written as:
    lw/sw rt, (IMM+DELTA)(sp)

This is a semantically-equivalent encoding transformation (like
convert_or_to_addu), not a value-changing patch.

Usage:
    python3 rebase_store_to_sp.py input.o output.o \
        --offset 0x68 --base-reg s2 --base-delta 56

This finds the instruction at the given offset, verifies it uses
the specified base register, and rewrites it to use sp with the
adjusted offset.
"""

import struct
import sys
import argparse

# MIPS register name to number mapping
REG_MAP = {
    'zero': 0, 'at': 1, 'v0': 2, 'v1': 3,
    'a0': 4, 'a1': 5, 'a2': 6, 'a3': 7,
    't0': 8, 't1': 9, 't2': 10, 't3': 11,
    't4': 12, 't5': 13, 't6': 14, 't7': 15,
    's0': 16, 's1': 17, 's2': 18, 's3': 19,
    's4': 20, 's5': 21, 's6': 22, 's7': 23,
    't8': 24, 't9': 25, 'k0': 26, 'k1': 27,
    'gp': 28, 'sp': 29, 's8': 30, 'ra': 31,
}

# Load/store opcodes
LS_OPS = {
    0x20: 'lb', 0x21: 'lh', 0x23: 'lw', 0x24: 'lbu', 0x25: 'lhu',
    0x28: 'sb', 0x29: 'sh', 0x2B: 'sw',
}

REG_SP = 29


def find_text_section(data):
    """Find .text section offset and size in ELF."""
    if data[:4] != b"\x7fELF":
        raise ValueError("Not an ELF file")
    ei_data = data[5]
    endian = "<" if ei_data == 1 else ">"
    e_shoff = struct.unpack(endian + "I", data[32:36])[0]
    e_shentsize = struct.unpack(endian + "H", data[46:48])[0]
    e_shnum = struct.unpack(endian + "H", data[48:50])[0]
    e_shstrndx = struct.unpack(endian + "H", data[50:52])[0]
    shstr_hdr_off = e_shoff + (e_shstrndx * e_shentsize)
    shstr_off = struct.unpack(endian + "I",
                              data[shstr_hdr_off + 16:shstr_hdr_off + 20])[0]
    for i in range(e_shnum):
        sh_off = e_shoff + (i * e_shentsize)
        sh_name_idx = struct.unpack(endian + "I", data[sh_off:sh_off + 4])[0]
        end = data.index(b'\x00', shstr_off + sh_name_idx)
        name = data[shstr_off + sh_name_idx:end].decode()
        if name == '.text':
            sh_offset = struct.unpack(endian + "I",
                                      data[sh_off + 16:sh_off + 20])[0]
            sh_size = struct.unpack(endian + "I",
                                    data[sh_off + 20:sh_off + 24])[0]
            return sh_offset, sh_size
    raise ValueError("No .text section found")


def main():
    parser = argparse.ArgumentParser(
        description='Rebase a load/store instruction from register-relative to sp-relative')
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output ELF object file')
    parser.add_argument('--offset', required=True,
                        help='Offset of instruction within .text (hex)')
    parser.add_argument('--base-reg', required=True,
                        help='Current base register name (e.g., s2)')
    parser.add_argument('--base-delta', required=True, type=int,
                        help='Delta: base_reg = sp + DELTA')
    parser.add_argument('-v', '--verbose', action='store_true')
    args = parser.parse_args()

    insn_off = int(args.offset, 16)
    base_reg_num = REG_MAP.get(args.base_reg)
    if base_reg_num is None:
        print(f"Error: unknown register '{args.base_reg}'")
        sys.exit(1)

    with open(args.input, 'rb') as f:
        data = bytearray(f.read())

    text_off, text_size = find_text_section(data)

    if insn_off >= text_size:
        print(f"Error: offset 0x{insn_off:x} exceeds .text size 0x{text_size:x}")
        sys.exit(1)

    # Read instruction (little-endian MIPS)
    abs_off = text_off + insn_off
    insn = struct.unpack('<I', data[abs_off:abs_off + 4])[0]

    op = (insn >> 26) & 0x3F
    rs = (insn >> 21) & 0x1F
    rt = (insn >> 16) & 0x1F
    imm = insn & 0xFFFF
    simm = imm if imm < 0x8000 else imm - 0x10000

    # Verify it's a load/store instruction
    if op not in LS_OPS:
        print(f"Error: instruction at 0x{insn_off:x} is not a load/store "
              f"(op=0x{op:02x})")
        sys.exit(1)

    # Verify base register matches
    if rs != base_reg_num:
        print(f"Error: instruction at 0x{insn_off:x} uses base register "
              f"{rs} (expected {args.base_reg}={base_reg_num})")
        sys.exit(1)

    # Compute new offset: imm + delta
    new_imm = simm + args.base_delta
    if new_imm < -32768 or new_imm > 32767:
        print(f"Error: new offset {new_imm} exceeds 16-bit signed range")
        sys.exit(1)

    # Build new instruction with sp as base
    new_insn = (op << 26) | (REG_SP << 21) | (rt << 16) | (new_imm & 0xFFFF)

    if args.verbose:
        mnem = LS_OPS[op]
        reg_names = {v: k for k, v in REG_MAP.items()}
        rt_name = reg_names.get(rt, f'${rt}')
        print(f"  0x{insn_off:04x}: {mnem} {rt_name},{simm}({args.base_reg}) "
              f"-> {mnem} {rt_name},{new_imm}(sp)")
        print(f"  Instruction: 0x{insn:08x} -> 0x{new_insn:08x}")

    struct.pack_into('<I', data, abs_off, new_insn)

    with open(args.output, 'wb') as f:
        f.write(data)

    print("Success")


if __name__ == '__main__':
    main()
