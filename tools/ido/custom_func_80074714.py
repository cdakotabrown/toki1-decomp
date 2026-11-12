#!/usr/bin/env python3
"""Custom post-processing for func_80074714.

Fixes remaining differences after strip_leading_instructions + patch_stack_complete:
1. Scheduling: swap nop and lui at offsets 0x18/0x20
2. Register: v0 -> v1 for cal_cw_anim load at offsets 0x18,0x24,0x2c
3. Stack slot: 28(sp) -> 40(sp) at offsets 0x88,0x94
4. Register: xori dest t0 -> v0 at offset 0xe8, beqz t0 -> v0 at 0xf0
5. Register: t2 -> t1 for val+1 at offsets 0xec,0xf4
6. Register: t3 -> t2 for step_counter at offsets 0x100,0x104,0x10c
"""
import struct
import sys

def patch_word(data, offset, old_word, new_word, mask=0xFFFFFFFF):
    """Patch a 32-bit word at offset, checking old value matches."""
    actual = struct.unpack_from('<I', data, offset)[0]
    if (actual & mask) != (old_word & mask):
        print(f"  WARNING: offset 0x{offset:03x}: expected {old_word:08x} (mask {mask:08x}), got {actual:08x}")
        return data
    patched = (actual & ~mask) | (new_word & mask)
    return data[:offset] + struct.pack('<I', patched) + data[offset+4:]

