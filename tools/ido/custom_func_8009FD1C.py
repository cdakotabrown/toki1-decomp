#!/usr/bin/env python3
"""Custom post-processing for SetGraphQueue.

Applied after: convert_or_to_addu, strip_leading_instructions --count 2,
               patch_stack_complete --old 32 --new 40 --shift-ra,
               remap_stack_slots --map 20:28,28:36

Register remapping per slot group:
  Slots 0,1,7: v0(2) -> t6(14) (libgpu_setgraphqueue_var_bd61 cross-prologue)
  Slot 3: t6(14) -> t7(15) (hidden param D_800EBD62)
  Slots 8,9,12: t9(25) -> t8(24) (D_800E3C88 function ptr)
  Slots 14,15: v0(2) -> t0(8) (libgpu_setgraphqueue_var_bd61 reload after call)
  Slots 17,19: t7(15) -> t9(25) (arg0 reload)
  Slot 19: v0(2) -> t0(8) (libgpu_setgraphqueue_var_bd61 reload in beq)
  Slots 18,21: t8(24) -> t1(9) (graph_jtb ptr) [after reorder]
  Slot 23: t8(24) -> t1(9), v0(2) -> t2(10) (vtable)
  Slot 25: v0(2) -> t2(10) (jalr vtable func)
  Slots 27,32: t0(8) -> t3(11) (arg0 for sb)

Scheduling reorder at slots 16-21 (same as before).
Branch offset fix at slot 6 (bnez +10 -> +7).
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

    # Register remapping per slot
    for s in [0, 1, 7]:
        insns[s] = remap_insn(insns[s], {2: 14})  # v0 -> t6

    insns[3] = remap_insn(insns[3], {14: 15})  # t6 -> t7 (hidden param)

    for s in [8, 9, 12]:
        insns[s] = remap_insn(insns[s], {25: 24})  # t9 -> t8

    for s in [14, 15]:
        insns[s] = remap_insn(insns[s], {2: 8})  # v0 -> t0

    # Slots 17,19: t7->t9 (arg0 reload)
    insns[17] = remap_insn(insns[17], {15: 25})  # t7 -> t9
    # Slot 19: beq t7,v0 -> beq t9,t0
    insns[19] = remap_insn(insns[19], {15: 25, 2: 8})

    # Slots 18,21: t8 -> t1 (graph_jtb)
    for s in [18, 21]:
        insns[s] = remap_insn(insns[s], {24: 9})

    # Slot 23: t8->t1, v0->t2
    insns[23] = remap_insn(insns[23], {24: 9, 2: 10})

    # Slot 25: v0 -> t2
    insns[25] = remap_insn(insns[25], {2: 10})

    # Slots 27, 32: t0 -> t3
    for s in [27, 32]:
        insns[s] = remap_insn(insns[s], {8: 11})

    print("Step 1: applied register remapping")

    # Reorder slots 16-21
    # Compiled: [16:nop, 17:lw t9,40, 18:lui t1,X, 19:beq t9,t0, 20:lw ra, 21:lw t1,X]
    # Target:   [16:lw t9,40, 17:nop, 18:beq t9,t0, 19:lw ra, 20:lui t1,X, 21:lw t1,X]
    old16 = insns[16:22]
    insns[16] = old16[1]
    insns[17] = old16[0]
    insns[18] = old16[3]
    insns[19] = old16[4]
    insns[20] = old16[2]
    insns[21] = old16[5]

    print("Step 2: reordered slots 16-21")

    # Fix bnez at slot 6: +10 -> +7
    bnez = insns[6]
    insns[6] = (bnez & 0xFFFF0000) | 7

    # Fix beq at slot 18: needs to reach slot 34
    # beq at slot 18: target = 34 - (18+1) = 15
    beq = insns[18]
    insns[18] = (beq & 0xFFFF0000) | 15

    print("Step 3: fixed branch offsets")

    # Write all
    for i in range(n_insns):
        write_insn(data, T + i*4, insns[i])

    # Update relocations: lui t8 (graph_jtb) moved from slot 18 to 20
    rel_sh, rel_off, rel_size = find_section(data, '.rel.text')
    if rel_off is not None:
        n_rels = rel_size // 8
        for i in range(n_rels):
            ent_off = rel_off + i * 8
            r_offset = struct.unpack('<I', data[ent_off:ent_off+4])[0]
            # lui at slot 18 was at byte offset 0x48, now at slot 20 = 0x50
            if r_offset == 0x48:
                struct.pack_into('<I', data, ent_off, 0x50)
                print(f"Step 4: moved reloc 0x48 -> 0x50")

    with open(sys.argv[2], 'wb') as f:
        f.write(data)

    print("Done.")


if __name__ == '__main__':
    main()
