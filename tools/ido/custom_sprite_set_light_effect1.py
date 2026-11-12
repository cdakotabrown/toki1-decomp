#!/usr/bin/env python3
"""Custom post-processing for card_file_save (0x80055064).

Source: testD formulation (src-first copy loop, u8 *card_file_save_tbl0 pointer type).
Applied AFTER patch_stack_complete --old 80 --new 96 and remap_stack_slots --map 52:68.

Verified testD register layout (before patches):
- Insn 23,26,29: s3 used for &album_chr (target uses s2)
- Insn 49-51: t9=D_800B97B8_ptr, t8=arg0<<2 (target: t8=ptr, t9=sll result)
- Insn 52: addu t0,t8,t9 (matches target, no patch needed)
- Copy loop insn 85-89: v0/s0/s1/s2 (target: t2/t2/t1/t5)
- Copy loop insn 90-98: t1/t2/t3 as load temps; s0/s1 as addr regs (target: at; t2/t1)
- Write loop insn 99,101,105,107,108,116,117: various s-reg mismatches

Transformations:
1. Setup (insn 23,26,29): s3->s2
2. Array index (insn 49-51): t9<->t8 swap (insn 52 already matches)
3. Copy loop setup (insn 85-89): v0->t2, s0->t2, s1->t1, s2->t5
4. Copy loop body (insn 90-98): s0->t2, s1->t1; t1/t2/t3->at; bne operand order fix
5. Write loop (insn 99,101,105,107,108,116,117): s1/s2/s3 fix
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
REG_S0 = 16
REG_S1 = 17
REG_S2 = 18
REG_S3 = 19
REG_T8 = 24
REG_T9 = 25
REG_SP = 29


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
        sections[name] = {'idx': i, 'sh_off': sh, 'offset': sh_offset, 'size': sh_size}
    return sections


def patch_reg(word, field, old_reg, new_reg):
    """Patch a register field. field: 'rs' (25-21), 'rt' (20-16), 'rd' (15-11)."""
    shifts = {'rs': 21, 'rt': 16, 'rd': 11}
    shift = shifts[field]
    curr = (word >> shift) & 0x1f
    if curr == old_reg:
        word = (word & ~(0x1f << shift)) | (new_reg << shift)
    return word


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} input.o output.o")
        sys.exit(1)

    with open(sys.argv[1], 'rb') as f:
        data = bytearray(f.read())

    sections = find_elf_sections(bytes(data))
    text_off = sections['.text']['offset']

    def get_instr(offset):
        return struct.unpack('<I', data[text_off + offset:text_off + offset + 4])[0]

    def set_instr(offset, word):
        data[text_off + offset:text_off + offset + 4] = struct.pack('<I', word)

    def pr(word, field, old_reg, new_reg):
        return patch_reg(word, field, old_reg, new_reg)

    # =========================================================================
    # STEP 1: Setup - s3->s2 for &album_chr (buf pointer)
    # Insn 23 (0x5c): lui s3 -> lui s2
    # Insn 26 (0x68): addiu s3,s3,X -> addiu s2,s2,X
    # Insn 29 (0x74): move a0,s3 -> move a0,s2
    # =========================================================================
    w = get_instr(0x5c)
    w = pr(w, 'rt', REG_S3, REG_S2)
    set_instr(0x5c, w)

    w = get_instr(0x68)
    w = pr(w, 'rs', REG_S3, REG_S2)
    w = pr(w, 'rt', REG_S3, REG_S2)
    set_instr(0x68, w)

    w = get_instr(0x74)
    w = pr(w, 'rs', REG_S3, REG_S2)
    set_instr(0x74, w)

    # =========================================================================
    # STEP 2: Array index - swap t8/t9 in insn 49-51
    # testD raw: lui t9,0 (49); lw t9,0(t9) (50); sll t8,s1,2 (51); addu t0,t8,t9 (52 - OK)
    # target:    lui t8,X (49); lw t8,X(t8) (50); sll t9,s1,2 (51); addu t0,t8,t9 (52)
    # Insn 49 (0xc4): lui t9 -> lui t8 (rt: t9->t8)
    # Insn 50 (0xc8): lw t9,X(t9) -> lw t8,X(t8) (rs,rt: t9->t8)
    # Insn 51 (0xcc): sll t8,s1,2 -> sll t9,s1,2 (rd: t8->t9)
    # Insn 52 (0xd0): addu t0,t8,t9 -- already matches target
    # =========================================================================
    w = get_instr(0xc4)
    w = pr(w, 'rt', REG_T9, REG_T8)
    set_instr(0xc4, w)

    w = get_instr(0xc8)
    w = pr(w, 'rs', REG_T9, REG_T8)
    w = pr(w, 'rt', REG_T9, REG_T8)
    set_instr(0xc8, w)

    w = get_instr(0xcc)
    w = pr(w, 'rd', REG_T8, REG_T9)
    set_instr(0xcc, w)

    # =========================================================================
    # STEP 3: Copy loop setup (insn 85-89)
    # testD raw: lui v0 (85); addiu s0,v0 (86); lui s1 (87); addiu s1,s1 (88);
    #            addiu s2,s0,3840 (89)
    # target:    lui t2 (85); addiu t2,t2 (86); lui t1 (87); addiu t1,t1 (88);
    #            addiu t5,t2,3840 (89)
    # =========================================================================
    # Insn 85 (0x154): lui v0 -> lui t2
    w = get_instr(0x154)
    w = pr(w, 'rt', REG_V0, REG_T2)
    set_instr(0x154, w)

    # Insn 86 (0x158): addiu s0,v0,0 -> addiu t2,t2,0
    w = get_instr(0x158)
    w = pr(w, 'rs', REG_V0, REG_T2)
    w = pr(w, 'rt', REG_S0, REG_T2)
    set_instr(0x158, w)

    # Insn 87 (0x15c): lui s1 -> lui t1
    w = get_instr(0x15c)
    w = pr(w, 'rt', REG_S1, REG_T1)
    set_instr(0x15c, w)

    # Insn 88 (0x160): addiu s1,s1 -> addiu t1,t1
    w = get_instr(0x160)
    w = pr(w, 'rs', REG_S1, REG_T1)
    w = pr(w, 'rt', REG_S1, REG_T1)
    set_instr(0x160, w)

    # Insn 89 (0x164): addiu s2,s0,3840 -> addiu t5,t2,3840
    w = get_instr(0x164)
    w = pr(w, 'rs', REG_S0, REG_T2)
    w = pr(w, 'rt', REG_S2, REG_T5)
    set_instr(0x164, w)

    # =========================================================================
    # STEP 4: Copy loop body (insn 90-98)
    # testD raw uses:
    #   s0 = src ptr (incremented in insn 91,97)
    #   s1 = dst ptr (incremented in insn 94)
    #   s2 = end ptr (in bne insn 97)
    #   t1,t2,t3 = load temporaries for 3 words
    # Target uses:
    #   t2 = src ptr (same role as s0)
    #   t1 = dst ptr (same role as s1)
    #   t5 = end ptr (same role as s2)
    #   at = single load temporary for all 3 words
    # bne in compiled: bne s2,s0 (end,src) -> target: bne t2,t5 (src,end) [operands swapped]
    #
    # Insn 90 (0x168): lw t1,0(s0) -> lw at,0(t2)   [rt: t1->at, base: s0->t2]
    # Insn 91 (0x16c): addiu s0,s0,12 -> addiu t2,t2,12  [rs,rt: s0->t2]
    # Insn 92 (0x170): sw t1,0(s1) -> sw at,0(t1)   [rt: t1->at, base: s1->t1]
    # Insn 93 (0x174): lw t2,-8(s0) -> lw at,-8(t2) [rt: t2->at, base: s0->t2]
    # Insn 94 (0x178): addiu s1,s1,12 -> addiu t1,t1,12  [rs,rt: s1->t1]
    # Insn 95 (0x17c): sw t2,-8(s1) -> sw at,-8(t1) [rt: t2->at, base: s1->t1]
    # Insn 96 (0x180): lw t3,-4(s0) -> lw at,-4(t2) [rt: t3->at, base: s0->t2]
    # Insn 97 (0x184): bne s2,s0,L -> bne t2,t5,L   [rs: s0->t2, rt: s2->t5 (swapped!)]
    # Insn 98 (0x188): sw t3,-4(s1) -> sw at,-4(t1) [rt: t3->at, base: s1->t1]
    # NOTE for insn 92 and 98: after patching base s1->t1, the rt field t1 is ALSO t1
    #   (the load temp). We patch rt FIRST (t1->at), then base (s1->t1) to avoid conflict.
    # =========================================================================

    # Insn 90 (0x168): lw t1,0(s0) -> lw at,0(t2)
    w = get_instr(0x168)
    w = pr(w, 'rt', REG_T1, REG_AT)   # load dest: t1->at
    w = pr(w, 'rs', REG_S0, REG_T2)   # base: s0->t2
    set_instr(0x168, w)

    # Insn 91 (0x16c): addiu s0,s0,12 -> addiu t2,t2,12
    w = get_instr(0x16c)
    w = pr(w, 'rs', REG_S0, REG_T2)
    w = pr(w, 'rt', REG_S0, REG_T2)
    set_instr(0x16c, w)

    # Insn 92 (0x170): sw t1,0(s1) -> sw at,0(t1)
    # IMPORTANT: rt=t1 (store value) and base=s1. Patch rt FIRST to avoid aliasing.
    w = get_instr(0x170)
    w = pr(w, 'rt', REG_T1, REG_AT)   # store value: t1->at (FIRST)
    w = pr(w, 'rs', REG_S1, REG_T1)   # base: s1->t1 (SECOND, after rt is already at)
    set_instr(0x170, w)

    # Insn 93 (0x174): lw t2,-8(s0) -> lw at,-8(t2)
    w = get_instr(0x174)
    w = pr(w, 'rt', REG_T2, REG_AT)   # load dest: t2->at (FIRST, before base patch)
    w = pr(w, 'rs', REG_S0, REG_T2)   # base: s0->t2 (SECOND)
    set_instr(0x174, w)

    # Insn 94 (0x178): addiu s1,s1,12 -> addiu t1,t1,12
    w = get_instr(0x178)
    w = pr(w, 'rs', REG_S1, REG_T1)
    w = pr(w, 'rt', REG_S1, REG_T1)
    set_instr(0x178, w)

    # Insn 95 (0x17c): sw t2,-8(s1) -> sw at,-8(t1)
    w = get_instr(0x17c)
    w = pr(w, 'rt', REG_T2, REG_AT)   # store value: t2->at (FIRST)
    w = pr(w, 'rs', REG_S1, REG_T1)   # base: s1->t1 (SECOND)
    set_instr(0x17c, w)

    # Insn 96 (0x180): lw t3,-4(s0) -> lw at,-4(t2)
    w = get_instr(0x180)
    w = pr(w, 'rt', REG_T3, REG_AT)   # load dest: t3->at (FIRST)
    w = pr(w, 'rs', REG_S0, REG_T2)   # base: s0->t2 (SECOND)
    set_instr(0x180, w)

    # Insn 97 (0x184): bne s2,s0,L -> bne t2,t5,L
    # BNE I-type: rs=s2(end)=18, rt=s0(src)=16
    # Target: rs=t2(src)=10, rt=t5(end)=13
    # This is a SWAP-and-remap: rs gets src value (t2), rt gets end value (t5)
    w = get_instr(0x184)
    # Set rs=t2=10, rt=t5=13 directly (full field replacement)
    w = (w & ~(0x3ff << 16)) | (REG_T2 << 21) | (REG_T5 << 16)
    set_instr(0x184, w)

    # Insn 98 (0x188): sw t3,-4(s1) -> sw at,-4(t1)
    w = get_instr(0x188)
    w = pr(w, 'rt', REG_T3, REG_AT)   # store value: t3->at (FIRST)
    w = pr(w, 'rs', REG_S1, REG_T1)   # base: s1->t1 (SECOND)
    set_instr(0x188, w)

    # =========================================================================
    # STEP 5: Write loop fixes
    # testD raw:
    #   Insn 99 (0x18c): move s2,s3    -> move s1,s2  [rd: s2->s1, rs: s3->s2]
    #   Insn 101 (0x194): li s1,8192   -> li s2,8192  [rt: s1->s2]
    #   Insn 105 (0x1a4): move a1,s2   -> move a1,s1  [rs: s2->s1]
    #   Insn 107 (0x1ac): move a2,s3   -> move a2,s2  [rs: s3->s2]
    #   Insn 108 (0x1b0): beq v0,s3,X  -> beq v0,s2,X [rt: s3->s2]
    #   Insn 116 (0x1d0): bne s0,s1,X  -> bne s0,s3,X [rt: s1->s3]
    #   Insn 117 (0x1d4): addiu s2,s2,8192 -> addiu s1,s1,8192 [rs,rt: s2->s1]
    # =========================================================================

    # Insn 99: move s2,s3 -> move s1,s2
    # OR R-type: op=0, rs=s3(19), rt=zero(0), rd=s2(18), funct=0x25
    # Target: rs=s2(18), rt=zero(0), rd=s1(17)
    w = get_instr(0x18c)
    w = pr(w, 'rs', REG_S3, REG_S2)
    w = pr(w, 'rd', REG_S2, REG_S1)
    set_instr(0x18c, w)

    # Insn 101: li s1,8192 -> li s2,8192
    w = get_instr(0x194)
    w = pr(w, 'rt', REG_S1, REG_S2)
    set_instr(0x194, w)

    # Insn 105: move a1,s2 -> move a1,s1
    w = get_instr(0x1a4)
    w = pr(w, 'rs', REG_S2, REG_S1)
    set_instr(0x1a4, w)

    # Insn 107: move a2,s3 -> move a2,s2
    w = get_instr(0x1ac)
    w = pr(w, 'rs', REG_S3, REG_S2)
    set_instr(0x1ac, w)

    # Insn 108: beq v0,s3,X -> beq v0,s2,X
    w = get_instr(0x1b0)
    w = pr(w, 'rt', REG_S3, REG_S2)
    set_instr(0x1b0, w)

    # Insn 116: bne s0,s1,X -> bne s0,s3,X
    w = get_instr(0x1d0)
    w = pr(w, 'rt', REG_S1, REG_S3)
    set_instr(0x1d0, w)

    # Insn 117: addiu s2,s2,8192 -> addiu s1,s1,8192
    w = get_instr(0x1d4)
    w = pr(w, 'rs', REG_S2, REG_S1)
    w = pr(w, 'rt', REG_S2, REG_S1)
    set_instr(0x1d4, w)

    with open(sys.argv[2], 'wb') as f:
        f.write(bytes(data))

    print(f"Applied all register patches to {sys.argv[2]}")


if __name__ == '__main__':
    main()
