#!/usr/bin/env python3
"""Custom post-processing for SD_VabLoadNext.

Fixes register allocation differences between compiled and target:
1. Cross-prologue: lui t0/lw t2,0(t0) -> lui t1/lw t1,-8828(t1) (compact form)
2. Loop limit: li t1,4 -> li t0,4
3. Val register: various t2->t1 patches throughout body
4. newval register: t9->t2 (lui t9,0x1000 etc.)
5. nor result: t4->t9
6. Intermediate val: and/or with t5->t1 (in-place modification pattern)
7. sra result: t6->t4
8. Second gSDSerialInfo reload: t0->t1 (compact form)
9. Second nor: t7->t5
10. Stack spill: sp+28 -> sp+44
11. Various other register field patches
"""
import struct
import sys

# MIPS register numbers
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


def find_text_section(data):
    """Find .text section offset and size in ELF."""
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


def get_insn(data, text_off, offset):
    """Get instruction word at text offset."""
    return struct.unpack('<I', data[text_off + offset:text_off + offset + 4])[0]


def set_insn(data, text_off, offset, word):
    """Set instruction word at text offset."""
    data[text_off + offset:text_off + offset + 4] = struct.pack('<I', word)


def patch_rs(word, new_rs):
    """Patch rs field (bits 25:21)."""
    return (word & ~(0x1F << 21)) | (new_rs << 21)


def patch_rt(word, new_rt):
    """Patch rt field (bits 20:16)."""
    return (word & ~(0x1F << 16)) | (new_rt << 16)


def patch_rd(word, new_rd):
    """Patch rd field (bits 15:11)."""
    return (word & ~(0x1F << 11)) | (new_rd << 11)


