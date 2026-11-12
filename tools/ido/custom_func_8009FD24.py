#!/usr/bin/env python3
"""Custom post-processing for func_8009FD24.

Applied after: convert_or_to_addu, strip_leading_instructions --count 4,
               strip_trailing_instructions --count 1 --nop-only,
               patch_stack_complete --old 32 --new 40 --shift-ra,
               remap_stack_slots --map 20:28,28:36

Register remapping:
  Slots 6,7,10: t9(25) -> t8(24) (D_800E3C88 function ptr)
  Slot 14: t8(24) -> t9(25) (arg0 reload)
  Slot 16: t8(24) -> t9(25) (beq source reg)
  Slots 20,22: v0(2) -> t2(10) (vtable func ptr)
  Slots 24,29: t2(10) -> t3(11) (arg0 for sb)

Scheduling reorder at slots 15-30:
  Move lui t1 from slot 15 to slot 18
  Insert nop at slot 15
  Shift remaining insns down by 1
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

    # Step 1: Register remapping
    # Slots 6,7,10: t9(25) -> t8(24)
    for s in [6, 7, 10]:
        insns[s] = remap_insn(insns[s], {25: 24})

    # Slot 14: t8(24) -> t9(25)
    insns[14] = remap_insn(insns[14], {24: 25})

    # Slot 16: beq t8,t0 -> beq t9,t0
    insns[16] = remap_insn(insns[16], {24: 25})

    # Slots 20,22: v0(2) -> t2(10)
    for s in [20, 22]:
        insns[s] = remap_insn(insns[s], {2: 10})

    # Slots 24,29: t2(10) -> t3(11)  [was "lw t2" and "sb t2"]
    for s in [24, 29]:
        insns[s] = remap_insn(insns[s], {10: 11})

    print("Step 1: applied register remapping")

    # Step 2: Scheduling reorder for slots 14-30
    # Compiled order (after reg remap):
    #   14: lw t9,40(sp)
    #   15: lui t1,0       <- hoisted, should be at 18
    #   16: beq t9,t0,+14
    #   17: lw ra,28(sp)
    #   18: lw t1,0(t1)
    #   19: li a0,1
    #   20: lw t2,52(t1)
    #   21: nop
    #   22: jalr t2
    #   23: nop
    #   24: lw t3,40(sp)
    #   25: lui at,0
    #   26: li a0,2
    #   27: move a1,zero
    #   28: jal X
    #   29: sb t3,0(at)
    #   30: lw ra,28(sp)
    #
    # Target order:
    #   14: lw t9,40(sp)
    #   15: nop            <- load delay
    #   16: beq t9,t0,+15
    #   17: lw ra,28(sp)
    #   18: lui t1,X       <- was at 15
    #   19: lw t1,X(t1)
    #   20: li a0,1        <- was at 19
    #   21: lw t2,52(t1)   <- was at 20
    #   22: nop
    #   23: jalr t2
    #   24: nop
    #   25: lw t3,40(sp)
    #   26: lui at,X
    #   27: li a0,2
    #   28: move a1,zero
    #   29: jal X
    #   30: sb t3,X(at)

    old = insns[14:35]
    # old[0]=lw t9,40  old[1]=lui t1    old[2]=beq t9,t0  old[3]=lw ra
    # old[4]=lw t1,0   old[5]=li a0,1   old[6]=lw t2,52   old[7]=nop
    # old[8]=jalr t2   old[9]=nop       old[10]=lw t3,40  old[11]=lui at
    # old[12]=li a0,2  old[13]=move a1  old[14]=jal X     old[15]=sb t3
    # old[16]=lw ra    old[17]=lw v0,36 old[18]=jr ra     old[19]=addiu sp
    # old[20]=nop

    new = [
        old[0],   # 14: lw t9,40(sp)
        0,        # 15: nop (load delay)
        old[2],   # 16: beq t9,t0 (fix offset below)
        old[3],   # 17: lw ra,28(sp)
        old[1],   # 18: lui t1,0  (was at 15)
        old[4],   # 19: lw t1,0(t1) (was at 18)
        old[5],   # 20: li a0,1 (was at 19)
        old[6],   # 21: lw t2,52(t1) (was at 20)
        old[7],   # 22: nop
        old[8],   # 23: jalr t2
        old[9],   # 24: nop
        old[10],  # 25: lw t3,40(sp)
        old[11],  # 26: lui at,0
        old[12],  # 27: li a0,2
        old[13],  # 28: move a1,zero
        old[14],  # 29: jal X
        old[15],  # 30: sb t3,0(at)
        old[16],  # 31: lw ra,28(sp)
        old[17],  # 32: lw v0,36(sp)
        old[18],  # 33: jr ra
        old[19],  # 34: addiu sp,sp,40
    ]

    insns[14:35] = new

    print("Step 2: reordered slots 14-30")

    # Step 3: Fix branch offsets
    # beq at slot 16: target = slot 31 (lw ra is at 31 in target)
    # Wait: target .L80 is at "lw v0,36(sp)" which is slot 32
    # beq offset = 32 - (16+1) = 15
    beq = insns[16]
    insns[16] = (beq & 0xFFFF0000) | 15

    print("Step 3: fixed beq branch offset to +15")

    # Step 4: Write all instructions back
    for i in range(n_insns):
        write_insn(data, T + i*4, insns[i])

    # Step 5: Update relocations for reordered instructions
    # Mapping: old byte offset -> new byte offset (for slots that moved)
    # lui t1 (graph_jtb hi): slot 15 -> slot 18  (0x3C -> 0x48)
    # lw t1  (graph_jtb lo): slot 18 -> slot 19  (0x48 -> 0x4C)
    # lui at (libgpu_setgraphqueue_var_bd61 hi): slot 25 -> slot 26  (0x64 -> 0x68)
    # sb t3  (libgpu_setgraphqueue_var_bd61 lo): slot 29 -> slot 30  (0x74 -> 0x78)
    # jal    (DMACallback): slot 28 -> slot 29  (0x70 -> 0x74)
    reloc_map = {
        15 * 4: 18 * 4,  # lui t1 (graph_jtb)
        18 * 4: 19 * 4,  # lw t1 (graph_jtb)
        25 * 4: 26 * 4,  # lui at (libgpu_setgraphqueue_var_bd61)
        29 * 4: 30 * 4,  # sb t3 (libgpu_setgraphqueue_var_bd61)
        28 * 4: 29 * 4,  # jal (DMACallback)
    }
    rel_sh, rel_off, rel_size = find_section(data, '.rel.text')
    if rel_off is not None:
        n_rels = rel_size // 8
        # First pass: collect all changes
        changes = []
        for i in range(n_rels):
            ent_off = rel_off + i * 8
            r_offset = struct.unpack('<I', data[ent_off:ent_off+4])[0]
            if r_offset in reloc_map:
                changes.append((ent_off, reloc_map[r_offset]))
        # Second pass: apply all changes
        for ent_off, new_off in changes:
            old_off = struct.unpack('<I', data[ent_off:ent_off+4])[0]
            struct.pack_into('<I', data, ent_off, new_off)
            print(f"Step 5: moved reloc {old_off:#x} -> {new_off:#x}")

    # Step 6: Rename D_800EBD61b -> libgpu_setgraphqueue_var_bd61 in symbol table
    strtab_sh, strtab_off, strtab_size = find_section(data, '.strtab')
    if strtab_off is not None:
        # Search for "D_800EBD61b" in .strtab
        needle = b'D_800EBD61b\x00'
        replace = b'libgpu_setgraphqueue_var_bd61\x00\x00'  # same length, null-padded
        pos = data.find(needle, strtab_off, strtab_off + strtab_size)
        if pos >= 0:
            data[pos:pos+len(replace)] = replace
            print("Step 6: renamed D_800EBD61b -> libgpu_setgraphqueue_var_bd61 in strtab")

    with open(sys.argv[2], 'wb') as f:
        f.write(data)

    print("Done.")


if __name__ == '__main__':
    main()
