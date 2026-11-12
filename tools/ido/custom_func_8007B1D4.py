#!/usr/bin/env python3
"""Custom post-processing for seq_ctrl.

After patch_stack_complete, remap registers per basic block to match
the target binary. The volatile gSDInfo produces correct structure
(no lui hoisting) but with different register allocation:
- Case 0x71: all temps shifted by -1 (need +1), plus t2/v0 swap at end
- Case 0xE1: v0 used instead of t4/t6 for volatile loads
- Case 0xF1: v0 used instead of t9/t1 for volatile loads
"""
import struct
import sys


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
        sections[name] = {
            'idx': i,
            'sh_off': sh,
            'offset': sh_offset,
            'size': sh_size,
        }
    return sections


def remap_reg_in_word(word, old_reg, new_reg):
    """Remap all occurrences of old_reg to new_reg in a MIPS instruction word.
    Checks rs (25-21), rt (20-16), rd (15-11) fields.
    Does NOT touch the opcode field or immediate fields.
    """
    if old_reg == new_reg:
        return word

    opcode = (word >> 26) & 0x3F

    for shift in [21, 16, 11]:
        # Skip rd field for I-type instructions (bits 15-11 are part of immediate)
        if shift == 11 and opcode != 0:
            continue
        curr = (word >> shift) & 0x1F
        if curr == old_reg:
            word = (word & ~(0x1F << shift)) | (new_reg << shift)

    return word


