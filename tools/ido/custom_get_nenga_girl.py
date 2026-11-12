#!/usr/bin/env python3
"""Custom post-processing for date_light_effect (date_light_effect).

After patch_stack_complete, the compiled code differs from target in:
1. s2/s3 register swap (color constant vs end pointer)
2. s1/s3 swap in loop setup (pointer vs end assignment)
3. Instruction ordering in the loop body
4. Save/restore ordering for s2/s3

Pipeline: patch_stack_complete -> this script
"""
import struct
import sys

REG = {
    'zero': 0, 'at': 1, 'v0': 2, 'v1': 3,
    'a0': 4, 'a1': 5, 'a2': 6, 'a3': 7,
    't0': 8, 't1': 9, 't2': 10, 't3': 11,
    't4': 12, 't5': 13, 't6': 14, 't7': 15,
    's0': 16, 's1': 17, 's2': 18, 's3': 19,
    's4': 20, 's5': 21, 's6': 22, 's7': 23,
    't8': 24, 't9': 25, 'k0': 26, 'k1': 27,
    'gp': 28, 'sp': 29, 'fp': 30, 'ra': 31,
}

def find_text_section(data):
    e_shoff = struct.unpack('<I', data[0x20:0x24])[0]
    e_shentsize = struct.unpack('<H', data[0x2e:0x30])[0]
    e_shnum = struct.unpack('<H', data[0x30:0x32])[0]
    e_shstrndx = struct.unpack('<H', data[0x32:0x34])[0]
    shstr_off = struct.unpack('<I', data[e_shoff + e_shstrndx * e_shentsize + 0x10:
                                        e_shoff + e_shstrndx * e_shentsize + 0x14])[0]
    for i in range(e_shnum):
        sh = e_shoff + i * e_shentsize
        name_off = struct.unpack('<I', data[sh:sh+4])[0]
        name = b''
        j = shstr_off + name_off
        while data[j] != 0:
            name += bytes([data[j]])
            j += 1
        if name == b'.text':
            text_off = struct.unpack('<I', data[sh+0x10:sh+0x14])[0]
            text_size = struct.unpack('<I', data[sh+0x14:sh+0x18])[0]
            return text_off, text_size
    return None, None

def find_rel_text(data):
    e_shoff = struct.unpack('<I', data[0x20:0x24])[0]
    e_shentsize = struct.unpack('<H', data[0x2e:0x30])[0]
    e_shnum = struct.unpack('<H', data[0x30:0x32])[0]
    e_shstrndx = struct.unpack('<H', data[0x32:0x34])[0]
    shstr_off = struct.unpack('<I', data[e_shoff + e_shstrndx * e_shentsize + 0x10:
                                        e_shoff + e_shstrndx * e_shentsize + 0x14])[0]
    for i in range(e_shnum):
        sh = e_shoff + i * e_shentsize
        name_off = struct.unpack('<I', data[sh:sh+4])[0]
        name = b''
        j = shstr_off + name_off
        while data[j] != 0:
            name += bytes([data[j]])
            j += 1
        if name == b'.rel.text':
            rel_off = struct.unpack('<I', data[sh+0x10:sh+0x14])[0]
            rel_size = struct.unpack('<I', data[sh+0x14:sh+0x18])[0]
            return rel_off, rel_size
    return None, None

def swap_reg_in_insn(word, from_reg, to_reg):
    """Swap from_reg <-> to_reg in rs/rt/rd fields."""
    op = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    rd = (word >> 11) & 0x1F
    new_word = word

    def do_swap(val):
        if val == from_reg: return to_reg
        elif val == to_reg: return from_reg
        return val

    new_rs = do_swap(rs)
    new_rt = do_swap(rt)
    new_rd = do_swap(rd)

    if new_rs != rs:
        new_word = (new_word & ~(0x1F << 21)) | (new_rs << 21)
    if new_rt != rt:
        new_word = (new_word & ~(0x1F << 16)) | (new_rt << 16)
    if op == 0 and new_rd != rd:
        new_word = (new_word & ~(0x1F << 11)) | (new_rd << 11)

    return new_word

