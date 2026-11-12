#!/usr/bin/env python3
"""Auto-detect and pad stack frames in MIPS ELF object files.

Unlike patch_stack_complete.py which requires knowing the old/new frame sizes,
this tool automatically detects all function prologues and adds a fixed padding
to each frame. This handles functions of ANY frame size.

Usage:
    python3 patch_stack_auto.py output.o --pad 16

This adds 16 bytes to every function's stack frame, adjusting:
  - Prologue: addiu sp,sp,-N -> addiu sp,sp,-(N+pad)
  - Epilogue: addiu sp,sp,N -> addiu sp,sp,(N+pad)
  - Argument area: load/store with offset >= N get adjusted by pad
"""

import sys
import struct
import argparse

REG_SP = 29
OP_ADDIU = 0x09
OP_LW = 0x23
OP_SW = 0x2B
OP_LBU = 0x24
OP_LB = 0x20
OP_LH = 0x21
OP_LHU = 0x25
OP_SB = 0x28
OP_SH = 0x29

LOAD_STORE_OPS = {OP_LW, OP_SW, OP_LBU, OP_LB, OP_LH, OP_LHU, OP_SB, OP_SH}

def decode_insn(word):
    op = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    imm = word & 0xFFFF
    if imm & 0x8000:
        imm_signed = imm - 0x10000
    else:
        imm_signed = imm
    return op, rs, rt, imm, imm_signed

def encode_insn(op, rs, rt, imm):
    return (op << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)

def find_text_section(data):
    if data[:4] != b'\x7fELF':
        return None, None
    e_shoff = struct.unpack('<I', data[0x20:0x24])[0]
    e_shentsize = struct.unpack('<H', data[0x2e:0x30])[0]
    e_shnum = struct.unpack('<H', data[0x30:0x32])[0]
    e_shstrndx = struct.unpack('<H', data[0x32:0x34])[0]
    shstr_hdr_off = e_shoff + e_shstrndx * e_shentsize
    shstr_off = struct.unpack('<I', data[shstr_hdr_off + 0x10:shstr_hdr_off + 0x14])[0]
    for i in range(e_shnum):
        sh_off = e_shoff + i * e_shentsize
        sh_name_idx = struct.unpack('<I', data[sh_off:sh_off+4])[0]
        sh_type = struct.unpack('<I', data[sh_off+4:sh_off+8])[0]
        name_end = data.find(b'\x00', shstr_off + sh_name_idx)
        name = data[shstr_off + sh_name_idx:name_end].decode('ascii', errors='ignore')
        if name == '.text' and sh_type == 1:
            text_off = struct.unpack('<I', data[sh_off + 0x10:sh_off + 0x14])[0]
            text_size = struct.unpack('<I', data[sh_off + 0x14:sh_off + 0x18])[0]
            return text_off, text_size
    return None, None

def patch_auto(data, pad, verbose=False):
    """Auto-detect all prologues and add pad bytes to each frame."""
    data = bytearray(data)

    text_off, text_size = find_text_section(data)
    if text_off is None:
        return None

    # Phase 1: Find all function prologues (addiu sp,sp,-N where N>0)
    functions = []
    for off in range(text_off, text_off + text_size, 4):
        word = struct.unpack('<I', data[off:off+4])[0]
        op, rs, rt, imm, imm_signed = decode_insn(word)
        if op == OP_ADDIU and rs == REG_SP and rt == REG_SP and imm_signed < 0:
            old_size = -imm_signed
            functions.append((off, old_size))

    if not functions:
        return bytes(data)

    # Phase 2: Patch each function
    patched = 0
    for func_idx, (prologue_off, old_size) in enumerate(functions):
        new_size = old_size + pad

        # Determine function boundary
        if func_idx + 1 < len(functions):
            func_end = functions[func_idx + 1][0]
        else:
            func_end = text_off + text_size

        if verbose:
            print(f"  Function at .text+0x{prologue_off - text_off:x}: frame {old_size} -> {new_size}")

        # Patch all instructions in this function
        for off in range(prologue_off, func_end, 4):
            word = struct.unpack('<I', data[off:off+4])[0]
            op, rs, rt, imm, imm_signed = decode_insn(word)

            new_word = None

            # Prologue: addiu sp,sp,-old_size
            if op == OP_ADDIU and rs == REG_SP and rt == REG_SP and imm_signed == -old_size:
                new_word = encode_insn(OP_ADDIU, REG_SP, REG_SP, (-new_size) & 0xFFFF)

            # Epilogue: addiu sp,sp,old_size
            elif op == OP_ADDIU and rs == REG_SP and rt == REG_SP and imm_signed == old_size:
                new_word = encode_insn(OP_ADDIU, REG_SP, REG_SP, new_size & 0xFFFF)

            # Argument area: load/store with sp base and offset >= old_size
            elif op in LOAD_STORE_OPS and rs == REG_SP and imm_signed >= old_size:
                new_offset = imm_signed + pad
                if new_offset <= 32767:
                    new_word = encode_insn(op, REG_SP, rt, new_offset & 0xFFFF)

            if new_word is not None:
                data[off:off+4] = struct.pack('<I', new_word)
                patched += 1

    if verbose:
        print(f"  Patched {patched} instructions in {len(functions)} function(s)")

    return bytes(data)

def main():
    parser = argparse.ArgumentParser(description='Auto-pad stack frames in MIPS ELF')
    parser.add_argument('file', help='ELF object file to patch (in-place)')
    parser.add_argument('--pad', type=int, required=True, help='Bytes to add to each frame')
    parser.add_argument('-v', '--verbose', action='store_true')
    args = parser.parse_args()

    with open(args.file, 'rb') as f:
        data = f.read()

    patched = patch_auto(data, args.pad, args.verbose)
    if patched is None:
        return 1

    with open(args.file, 'wb') as f:
        f.write(patched)

    return 0

if __name__ == "__main__":
    sys.exit(main())