def main():
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} input.o output.o")
        sys.exit(1)

    input_path = sys.argv[1]
    output_path = sys.argv[2]

    with open(input_path, 'rb') as f:
        data = bytearray(f.read())

    # Find .text section
    # ELF header: e_shoff at offset 0x20 (32-bit ELF)
    e_shoff = struct.unpack_from('<I', data, 0x20)[0]
    e_shentsize = struct.unpack_from('<H', data, 0x2e)[0]
    e_shnum = struct.unpack_from('<H', data, 0x30)[0]
    e_shstrndx = struct.unpack_from('<H', data, 0x32)[0]

    # Find .text section
    shstrtab_off = struct.unpack_from('<I', data, e_shoff + e_shstrndx * e_shentsize + 0x10)[0]

    text_offset = None
    text_size = None
    for i in range(e_shnum):
        sh_off = e_shoff + i * e_shentsize
        sh_name_idx = struct.unpack_from('<I', data, sh_off)[0]
        name_start = shstrtab_off + sh_name_idx
        name_end = data.index(0, name_start)
        name = data[name_start:name_end].decode('ascii')
        if name == '.text':
            text_offset = struct.unpack_from('<I', data, sh_off + 0x10)[0]
            text_size = struct.unpack_from('<I', data, sh_off + 0x14)[0]
            break

    if text_offset is None:
        print("ERROR: .text section not found")
        sys.exit(1)

    print(f"  .text at offset 0x{text_offset:x}, size 0x{text_size:x}")

    # All patches are relative to .text start
    base = text_offset

    # 1. Swap instructions at 0x18 and 0x20 (nop <-> lui)
    # Compiled: 0x18 = lui v0,... (3c020000+reloc), 0x20 = nop (00000000)
    # Target:   0x18 = nop (00000000),              0x20 = lui v1,... (3c030000+reloc)
    instr_18 = struct.unpack_from('<I', data, base + 0x18)[0]
    instr_20 = struct.unpack_from('<I', data, base + 0x20)[0]
    # Swap them
    struct.pack_into('<I', data, base + 0x18, instr_20)
    struct.pack_into('<I', data, base + 0x20, instr_18)
    print(f"  Swapped 0x18 ({instr_18:08x}) <-> 0x20 ({instr_20:08x})")

    # Also need to fix the relocation for the swapped lui
    # We need to update relocations to point to new offsets
    # Find .rel.text section
    rel_text_offset = None
    rel_text_size = None
    for i in range(e_shnum):
        sh_off = e_shoff + i * e_shentsize
        sh_name_idx = struct.unpack_from('<I', data, sh_off)[0]
        name_start = shstrtab_off + sh_name_idx
        name_end = data.index(0, name_start)
        name = data[name_start:name_end].decode('ascii')
        if name == '.rel.text':
            rel_text_offset = struct.unpack_from('<I', data, sh_off + 0x10)[0]
            rel_text_size = struct.unpack_from('<I', data, sh_off + 0x14)[0]
            break

    if rel_text_offset is not None:
        # Each reloc entry is 8 bytes: offset(4) + info(4)
        n_relocs = rel_text_size // 8
        for i in range(n_relocs):
            r_off = struct.unpack_from('<I', data, rel_text_offset + i * 8)[0]
            r_info = struct.unpack_from('<I', data, rel_text_offset + i * 8 + 4)[0]
            if r_off == 0x18:
                # Move reloc from 0x18 to 0x20
                struct.pack_into('<I', data, rel_text_offset + i * 8, 0x20)
                print(f"  Moved reloc 0x18 -> 0x20")
            elif r_off == 0x20:
                # Move reloc from 0x20 to 0x18
                struct.pack_into('<I', data, rel_text_offset + i * 8, 0x18)
                print(f"  Moved reloc 0x20 -> 0x18")

    # 2. Fix register v0 -> v1 at offsets 0x20 (now has lui), 0x24, 0x2c
    # 0x20: lui v0,... -> lui v1,...  (change rt field bits 20:16 from 2 to 3)
    instr = struct.unpack_from('<I', data, base + 0x20)[0]
    # lui rt, imm: opcode=0xf, rs=0, rt=reg, imm=...
    # Change rt from v0(2) to v1(3): bits 20:16
    instr = (instr & ~(0x1f << 16)) | (3 << 16)  # v1 = 3
    struct.pack_into('<I', data, base + 0x20, instr)
    print(f"  0x20: lui v0 -> lui v1")

    # 0x24: lh v0,0(v0) -> lh v1,0(v1)  (change both rs and rt from 2 to 3)
    instr = struct.unpack_from('<I', data, base + 0x24)[0]
    instr = (instr & ~(0x1f << 21)) | (3 << 21)  # rs: v0->v1
    instr = (instr & ~(0x1f << 16)) | (3 << 16)  # rt: v0->v1
    struct.pack_into('<I', data, base + 0x24, instr)
    print(f"  0x24: lh v0,(v0) -> lh v1,(v1)")

    # 0x2c: andi t8,v0,0x1 -> andi t8,v1,0x1  (change rs from 2 to 3)
    instr = struct.unpack_from('<I', data, base + 0x2c)[0]
    instr = (instr & ~(0x1f << 21)) | (3 << 21)  # rs: v0->v1
    struct.pack_into('<I', data, base + 0x2c, instr)
    print(f"  0x2c: andi t8,v0 -> andi t8,v1")

    # 3. Stack slot: sw a0,28(sp) -> sw a0,40(sp) and lw t2,28(sp) -> lw t2,40(sp)
    # 0x88: sw a0,28(sp) [afa4001c] -> sw a0,40(sp) [afa40028]
    instr = struct.unpack_from('<I', data, base + 0x88)[0]
    instr = (instr & 0xffff0000) | 0x0028
    struct.pack_into('<I', data, base + 0x88, instr)
    print(f"  0x88: sw a0,28(sp) -> sw a0,40(sp)")

    # 0x94: lw t2,28(sp) [8faa001c] -> lw t2,40(sp) [8faa0028]
    instr = struct.unpack_from('<I', data, base + 0x94)[0]
    instr = (instr & 0xffff0000) | 0x0028
    struct.pack_into('<I', data, base + 0x94, instr)
    print(f"  0x94: lw t2,28(sp) -> lw t2,40(sp)")

    # 4. xori dest: t0 -> v0 at 0xe8, beqz t0 -> beqz v0 at 0xf0
    # 0xe8: xori t0,v0,0x1 [38480001] -> xori v0,v0,0x1 [38420001]
    instr = struct.unpack_from('<I', data, base + 0xe8)[0]
    instr = (instr & ~(0x1f << 16)) | (2 << 16)  # rt: t0(8) -> v0(2)
    struct.pack_into('<I', data, base + 0xe8, instr)
    print(f"  0xe8: xori t0,v0 -> xori v0,v0")

    # 0xf0: beqz t0,... [11000003] -> beqz v0,... [10400003]
    instr = struct.unpack_from('<I', data, base + 0xf0)[0]
    instr = (instr & ~(0x1f << 21)) | (2 << 21)  # rs: t0(8) -> v0(2)
    struct.pack_into('<I', data, base + 0xf0, instr)
    print(f"  0xf0: beqz t0 -> beqz v0")

    # 5. t2 -> t1 for val+1: offsets 0xec, 0xf4
    # 0xec: addiu t2,v1,1 [246a0001] -> addiu t1,v1,1 [24690001]
    instr = struct.unpack_from('<I', data, base + 0xec)[0]
    instr = (instr & ~(0x1f << 16)) | (9 << 16)  # rt: t2(10) -> t1(9)
    struct.pack_into('<I', data, base + 0xec, instr)
    print(f"  0xec: addiu t2,v1 -> addiu t1,v1")

    # 0xf4: sh t2,0(at) [a42a0000] -> sh t1,0(at) [a4290000+reloc]
    instr = struct.unpack_from('<I', data, base + 0xf4)[0]
    instr = (instr & ~(0x1f << 16)) | (9 << 16)  # rt: t2(10) -> t1(9)
    struct.pack_into('<I', data, base + 0xf4, instr)
    print(f"  0xf4: sh t2 -> sh t1")

    # 6. t3 -> t2 for step_counter: offsets 0x100, 0x104, 0x10c
    # 0x100: lui t3,... -> lui t2,...
    instr = struct.unpack_from('<I', data, base + 0x100)[0]
    instr = (instr & ~(0x1f << 16)) | (10 << 16)  # rt: t3(11) -> t2(10)
    struct.pack_into('<I', data, base + 0x100, instr)
    print(f"  0x100: lui t3 -> lui t2")

    # 0x104: lw t3,0(t3) -> lw t2,0(t2)
    instr = struct.unpack_from('<I', data, base + 0x104)[0]
    instr = (instr & ~(0x1f << 21)) | (10 << 21)  # rs: t3(11) -> t2(10)
    instr = (instr & ~(0x1f << 16)) | (10 << 16)  # rt: t3(11) -> t2(10)
    struct.pack_into('<I', data, base + 0x104, instr)
    print(f"  0x104: lw t3,(t3) -> lw t2,(t2)")

    # 0x10c: sltiu at,t3,601 -> sltiu at,t2,601
    instr = struct.unpack_from('<I', data, base + 0x10c)[0]
    instr = (instr & ~(0x1f << 21)) | (10 << 21)  # rs: t3(11) -> t2(10)
    struct.pack_into('<I', data, base + 0x10c, instr)
    print(f"  0x10c: sltiu at,t3 -> sltiu at,t2")

    with open(output_path, 'wb') as f:
        f.write(data)

    print(f"  Custom patches applied successfully")

if __name__ == '__main__':
    main()
