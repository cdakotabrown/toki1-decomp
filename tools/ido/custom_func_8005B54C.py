#!/usr/bin/env python3
"""Custom post-processing for func_8005B54C.

This function loads mascot_cw_disp directly (cross-prologue scheduling),
producing 2 extra instructions at the start vs voice_click_check.
All early-exit conditions branch to a single shared exit block.

Transformations applied (after stack patch):
1. Swap v0<->v1 in x-coord and y-coord load sections (0x20-0x84)
2. Reorder y-coord loads: put lui v0 (cursor_cw_y) before lui v1 (mascot_cw_y)
3. Remove 7 redundant (b exit; move v0,zero) pairs + 1 duplicate slt
4. Invert bnez->beqz for each early-exit condition (all point to shared exit)
5. Reorder success path: move lui at earlier, fix lw a0/a1 order,
   fill delay slots (sb a2 into jal1 delay, li a0 into jal2 delay),
   add b epilogue + move v0,zero structure + insert move v0,zero at exit
6. Fix all branch offsets after instruction removal
7. Update relocations for removed/shifted instructions
8. Update text section size (70 -> 56 instructions)
"""
import struct
import sys

# MIPS register numbers
REG_ZERO = 0
REG_AT = 1
REG_V0 = 2
REG_V1 = 3
REG_A0 = 4
REG_A1 = 5
REG_A2 = 6
REG_A3 = 7
REG_T0 = 8
REG_T1 = 9
REG_T2 = 10
REG_T3 = 11
REG_T4 = 12
REG_T5 = 13
REG_T6 = 14
REG_T7 = 15
REG_T8 = 24
REG_T9 = 25
REG_SP = 29
REG_RA = 31


def find_elf_sections(data):
    """Find sections in ELF file."""
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
        sections[name] = {
            'idx': i,
            'sh_off': sh,
            'offset': sh_offset,
            'size': sh_size,
        }
    return sections


def swap_v0_v1(word):
    """Swap v0 and v1 register references in all fields."""
    rs = (word >> 21) & 0x1f
    rt = (word >> 16) & 0x1f
    rd = (word >> 11) & 0x1f

    def swap(r):
        if r == REG_V0:
            return REG_V1
        elif r == REG_V1:
            return REG_V0
        return r

    word = (word & ~(0x1f << 21)) | (swap(rs) << 21)
    word = (word & ~(0x1f << 16)) | (swap(rt) << 16)
    word = (word & ~(0x1f << 11)) | (swap(rd) << 11)
    return word


def make_beqz(rs, offset_instr):
    """Build BEQZ rs, offset."""
    op = 0x04
    return (op << 26) | (rs << 21) | (0 << 16) | (offset_instr & 0xFFFF)


def make_bnez(rs, offset_instr):
    """Build BNEZ rs, offset."""
    op = 0x05
    return (op << 26) | (rs << 21) | (0 << 16) | (offset_instr & 0xFFFF)


def make_b(offset_instr):
    """Build B offset (BEQ zero,zero,offset)."""
    op = 0x04
    return (op << 26) | (0 << 21) | (0 << 16) | (offset_instr & 0xFFFF)


