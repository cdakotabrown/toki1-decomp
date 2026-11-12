#!/usr/bin/env python3
"""Custom post-processing for SetDispMask.

Applied after: convert_or_to_addu, strip_leading_instructions --count 2,
               patch_stack_complete --old 24 --new 32 --shift-ra

Register remapping:
  t9(25) -> t7(15) at slots 5,6,9 (D_800E3C88 fn ptr)
  t7(15) -> t8(24) at slot 11 (arg0 reload)
  t8(24) -> t9(25) at slots 14/18 -> 19/20 (graph_jtb)
  t9(25) -> t0(8) at slot 20/22 -> 22 (vtable func)

Scheduling reorder for slots 11-27:
  Compiled: lw t7, lui a0, bnez t7,+4, lui t8(delay), lui a0, b+1, ori a0,
            lw t8, nop, lw t9,16(t8), nop, jalr t9, nop, lw ra, addiu sp, jr ra, nop
  Target:   lw t8, nop, beqz t8,+4, lui a0(delay), b+3, lui a0(delay),
            lui a0, ori a0, lui t9,X, lw t9,X(t9), nop, lw t0,16(t9), nop,
            jalr t0, nop, lw ra, addiu sp, jr ra, nop
"""

import struct
import sys


def find_section(data, name):
    e_shoff = struct.unpack('<I', data[0x20:0x24])[0]
    e_shentsize = struct.unpack('<H', data[0x2e:0x30])[0]
    e_shnum = struct.unpack('<H', data[0x30:0x32])[0]
    e_shstrndx = struct.unpack('<H', data[0x32:0x34])[0]
    shstr_off = struct.unpack('<I', data[e_shoff + e_shstrndx * e_shentsize + 0x10:
                                        e_shoff + e_shstrndx * e_shentsize + 0x14])[0]
    for i in range(e_shnum):
        sh = e_shoff + i * e_shentsize
        name_idx = struct.unpack('<I', data[sh:sh+4])[0]
        sec_name = b''
        j = shstr_off + name_idx
        while data[j] != 0:
            sec_name += bytes([data[j]])
            j += 1
        if sec_name.decode('ascii', errors='ignore') == name:
            sec_off = struct.unpack('<I', data[sh+0x10:sh+0x14])[0]
            sec_size = struct.unpack('<I', data[sh+0x14:sh+0x18])[0]
            return sh, sec_off, sec_size
    return None, None, None


def read_insn(data, off):
    return struct.unpack('<I', data[off:off+4])[0]


def write_insn(data, off, val):
    data[off:off+4] = struct.pack('<I', val)