def patch_imm(word, new_imm):
    """Patch immediate field (bits 15:0)."""
    return (word & 0xFFFF0000) | (new_imm & 0xFFFF)


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

    patches = []

    # --- Cross-prologue fix: compact form ---
    # 0x00: lui t0,HI  ->  lui t1,HI  (rt: t0->t1)
    w = get_insn(data, text_off, 0x00)
    w = patch_rt(w, REG_T1)
    set_insn(data, text_off, 0x00, w)
    patches.append("0x00: lui t0->t1 (cross-prologue compact)")

    # 0x04: lw t2,LO(t0)  ->  lw t1,LO(t1)  (rt: t2->t1, rs: t0->t1)
    w = get_insn(data, text_off, 0x04)
    w = patch_rt(w, REG_T1)
    w = patch_rs(w, REG_T1)
    set_insn(data, text_off, 0x04, w)
    patches.append("0x04: lw t2,0(t0)->lw t1,LO(t1) (compact)")

    # 0x18: li t1,4  ->  li t0,4  (rt: t1->t0)
    w = get_insn(data, text_off, 0x18)
    w = patch_rt(w, REG_T0)
    set_insn(data, text_off, 0x18, w)
    patches.append("0x18: li t1->t0 (loop limit)")

    # 0x28: and v1,t2,at  ->  and v1,t1,at  (rs: t2->t1)
    w = get_insn(data, text_off, 0x28)
    w = patch_rs(w, REG_T1)
    set_insn(data, text_off, 0x28, w)
    patches.append("0x28: and v1,t2->t1,at (val register)")

    # 0x44: lui t9,0x1000  ->  lui t2,0x1000  (rt: t9->t2)
    w = get_insn(data, text_off, 0x44)
    w = patch_rt(w, REG_T2)
    set_insn(data, text_off, 0x44, w)
    patches.append("0x44: lui t9->t2 (newval base)")

    # 0x48: sllv v1,t9,v0  ->  sllv v1,t2,v0  (rs: t9->t2)
    # sllv: rs (bits 25:21) = shift amount, rt (bits 20:16) = value to shift
    # Actually for sllv: rd=v1, rt=t9, rs=v0 in MIPS encoding
    # sllv rd,rt,rs -> rd = rt << rs
    # The disassembly shows "sllv v1,t9,v0" meaning rd=v1, rt=t9, rs=v0
    w = get_insn(data, text_off, 0x48)
    w = patch_rt(w, REG_T2)
    set_insn(data, text_off, 0x48, w)
    patches.append("0x48: sllv v1,t9->t2,v0 (newval)")

    # 0x54: nor t4,a1,zero  ->  nor t9,a1,zero  (rd: t4->t9)
    w = get_insn(data, text_off, 0x54)
    w = patch_rd(w, REG_T9)
    set_insn(data, text_off, 0x54, w)
    patches.append("0x54: nor t4->t9,a1,zero (inverted mask)")

    # 0x58: and t5,t4,t2  ->  and t1,t1,t9  (rd: t5->t1, rs: t4->t1, rt: t2->t9)
    w = get_insn(data, text_off, 0x58)
    w = patch_rd(w, REG_T1)
    w = patch_rs(w, REG_T1)
    w = patch_rt(w, REG_T9)
    set_insn(data, text_off, 0x58, w)
    patches.append("0x58: and t5,t4,t2->and t1,t1,t9 (in-place val update)")

    # 0x60: or t2,t5,t3  ->  or t1,t1,t3  (rd: t2->t1, rs: t5->t1)
    w = get_insn(data, text_off, 0x60)
    w = patch_rd(w, REG_T1)
    w = patch_rs(w, REG_T1)
    set_insn(data, text_off, 0x60, w)
    patches.append("0x60: or t2,t5->or t1,t1,t3 (in-place val update)")

    # 0x64: sra t6,a0,0x10  ->  sra t4,a0,0x10  (rd: t6->t4)
    w = get_insn(data, text_off, 0x64)
    w = patch_rd(w, REG_T4)
    set_insn(data, text_off, 0x64, w)
    patches.append("0x64: sra t6->t4,a0 (s16 cast)")

    # 0x6C: sw t2,0(at)  ->  sw t1,-8828(at)  (rt: t2->t1)
    # (relocation will fix the address; we just fix the register)
    w = get_insn(data, text_off, 0x6C)
    w = patch_rt(w, REG_T1)
    set_insn(data, text_off, 0x6C, w)
    patches.append("0x6C: sw t2->t1,0(at) (store val)")

    # 0x70: move a0,t6  ->  move a0,t4
    # move = or a0,t6,zero: rs=t6(14), rt=zero(0), rd=a0(4)
    # In MIPS, "move a0,t6" is "or a0,t6,zero" -> rs=t6, rt=zero, rd=a0
    # Actually "move" is encoded as addu rd,rs,zero, so rs field holds the source
    w = get_insn(data, text_off, 0x70)
    # Check encoding: addu rd,rs,$0 -> op=0, rs=t6, rt=0, rd=a0, funct=0x21
    # OR it could be "or a0,t6,zero" -> op=0, rs=t6, rt=0, rd=a0, funct=0x25
    # Patch rs field (source register): t6->t4
    w = patch_rs(w, REG_T4)
    set_insn(data, text_off, 0x70, w)
    patches.append("0x70: move a0,t6->t4 (arg to func)")

    # 0x78: sw t3,28(sp)  ->  sw t3,44(sp)  (imm 28->44)
    w = get_insn(data, text_off, 0x78)
    w = patch_imm(w, 44)
    set_insn(data, text_off, 0x78, w)
    patches.append("0x78: sw t3,28->44(sp) (spill offset)")

    # 0x7C: lw v1,28(sp)  ->  lw v1,44(sp)  (imm 28->44)
    w = get_insn(data, text_off, 0x7C)
    w = patch_imm(w, 44)
    set_insn(data, text_off, 0x7C, w)
    patches.append("0x7C: lw v1,28->44(sp) (reload offset)")

    # 0x80: lui t0,HI  ->  lui t1,HI  (rt: t0->t1, compact reload)
    w = get_insn(data, text_off, 0x80)
    w = patch_rt(w, REG_T1)
    set_insn(data, text_off, 0x80, w)
    patches.append("0x80: lui t0->t1 (second gSDSerialInfo reload)")

    # 0x84: lw t0,0(t0)  ->  lw t1,-8828(t1)  (rt: t0->t1, rs: t0->t1, compact)
    w = get_insn(data, text_off, 0x84)
    w = patch_rt(w, REG_T1)
    w = patch_rs(w, REG_T1)
    set_insn(data, text_off, 0x84, w)
    patches.append("0x84: lw t0,0(t0)->lw t1,LO(t1) (compact reload)")

    # 0x88: nor t7,v1,zero  ->  nor t5,v1,zero  (rd: t7->t5)
    w = get_insn(data, text_off, 0x88)
    w = patch_rd(w, REG_T5)
    set_insn(data, text_off, 0x88, w)
    patches.append("0x88: nor t7->t5,v1,zero (inverted newval)")

    # 0x90: and t0,t7,t0  ->  and t1,t1,t5  (rd: t0->t1, rs: t7->t1, rt: t0->t5)
    w = get_insn(data, text_off, 0x90)
    w = patch_rd(w, REG_T1)
    w = patch_rs(w, REG_T1)
    w = patch_rt(w, REG_T5)
    set_insn(data, text_off, 0x90, w)
    patches.append("0x90: and t0,t7,t0->and t1,t1,t5 (val & ~newval)")

    # 0x98: sw t0,0(at)  ->  sw t1,-8828(at)  (rt: t0->t1)
    w = get_insn(data, text_off, 0x98)
    w = patch_rt(w, REG_T1)
    set_insn(data, text_off, 0x98, w)
    patches.append("0x98: sw t0->t1,0(at) (final store)")

    # 0xA0: bne v0,t1,...  ->  bne v0,t0,...  (rt: t1->t0)
    w = get_insn(data, text_off, 0xA0)
    w = patch_rt(w, REG_T0)
    set_insn(data, text_off, 0xA0, w)
    patches.append("0xA0: bne v0,t1->t0 (loop test)")

    # --- Instruction swap: nor t9,a1,zero <-> and t3,v1,at ---
    # Compiled: 0x50: and t3,v1,at  then  0x54: nor t4,a1,zero (before reg patches)
    # After reg patches on nor: 0x54 becomes nor t9,a1,zero
    # Target order: nor t9,a1,zero first, then and t3,v1,at
    # In raw compiled: and t3 is at 0x50, nor t4 is at 0x54
    # After register patches, we still need to swap them
    # Swap instructions at 0x50 and 0x54 in raw .o (which is 0x48/0x4C after strip)
    w50 = get_insn(data, text_off, 0x50)
    w54 = get_insn(data, text_off, 0x54)
    set_insn(data, text_off, 0x50, w54)
    set_insn(data, text_off, 0x54, w50)
    patches.append("0x50/0x54: swap nor<->and instructions (scheduling)")

    with open(sys.argv[2], 'wb') as f:
        f.write(data)

    print(f"Applied {len(patches)} patches:")
    for p in patches:
        print(f"  {p}")
    print("Done.")


if __name__ == '__main__':
    main()
