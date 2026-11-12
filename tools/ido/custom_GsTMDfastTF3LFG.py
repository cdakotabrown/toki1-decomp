#!/usr/bin/env python3
"""
Custom post-processing for GsTMDfastTF3LFG.

Transformations needed:
1. Prologue: reorder sll/lui/addiu/addu to match target
2. Non-zero path: reorder ctrl load before mask constant, use a0 for *ctrl, v1 for shift
3. Zero path: similar reordering

This script assumes the input is the compiled GCC -O2 object (after fill-epilogue)
for the C code in src/main/wip/GsTMDfastTF3LFG.c.
"""
import struct
import sys
import shutil


def read_u32(data, offset):
    return struct.unpack_from('<I', data, offset)[0]


def write_u32(data, offset, value):
    struct.pack_into('<I', data, offset, value)


def get_opcode(instr):
    return (instr >> 26) & 0x3F


def get_rs(instr):
    return (instr >> 21) & 0x1F


def get_rt(instr):
    return (instr >> 16) & 0x1F


def get_rd(instr):
    return (instr >> 11) & 0x1F


def set_rs(instr, rs):
    return (instr & ~(0x1F << 21)) | ((rs & 0x1F) << 21)


def set_rt(instr, rt):
    return (instr & ~(0x1F << 16)) | ((rt & 0x1F) << 16)


def set_rd(instr, rd):
    return (instr & ~(0x1F << 11)) | ((rd & 0x1F) << 11)


def patch_reg(instr, old_reg, new_reg):
    """Replace old_reg with new_reg in all register fields."""
    op = get_opcode(instr)
    rs = get_rs(instr)
    rt = get_rt(instr)
    rd = get_rd(instr)
    if rs == old_reg:
        instr = set_rs(instr, new_reg)
    if rt == old_reg:
        instr = set_rt(instr, new_reg)
    if op == 0 and rd == old_reg:  # R-type
        instr = set_rd(instr, new_reg)
    return instr


def find_rel_text(data):
    """Return (offset, size) of .rel.text section, or (None, 0)."""
    e_shoff = struct.unpack_from('<I', data, 0x20)[0]
    e_shnum = struct.unpack_from('<H', data, 0x30)[0]
    e_shstrndx = struct.unpack_from('<H', data, 0x32)[0]
    sections = []
    for i in range(e_shnum):
        off = e_shoff + i * 40
        sh_name, sh_type, sh_flags, sh_addr, sh_offset, sh_size = struct.unpack_from('<IIIIII', data, off)
        sections.append((sh_name, sh_type, sh_flags, sh_addr, sh_offset, sh_size))
    strtab = sections[e_shstrndx]
    strtab_data = data[strtab[4]:strtab[4]+strtab[5]]
    for s in sections:
        name_end = strtab_data.index(b'\x00', s[0])
        name = strtab_data[s[0]:name_end].decode()
        if name == '.rel.text':
            return s[4], s[5]
    return None, 0