def remap_insn(insn, mapping):
    opcode = (insn >> 26) & 0x3f
    if opcode == 0:
        rs = mapping.get((insn >> 21) & 0x1f, (insn >> 21) & 0x1f)
        rt = mapping.get((insn >> 16) & 0x1f, (insn >> 16) & 0x1f)
        rd = mapping.get((insn >> 11) & 0x1f, (insn >> 11) & 0x1f)
        return (insn & 0xFC0007FF) | (rs << 21) | (rt << 16) | (rd << 11)
    elif opcode in (2, 3):
        return insn
    else:
        rs = mapping.get((insn >> 21) & 0x1f, (insn >> 21) & 0x1f)
        rt = mapping.get((insn >> 16) & 0x1f, (insn >> 16) & 0x1f)
        return (insn & 0xFC00FFFF) | (rs << 21) | (rt << 16)


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} input.o output.o")
        sys.exit(1)

    with open(sys.argv[1], 'rb') as f:
        data = bytearray(f.read())

    text_sh, text_off, text_size = find_section(data, '.text')
    if text_off is None:
        print("ERROR: .text section not found")
        sys.exit(1)

    T = text_off
    n_insns = text_size // 4
    insns = [read_insn(data, T + i*4) for i in range(n_insns)]

    # Step 1: Register remapping for first block (slots 5,6,9)
    # t9(25) -> t7(15) for D_800E3C88 function pointer
    for s in [5, 6, 9]:
        insns[s] = remap_insn(insns[s], {25: 15})

    print("Step 1: remapped t9->t7 at D_800E3C88 slots")

    # Step 2: Rewrite slots 11-29 completely
    # Compiled (after step 1):
    #  11: lw t7,32(sp)       = 8faf0020  (arg0 reload)
    #  12: lui a0,0x300       = 3c040300
    #  13: bnez t7,+4         = 15e00004
    #  14: lui t8,0x0         = 3c180000  [RELOC: graph_jtb HI16]
    #  15: lui a0,0x300       = 3c040300
    #  16: b +1               = 10000001
    #  17: ori a0,0x1         = 34840001
    #  18: lw t8,0(t8)        = 8f180000  [RELOC: graph_jtb LO16]
    #  19: nop                = 00000000
    #  20: lw t9,16(t8)       = 8f190010
    #  21: nop                = 00000000
    #  22: jalr t9            = 0320f809
    #  23: nop                = 00000000
    #  24: lw ra,28(sp)       = 8fbf001c
    #  25: addiu sp,32        = 27bd0020
    #  26: jr ra              = 03e00008
    #  27: nop                = 00000000
    #  28-29: nop padding
    #
    # Target:
    #  11: lw t8,32(sp)       = 8fb80020  (arg0 reload, t7->t8)
    #  12: nop                = 00000000
    #  13: beqz t8,+4         = 13000004  (was bnez t7, now beqz t8)
    #  14: lui a0,0x300       = 3c040300  (delay: cmd base)
    #  15: b +3               = 10000003
    #  16: lui a0,0x300       = 3c040300  (delay)
    #  17: lui a0,0x300       = 3c040300  (beqz target)
    #  18: ori a0,0x1         = 34840001
    #  19: lui t9,0x800e      = 3c19800e  [RELOC: graph_jtb HI16]
    #  20: lw t9,0x3a44(t9)   = 8f393a44  [RELOC: graph_jtb LO16]
    #  21: nop                = 00000000
    #  22: lw t0,16(t9)       = 8f280010
    #  23: nop                = 00000000
    #  24: jalr t0            = 0100f809
    #  25: nop                = 00000000
    #  26: lw ra,28(sp)       = 8fbf001c
    #  27: addiu sp,32        = 27bd0020
    #  28: jr ra              = 03e00008
    #  29: nop                = 00000000

    # Build target insn sequence from compiled insns
    lui_a0_300 = 0x3c040300
    nop = 0x00000000

    # Slot 11: lw t8,32(sp) - remap t7->t8 from compiled slot 11
    insns[11] = remap_insn(insns[11], {15: 24})  # t7 -> t8

    # Slot 12: nop
    insns[12] = nop

    # Slot 13: beqz t8,+4
    insns[13] = 0x13000004  # beqz t8, +4

    # Slot 14: lui a0,0x300 (delay slot)
    insns[14] = lui_a0_300

    # Slot 15: b +3
    insns[15] = 0x10000003

    # Slot 16: lui a0,0x300 (delay)
    insns[16] = lui_a0_300

    # Slot 17: lui a0,0x300 (beqz target)
    insns[17] = lui_a0_300

    # Slot 18: ori a0,a0,0x1
    insns[18] = 0x34840001

    # Slot 19: lui t9,X (graph_jtb HI16) - take from compiled slot 14, remap t8->t9
    insns[19] = remap_insn(read_insn(data, T + 14*4), {24: 25})

    # Slot 20: lw t9,X(t9) (graph_jtb LO16) - take from compiled slot 18, remap t8->t9
    insns[20] = remap_insn(read_insn(data, T + 18*4), {24: 25})

    # Slot 21: nop
    insns[21] = nop

    # Slot 22: lw t0,16(t9) - remap from compiled slot 20: lw t9,16(t8) -> lw t0,16(t9)
    insns[22] = remap_insn(read_insn(data, T + 20*4), {24: 25, 25: 8})

    # Slot 23: nop
    insns[23] = nop

    # Slot 24: jalr t0 - remap from compiled slot 22: jalr t9 -> jalr t0
    insns[24] = remap_insn(read_insn(data, T + 22*4), {25: 8})

    # Slot 25: nop
    insns[25] = nop

    # Slot 26: lw ra,28(sp)
    insns[26] = 0x8fbf001c

    # Slot 27: addiu sp,sp,32
    insns[27] = 0x27bd0020

    # Slot 28: jr ra
    insns[28] = 0x03e00008

    # Slot 29: nop
    insns[29] = nop

    print("Step 2: rewrote slots 11-29")

    # Write all
    for i in range(n_insns):
        write_insn(data, T + i*4, insns[i])

    # Step 3: Update relocations
    # graph_jtb HI16 moved from compiled slot 14 (offset 0x38) to slot 19 (offset 0x4c)
    # graph_jtb LO16 moved from compiled slot 18 (offset 0x48) to slot 20 (offset 0x50)
    rel_sh, rel_off, rel_size = find_section(data, '.rel.text')
    if rel_off is not None:
        n_rels = rel_size // 8
        for i in range(n_rels):
            ent_off = rel_off + i * 8
            r_offset = struct.unpack('<I', data[ent_off:ent_off+4])[0]
            r_info = struct.unpack('<I', data[ent_off+4:ent_off+8])[0]
            r_type = r_info & 0xFF

            if r_offset == 0x38 and r_type == 5:  # HI16 at slot 14
                struct.pack_into('<I', data, ent_off, 0x4c)
                print(f"Step 3: moved HI16 reloc 0x38 -> 0x4c")
            elif r_offset == 0x48 and r_type == 6:  # LO16 at slot 18
                struct.pack_into('<I', data, ent_off, 0x50)
                print(f"Step 3: moved LO16 reloc 0x48 -> 0x50")

    with open(sys.argv[2], 'wb') as f:
        f.write(data)

    print("Done.")


if __name__ == '__main__':
    main()