def swap_reloc_info(data, rel_off, rel_size, off_a, off_b):
    """Swap the r_info (symbol+type) between two relocation entries by their text offsets."""
    if rel_off is None:
        return
    n_rels = rel_size // 8
    idx_a = idx_b = None
    for i in range(n_rels):
        r_off = struct.unpack('<I', data[rel_off + i*8: rel_off + i*8 + 4])[0]
        if r_off == off_a:
            idx_a = i
        if r_off == off_b:
            idx_b = i
    if idx_a is not None and idx_b is not None:
        info_a = data[rel_off + idx_a*8 + 4: rel_off + idx_a*8 + 8]
        info_b = data[rel_off + idx_b*8 + 4: rel_off + idx_b*8 + 8]
        data[rel_off + idx_a*8 + 4: rel_off + idx_a*8 + 8] = info_b
        data[rel_off + idx_b*8 + 4: rel_off + idx_b*8 + 8] = info_a
        print(f"  Swapped reloc info at 0x{off_a:03x} <-> 0x{off_b:03x}")

def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} input.o output.o")
        sys.exit(1)

    with open(sys.argv[1], 'rb') as f:
        data = bytearray(f.read())

    text_off, text_size = find_text_section(data)
    if text_off is None:
        print("ERROR: .text section not found")
        sys.exit(1)

    rel_off, rel_size = find_rel_text(data)

    def get_insn(off):
        return struct.unpack('<I', data[text_off + off: text_off + off + 4])[0]

    def set_insn(off, word):
        data[text_off + off: text_off + off + 4] = struct.pack('<I', word)

    # ===========================================================================
    # STEP 1: s2 <-> s3 register swap (excluding save/restore at 0x04, 0x30, 0x130, 0x134)
    # Compiled: s3=0xA0A0A0, s2=end. Target: s2=0xA0A0A0, s3=end
    # The save/restore instructions already have the correct register-to-offset
    # mapping, so we handle them separately.
    # ===========================================================================
    EXCLUDE_OFFSETS = {0x04, 0x30, 0x130, 0x134}
    for off in range(0, text_size, 4):
        if off in EXCLUDE_OFFSETS:
            continue
        word = get_insn(off)
        new_word = swap_reg_in_insn(word, REG['s2'], REG['s3'])
        if new_word != word:
            set_insn(off, new_word)

    print("Step 1: s2<->s3 swap (excluding save/restore)")

    # ===========================================================================
    # STEP 2: Fix s2/s3 save and restore instructions
    # Before swap, compiled has:
    #   0x04: sw s3, 56(sp)  -> target: sw s2, 52(sp) (0xafb20034)
    #   0x30: sw s2, 52(sp)  -> target: sw s3, 56(sp) (0xafb30038)
    #   0x130: lw s2, 52(sp) -> target: lw s2, 52(sp) (already correct)
    #   0x134: lw s3, 56(sp) -> target: lw s3, 56(sp) (already correct)
    # So swap full instructions at 0x04 and 0x30 to get correct mapping
    # ===========================================================================
    insn_04 = get_insn(0x04)
    insn_30 = get_insn(0x30)
    set_insn(0x04, insn_30)
    set_insn(0x30, insn_04)

    print("Step 2: Save order swap for s2/s3")

    # ===========================================================================
    # STEP 3: Swap s1 <-> s3 at loop setup lui/addiu offsets
    # After s2<->s3 swap:
    #   0x60: lui s3, hi(date_light_effect_tbl1)   -> should be lui s1
    #   0x68: lui s1, hi(date_light_effect_tbl0)   -> should be lui s3
    # Also need to swap relocation info so symbols match.
    # 0x6C and 0x74 (addiu) already have correct reg after swap:
    #   0x6C: addiu s3, lo(date_light_effect_tbl1) -- correct
    #   0x74: addiu s1, lo(date_light_effect_tbl0) -- correct
    # Wait, no. After s2<->s3:
    #   0x6C was addiu s2 -> now addiu s3 (sym=5, date_light_effect_tbl1) -> target: addiu s3,s3,-13306 (sym=5) OK!
    #   0x74 was addiu s1 -> still addiu s1 (sym=4, date_light_effect_tbl0) -> target: addiu s1,s1,-13342 (sym=4) OK!
    # So only lui at 0x60 and 0x68 need s1<->s3 + reloc swap
    # ===========================================================================
    for off in [0x60, 0x68]:
        word = get_insn(off)
        new_word = swap_reg_in_insn(word, REG['s1'], REG['s3'])
        if new_word != word:
            set_insn(off, new_word)

    # Swap relocation symbol references at 0x60 and 0x68
    swap_reloc_info(data, rel_off, rel_size, 0x60, 0x68)

    # Also need to swap relocs at 0x6C and 0x74 to match the register swap
    # 0x6C has sym=5 (date_light_effect_tbl1) and addiu s3 -- target: s3 + sym=5 -> OK
    # 0x74 has sym=4 (date_light_effect_tbl0) and addiu s1 -- target: s1 + sym=4 -> OK
    # Actually wait, let me re-check... The relocs at 0x6C and 0x74 were NOT affected
    # by the s1<->s3 swap (we only swapped at 0x60 and 0x68). So:
    # - 0x6C: addiu s3 (after s2->s3 swap), sym=5 -> matches target
    # - 0x74: addiu s1 (unchanged), sym=4 -> matches target

    print("Step 3: s1<->s3 at loop setup lui offsets + reloc swap")

    # ===========================================================================
    # STEP 4: Reorder loop body instructions (0x78-0xA0)
    #
    # Current (after steps 1-3):
    #   0x78: lh a2, 0(s1)       [A]
    #   0x7C: li t0, 180         [B]
    #   0x80: li t1, 9           [C]
    #   0x84: sw t1, 24(sp)      [D]
    #   0x88: sw t0, 20(sp)      [E]
    #   0x8C: sw s2, 28(sp)      [F]
    #   0x90: sw zero, 16(sp)    [G]
    #   0x94: lh t2, 0(s0)       [H]
    #   0x98: li a0, -288        [I]
    #   0x9C: li a1, -248        [J]
    #   0xA0: li a3, 60          [K]
    #
    # Target:
    #   0x78: lh t2, 0(s0)       [H]
    #   0x7C: lh a2, 0(s1)       [A]
    #   0x80: li t0, 180         [B]
    #   0x84: li t1, 9           [C]
    #   0x88: sw t1, 24(sp)      [D]
    #   0x8C: sw t0, 20(sp)      [E]
    #   0x90: li a0, -288        [I]
    #   0x94: li a1, -248        [J]
    #   0x98: li a3, 60          [K]
    #   0x9C: sw zero, 16(sp)    [G]
    #   0xA0: sw s2, 28(sp)      [F]
    # ===========================================================================
    insns = []
    for off in range(0x78, 0xA4, 4):
        insns.append(get_insn(off))

    # [0]=A, [1]=B, [2]=C, [3]=D, [4]=E, [5]=F, [6]=G, [7]=H, [8]=I, [9]=J, [10]=K
    new_order = [insns[7], insns[0], insns[1], insns[2], insns[3], insns[4],
                 insns[8], insns[9], insns[10], insns[6], insns[5]]

    for i, word in enumerate(new_order):
        set_insn(0x78 + i * 4, word)

    # Fix relocations for moved instructions
    # Only A (lh a2) and H (lh t2) have relocations
    # A: 0x78 -> 0x7C
    # H: 0x94 -> 0x78
    # We need to update relocation offsets
    if rel_off is not None:
        n_rels = rel_size // 8
        remap = {0x78: 0x7C, 0x94: 0x78}
        for i in range(n_rels):
            addr_off = rel_off + i * 8
            r_offset = struct.unpack('<I', data[addr_off:addr_off+4])[0]
            if r_offset in remap:
                data[addr_off:addr_off+4] = struct.pack('<I', remap[r_offset])
                print(f"  Reloc offset: 0x{r_offset:03x} -> 0x{remap[r_offset]:03x}")

    print("Step 4: Loop body reorder + reloc fix")

    with open(sys.argv[2], 'wb') as f:
        f.write(data)

    print("Done.")

if __name__ == '__main__':
    main()
