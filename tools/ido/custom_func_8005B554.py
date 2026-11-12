#!/usr/bin/env python3
"""Custom post-processing for voice_click_check.

This function uses t6 as a hidden input parameter (non-standard calling convention)
and has a unique control flow pattern where all early-exit conditions branch to a
single shared exit block.

Transformations applied (after stack patch):
1. Remap a0 -> t6 in instruction at offset 0x04 (hidden parameter)
2. Swap v0<->v1 in x-coord and y-coord load sections (0x18-0x7c)
3. Reorder y-coord loads: put lui v0 (cursor_cw_y) before lui v1 (mascot_cw_y)
4. Remove 7 redundant (b exit; move v0,zero) pairs + 1 duplicate slt
5. Invert bnez->beqz for each early-exit condition (all point to shared exit)
6. Reorder success path: move lui at earlier, fix lw a0/a1 order,
   fill delay slots (sb a2 into jal1 delay, li a0 into jal2 delay),
   add b epilogue + move v0,zero structure
7. Fix all branch offsets after instruction removal
8. Update relocations for removed/shifted instructions
9. Update text section size (68 -> 54 instructions)
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


def patch_reg(word, field, old_reg, new_reg):
    """Patch a register field in a MIPS instruction word."""
    shifts = {'rs': 21, 'rt': 16, 'rd': 11}
    shift = shifts[field]
    curr = (word >> shift) & 0x1f
    if curr == old_reg:
        word = (word & ~(0x1f << shift)) | (new_reg << shift)
    return word


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
    relocs = {}  # offset -> (info)
    if rel_off and rel_size:
        num_relocs_orig = rel_size // 8
        for i in range(num_relocs_orig):
            r_base = rel_off + i * 8
            r_offset = struct.unpack('<I', data[r_base:r_base + 4])[0]
            r_info = struct.unpack('<I', data[r_base + 4:r_base + 8])[0]
            relocs[r_offset] = r_info

    # =========================================================================
    # STEP 1: Remap a0 -> t6 in instruction at 0x04 (hidden parameter)
    # =========================================================================
    w = get_instr(0x04)
    w = patch_reg(w, 'rs', REG_A0, REG_T6)
    set_instr(0x04, w)
    print("Step 1: Remapped a0->t6 at 0x04")

    # =========================================================================
    # STEP 2: Swap v0<->v1 in coordinate section (0x18-0x7c)
    # =========================================================================
    for off in range(0x18, 0x80, 4):
        w = get_instr(off)
        new_w = swap_v0_v1(w)
        if new_w != w:
            set_instr(off, new_w)
    print("Step 2: Swapped v0<->v1 in coordinate section 0x18-0x7c")

    # =========================================================================
    # STEP 3: Read all instructions into a mutable list for reordering
    # =========================================================================
    all_instrs = []
    for i in range(text_size // 4):
        all_instrs.append(get_instr(i * 4))

    # Also maintain reloc list parallel to instruction list
    # reloc_list[i] = info if instruction i has a reloc, else None
    reloc_list = [relocs.get(i * 4, None) for i in range(text_size // 4)]

    # =========================================================================
    # STEP 4: Reorder y-coord loads (0x48-0x5c) to match target scheduling
    # Compiled (after v0<->v1 swap):
    #   0x48: lui v1, mascot_cw_y (HI reloc sym=5)
    #   0x54: lh  v1, mascot_cw_y (LO reloc sym=5)  [was at 0x48 before step2]
    #   0x58: lui v0, cursor_cw_y (HI reloc sym=6)   [was lui v1 before step2]
    #   0x5c: lh  v0, cursor_cw_y (LO reloc sym=6)   [was lh v1 before step2]
    # Target:
    #   0x38: lui v0, cursor_cw_y (HI reloc sym=6)   <- v0 comes FIRST
    #   0x3c: lui v1, mascot_cw_y (HI reloc sym=5)
    #   0x40: lh  v1, mascot_cw_y (LO reloc sym=5)
    #   0x44: lh  v0, cursor_cw_y (LO reloc sym=6)
    #
    # In the compiled original indices: 0x48=18, 0x54=21, 0x58=22, 0x5c=23
    # Wait: after the pair removal at 0x4c-0x50, 0x54 would move up.
    # But we're working with the FULL compiled list (before removal) here.
    # So 0x48=index 18 (lui v1,mascot_cw_y), 0x54=index 21 (lh v1), 0x58=22 (lui v0), 0x5c=23 (lh v0)
    #
    # We need to reorder WITHIN the original list at these indices.
    # Original indices 18,21,22,23 should become:
    #   idx18: lui v0 (was idx22: 0x58)  -- move lui v0 to first
    #   idx21: lui v1 (was idx18: 0x48)
    #   idx22: lh  v1 (was idx21: 0x54)  -- wait, 0x54 is index 0x54/4=21
    #   idx23: lh  v0 (was idx23: 0x5c)  -- stays
    # BUT indices 19 and 20 are the removed pair (0x4c=19, 0x50=20).
    # After removal, the adjacent indices in the compacted list are 18->21->22->23.
    # The reordering should happen IN THE COMPACTED LIST.
    # Let me just do the reordering after the compaction step.
    print("Step 3: Will reorder y-coord section after compaction")

    # =========================================================================
    # STEP 5: Define instruction removals
    # =========================================================================
    # 7 (b exit; move v0,zero) pairs + 1 duplicate slt at,v1,v0
    # Compiled pairs (b exit = offset, move v0,zero = offset+4):
    remove_pairs = [0x10, 0x38, 0x4c, 0x70, 0x84, 0xa0, 0xc4]
    # Also remove the duplicate slt at,v1,v0 at 0x78
    extra_remove = [0x78]

    remove_set = set()
    for pair_start in remove_pairs:
        remove_set.add(pair_start // 4)
        remove_set.add((pair_start + 4) // 4)
    for off in extra_remove:
        remove_set.add(off // 4)

    # =========================================================================
    # STEP 6: Build compacted instruction + reloc lists
    # =========================================================================
    new_instrs = []
    new_relocs_list = []   # (new_offset, info) pairs
    old_to_new = {}        # old index -> new index

    for i, instr in enumerate(all_instrs):
        if i not in remove_set:
            new_idx = len(new_instrs)
            old_to_new[i] = new_idx
            new_instrs.append(instr)
            # Carry over reloc if this instruction had one
            r = reloc_list[i]
            new_relocs_list.append((new_idx * 4, r) if r is not None else None)

    TARGET_INSTR_COUNT = 53  # 68 - 14 - 1 = 53 instructions so far
    assert len(new_instrs) == TARGET_INSTR_COUNT, \
        f"Expected {TARGET_INSTR_COUNT} instructions after removal, got {len(new_instrs)}"
    print(f"Step 5/6: Compacted to {len(new_instrs)} instructions (before adding move v0,zero)")

    # =========================================================================
    # STEP 7: Reorder y-coord loads in compacted list
    # In compacted list, the y-coord section starts where old 0x48 ended up.
    # old 0x48 (index 18) -> new index: old_to_new[18]
    # old 0x54 (index 21) -> new index: old_to_new[21]
    # old 0x58 (index 22) -> new index: old_to_new[22]
    # old 0x5c (index 23) -> new index: old_to_new[23]
    # Old 0x4c=19, 0x50=20 were removed (pair).
    # So in compacted: old 18,21,22,23 map to consecutive new indices.
    # =========================================================================
    i18 = old_to_new[0x48 // 4]  # lui v1, mascot_cw_y
    i21 = old_to_new[0x54 // 4]  # lh  v1, mascot_cw_y
    i22 = old_to_new[0x58 // 4]  # lui v0, cursor_cw_y
    i23 = old_to_new[0x5c // 4]  # lh  v0, cursor_cw_y

    # Desired order: lui v0 (was i22), lui v1 (was i18), lh v1 (was i21), lh v0 (was i23)
    orig_i18 = (new_instrs[i18], new_relocs_list[i18])
    orig_i21 = (new_instrs[i21], new_relocs_list[i21])
    orig_i22 = (new_instrs[i22], new_relocs_list[i22])
    orig_i23 = (new_instrs[i23], new_relocs_list[i23])

    # Place in target order at positions i18, i18+1, i18+2, i18+3
    assert i21 == i18 + 1 and i22 == i18 + 2 and i23 == i18 + 3, \
        f"Y-coord indices not consecutive: {i18},{i21},{i22},{i23}"

    new_instrs[i18] = orig_i22[0]      # lui v0 (cursor_cw_y)
    new_relocs_list[i18] = (i18 * 4, orig_i22[1][1]) if orig_i22[1] else None

    new_instrs[i18 + 1] = orig_i18[0]  # lui v1 (mascot_cw_y)
    new_relocs_list[i18 + 1] = (( i18 + 1) * 4, orig_i18[1][1]) if orig_i18[1] else None

    new_instrs[i18 + 2] = orig_i21[0]  # lh  v1 (mascot_cw_y)
    new_relocs_list[i18 + 2] = ((i18 + 2) * 4, orig_i21[1][1]) if orig_i21[1] else None

    new_instrs[i18 + 3] = orig_i23[0]  # lh  v0 (cursor_cw_y)
    new_relocs_list[i18 + 3] = ((i18 + 3) * 4, orig_i23[1][1]) if orig_i23[1] else None

    print(f"Step 7: Reordered y-coord loads at new indices {i18}-{i18+3}")

    # =========================================================================
    # STEP 8: Reorder success path instructions in compacted list
    # The success path in compiled (old offsets 0xcc-0x104):
    #   0xcc: lbu a2, mascot_cw_frame  [reloc LO sym=10]
    #   0xd0: lui at, now_girl  [reloc HI sym=12]
    #   0xd4: addiu t8, t8, D_800B9CC0_lo  [reloc LO sym=11]
    #   0xd8: sll t7, a2, 3
    #   0xdc: addu v0, t7, t8
    #   0xe0: sb a2, D_800EF6BF_lo(at)  [reloc LO sym=12]
    #   0xe4: lw a1, 4(v0)
    #   0xe8: lw a0, 0(v0)
    #   0xec: jal set_XAsector  [reloc R26 sym=13]
    #   0xf0: nop
    #   0xf4: jal device_cmd_send  [reloc R26 sym=14]
    #   0xf8: li a0, 768
    #   0xfc: li v0, 1
    #   0x100: epilogue (lw ra, addiu sp, jr ra, nop)
    #
    # Target success path (offsets from func start):
    #   0x94: lbu a2, mascot_cw_frame
    #   0x98: addiu t8, t8, D_800B9CC0_lo
    #   0x9c: sll t7, a2, 3
    #   0xa0: addu v0, t7, t8
    #   0xa4: lw a0, 0(v0)   <- a0 first!
    #   0xa8: lw a1, 4(v0)
    #   0xac: lui at, now_girl   <- lui at moved here
    #   0xb0: jal set_XAsector
    #   0xb4: sb a2, D_800EF6BF_lo(at)  [delay slot]
    #   0xb8: jal device_cmd_send
    #   0xbc: li a0, 768   [delay slot, MOVED into jal delay]
    #   0xc0: b epilogue
    #   0xc4: li v0, 1  [delay slot of b]
    #   0xc8: move v0, zero  [failure exit, must be ADDED]
    #   0xcc: lw ra, 20(sp)  [epilogue]
    # =========================================================================

    # In compacted list, old 0xcc corresponds to old_to_new[0xcc//4]
    # After removing 7*2+1=15 instructions from 0x10,0x14,...,0xc4,0xc8 + 0x78:
    # Old 0xcc (index 51) -> new index?
    i_lbu = old_to_new[0xcc // 4]    # lbu a2
    i_luiat = old_to_new[0xd0 // 4]  # lui at (now_girl)
    i_addiu_t8 = old_to_new[0xd4 // 4]  # addiu t8
    i_sll = old_to_new[0xd8 // 4]    # sll t7
    i_addu = old_to_new[0xdc // 4]   # addu v0
    i_sb = old_to_new[0xe0 // 4]     # sb a2
    i_lwa1 = old_to_new[0xe4 // 4]   # lw a1,4(v0)
    i_lwa0 = old_to_new[0xe8 // 4]   # lw a0,0(v0)
    i_jal1 = old_to_new[0xec // 4]   # jal set_XAsector
    i_nop = old_to_new[0xf0 // 4]    # nop (jal1 delay slot)
    i_jal2 = old_to_new[0xf4 // 4]   # jal device_cmd_send
    i_lia0 = old_to_new[0xf8 // 4]   # li a0,768
    i_liv0 = old_to_new[0xfc // 4]   # li v0,1
    i_epilogue = old_to_new[0x100 // 4]  # lw ra,20(sp)

    print(f"  Success path old indices: lbu={i_lbu}, luiat={i_luiat}, addiu_t8={i_addiu_t8}")
    print(f"    sll={i_sll}, addu={i_addu}, sb={i_sb}, lwa1={i_lwa1}, lwa0={i_lwa0}")
    print(f"    jal1={i_jal1}, nop={i_nop}, jal2={i_jal2}, lia0={i_lia0}, liv0={i_liv0}")

    # Save the instruction/reloc data
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

    # Target layout starting at i_lbu:
    # i_lbu+0: lbu a2           (same)
    # i_lbu+1: addiu t8         (was i_addiu_t8)
    # i_lbu+2: sll t7           (was i_sll)
    # i_lbu+3: addu v0          (was i_addu)
    # i_lbu+4: lw a0, 0(v0)    (was i_lwa0 -- SWAPPED)
    # i_lbu+5: lw a1, 4(v0)    (was i_lwa1 -- SWAPPED)
    # i_lbu+6: lui at           (was i_luiat -- MOVED)
    # i_lbu+7: jal1             (same)
    # i_lbu+8: sb a2 (delay)   (was i_sb -- MOVED into delay)
    # i_lbu+9: jal2             (same)
    # i_lbu+10: li a0,768 (delay) (was i_lia0 -- MOVED into delay)
    # i_lbu+11: b epilogue      (was i_liv0... no, need to add b instr)
    # Actually the slot at i_nop was 'nop', now it becomes 'sb a2' delay slot
    # and i_lia0 becomes jal2 delay slot 'li a0,768'
    # Then we need to insert 'b epilogue; li v0,1 (delay); move v0,zero'
    # But we can't easily INSERT -- we have fixed slots.
    # The instruction count: current = 53. Need = 54.
    # We need to ADD one instruction (move v0,zero).
    # Current slot usage: i_lbu to i_epilogue+3 = 13 slots.
    # New usage: 14 slots (add 'move v0,zero' before epilogue).
    # Strategy: repurpose the existing slots:
    #   i_nop -> sb a2 (delay slot of jal1)        [repurpose nop]
    #   i_lia0 -> li a0,768 (delay slot of jal2)    [already there, correct]
    #   i_liv0 -> b epilogue                        [repurpose li v0,1]
    #   i_epilogue -> li v0,1 (delay slot of b)     [put li v0,1 here]
    #   After epilogue: APPEND move v0,zero         [new instruction]
    # This gives: epilogue block = li v0,1; lw ra; addiu sp; jr ra; nop
    # But we need: li v0,1; move v0,zero; lw ra; addiu sp; jr ra; nop
    # So: i_epilogue = li v0,1, then append move v0,zero, then lw ra, addiu sp, jr ra, nop

    # Actually the target layout from the end:
    # Target offset 0xBC: b 0x8005b61c  [= b epilogue, skip move v0,zero]
    # Target offset 0xC0: li v0,1       [delay slot of b]
    # Target offset 0xC4: move v0,zero  [failure exit]
    # Target offset 0xC8: lw ra,20(sp)  [epilogue]
    # Target offset 0xCC: addiu sp,sp,40
    # Target offset 0xD0: jr ra
    # Target offset 0xD4: nop

    # So from i_liv0 slot: the 'b epilogue' needs to jump 2 instructions forward
    # (over li v0,1 delay + move v0,zero = 2 instrs, land at lw ra).
    # In the current compacted list (53 instrs), i_epilogue slots are:
    #   i_epilogue+0: lw ra,20(sp)
    #   i_epilogue+1: addiu sp,sp,40
    #   i_epilogue+2: jr ra
    #   i_epilogue+3: nop
    # And the last instruction is i_epilogue+3 = index 52.
    # We need to INSERT 'move v0,zero' at index 52 (before the current epilogue),
    # making: ...; li v0,1 (delay); move v0,zero; lw ra; addiu sp; jr ra; nop.
    # After insertion, total = 54 instructions.

    # But Python lists don't have a clean way to insert while maintaining all the
    # index mappings. Let's just rebuild the layout from i_lbu onward:

    # Current compacted list has 53 instructions.
    # We'll rebuild positions i_lbu to end (52):
    start = i_lbu
    end_orig = len(new_instrs)  # = 53

    # Build new tail (14 instructions for success path + 4 for epilogue = actually let me count)
    # The full tail from i_lbu to end should be:
    #   lbu, addiu_t8, sll, addu, lwa0, lwa1, luiat, jal1, sb(delay), jal2, li_a0(delay)
    #   b_epilogue, li_v0_1(delay), move_v0_zero, lw_ra, addiu_sp, jr_ra, nop
    # = 18 instructions

    # Current tail (from i_lbu to index 52 = 13 instructions for success path + 4 epilogue = 17)
    # 53 - i_lbu should equal 17. Let's check.
    tail_count = end_orig - start
    print(f"  Current tail from i_lbu({start}) to end: {tail_count} instructions")
    # Expected: 53 - i_lbu = tail. We need 18 instructions.
    # So we add 1 instruction (move v0,zero).

    # New tail to write (18 instructions):
    def make_reloc(new_off, old_reloc_tuple):
        """Create reloc tuple for new offset."""
        if old_reloc_tuple is None:
            return None
        r_info = old_reloc_tuple[1] if isinstance(old_reloc_tuple, tuple) else old_reloc_tuple
        return (new_off, r_info)

    # Compute epilogue b offset: b instruction is at position start+11 in new layout
    # b target = start+13 (lw ra); offset = target - (branch_pos + 1) = (start+13) - (start+12) = 1
    # Wait: b is at new_instrs[start+11], delay slot = start+12 (li v0,1)
    # We branch to start+13 (move v0,zero? No -- we branch to lw ra!)
    # b 0x8005b61c = b to lw ra. lw ra is at position start+13 in new tail.
    # b at position start+11: delay slot=start+12, target=start+14 (lw ra).
    # offset = target - (branch+1) = (start+14) - (start+12) = 2
    b_offset = 2  # jump over move v0,zero to lw ra

    new_tail = [
        # (instr, reloc_info or None)
        (ir_lbu[0], ir_lbu[1]),                  # 0: lbu a2
        (ir_addiu_t8[0], ir_addiu_t8[1]),         # 1: addiu t8
        (ir_sll[0], ir_sll[1]),                   # 2: sll t7
        (ir_addu[0], ir_addu[1]),                 # 3: addu v0
        (ir_lwa0[0], ir_lwa0[1]),                 # 4: lw a0,0(v0) [swapped from lwa1]
        (ir_lwa1[0], ir_lwa1[1]),                 # 5: lw a1,4(v0) [swapped from lwa0]
        (ir_luiat[0], ir_luiat[1]),               # 6: lui at
        (ir_jal1[0], ir_jal1[1]),                 # 7: jal set_XAsector
        (ir_sb[0], ir_sb[1]),                     # 8: sb a2 [delay slot of jal1]
        (ir_jal2[0], ir_jal2[1]),                 # 9: jal device_cmd_send
        (ir_lia0[0], ir_lia0[1]),                 # 10: li a0,768 [delay of jal2]
        (make_b(b_offset), None),                 # 11: b epilogue
        (ir_liv0[0], ir_liv0[1]),                 # 12: li v0,1 [delay of b]
        (make_move_zero(REG_V0), None),           # 13: move v0,zero [failure exit - NEW]
        # epilogue (4 instructions)
    ]

    # Epilogue from old compacted list:
    epi_instrs = [(new_instrs[i_epilogue + j], new_relocs_list[i_epilogue + j])
                  for j in range(4)]  # lw ra, addiu sp, jr ra, nop
    new_tail.extend(epi_instrs)  # total new_tail = 18

    assert len(new_tail) == 18, f"Expected 18 tail instructions, got {len(new_tail)}"

    # Now rebuild new_instrs and new_relocs_list from start onward
    new_instrs = new_instrs[:start] + [t[0] for t in new_tail]
    new_relocs_list = new_relocs_list[:start] + [t[1] for t in new_tail]

    # Update reloc offsets in the tail
    for j in range(len(new_tail)):
        new_idx = start + j
        r = new_relocs_list[new_idx]
        if r is not None:
            if isinstance(r, tuple):
                new_relocs_list[new_idx] = (new_idx * 4, r[1])
            else:
                new_relocs_list[new_idx] = (new_idx * 4, r)

    TARGET_INSTR_COUNT = 54
    assert len(new_instrs) == TARGET_INSTR_COUNT, \
        f"Expected {TARGET_INSTR_COUNT} instructions, got {len(new_instrs)}"
    print(f"Step 8: Reordered success path, added move v0,zero. Now {len(new_instrs)} instructions")

    # =========================================================================
    # STEP 9: Fix inverted branch instructions
    # The exit point (move v0,zero) is now at index 53-5=48? Let me count:
    # From start, move v0,zero is at position start+13.
    # Total = start + 18 = start + 18.
    # If start = 53 - 17 = 36 (guessing), then move_v0_zero = 36+13 = 49.
    # Actually: total = 54, tail = 18, start = 54 - 18 = 36.
    # So move_v0_zero is at index 36+13 = 49.
    # =========================================================================
    exit_new_idx = start + 13  # index of 'move v0,zero'
    print(f"  Exit (move v0,zero) at new index {exit_new_idx} (offset 0x{exit_new_idx*4:02x})")

    # Branches to invert and redirect to exit_new_idx:
    # All beqz/bnez that were converted: at compiled offsets 0x08, 0x30, 0x44, 0x68, 0x7c, 0x98, 0xbc
    # Their new positions: old_to_new[0x08//4], etc.
    invert_old_offsets = [0x08, 0x30, 0x44, 0x68, 0x7c, 0x98, 0xbc]

    for inv_off in invert_old_offsets:
        old_idx = inv_off // 4
        new_idx = old_to_new[old_idx]
        w = new_instrs[new_idx]

        # Compute new branch offset: from instruction after branch to exit
        branch_offset = exit_new_idx - (new_idx + 1)

        op = (w >> 26) & 0x3f
        rs = (w >> 21) & 0x1f

        if op == 0x05:  # BNEZ -> BEQZ
            new_op = 0x04
        elif op == 0x04:  # BEQZ -> BNEZ (for check7 which was already beqz)
            new_op = 0x05
        else:
            print(f"  WARNING: Unexpected opcode 0x{op:02x} at compiled 0x{inv_off:02x}")
            continue

        new_w = (new_op << 26) | (rs << 21) | (0 << 16) | (branch_offset & 0xFFFF)
        new_instrs[new_idx] = new_w
        print(f"  Branch at compiled 0x{inv_off:02x} -> new idx {new_idx}: "
              f"offset={branch_offset} (to exit at {exit_new_idx})")

    print("Step 9: Inverted branches and fixed offsets")

    # =========================================================================
    # STEP 10: Write compacted instructions back to .text section
    # =========================================================================
    new_text_size = TARGET_INSTR_COUNT * 4  # 54 * 4 = 216 bytes
    for i, instr in enumerate(new_instrs):
        abs_off = text_off + i * 4
        data[abs_off:abs_off + 4] = struct.pack('<I', instr)

    text_sh_off = text_sh['sh_off']
    struct.pack_into('<I', data, text_sh_off + 20, new_text_size)
    print(f"Step 10: Wrote {TARGET_INSTR_COUNT} instructions")

    # =========================================================================
    # STEP 11: Write updated relocations
    # =========================================================================
    if rel_off and rel_size:
        valid_relocs = [(off, info) for entry in new_relocs_list
                        if entry is not None
                        for off, info in [entry if isinstance(entry, tuple) else (0, entry)]]

        # Actually new_relocs_list contains (new_off, info) or None
        final_relocs = []
        for entry in new_relocs_list:
            if entry is not None:
                if isinstance(entry, tuple):
                    final_relocs.append(entry)
                else:
                    # Shouldn't happen, but handle gracefully
                    pass

        for i, (r_offset, r_info) in enumerate(final_relocs):
            r_base = rel_off + i * 8
            struct.pack_into('<I', data, r_base, r_offset)
            struct.pack_into('<I', data, r_base + 4, r_info)

        new_rel_size = len(final_relocs) * 8
        rel_sh_off = rel_sh['sh_off']
        struct.pack_into('<I', data, rel_sh_off + 20, new_rel_size)
        print(f"Step 11: Updated {len(final_relocs)} relocations")

    # =========================================================================
    # STEP 12: Update symbol table
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
                print(f"Step 12: Updated symbol {i} size 0x{text_size:x} -> 0x{new_text_size:x}")

    with open(sys.argv[2], 'wb') as f:
        f.write(data)

    print("Done.")


if __name__ == '__main__':
    main()