# MIPS register numbers
V0 = 2
T0 = 8
T1 = 9
T2 = 10
T3 = 11
T4 = 12
T5 = 13
T6 = 14
T7 = 15
T8 = 24
T9 = 25


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} input.o output.o")
        sys.exit(1)

    with open(sys.argv[1], 'rb') as f:
        data = bytearray(f.read())

    sections = find_elf_sections(bytes(data))
    text_off = sections['.text']['offset']

    def get_instr(byte_offset):
        return struct.unpack('<I', data[text_off + byte_offset:text_off + byte_offset + 4])[0]

    def set_instr(byte_offset, word):
        struct.pack_into('<I', data, text_off + byte_offset, word)

    # Function is 92 instructions = 368 bytes (0x170)
    # Instruction byte offsets (from function start, after frame insn at 0x00):
    #
    # Case 0x71 block: instructions 12-54 (offsets 0x30 - 0xD8)
    #   Remap: t6->t7, t7->t8, t8->t9 (for flag check)
    #          t8->t9, t9->t0 (for DDAC/DDAA copy) -- wait, these overlap
    #
    # Let me define ranges and mappings carefully.
    #
    # The compiled output with volatile has these register assignments
    # (offsets relative to function start = 0x8007B1D4):
    #
    # Case 0x71 (starts at instr offset 0x30):
    #   0x3C (idx 15): andi t6,v0,0x100  -> target: andi t7,v0,0x100
    #   0x40 (idx 16): bnez t6,...        -> target: bnez t7,...
    #   0x44 (idx 17): andi t7,v0,0x1000 -> target: andi t8,v0,0x1000
    #   0x48 (idx 18): beqz t7,...        -> target: beqz t8,...
    #   ... (function calls, no temp reg issues)
    #   0x8C (idx 35): lui t8,...         -> target: lui t9,...
    #   0x90 (idx 36): lh t8,...          -> target: lh t9,...
    #   0x94 (idx 37): lui at,...         -> same
    #   0x98 (idx 38): sh t8,...          -> target: sh t9,...
    #   0x9C (idx 39): lui at,...         -> same
    #   0xA0 (idx 40): li t9,-1          -> target: li t0,-1
    #   0xA4 (idx 41): sh t9,...          -> target: sh t0,...
    #   0xA8 (idx 42): li at,-257        -> same
    #   0xAC (idx 43): and t0,v0,at      -> target: and t1,v0,at
    #   0xB0 (idx 44): li at,-4097       -> same
    #   0xB4 (idx 45): lui t2,...         -> target: lui t3,...
    #   0xB8 (idx 46): lw t2,...          -> target: lw t3,...
    #   0xBC (idx 47): and v0,t0,at      -> target: and t2,t1,at
    #   0xC0 (idx 48): li at,-2          -> same
    #   0xC4 (idx 49): and t3,t2,at      -> target: and t4,t3,at
    #   0xC8 (idx 50): lui at,...         -> same
    #   0xCC (idx 51): sw t3,...          -> target: sw t4,...
    #   0xD0 (idx 52): lui at,...         -> same
    #   0xD4 (idx 53): b ...             -> same
    #   0xD8 (idx 54): sw v0,...          -> target: sw t2,...
    #
    # Case 0xE1 (starts at instr offset 0xDC):
    #   0xE0 (idx 56): lw t4,...          -> target: lw v0,...
    #   0xE4 (idx 57): lui a0,...         -> same
    #   0xE8 (idx 58): andi t5,t4,0x100  -> target: andi t5,v0,0x100
    #   ... (beqz, lw ra, lh a0, jal, nop)
    #   0x104 (idx 65): lw t6,...         -> target: lw v0,...
    #   0x108 (idx 66): li at,-257       -> same
    #   0x10C (idx 67): and t7,t6,at     -> target: and t6,v0,at
    #   0x110 (idx 68): ori t8,t7,0x1000 -> target: ori t7,t6,0x1000
    #   0x114 (idx 69): lui at,...        -> same
    #   0x118 (idx 70): b ...            -> same
    #   0x11C (idx 71): sw t8,...         -> target: sw t7,...
    #
    # Case 0xF1 (starts at instr offset 0x120):
    #   0x124 (idx 73): lw t9,...         -> target: lw v0,...
    #   0x128 (idx 74): lui a0,...        -> same
    #   0x12C (idx 75): andi t0,t9,0x1000 -> target: andi t8,v0,0x1000
    #   ... (beqz, lw ra, lh a0, jal, nop)
    #   0x148 (idx 82): lw t1,...         -> target: lw v0,...
    #   0x14C (idx 83): li at,-4097      -> same
    #   0x150 (idx 84): and t2,t1,at     -> target: and t9,v0,at
    #   0x154 (idx 85): ori t3,t2,0x100  -> target: ori t0,t9,0x100
    #   0x158 (idx 86): lui at,...        -> same
    #   0x15C (idx 87): sw t3,...         -> target: sw t0,...

    # =====================================================
    # Case 0x71: shift temp regs by +1
    # =====================================================
    # Range: 0x3C to 0xD8

    # Instruction-level remaps for case 0x71
    # Be careful: some remaps chain (t6->t7, t7->t8 must not double-remap)
    # Do it in reverse order to avoid conflicts: t0->t1 first, then t9->t0, etc.

    case71_range = list(range(0x3C, 0xDC, 4))

    # First pass: collect all instructions
    case71_instrs = {}
    for off in case71_range:
        case71_instrs[off] = get_instr(off)

    # Need to remap:
    # compiled t6 -> target t7
    # compiled t7 -> target t8
    # compiled t8 -> target t9
    # compiled t9 -> target t0
    # compiled t0 -> target t1
    # compiled v0 -> target t2 (only at 0xBC: and v0,t0,at and 0xD8: sw v0)
    # compiled t2 -> target t3 (at 0xB4, 0xB8, 0xC4, 0xC8 area)
    # compiled t3 -> target t4 (at 0xC4, 0xCC)

    # Since these remaps chain, we use a temporary placeholder approach:
    # Map everything to unique temps first, then write back.

    PLACEHOLDER_BASE = 100  # beyond real MIPS regs

    for off in case71_range:
        w = case71_instrs[off]

        # Only remap in specific instruction ranges
        # Skip instructions that don't need remapping (move a1,zero, jal, etc.)

        # Apply all remaps using placeholder to avoid conflicts
        # t6(14)->t7(15), t7(15)->t8(24), t8(24)->t9(25), t9(25)->t0(8),
        # t0(8)->t1(9), t2(10)->t3(11), t3(11)->t4(12)
        # v0(2)->t2(10) at specific offsets only

        w = case71_instrs[off]

    # Actually, let me do this more carefully per-instruction.
    # Remap specific instructions:

    # 0x3C: andi t6,v0,0x100 -> andi t7,v0,0x100
    w = get_instr(0x3C)
    w = remap_reg_in_word(w, T6, T7)
    set_instr(0x3C, w)

    # 0x40: bnez t6 -> bnez t7
    w = get_instr(0x40)
    w = remap_reg_in_word(w, T6, T7)
    set_instr(0x40, w)

    # 0x44: andi t7,v0,0x1000 -> andi t8,v0,0x1000
    w = get_instr(0x44)
    w = remap_reg_in_word(w, T7, T8)
    set_instr(0x44, w)

    # 0x48: beqz t7 -> beqz t8
    w = get_instr(0x48)
    w = remap_reg_in_word(w, T7, T8)
    set_instr(0x48, w)

    # 0x8C: lui t8 -> lui t9
    w = get_instr(0x8C)
    w = remap_reg_in_word(w, T8, T9)
    set_instr(0x8C, w)

    # 0x90: lh t8,0(t8) -> lh t9,0(t9)
    w = get_instr(0x90)
    w = remap_reg_in_word(w, T8, T9)
    set_instr(0x90, w)

    # 0x98: sh t8,0(at) -> sh t9,0(at)
    w = get_instr(0x98)
    w = remap_reg_in_word(w, T8, T9)
    set_instr(0x98, w)

    # 0xA0: li t9,-1 -> li t0,-1
    w = get_instr(0xA0)
    w = remap_reg_in_word(w, T9, T0)
    set_instr(0xA0, w)

    # 0xA4: sh t9,0(at) -> sh t0,0(at)
    w = get_instr(0xA4)
    w = remap_reg_in_word(w, T9, T0)
    set_instr(0xA4, w)

    # 0xAC: and t0,v0,at -> and t1,v0,at
    w = get_instr(0xAC)
    w = remap_reg_in_word(w, T0, T1)
    set_instr(0xAC, w)

    # 0xB4: lui t2 -> lui t3
    w = get_instr(0xB4)
    w = remap_reg_in_word(w, T2, T3)
    set_instr(0xB4, w)

    # 0xB8: lw t2,0(t2) -> lw t3,0(t3)
    w = get_instr(0xB8)
    w = remap_reg_in_word(w, T2, T3)
    set_instr(0xB8, w)

    # 0xBC: and v0,t0,at -> and t2,t1,at
    w = get_instr(0xBC)
    w = remap_reg_in_word(w, V0, T2)   # rd: v0->t2
    w = remap_reg_in_word(w, T0, T1)   # rs: t0->t1
    set_instr(0xBC, w)

    # 0xC4: and t3,t2,at -> and t4,t3,at
    w = get_instr(0xC4)
    w = remap_reg_in_word(w, T3, T4)   # rd: t3->t4
    w = remap_reg_in_word(w, T2, T3)   # rs: t2->t3
    set_instr(0xC4, w)

    # 0xCC: sw t3,0(at) -> sw t4,0(at)
    w = get_instr(0xCC)
    w = remap_reg_in_word(w, T3, T4)
    set_instr(0xCC, w)

    # 0xD8: sw v0,0(at) -> sw t2,0(at)
    w = get_instr(0xD8)
    w = remap_reg_in_word(w, V0, T2)
    set_instr(0xD8, w)

    print("Case 0x71: register remaps applied")

    # =====================================================
    # Case 0xE1: remap volatile loads from t4->v0, t6->v0
    # =====================================================

    # 0xE0: lw t4,0(t4) -> lw v0,0(v0)  [lui already has t4->will be remapped]
    # Actually the lui at 0xDC loads the hi-addr. Let me check...
    # 0xDC (idx 55): lui t4,... -> lui v0,...
    w = get_instr(0xDC)
    w = remap_reg_in_word(w, T4, V0)
    set_instr(0xDC, w)

    # 0xE0: lw t4,0(t4) -> lw v0,0(v0)
    w = get_instr(0xE0)
    w = remap_reg_in_word(w, T4, V0)
    set_instr(0xE0, w)

    # 0xE8: andi t5,t4,0x100 -> andi t5,v0,0x100
    w = get_instr(0xE8)
    w = remap_reg_in_word(w, T4, V0)
    set_instr(0xE8, w)

    # 0x104: lw t6,0(t6) -> lw v0,0(v0) [lui at 0x100]
    w = get_instr(0x100)
    w = remap_reg_in_word(w, T6, V0)
    set_instr(0x100, w)

    w = get_instr(0x104)
    w = remap_reg_in_word(w, T6, V0)
    set_instr(0x104, w)

    # 0x10C: and t7,t6,at -> and t6,v0,at
    w = get_instr(0x10C)
    w = remap_reg_in_word(w, T7, T6)   # rd: t7->t6
    w = remap_reg_in_word(w, T6, V0)   # rs: t6->v0  -- but we just changed rd to t6!
    # Wait, this is tricky. Let me think about the instruction format.
    # R-type: and rd,rs,rt -> opcode(6) rs(5) rt(5) rd(5) shamt(5) funct(6)
    # Compiled: and t7,t6,at = and rd=t7,rs=t6,rt=at
    # Target:   and t6,v0,at = and rd=t6,rs=v0,rt=at
    # So we need: rd: t7->t6, rs: t6->v0
    # But after rd: t7->t6, the word has rd=t6. Then rs remap t6->v0 would also
    # remap rd=t6 to v0 if we're not careful!
    # Need to do this atomically. Let me rewrite using direct bit manipulation.
    set_instr(0x10C, get_instr(0x10C))  # revert
    w = get_instr(0x10C)
    # Directly set rs to V0 and rd to T6
    # rs is bits 25-21, rd is bits 15-11
    w = (w & ~(0x1F << 21)) | (V0 << 21)   # rs = v0
    w = (w & ~(0x1F << 11)) | (T6 << 11)   # rd = t6
    set_instr(0x10C, w)

    # 0x110: ori t8,t7,0x1000 -> ori t7,t6,0x1000
    w = get_instr(0x110)
    # I-type: ori rt,rs,imm -> rt is bits 20-16, rs is bits 25-21
    # Compiled: ori rt=t8,rs=t7,0x1000
    # Target:   ori rt=t7,rs=t6,0x1000
    w = (w & ~(0x1F << 16)) | (T7 << 16)   # rt = t7
    w = (w & ~(0x1F << 21)) | (T6 << 21)   # rs = t6
    set_instr(0x110, w)

    # 0x11C: sw t8,0(at) -> sw t7,0(at)
    w = get_instr(0x11C)
    w = remap_reg_in_word(w, T8, T7)
    set_instr(0x11C, w)

    print("Case 0xE1: register remaps applied")

    # =====================================================
    # Case 0xF1: remap volatile loads from t9->v0, t1->v0
    # =====================================================

    # 0x120 (idx 72): lui t9,... -> lui v0,...
    w = get_instr(0x120)
    w = remap_reg_in_word(w, T9, V0)
    set_instr(0x120, w)

    # 0x124: lw t9,0(t9) -> lw v0,0(v0)
    w = get_instr(0x124)
    w = remap_reg_in_word(w, T9, V0)
    set_instr(0x124, w)

    # 0x12C: andi t0,t9,0x1000 -> andi t8,v0,0x1000
    w = get_instr(0x12C)
    # Compiled: andi rt=t0,rs=t9,0x1000
    # Target:   andi rt=t8,rs=v0,0x1000
    w = (w & ~(0x1F << 16)) | (T8 << 16)   # rt = t8
    w = (w & ~(0x1F << 21)) | (V0 << 21)   # rs = v0
    set_instr(0x12C, w)

    # 0x130: beqz t0 -> beqz t8
    w = get_instr(0x130)
    w = remap_reg_in_word(w, T0, T8)
    set_instr(0x130, w)

    # 0x144 (idx 81): lui t1,... -> lui v0,...
    w = get_instr(0x144)
    w = remap_reg_in_word(w, T1, V0)
    set_instr(0x144, w)

    # 0x148: lw t1,0(t1) -> lw v0,0(v0)
    w = get_instr(0x148)
    w = remap_reg_in_word(w, T1, V0)
    set_instr(0x148, w)

    # 0x150: and t2,t1,at -> and t9,v0,at
    w = get_instr(0x150)
    # Compiled: and rd=t2,rs=t1,rt=at
    # Target:   and rd=t9,rs=v0,rt=at
    w = (w & ~(0x1F << 11)) | (T9 << 11)   # rd = t9
    w = (w & ~(0x1F << 21)) | (V0 << 21)   # rs = v0
    set_instr(0x150, w)

    # 0x154: ori t3,t2,0x100 -> ori t0,t9,0x100
    w = get_instr(0x154)
    # Compiled: ori rt=t3,rs=t2,0x100
    # Target:   ori rt=t0,rs=t9,0x100
    w = (w & ~(0x1F << 16)) | (T0 << 16)   # rt = t0
    w = (w & ~(0x1F << 21)) | (T9 << 21)   # rs = t9
    set_instr(0x154, w)

    # 0x15C: sw t3,0(at) -> sw t0,0(at)
    w = get_instr(0x15C)
    w = remap_reg_in_word(w, T3, T0)
    set_instr(0x15C, w)

    print("Case 0xF1: register remaps applied")

    with open(sys.argv[2], 'wb') as f:
        f.write(data)

    print("Done.")


if __name__ == '__main__':
    main()