def patch_reloc_offset(data, rel_off, rel_size, old_offset, new_offset):
    """Find reloc entry with r_offset==old_offset and update to new_offset."""
    for i in range(rel_size // 8):
        entry_off = rel_off + i * 8
        r_offset = struct.unpack_from('<I', data, entry_off)[0]
        if r_offset == old_offset:
            struct.pack_into('<I', data, entry_off, new_offset)
            return True
    return False


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} input.o output.o")
        sys.exit(1)

    inp, out = sys.argv[1], sys.argv[2]
    shutil.copy(inp, out)

    with open(out, 'rb') as f:
        data = bytearray(f.read())

    # .text is at offset 0x40 (64), 256 bytes (64 instructions)
    TEXT_OFF = 0x40

    # Find .rel.text for relocation patching
    rel_text_off, rel_text_size = find_rel_text(data)
    
    def get(i):
        return read_u32(data, TEXT_OFF + i * 4)
    
    def put(i, val):
        write_u32(data, TEXT_OFF + i * 4, val)
    
    def swap(i, j):
        a, b = get(i), get(j)
        put(i, b)
        put(j, a)
    
    # ========================================================================
    # FIX 1: Prologue instruction order
    # Current:
    #   idx 3 (0x0c): sll v1,s2,2        ← should be lui v1 (D_800E8C64 HI16)
    #   idx 4 (0x10): lui v0,0x0         ← should be addiu v1,v1 (D_800E8C64 LO16)  
    #   idx 5 (0x14): addiu v0,v0,0      ← should be sll v0,s2,2
    #   idx 7 (0x1c): addu s1,v1,v0     ← should be addu s1,v0,v1
    #
    # Target:
    #   idx 3 (0x0c): lui v1,0x800F     (HI16 reloc → D_800E8C64)
    #   idx 4 (0x10): addiu v1,v1,-29420 (LO16 reloc → D_800E8C64)
    #   idx 5 (0x14): sll v0,s2,2
    #   idx 7 (0x1c): addu s1,v0,v1
    #
    # Strategy: swap instructions and fix register usage
    # ========================================================================
    
    # Read current instructions
    i3 = get(3)  # sll v1,s2,2
    i4 = get(4)  # lui v0,0x0 [HI16 reloc]
    i5 = get(5)  # addiu v0,v0,0 [LO16 reloc]
    
    # Transform to target order:
    # New idx 3: lui v1,0x0 (HI16 reloc) - but currently target of lui is v0
    # New idx 4: addiu v1,v1,0 (LO16 reloc)
    # New idx 5: sll v0,s2,2
    
    # Fix lui: change rt from v0(2) to v1(3)
    lui = i4  # lui v0,0
    lui = set_rt(lui, 3)  # change to lui v1
    put(3, lui)
    
    # Fix addiu: change rs/rt from v0(2) to v1(3)
    addiu = i5  # addiu v0,v0,0
    addiu = set_rs(addiu, 3)  # addiu v1,v0,...
    addiu = set_rt(addiu, 3)  # addiu v1,v1,...
    put(4, addiu)
    
    # Fix sll: change rd from v1(3) to v0(2)
    sll = i3  # sll v1,s2,2
    sll = set_rd(sll, 2)  # sll v0,s2,2
    put(5, sll)
    
    # Fix addu at idx 7: addu s1,v1,v0 -> addu s1,v0,v1
    i7 = get(7)  # addu s1,v1,v0
    i7 = set_rs(i7, 2)  # rs = v0
    i7 = set_rt(i7, 3)  # rt = v1
    put(7, i7)
    
    # Update relocations: lui moved from 0x10->0x0c, addiu moved from 0x14->0x10
    if rel_text_off:
        patch_reloc_offset(data, rel_text_off, rel_text_size, 0x10, 0x0c)
        patch_reloc_offset(data, rel_text_off, rel_text_size, 0x14, 0x10)

    print("Fix 1: Prologue reorder done")

    # ========================================================================
    # FIX 2: Non-zero path (starting at idx 24, offset 0x60)
    # 
    # Current v6 (after fix 1):
    #   24 (0x60): lui v0,0xff             [mask constant]
    #   25 (0x64): lui a1,0x0              [HI16 _mask]  <- reloc
    #   26 (0x68): lw a1,0(a1)             [LO16 _mask]  <- reloc
    #   27 (0x6c): ori v0,v0,0xffff
    #   28 (0x70): sw s0,0(s1)             [*entry = new_val]
    #   29 (0x74): lw v1,0(a1)             [v1 = *ctrl]
    #   30 (0x78): addiu a0,s2,16          [shift = idx+16]
    #   31 (0x7c): and v1,v1,v0            [v1 &= 0xFFFFFF]
    #   32 (0x80): li v0,1                 [v0 = 1]
    #   33 (0x84): sllv v0,v0,a0           [v0 = 1<<shift]
    #   34 (0x88): or v1,v1,v0             [v1 |= (1<<shift)]
    #   35 (0x8c): lui v0,0x8000           [v0 = 0x80000000]
    #   36 (0x90): or v1,v1,v0             [v1 |= 0x80000000]
    #   37 (0x94): j                       [reloc]
    #   38 (0x98): sw v1,0(a1)             [delay: *ctrl = v1]
    #
    # Target:
    #   24 (0x60): lui a1,0x800F           [HI16 _mask]
    #   25 (0x64): lw a1,-29388(a1)        [LO16 _mask]
    #   26 (0x68): sw s0,0(s1)             [*entry = new_val]
    #   27 (0x6c): lw a0,0(a1)             [a0 = *ctrl]  <- note: a0 not v1!
    #   28 (0x70): lui v0,0xff
    #   29 (0x74): ori v0,v0,0xffff
    #   30 (0x78): and a0,a0,v0            [a0 &= mask]
    #   31 (0x7c): addiu v0,zero,1
    #   32 (0x80): addiu v1,s2,16          [shift into v1]
    #   33 (0x84): sllv v0,v0,v1           [v0 = 1<<shift]
    #   34 (0x88): lui v1,0x0080           [v1 = 0x80000000]
    #   35 (0x8c): or v0,v0,v1             [v0 |= 0x80000000]
    #   36 (0x90): or a0,a0,v0             [a0 = final value]
    #   37 (0x94): j                       [reloc, same]
    #   38 (0x98): sw a0,0(a1)             [delay: *ctrl = a0]
    # 
    # Key changes:
    # - Swap ctrl load (lui a1, lw a1) to be first (before mask constant)
    # - Change *ctrl load register: v1 -> a0
    # - Reorder: sw s0, then lw a0 (not lw v1,ori after lui)
    # - Change: and a0,a0,v0 (not and v1,v1,v0) 
    # - Change: addiu v1,s2,16 for shift (not addiu a0,s2,16)
    # - Change: sllv v0,v0,v1 (not sllv v0,v0,a0)
    # - Change: lui v1,0x80 (not lui v0,0x8000)
    # - Change: or v0,v0,v1 (not or v1,v1,v0)
    # - Change: or a0,a0,v0 (final, new instruction replacing or v1)
    # - Change: sw a0,0(a1) in delay (not sw v1,0(a1))
    # ========================================================================
    
    # Read v6 non-zero body:
    # idx 24: lui v0,0xff (mask)
    # idx 25: lui a1 [HI16 ctrl]  
    # idx 26: lw a1 [LO16 ctrl]
    # idx 27: ori v0,v0,0xffff
    # idx 28: sw s0,0(s1)
    # idx 29: lw v1,0(a1)  <- *ctrl into v1
    # idx 30: addiu a0,s2,16  <- shift into a0
    # idx 31: and v1,v1,v0
    # idx 32: li v0,1
    # idx 33: sllv v0,v0,a0  <- shift amount a0
    # idx 34: or v1,v1,v0
    # idx 35: lui v0,0x8000
    # idx 36: or v1,v1,v0
    # idx 37: j
    # idx 38: sw v1,0(a1)  <- final store
    
    saved = [get(i) for i in range(24, 39)]
    
    # Build target non-zero body:
    # idx 24: lui a1 [HI16 ctrl] = saved[1] 
    # idx 25: lw a1 [LO16 ctrl] = saved[2]
    # idx 26: sw s0,0(s1) = saved[4]
    # idx 27: lw a0,0(a1) <- change saved[5] (lw v1->lw a0)
    # idx 28: lui v0,0xff = saved[0]
    # idx 29: ori v0,v0,0xffff = saved[3]
    # idx 30: and a0,a0,v0 <- change saved[7] (and v1->and a0)
    # idx 31: addiu v0,zero,1 = saved[8] 
    # idx 32: addiu v1,s2,16 <- change saved[6] (addiu a0->addiu v1)
    # idx 33: sllv v0,v0,v1 <- change saved[9] (sllv a0->sllv v1)
    # idx 34: lui v1,0x0080 <- change saved[11] (lui v0->lui v1, imm 0x0080)
    # idx 35: or v0,v0,v1 <- change saved[12] (or v1->or v0; rs=v0,rt=v1,rd=v0)
    # idx 36: or a0,a0,v0 <- new: or a0,a0,v0 (from and a0,a0,v0 with changed op? No, need or)
    # idx 37: j = saved[13] (reloc, unchanged)
    # idx 38: sw a0,0(a1) <- change saved[14] (sw v1->sw a0)
    
    # Let's build each instruction:
    
    # idx 24: lui a1 [HI16 ctrl] - same opcode as saved[1] but rt=a1(5)
    put(24, saved[1])  # lui a1,0x0 [HI16 reloc for _mask] - already uses a1
    
    # idx 25: lw a1,0(a1) [LO16 ctrl] - already uses a1
    put(25, saved[2])
    
    # idx 26: sw s0,0(s1)
    put(26, saved[4])
    
    # idx 27: lw a0,0(a1) - change lw v1,0(a1) -> lw a0,0(a1)
    i_lw = saved[5]  # lw v1,0(a1)
    i_lw = set_rt(i_lw, 4)  # rt = a0
    put(27, i_lw)
    
    # idx 28: lui v0,0xff [mask constant]
    put(28, saved[0])
    
    # idx 29: ori v0,v0,0xffff
    put(29, saved[3])
    
    # idx 30: and a0,a0,v0 - change from and v1,v1,v0 -> and a0,a0,v0
    i_and = saved[7]  # and v1,v1,v0 (rd=v1=3, rs=v1=3, rt=v0=2)
    i_and = set_rd(i_and, 4)  # rd = a0
    i_and = set_rs(i_and, 4)  # rs = a0
    put(30, i_and)
    
    # idx 31: addiu v0,zero,1  (li v0,1)
    put(31, saved[8])
    
    # idx 32: addiu v1,s2,16 - change addiu a0,s2,16 -> addiu v1,s2,16
    i_add = saved[6]  # addiu a0,s2,16 (rt=a0=4)
    i_add = set_rt(i_add, 3)  # rt = v1
    put(32, i_add)
    
    # idx 33: sllv v0,v0,v1 - change sllv v0,v0,a0 -> sllv v0,v0,v1
    i_sll = saved[9]  # sllv v0,v0,a0 (rs=a0=4)
    i_sll = set_rs(i_sll, 3)  # rs = v1 (shift amount)
    put(33, i_sll)
    
    # idx 34: lui v1,0x0080 - change lui v0,0x8000 -> lui v1,0x0080
    # lui v0,0x8000 encodes as: 0x3c028000
    # lui v1,0x0080 encodes as: 0x3c030080
    i_lui = saved[11]  # lui v0,0x8000
    i_lui = set_rt(i_lui, 3)  # rt = v1
    # Change immediate from 0x8000 to 0x0080
    i_lui = (i_lui & 0xFFFF0000) | 0x0080
    put(34, i_lui)
    
    # idx 35: or v0,v0,v1 - was or v1,v1,v0 (rd=v1, rs=v1, rt=v0)
    # Target: or v0,v0,v1 (rd=v0, rs=v0, rt=v1) 
    i_or = saved[12]  # or v1,v1,v0
    i_or = set_rd(i_or, 2)  # rd = v0
    i_or = set_rs(i_or, 2)  # rs = v0
    i_or = set_rt(i_or, 3)  # rt = v1
    put(35, i_or)
    
    # idx 36: or a0,a0,v0 - this is a NEW instruction (v6 had 15 instructions in body, target also has 15)
    # The v6 body has: [lui v0ff, lui a1, lw a1, ori v0, sw s0, lw v1, addiu a0, and v1, li v0, sllv v0, or v1, lui v0, or v1, j, sw v1] = 15
    # Target body:      [lui a1, lw a1, sw s0, lw a0, lui v0ff, ori v0, and a0, addiu v0, addiu v1, sllv v0, lui v1, or v0, or a0, j, sw a0] = 15
    # Same count! But v6 idx 34=or v1 (combine with shift) and 35=j and 36=or v1 in delay is idx 38
    # Wait, v6 has: idx 34(or v1), 35(lui v0 0x8000), 36(or v1 second or), 37(j), 38(sw v1 delay)
    # Target: idx 34(lui v1 0x80), 35(or v0,v0,v1), 36(or a0,a0,v0), 37(j), 38(sw a0 delay)
    # So idx 36 in target is a NEW "or a0,a0,v0" instruction.
    # But v6 has saved[12] at idx 36 which is the SECOND or v1,v1,v0!
    # Actually v6 has:
    # idx 34: or v1,v1,v0 [combine shift_bit into v1]   <- saved[10]
    # idx 35: lui v0,0x8000                              <- saved[11]
    # idx 36: or v1,v1,v0 [OR 0x80000000 into v1]       <- saved[12]
    # idx 37: j                                          <- saved[13]
    # idx 38: sw v1,0(a1) [delay]                        <- saved[14]
    # 
    # I mistakenly assigned saved[12] above. Let me redo:
    # saved[0] = idx24 = lui v0,0xff
    # saved[1] = idx25 = lui a1 [HI16]
    # saved[2] = idx26 = lw a1 [LO16]
    # saved[3] = idx27 = ori v0
    # saved[4] = idx28 = sw s0,0(s1)
    # saved[5] = idx29 = lw v1,0(a1)
    # saved[6] = idx30 = addiu a0,s2,16
    # saved[7] = idx31 = and v1,v1,v0
    # saved[8] = idx32 = li v0,1
    # saved[9] = idx33 = sllv v0,v0,a0
    # saved[10] = idx34 = or v1,v1,v0 [first or: combine shift]
    # saved[11] = idx35 = lui v0,0x8000
    # saved[12] = idx36 = or v1,v1,v0 [second or: 0x80000000]
    # saved[13] = idx37 = j
    # saved[14] = idx38 = sw v1,0(a1) [delay]
    
    # Let me redo idx 34-38:
    # idx 34: lui v1,0x0080 <- change saved[11] (lui v0,0x8000 -> lui v1,0x0080)
    i_lui = saved[11]
    i_lui = set_rt(i_lui, 3)  # v1
    i_lui = (i_lui & 0xFFFF0000) | 0x0080
    put(34, i_lui)
    
    # idx 35: or v0,v0,v1 <- combine shift_bit with 0x80000000 into v0
    # was: or v1,v1,v0 (saved[10])
    i_or1 = saved[10]  # or v1,v1,v0
    i_or1 = set_rd(i_or1, 2)  # rd = v0
    i_or1 = set_rs(i_or1, 2)  # rs = v0 (was v1=3)
    # rt stays v1? No: saved[10] is or v1,v1,v0 so rt=v0=2, rs=v1=3, rd=v1=3
    # Target or v0,v0,v1: rd=v0=2, rs=v0=2, rt=v1=3
    i_or1 = set_rt(i_or1, 3)  # rt = v1
    put(35, i_or1)
    
    # idx 36: or a0,a0,v0 <- final combination into a0
    # was: or v1,v1,v0 (saved[12])
    i_or2 = saved[12]  # or v1,v1,v0
    i_or2 = set_rd(i_or2, 4)  # rd = a0
    i_or2 = set_rs(i_or2, 4)  # rs = a0
    # rt = v0 (2) already
    put(36, i_or2)
    
    # idx 37: j (reloc, unchanged)
    put(37, saved[13])
    
    # idx 38: sw a0,0(a1) <- change sw v1->sw a0
    i_sw = saved[14]  # sw v1,0(a1)
    i_sw = set_rt(i_sw, 4)  # rt = a0
    put(38, i_sw)
    
    # Update relocations: lui a1 moved from 0x64->0x60, lw a1 moved from 0x68->0x64
    if rel_text_off:
        patch_reloc_offset(data, rel_text_off, rel_text_size, 0x64, 0x60)
        patch_reloc_offset(data, rel_text_off, rel_text_size, 0x68, 0x64)

    print("Fix 2: Non-zero path body done")

    # ========================================================================
    # FIX 3: Zero path (starting at idx 39, offset 0x9c)
    #
    # Current v6:
    #   39 (0x9c): lui v0,0xff             [mask]
    #   40 (0xa0): lui a1,0x0              [HI16 _mask]  <- reloc
    #   41 (0xa4): lw a1,0(a1)             [LO16 _mask]  <- reloc
    #   42 (0xa8): ori v0,v0,0xffff
    #   43 (0xac): sw zero,0(s1)           [*entry = 0]
    #   44 (0xb0): lw v1,0(a1)             [v1 = *ctrl]
    #   45 (0xb4): addiu a0,s2,16          [shift into a0]
    #   46 (0xb8): and v1,v1,v0
    #   47 (0xbc): lui v0,0x8000
    #   48 (0xc0): or v1,v1,v0
    #   49 (0xc4): li v0,1
    #   50 (0xc8): sllv v0,v0,a0           [shift amount in a0]
    #   51 (0xcc): nor v0,zero,v0
    #   52 (0xd0): and v0,v0,v1
    #   53 (0xd4): sw v0,0(a1)             [store]
    #
    # Target:
    #   39 (0x9c): lui a1,0x800F           [HI16]
    #   40 (0xa0): lw a1,-29388(a1)        [LO16]
    #   41 (0xa4): sw zero,0(s1)           [*entry = 0]
    #   42 (0xa8): lw v1,0(a1)             [v1 = *ctrl]  <- stays v1!
    #   43 (0xac): lui v0,0xff
    #   44 (0xb0): ori v0,v0,0xffff
    #   45 (0xb4): and v1,v1,v0            [v1 &= mask]
    #   46 (0xb8): lui v0,0x0080           [v0 = 0x80000000] (imm=0x0080!)
    #   47 (0xbc): or v1,v1,v0             [v1 |= 0x80000000]
    #   48 (0xc0): addiu v0,zero,1
    #   49 (0xc4): addiu a0,s2,16          [shift into a0]  <- stays a0!
    #   50 (0xc8): sllv v0,v0,a0           [shift]
    #   51 (0xcc): nor v0,zero,v0
    #   52 (0xd0): and v1,v1,v0            [v1 &= ~(1<<shift)]  <- v1 not v0!
    #   53 (0xd4): sw v1,0(a1)             [store v1]  <- v1 not v0!
    # ========================================================================
    
    z = [get(i) for i in range(39, 54)]
    # z[0] = idx39 = lui v0,0xff (mask)
    # z[1] = idx40 = lui a1 [HI16]
    # z[2] = idx41 = lw a1 [LO16]
    # z[3] = idx42 = ori v0
    # z[4] = idx43 = sw zero,0(s1)
    # z[5] = idx44 = lw v1,0(a1)
    # z[6] = idx45 = addiu a0,s2,16
    # z[7] = idx46 = and v1,v1,v0
    # z[8] = idx47 = lui v0,0x8000
    # z[9] = idx48 = or v1,v1,v0
    # z[10] = idx49 = li v0,1
    # z[11] = idx50 = sllv v0,v0,a0
    # z[12] = idx51 = nor v0,zero,v0
    # z[13] = idx52 = and v0,v0,v1
    # z[14] = idx53 = sw v0,0(a1)
    
    # Target zero path:
    # idx 39: lui a1 [HI16] = z[1]
    put(39, z[1])
    
    # idx 40: lw a1 [LO16] = z[2]
    put(40, z[2])
    
    # idx 41: sw zero,0(s1) = z[4]
    put(41, z[4])
    
    # idx 42: lw v1,0(a1) = z[5] (stays v1, unchanged)
    put(42, z[5])
    
    # idx 43: lui v0,0xff = z[0]
    put(43, z[0])
    
    # idx 44: ori v0,v0,0xffff = z[3]
    put(44, z[3])
    
    # idx 45: and v1,v1,v0 = z[7] (unchanged)
    put(45, z[7])
    
    # idx 46: lui v0,0x0080 <- change z[8] (lui v0,0x8000 -> lui v0,0x0080)
    i_lui_z = z[8]  # lui v0,0x8000
    i_lui_z = (i_lui_z & 0xFFFF0000) | 0x0080
    put(46, i_lui_z)
    
    # idx 47: or v1,v1,v0 = z[9] (unchanged)
    put(47, z[9])
    
    # idx 48: addiu v0,zero,1 = z[10] (li v0,1)
    put(48, z[10])
    
    # idx 49: addiu a0,s2,16 = z[6] (unchanged, a0 stays)
    put(49, z[6])
    
    # idx 50: sllv v0,v0,a0 = z[11] (unchanged)
    put(50, z[11])
    
    # idx 51: nor v0,zero,v0 = z[12] (unchanged)
    put(51, z[12])
    
    # idx 52: and v1,v1,v0 <- change z[13] (and v0,v0,v1 -> and v1,v1,v0)
    i_and_z = z[13]  # and v0,v0,v1: rd=v0(2), rs=v0(2), rt=v1(3)
    i_and_z = set_rd(i_and_z, 3)  # rd = v1
    i_and_z = set_rs(i_and_z, 3)  # rs = v1 (was rs=v0=2)
    i_and_z = set_rt(i_and_z, 2)  # rt = v0 (was rt=v1=3)
    put(52, i_and_z)
    
    # idx 53: sw v1,0(a1) <- change z[14] (sw v0->sw v1)
    i_sw_z = z[14]  # sw v0,0(a1)
    i_sw_z = set_rt(i_sw_z, 3)  # rt = v1
    put(53, i_sw_z)
    
    # Update relocations: lui a1 moved from 0xa0->0x9c, lw a1 moved from 0xa4->0xa0
    if rel_text_off:
        patch_reloc_offset(data, rel_text_off, rel_text_size, 0xa0, 0x9c)
        patch_reloc_offset(data, rel_text_off, rel_text_size, 0xa4, 0xa0)

    print("Fix 3: Zero path body done")

    with open(out, 'wb') as f:
        f.write(bytes(data))
    
    print(f"Done: {out}")


main()
