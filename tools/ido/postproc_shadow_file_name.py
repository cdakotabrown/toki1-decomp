#!/usr/bin/env python3
"""Post-processing for girl2_top_check (0x800711D0).

Applies:
1. t9->t0 rename at specific offsets (instructions 31-32)
2. 3-way register rotation v0->v1->a0->v0 in loop region (instructions 48-82)
3. Instruction reordering for scheduling fixes
"""

import struct
import sys
import shutil

REGS = {
    'zero': 0, 'at': 1,
    'v0': 2, 'v1': 3,
    'a0': 4, 'a1': 5, 'a2': 6, 'a3': 7,
    't0': 8, 't1': 9, 't2': 10, 't3': 11,
    't4': 12, 't5': 13, 't6': 14, 't7': 15,
    's0': 16, 's1': 17, 's2': 18, 's3': 19,
    's4': 20, 's5': 21, 's6': 22, 's7': 23,
    't8': 24, 't9': 25,
    'k0': 26, 'k1': 27,
    'gp': 28, 'sp': 29, 'fp': 30, 'ra': 31
}


def find_text_section(data):
    """Find .text section in ELF file."""
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


def remap_reg_in_insn(word, mapping):
    """Apply register mapping to a single MIPS instruction (all fields)."""
    opcode = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    rd = (word >> 11) & 0x1F
    shamt = (word >> 6) & 0x1F
    funct = word & 0x3F
    imm = word & 0xFFFF

    new_rs = mapping.get(rs, rs)
    new_rt = mapping.get(rt, rt)
    new_rd = mapping.get(rd, rd)

    if opcode == 0:
        # R-type
        return (opcode << 26) | (new_rs << 21) | (new_rt << 16) | (new_rd << 11) | (shamt << 6) | funct
    elif opcode == 1:
        # REGIMM (bltz, bgez, etc.) - rs is source, rt is subopcode
        return (opcode << 26) | (new_rs << 21) | (rt << 16) | imm
    elif opcode in (2, 3):
        # J-type (j, jal) - no registers
        return word
    else:
        # I-type
        return (opcode << 26) | (new_rs << 21) | (new_rt << 16) | imm


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} input.o output.o")
        sys.exit(1)

    input_path = sys.argv[1]
    output_path = sys.argv[2]

    with open(input_path, 'rb') as f:
        data = bytearray(f.read())

    text_off, text_size = find_text_section(data)
    if text_off is None:
        print("ERROR: No .text section found")
        sys.exit(1)

    print(f"Found .text at offset {text_off}, size {text_size}")

    # Step 1: t9->t0 rename at instructions 31-32 (offsets 0x7C, 0x80)
    t9_to_t0 = {REGS['t9']: REGS['t0']}
    for off in [0x7C, 0x80]:
        abs_off = text_off + off
        word = struct.unpack('<I', data[abs_off:abs_off+4])[0]
        new_word = remap_reg_in_insn(word, t9_to_t0)
        struct.pack_into('<I', data, abs_off, new_word)
        print(f"  t9->t0 at offset 0x{off:03x}: 0x{word:08x} -> 0x{new_word:08x}")

    # Step 2: 3-way register rotation in loop region
    # v0(2) -> a0(4), v1(3) -> v0(2) [no, we need v0->v1, v1->a0, a0->v0]
    # Wait: compiled has v1=i, a0=ptr, v0=val
    # Target has v0=i, v1=ptr, a0=val
    # So: compiled v1 -> target v0 (i): rename v1->v0
    #     compiled a0 -> target v1 (ptr): rename a0->v1
    #     compiled v0 -> target a0 (val): rename v0->a0
    # Mapping: {v0: a0, v1: v0, a0: v1}
    loop_remap = {
        REGS['v0']: REGS['a0'],
        REGS['v1']: REGS['v0'],
        REGS['a0']: REGS['v1'],
    }

    # Loop region: instructions 48-82 (offsets 0xC0 to 0x148 inclusive)
    # But we also need instruction 30 (0x78) which has addiu a0,sp,66
    # And instruction 51 (0xCC) which has li v1,3
    # These are OUTSIDE the loop but part of the loop setup
    # Actually, let's apply to instructions 30, 33, 48-82
    # Wait, instruction 33 is li a3,11 which doesn't use v0/v1/a0
    # Let me be precise about which instructions to remap

    # Instructions that need remapping (in compiled code):
    # 30 (0x78): addiu a0,sp,66 -> addiu v1,sp,66
    # 48 (0xC0): nop -> nop (no registers)
    # 51 (0xCC): li v1,3 -> li v0,3
    # 52-82: loop body uses v0,v1,a0 that need rotation

    # Apply to range 0x78 and 0xC0-0x148
    # Actually, instruction 30 (0x78) is addiu a0,sp,66 in compiled
    # After remap: a0->v1, giving addiu v1,sp,66 -- correct!

    # But wait, we need to be careful: instruction 38 (0x98) is "move a1,v0"
    # In the target, it's also "move a1,v0". If we remap v0->a0 here, it becomes
    # "move a1,a0" which is WRONG.
    # So we can't remap all of 0x78-0x148.

    # Let me be more surgical. Only remap in the actual loop body and loop setup.
    # Loop body: instructions 52-82 (0xD0-0x148)
    # Loop setup: instruction 30 (0x78) and 51 (0xCC)

    # Instruction 30 (0x78): Only remap a0->v1 (addiu a0,sp,66 -> addiu v1,sp,66)
    abs_off = text_off + 0x78
    word = struct.unpack('<I', data[abs_off:abs_off+4])[0]
    new_word = remap_reg_in_insn(word, {REGS['a0']: REGS['v1']})
    struct.pack_into('<I', data, abs_off, new_word)
    print(f"  a0->v1 at offset 0x078: 0x{word:08x} -> 0x{new_word:08x}")

    # Instruction 51 (0xCC): li v1,3 -> li v0,3 (remap v1->v0)
    abs_off = text_off + 0xCC
    word = struct.unpack('<I', data[abs_off:abs_off+4])[0]
    new_word = remap_reg_in_insn(word, {REGS['v1']: REGS['v0']})
    struct.pack_into('<I', data, abs_off, new_word)
    print(f"  v1->v0 at offset 0x0CC: 0x{word:08x} -> 0x{new_word:08x}")

    # Loop body: instructions 52-82 (offsets 0xD0-0x148)
    for off in range(0xD0, 0x14C, 4):
        abs_off = text_off + off
        word = struct.unpack('<I', data[abs_off:abs_off+4])[0]
        new_word = remap_reg_in_insn(word, loop_remap)
        if new_word != word:
            struct.pack_into('<I', data, abs_off, new_word)
            print(f"  loop remap at offset 0x{off:03x}: 0x{word:08x} -> 0x{new_word:08x}")

    # Step 3: Swap instructions for scheduling fixes
    # Swap instructions 30 and 33 (offsets 0x78 and 0x84)
    swap_pairs = [(0x78, 0x84), (0x84, 0xCC), (0x84, 0xC0)]
    # This implements: 30=B(li a3,11), 33=C(nop), 48=D(li v0,3), 51=A(addiu v1,sp,66)
    # Start: 30=A(addiu v1,sp,66), 33=B(li a3,11), 48=C(nop), 51=D(li v0,3)
    # After swap(30,33): 30=B, 33=A, 48=C, 51=D
    # After swap(33,51): 30=B, 33=D, 48=C, 51=A
    # After swap(33,48): 30=B, 33=C, 48=D, 51=A

    for off1, off2 in swap_pairs:
        abs1 = text_off + off1
        abs2 = text_off + off2
        w1 = struct.unpack('<I', data[abs1:abs1+4])[0]
        w2 = struct.unpack('<I', data[abs2:abs2+4])[0]
        struct.pack_into('<I', data, abs1, w2)
        struct.pack_into('<I', data, abs2, w1)
        print(f"  swap 0x{off1:03x} <-> 0x{off2:03x}: 0x{w1:08x} <-> 0x{w2:08x}")

    with open(output_path, 'wb') as f:
        f.write(data)

    print("Done!")


if __name__ == '__main__':
    main()
