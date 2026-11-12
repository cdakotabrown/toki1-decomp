#!/usr/bin/env python3
"""Custom post-processing for func_80085C3C.

Cross-prologue function with hidden t9 parameter. The thunk pre-loads
at=0x800D0000 and t9=value for ss_s_kind. The compiled C produces:

  0: lui  v0,hi(sss_sound)    [cross-prologue]
  1: lw   v0,lo(sss_sound)(v0)[cross-prologue]
  2: lui  at,hi(ss_s_kind)    [REDUNDANT - thunk provides at]
  3: addiu sp,sp,-40
  4: sw   v0,lo(ss_s_kind)(at)[store hidden param - should use t9]
  5: sw   ra,20(sp)
  6: lui  at,hi(sss_so)
  7-14: call + epilogue

This script:
  1. Removes instruction 2 (redundant lui at - thunk already has at loaded)
  2. Compacts code and updates relocations
  3. Remaps v0->t0 in cross-prologue load and second store (insns 0,1,10)
  4. Remaps v0->t9 in first store (insn 3) - hidden parameter from thunk
"""
import struct
import sys

V0 = 2
T0 = 8
T9 = 25


def find_elf_sections(data):
    e_shoff = struct.unpack('<I', data[0x20:0x24])[0]
    e_shentsize = struct.unpack('<H', data[0x2e:0x30])[0]
    e_shnum = struct.unpack('<H', data[0x30:0x32])[0]
    e_shstrndx = struct.unpack('<H', data[0x32:0x34])[0]
    shstr_sh = e_shoff + e_shstrndx * e_shentsize
    shstr_off = struct.unpack('<I', data[shstr_sh + 16:shstr_sh + 20])[0]
    sections = {}
    for i in range(e_shnum):
        sh = e_shoff + i * e_shentsize
        name_idx = struct.unpack('<I', data[sh:sh + 4])[0]
        name_end = data.index(b'\x00', shstr_off + name_idx)
        name = data[shstr_off + name_idx:name_end].decode()
        sh_offset = struct.unpack('<I', data[sh + 16:sh + 20])[0]
        sh_size = struct.unpack('<I', data[sh + 20:sh + 24])[0]
        sections[name] = {'idx': i, 'sh_off': sh,
                          'offset': sh_offset, 'size': sh_size}
    return sections


def remap_reg_in_insn(word, old_reg, new_reg):
    """Remap all occurrences of old_reg to new_reg in rs, rt, rd fields."""
    for shift in [21, 16, 11]:
        if ((word >> shift) & 0x1F) == old_reg:
            word = (word & ~(0x1F << shift)) | (new_reg << shift)
    return word


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} input.o output.o")
        sys.exit(1)

    with open(sys.argv[1], 'rb') as f:
        data = bytearray(f.read())

    sections = find_elf_sections(bytes(data))
    text_sh = sections['.text']
    text_off = text_sh['offset']
    text_size = text_sh['size']
    n_insns = text_size // 4

    print(f"Input: {n_insns} instructions ({text_size} bytes)")
    assert n_insns == 16, f"Expected 16 instructions, got {n_insns}"

    # Read all instructions
    insns = []
    for i in range(n_insns):
        insns.append(struct.unpack('<I', data[text_off + i*4:text_off + i*4 + 4])[0])

    # Verify expected structure
    assert (insns[0] >> 26) == 0x0F, "insn 0 should be lui"
    assert (insns[2] >> 26) == 0x0F, "insn 2 should be lui at (to remove)"
    assert insns[3] == 0x27bdffd8, "insn 3 should be addiu sp,sp,-40"

    # Step 1: Remove instruction 2 (first lui at for ss_s_kind)
    removed_idx = 2
    new_insns = insns[:removed_idx] + insns[removed_idx + 1:]
    print(f"Removed instruction {removed_idx} (lui at for ss_s_kind)")

    # After removal, new indices:
    # 0: lui v0       -> remap v0->t0
    # 1: lw v0        -> remap v0->t0
    # 2: addiu sp,-40
    # 3: sw v0,(at)   -> remap v0->t9 (hidden param)
    # 4: sw ra,20(sp)
    # 5: lui at       (for sss_so)
    # 6-8: li a0,a1,a2
    # 9: jal
    # 10: sw v0,(at)  -> remap v0->t0
    # 11-14: epilogue

    # Step 2: Register remapping
    # v0 -> t0 in instructions 0, 1, 10 (cross-prologue load + second store)
    for idx in [0, 1, 10]:
        new_insns[idx] = remap_reg_in_insn(new_insns[idx], V0, T0)
        print(f"  insn {idx}: v0 -> t0")

    # v0 -> t9 in instruction 3 (first store - hidden param from thunk)
    new_insns[3] = remap_reg_in_insn(new_insns[3], V0, T9)
    print(f"  insn 3: v0 -> t9 (hidden param)")

    # Step 3: Write new instructions to .text
    new_text_size = len(new_insns) * 4
    for i, w in enumerate(new_insns):
        struct.pack_into('<I', data, text_off + i * 4, w)

    # Zero out the remaining bytes
    for i in range(new_text_size, text_size):
        data[text_off + i] = 0

    # Update .text section size
    struct.pack_into('<I', data, text_sh['sh_off'] + 20, new_text_size)

    # Step 4: Update relocations - remove reloc for removed instruction,
    # adjust offsets for subsequent instructions
    rel_sh = sections.get('.rel.text')
    if rel_sh:
        rel_off = rel_sh['offset']
        rel_size = rel_sh['size']
        n_relocs = rel_size // 8
        new_relocs = []
        for i in range(n_relocs):
            r_offset = struct.unpack('<I', data[rel_off + i*8:rel_off + i*8 + 4])[0]
            r_info = struct.unpack('<I', data[rel_off + i*8 + 4:rel_off + i*8 + 8])[0]
            insn_idx = r_offset // 4
            if insn_idx == removed_idx:
                print(f"  Removed relocation for instruction {insn_idx}")
                continue
            if insn_idx > removed_idx:
                r_offset -= 4
            new_relocs.append((r_offset, r_info))

        # Write updated relocations
        for i, (r_off, r_info) in enumerate(new_relocs):
            struct.pack_into('<I', data, rel_off + i * 8, r_off)
            struct.pack_into('<I', data, rel_off + i * 8 + 4, r_info)

        # Zero out remaining
        for i in range(len(new_relocs) * 8, rel_size):
            data[rel_off + i] = 0

        # Update rel section size
        new_rel_size = len(new_relocs) * 8
        struct.pack_into('<I', data, rel_sh['sh_off'] + 20, new_rel_size)
        print(f"  Relocations: {n_relocs} -> {len(new_relocs)}")

    # Step 5: Update symbol sizes
    symtab_sh = sections.get('.symtab')
    if symtab_sh:
        sym_off = symtab_sh['offset']
        num_syms = symtab_sh['size'] // 16
        for i in range(num_syms):
            s = sym_off + i * 16
            st_size = struct.unpack('<I', data[s + 8:s + 12])[0]
            st_shndx = struct.unpack('<H', data[s + 14:s + 16])[0]
            if st_shndx == text_sh['idx'] and st_size > 0:
                struct.pack_into('<I', data, s + 8, new_text_size)

    print(f"Output: {len(new_insns)} instructions ({new_text_size} bytes)")

    with open(sys.argv[2], 'wb') as f:
        f.write(data)

    print("Done: func_80085C3C custom post-processing complete")


if __name__ == '__main__':
    main()