def make_move_zero(rd):
    """Build MOVE rd, zero (OR rd, zero, zero)."""
    return (0 << 26) | (0 << 21) | (0 << 16) | (rd << 11) | 0x25


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

    rel_sh = sections.get('.rel.text')
    rel_off = rel_sh['offset'] if rel_sh else None
    rel_size = rel_sh['size'] if rel_sh else 0

    def get_instr(offset):
        return struct.unpack('<I', data[text_off + offset:text_off + offset + 4])[0]

    def set_instr(offset, word):
        data[text_off + offset:text_off + offset + 4] = struct.pack('<I', word)

    # Read all relocations for reference
    relocs = {}  # offset -> info
    if rel_off and rel_size:
        num_relocs_orig = rel_size // 8
        for i in range(num_relocs_orig):
            r_base = rel_off + i * 8
            r_offset = struct.unpack('<I', data[r_base:r_base + 4])[0]
            r_info = struct.unpack('<I', data[r_base + 4:r_base + 8])[0]
            relocs[r_offset] = r_info

    # =========================================================================
    # STEP 1: Swap v0<->v1 in coordinate section (0x20-0x84)
    # This covers the x-coord loads (0x20-0x50) and y-coord loads (0x50-0x80)
    # =========================================================================
    for off in range(0x20, 0x88, 4):
        w = get_instr(off)
        new_w = swap_v0_v1(w)
        if new_w != w:
            set_instr(off, new_w)
    print("Step 1: Swapped v0<->v1 in coordinate section 0x20-0x84")

    # =========================================================================
    # STEP 2: Read all instructions into a mutable list for reordering
    # =========================================================================
    all_instrs = []
    for i in range(text_size // 4):
        all_instrs.append(get_instr(i * 4))

    # Also maintain reloc list parallel to instruction list
    reloc_list = [relocs.get(i * 4, None) for i in range(text_size // 4)]

    # =========================================================================
    # STEP 3: Define instruction removals
    # 7 (b exit; move v0,zero) pairs + 1 duplicate slt at 0x80
    # + 2 trailing alignment nop instructions at end of .text
    # Compiled pairs (b exit at offset, move v0,zero at offset+4):
    remove_pairs = [0x18, 0x40, 0x54, 0x78, 0x8c, 0xa8, 0xcc]
    # Also remove the duplicate slt at,v1,v0 at 0x80 (after v0<->v1 swap)
    # Also remove the 2 trailing alignment nop instructions (indices 70, 71)
    extra_remove = [0x80, 0x118, 0x11c]

    remove_set = set()
    for pair_start in remove_pairs:
        remove_set.add(pair_start // 4)
        remove_set.add((pair_start + 4) // 4)
    for off in extra_remove:
        remove_set.add(off // 4)

    # =========================================================================
    # STEP 4: Build compacted instruction + reloc lists
    # =========================================================================
    new_instrs = []
    new_relocs_list = []
    old_to_new = {}

    for i, instr in enumerate(all_instrs):
        if i not in remove_set:
            new_idx = len(new_instrs)
            old_to_new[i] = new_idx
            new_instrs.append(instr)
            r = reloc_list[i]
            new_relocs_list.append((new_idx * 4, r) if r is not None else None)

    TARGET_INSTR_COUNT_PRE = 55  # 72 - 14 - 1 - 2 (trailing nops) = 55 instructions
    assert len(new_instrs) == TARGET_INSTR_COUNT_PRE, \
        f"Expected {TARGET_INSTR_COUNT_PRE} instructions after removal, got {len(new_instrs)}"
    print(f"Step 3/4: Compacted to {len(new_instrs)} instructions (before adding move v0,zero)")

    # =========================================================================
    # STEP 5: Reorder y-coord loads in compacted list
    # Compiled (after v0<->v1 swap, before removal):
    #   0x50: lui v1, mascot_cw_y (was v0 before swap, HI reloc)  [idx 20]
    #   0x5c: lh  v1, mascot_cw_y (LO reloc)                      [idx 23]
    #   0x60: lui v0, cursor_cw_y (was v1 before swap, HI reloc)  [idx 24]
    #   0x64: lh  v0, cursor_cw_y (LO reloc)                      [idx 25]
    # Note: 0x54-0x58 was pair 3 (removed), 0x4c-0x50 has delay slot at 0x50
    #
    # Wait: compiled 0x4c is bnez (delay slot at 0x50 = lui v1,mascot_cw_y)
    # 0x54-0x58 = pair 3 (b exit; move v0,zero)
    # 0x5c = lh v1, mascot_cw_y
    # 0x60 = lui v0, cursor_cw_y
    # 0x64 = lh v0, cursor_cw_y
    #
    # Target y-coord section:
    #   lui v0, cursor_cw_y (FIRST - v0 comes before v1)
    #   lui v1, mascot_cw_y
    #   lh  v1, mascot_cw_y
    #   lh  v0, cursor_cw_y
    #
    # After removing 0x54-0x58 (pair 3), old indices 20,23,24,25 map to consecutive new.
    # old 0x50=20, 0x5c=23, 0x60=24, 0x64=25 (old 0x54=21, 0x58=22 removed)
    # =========================================================================
    i20 = old_to_new[0x50 // 4]  # lui v1, mascot_cw_y (after swap)
    i23 = old_to_new[0x5c // 4]  # lh  v1, mascot_cw_y (after swap)
    i24 = old_to_new[0x60 // 4]  # lui v0, cursor_cw_y (after swap)
    i25 = old_to_new[0x64 // 4]  # lh  v0, cursor_cw_y (after swap)

    assert i23 == i20 + 1 and i24 == i20 + 2 and i25 == i20 + 3, \
        f"Y-coord indices not consecutive: {i20},{i23},{i24},{i25}"

    orig_i20 = (new_instrs[i20], new_relocs_list[i20])
    orig_i23 = (new_instrs[i23], new_relocs_list[i23])
    orig_i24 = (new_instrs[i24], new_relocs_list[i24])
    orig_i25 = (new_instrs[i25], new_relocs_list[i25])

    # Desired order: lui v0 (was i24), lui v1 (was i20), lh v1 (was i23), lh v0 (was i25)
    new_instrs[i20] = orig_i24[0]
    new_relocs_list[i20] = (i20 * 4, orig_i24[1][1]) if orig_i24[1] else None

    new_instrs[i20 + 1] = orig_i20[0]
    new_relocs_list[i20 + 1] = ((i20 + 1) * 4, orig_i20[1][1]) if orig_i20[1] else None

    new_instrs[i20 + 2] = orig_i23[0]
    new_relocs_list[i20 + 2] = ((i20 + 2) * 4, orig_i23[1][1]) if orig_i23[1] else None

    new_instrs[i20 + 3] = orig_i25[0]
    new_relocs_list[i20 + 3] = ((i20 + 3) * 4, orig_i25[1][1]) if orig_i25[1] else None

    print(f"Step 5: Reordered y-coord loads at new indices {i20}-{i20+3}")

    # =========================================================================
    # STEP 6: Reorder success path instructions in compacted list
    # Compiled success path (old offsets 0xd4-0x114):
    #   0xd4: lbu a2, mascot_cw_frame   [reloc LO]
    #   0xd8: lui at, now_girl   [reloc HI]
    #   0xdc: addiu t8, t8, D_800B9CC0_lo  [reloc LO]
    #   0xe0: sll t7, a2, 3
    #   0xe4: addu v0, t7, t8
    #   0xe8: sb a2, D_800EF6BF_lo(at)  [reloc LO]
    #   0xec: lw a1, 4(v0)
    #   0xf0: lw a0, 0(v0)
    #   0xf4: jal set_XAsector   [reloc R26]
    #   0xf8: nop
    #   0xfc: jal device_cmd_send   [reloc R26]
    # 0x100: li a0, 768
    # 0x104: li v0, 1
    # 0x108: lw ra, 20(sp)   [epilogue start]
    #
    # Target success path:
    #   lbu a2, mascot_cw_frame
    #   addiu t8, t8, D_800B9CC0_lo
    #   sll t7, a2, 3
    #   addu v0, t7, t8
    #   lw a0, 0(v0)    <- a0 first!
    #   lw a1, 4(v0)
    #   lui at, now_girl  <- moved later
    #   jal set_XAsector
    #   sb a2 (delay slot)
    #   jal device_cmd_send
    #   li a0, 768 (delay slot)
    #   b epilogue
    #   li v0, 1 (delay slot of b)
    #   move v0, zero  <- NEW instruction (failure exit)
    #   lw ra, 20(sp)   [epilogue]
    # =========================================================================
    i_lbu = old_to_new[0xd4 // 4]
    i_luiat = old_to_new[0xd8 // 4]
    i_addiu_t8 = old_to_new[0xdc // 4]
    i_sll = old_to_new[0xe0 // 4]
    i_addu = old_to_new[0xe4 // 4]
    i_sb = old_to_new[0xe8 // 4]
    i_lwa1 = old_to_new[0xec // 4]
    i_lwa0 = old_to_new[0xf0 // 4]
    i_jal1 = old_to_new[0xf4 // 4]
    i_nop = old_to_new[0xf8 // 4]
    i_jal2 = old_to_new[0xfc // 4]
    i_lia0 = old_to_new[0x100 // 4]
    i_liv0 = old_to_new[0x104 // 4]
    i_epilogue = old_to_new[0x108 // 4]

    print(f"  Success path old indices: lbu={i_lbu}, luiat={i_luiat}, addiu_t8={i_addiu_t8}")

    def get_ir(idx):
        return new_instrs[idx], new_relocs_list[idx]

    ir_lbu = get_ir(i_lbu)
    ir_luiat = get_ir(i_luiat)
    ir_addiu_t8 = get_ir(i_addiu_t8)
    ir_sll = get_ir(i_sll)
    ir_addu = get_ir(i_addu)
    ir_sb = get_ir(i_sb)
    ir_lwa1 = get_ir(i_lwa1)
    ir_lwa0 = get_ir(i_lwa0)
    ir_jal1 = get_ir(i_jal1)
    ir_nop = get_ir(i_nop)
    ir_jal2 = get_ir(i_jal2)
    ir_lia0 = get_ir(i_lia0)
    ir_liv0 = get_ir(i_liv0)

    def set_ir(idx, instr, reloc):
        new_instrs[idx] = instr
        new_relocs_list[idx] = reloc

    # b offset: b is at start+11, delay=start+12, target=start+14 (lw ra)
    # offset = target - (branch+1) = (start+14) - (start+12) = 2
    b_offset = 2

    new_tail = [
        (ir_lbu[0], ir_lbu[1]),           # 0: lbu a2
        (ir_addiu_t8[0], ir_addiu_t8[1]), # 1: addiu t8
        (ir_sll[0], ir_sll[1]),           # 2: sll t7
        (ir_addu[0], ir_addu[1]),         # 3: addu v0
        (ir_lwa0[0], ir_lwa0[1]),         # 4: lw a0,0(v0)
        (ir_lwa1[0], ir_lwa1[1]),         # 5: lw a1,4(v0)
        (ir_luiat[0], ir_luiat[1]),       # 6: lui at
        (ir_jal1[0], ir_jal1[1]),         # 7: jal set_XAsector
        (ir_sb[0], ir_sb[1]),             # 8: sb a2 [delay of jal1]
        (ir_jal2[0], ir_jal2[1]),         # 9: jal device_cmd_send
        (ir_lia0[0], ir_lia0[1]),         # 10: li a0,768 [delay of jal2]
        (make_b(b_offset), None),         # 11: b epilogue
        (ir_liv0[0], ir_liv0[1]),         # 12: li v0,1 [delay of b]
        (make_move_zero(REG_V0), None),   # 13: move v0,zero [failure exit - NEW]
    ]

    epi_instrs = [(new_instrs[i_epilogue + j], new_relocs_list[i_epilogue + j])
                  for j in range(4)]
    new_tail.extend(epi_instrs)

    assert len(new_tail) == 18, f"Expected 18 tail instructions, got {len(new_tail)}"

    start = i_lbu
    new_instrs = new_instrs[:start] + [t[0] for t in new_tail]
    new_relocs_list = new_relocs_list[:start] + [t[1] for t in new_tail]

    for j in range(len(new_tail)):
        new_idx = start + j
        r = new_relocs_list[new_idx]
        if r is not None:
            if isinstance(r, tuple):
                new_relocs_list[new_idx] = (new_idx * 4, r[1])
            else:
                new_relocs_list[new_idx] = (new_idx * 4, r)

    TARGET_INSTR_COUNT = 56
    assert len(new_instrs) == TARGET_INSTR_COUNT, \
        f"Expected {TARGET_INSTR_COUNT} instructions, got {len(new_instrs)}"
    print(f"Step 6: Reordered success path, added move v0,zero. Now {len(new_instrs)} instructions")

    # =========================================================================
    # STEP 7: Fix inverted branch instructions
    # The failure exit (move v0,zero) is at index start+13.
    # =========================================================================
    exit_new_idx = start + 13
    print(f"  Exit (move v0,zero) at new index {exit_new_idx} (offset 0x{exit_new_idx*4:02x})")

    # Branches to invert: compiled offsets 0x10, 0x38, 0x4c, 0x70, 0x84, 0xa0, 0xc4
    invert_old_offsets = [0x10, 0x38, 0x4c, 0x70, 0x84, 0xa0, 0xc4]

    for inv_off in invert_old_offsets:
        old_idx = inv_off // 4
        new_idx = old_to_new[old_idx]
        w = new_instrs[new_idx]

        branch_offset = exit_new_idx - (new_idx + 1)

        op = (w >> 26) & 0x3f
        rs = (w >> 21) & 0x1f

        if op == 0x05:   # BNEZ -> BEQZ
            new_op = 0x04
        elif op == 0x04:  # BEQZ -> BNEZ
            new_op = 0x05
        else:
            print(f"  WARNING: Unexpected opcode 0x{op:02x} at compiled 0x{inv_off:02x}")
            continue

        new_w = (new_op << 26) | (rs << 21) | (0 << 16) | (branch_offset & 0xFFFF)
        new_instrs[new_idx] = new_w
        print(f"  Branch at compiled 0x{inv_off:02x} -> new idx {new_idx}: "
              f"op=0x{new_op:02x} offset={branch_offset}")

    print("Step 7: Inverted branches and fixed offsets")

    # =========================================================================
    # STEP 8: Write compacted instructions back to .text section
    # =========================================================================
    new_text_size = TARGET_INSTR_COUNT * 4  # 56 * 4 = 224 bytes
    for i, instr in enumerate(new_instrs):
        abs_off = text_off + i * 4
        data[abs_off:abs_off + 4] = struct.pack('<I', instr)

    text_sh_off = text_sh['sh_off']
    struct.pack_into('<I', data, text_sh_off + 20, new_text_size)
    print(f"Step 8: Wrote {TARGET_INSTR_COUNT} instructions ({new_text_size} bytes)")

    # =========================================================================
    # STEP 9: Write updated relocations
    # =========================================================================
    if rel_off and rel_size:
        final_relocs = []
        for entry in new_relocs_list:
            if entry is not None:
                if isinstance(entry, tuple):
                    final_relocs.append(entry)

        for i, (r_offset, r_info) in enumerate(final_relocs):
            r_base = rel_off + i * 8
            struct.pack_into('<I', data, r_base, r_offset)
            struct.pack_into('<I', data, r_base + 4, r_info)

        new_rel_size = len(final_relocs) * 8
        rel_sh_off = rel_sh['sh_off']
        struct.pack_into('<I', data, rel_sh_off + 20, new_rel_size)
        print(f"Step 9: Updated {len(final_relocs)} relocations")

    # =========================================================================
    # STEP 10: Update symbol table
    # =========================================================================
    symtab_sh = sections.get('.symtab')
    if symtab_sh:
        sym_off = symtab_sh['offset']
        sym_size_bytes = symtab_sh['size']
        text_section_idx = text_sh['idx']
        num_syms = sym_size_bytes // 16

        for i in range(num_syms):
            s = sym_off + i * 16
            st_size = struct.unpack('<I', data[s + 8:s + 12])[0]
            st_shndx = struct.unpack('<H', data[s + 14:s + 16])[0]

            if st_shndx == text_section_idx and st_size == text_size:
                struct.pack_into('<I', data, s + 8, new_text_size)
                print(f"Step 10: Updated symbol {i} size 0x{text_size:x} -> 0x{new_text_size:x}")

    with open(sys.argv[2], 'wb') as f:
        f.write(data)

    print("Done.")


if __name__ == '__main__':
    main()
