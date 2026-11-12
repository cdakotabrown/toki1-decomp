#!/usr/bin/env python3
"""Custom post-processing for func_80074710.

Same function body as func_80074714, but entry is 4 bytes earlier (1 cross-prologue
instruction stripped instead of 2). All patch offsets are +4 from func_80074714's.

Fixes remaining differences after strip_leading_instructions + patch_stack_complete:
1. Scheduling: swap nop and lui at offsets 0x1c/0x24
2. Register: v0 -> v1 for cal_cw_anim load at offsets 0x24,0x28,0x30
3. Stack slot: 28(sp) -> 40(sp) at offsets 0x8c,0x98
4. Register: xori dest t0 -> v0 at offset 0xec, beqz t0 -> v0 at 0xf4
5. Register: t2 -> t1 for val+1 at offsets 0xf0,0xf8
6. Register: t3 -> t2 for step_counter at offsets 0x104,0x108,0x110
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
    e_shoff = struct.unpack_from('<I', data, 0x20)[0]
    e_shentsize = struct.unpack_from('<H', data, 0x2e)[0]
    e_shnum = struct.unpack_from('<H', data, 0x30)[0]
    e_shstrndx = struct.unpack_from('<H', data, 0x32)[0]

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

    base = text_offset

    # Find .rel.text section for relocation fixups
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

    # 1. Swap instructions at 0x1c and 0x24 (nop <-> lui)
    # Compiled: 0x1c = lui v0,... 0x24 = nop
    # Target:   0x1c = nop,       0x24 = lui v1,...
    instr_1c = struct.unpack_from('<I', data, base + 0x1c)[0]
    instr_24 = struct.unpack_from('<I', data, base + 0x24)[0]
    struct.pack_into('<I', data, base + 0x1c, instr_24)
    struct.pack_into('<I', data, base + 0x24, instr_1c)
    print(f"  Swapped 0x1c ({instr_1c:08x}) <-> 0x24 ({instr_24:08x})")

    # Fix relocations for the swapped instructions
    if rel_text_offset is not None:
        n_relocs = rel_text_size // 8
        for i in range(n_relocs):
            r_off = struct.unpack_from('<I', data, rel_text_offset + i * 8)[0]
            if r_off == 0x1c:
                struct.pack_into('<I', data, rel_text_offset + i * 8, 0x24)
                print(f"  Moved reloc 0x1c -> 0x24")
            elif r_off == 0x24:
                struct.pack_into('<I', data, rel_text_offset + i * 8, 0x1c)
                print(f"  Moved reloc 0x24 -> 0x1c")

    # 2. Fix register v0 -> v1 at offsets 0x24 (now has lui), 0x28, 0x30
    # 0x24: lui v0,... -> lui v1,...
    instr = struct.unpack_from('<I', data, base + 0x24)[0]
    instr = (instr & ~(0x1f << 16)) | (3 << 16)  # v1 = 3
    struct.pack_into('<I', data, base + 0x24, instr)
    print(f"  0x24: lui v0 -> lui v1")

    # 0x28: lh v0,0(v0) -> lh v1,0(v1)
    instr = struct.unpack_from('<I', data, base + 0x28)[0]
    instr = (instr & ~(0x1f << 21)) | (3 << 21)  # rs: v0->v1
    instr = (instr & ~(0x1f << 16)) | (3 << 16)  # rt: v0->v1
    struct.pack_into('<I', data, base + 0x28, instr)
    print(f"  0x28: lh v0,(v0) -> lh v1,(v1)")

    # 0x30: andi t8,v0,0x1 -> andi t8,v1,0x1
    instr = struct.unpack_from('<I', data, base + 0x30)[0]
    instr = (instr & ~(0x1f << 21)) | (3 << 21)  # rs: v0->v1
    struct.pack_into('<I', data, base + 0x30, instr)
    print(f"  0x30: andi t8,v0 -> andi t8,v1")

    # 3. Stack slot: sw a0,28(sp) -> sw a0,40(sp) and lw t2,28(sp) -> lw t2,40(sp)
    # 0x8c: sw a0,28(sp) -> sw a0,40(sp)
    instr = struct.unpack_from('<I', data, base + 0x8c)[0]
    instr = (instr & 0xffff0000) | 0x0028
    struct.pack_into('<I', data, base + 0x8c, instr)
    print(f"  0x8c: sw a0,28(sp) -> sw a0,40(sp)")

    # 0x98: lw t2,28(sp) -> lw t2,40(sp)
    instr = struct.unpack_from('<I', data, base + 0x98)[0]
    instr = (instr & 0xffff0000) | 0x0028
    struct.pack_into('<I', data, base + 0x98, instr)
    print(f"  0x98: lw t2,28(sp) -> lw t2,40(sp)")

    # 4. xori dest: t0 -> v0 at 0xec, beqz t0 -> beqz v0 at 0xf4
    # 0xec: xori t0,v0,0x1 -> xori v0,v0,0x1
    instr = struct.unpack_from('<I', data, base + 0xec)[0]
    instr = (instr & ~(0x1f << 16)) | (2 << 16)  # rt: t0(8) -> v0(2)
    struct.pack_into('<I', data, base + 0xec, instr)
    print(f"  0xec: xori t0,v0 -> xori v0,v0")

    # 0xf4: beqz t0,... -> beqz v0,...
    instr = struct.unpack_from('<I', data, base + 0xf4)[0]
    instr = (instr & ~(0x1f << 21)) | (2 << 21)  # rs: t0(8) -> v0(2)
    struct.pack_into('<I', data, base + 0xf4, instr)
    print(f"  0xf4: beqz t0 -> beqz v0")

    # 5. t2 -> t1 for val+1: offsets 0xf0, 0xf8
    # 0xf0: addiu t2,v1,1 -> addiu t1,v1,1
    instr = struct.unpack_from('<I', data, base + 0xf0)[0]
    instr = (instr & ~(0x1f << 16)) | (9 << 16)  # rt: t2(10) -> t1(9)
    struct.pack_into('<I', data, base + 0xf0, instr)
    print(f"  0xf0: addiu t2,v1 -> addiu t1,v1")

    # 0xf8: sh t2,0(at) -> sh t1,0(at)
    instr = struct.unpack_from('<I', data, base + 0xf8)[0]
    instr = (instr & ~(0x1f << 16)) | (9 << 16)  # rt: t2(10) -> t1(9)
    struct.pack_into('<I', data, base + 0xf8, instr)
    print(f"  0xf8: sh t2 -> sh t1")

    # 6. t3 -> t2 for step_counter: offsets 0x104, 0x108, 0x110
    # 0x104: lui t3,... -> lui t2,...
    instr = struct.unpack_from('<I', data, base + 0x104)[0]
    instr = (instr & ~(0x1f << 16)) | (10 << 16)  # rt: t3(11) -> t2(10)
    struct.pack_into('<I', data, base + 0x104, instr)
    print(f"  0x104: lui t3 -> lui t2")

    # 0x108: lw t3,0(t3) -> lw t2,0(t2)
    instr = struct.unpack_from('<I', data, base + 0x108)[0]
    instr = (instr & ~(0x1f << 21)) | (10 << 21)  # rs: t3(11) -> t2(10)
    instr = (instr & ~(0x1f << 16)) | (10 << 16)  # rt: t3(11) -> t2(10)
    struct.pack_into('<I', data, base + 0x108, instr)
    print(f"  0x108: lw t3,(t3) -> lw t2,(t2)")

    # 0x110: sltiu at,t3,601 -> sltiu at,t2,601
    instr = struct.unpack_from('<I', data, base + 0x110)[0]
    instr = (instr & ~(0x1f << 21)) | (10 << 21)  # rs: t3(11) -> t2(10)
    struct.pack_into('<I', data, base + 0x110, instr)
    print(f"  0x110: sltiu at,t3 -> sltiu at,t2")

    with open(output_path, 'wb') as f:
        f.write(data)

    print(f"  Custom patches applied successfully")

if __name__ == '__main__':
    main()
