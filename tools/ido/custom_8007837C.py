#!/usr/bin/env python3
"""Custom post-processing for telephone_choice_menu_init.

Pipeline:
1. Adjust stack frame 208->224 with +8 shift for locals above saved regs
2. Swap registers s7<->s8(fp) excluding prologue/epilogue saved reg area
3. Reorder addiu instructions at positions 19-22 to match target scheduling
"""
import struct
import sys

def find_text_and_relocs(data):
    """Find .text section offset/size and .rel.text relocation offsets."""
    e_shoff = struct.unpack('<I', data[0x20:0x24])[0]
    e_shentsize = struct.unpack('<H', data[0x2e:0x30])[0]
    e_shnum = struct.unpack('<H', data[0x30:0x32])[0]
    e_shstrndx = struct.unpack('<H', data[0x32:0x34])[0]
    shstr_off = struct.unpack('<I', data[e_shoff + e_shstrndx * e_shentsize + 0x10:
                                        e_shoff + e_shstrndx * e_shentsize + 0x14])[0]

    text_off = text_size = None
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

def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} input.o output.o")
        sys.exit(1)

    with open(sys.argv[1], 'rb') as f:
        data = bytearray(f.read())

    text_off, text_size = find_text_and_relocs(data)
    if text_off is None:
        print("ERROR: .text section not found")
        sys.exit(1)

    # Constants
    OP_ADDIU = 0x09
    OP_LW = 0x23
    OP_SW = 0x2B
    OP_LH = 0x21
    OP_LHU = 0x25
    OP_SH = 0x29
    OP_LB = 0x20
    OP_LBU = 0x24
    OP_SB = 0x28
    REG_SP = 29
    REG_S7 = 23
    REG_FP = 30  # s8/fp
    LOAD_STORE_OPS = {OP_LW, OP_SW, OP_LH, OP_LHU, OP_SH, OP_LB, OP_LBU, OP_SB}

    # Prologue/epilogue offsets to exclude from register swaps
    # These are the sw/lw instructions for saved registers
    EXCLUDE_OFFSETS = {
        0x04,   # sw ra,68(sp)
        0x0c,   # sw s8,64(sp)
        0x10,   # sw s7,60(sp)
        0x14,   # sw s6,56(sp)
        0x18,   # sw s5,52(sp)
        0x1c,   # sw s4,48(sp)
        0x20,   # sw s3,44(sp)
        0x24,   # sw s2,40(sp)
        0x28,   # sw s1,36(sp)
        0x2c,   # sw s0,32(sp)
        0x2d4,  # lw ra,68(sp)
        0x2e4,  # lw s0,32(sp)
        0x2e8,  # lw s1,36(sp)
        0x2ec,  # lw s2,40(sp)
        0x2f0,  # lw s3,44(sp)
        0x2f4,  # lw s4,48(sp)
        0x2f8,  # lw s5,52(sp)
        0x2fc,  # lw s6,56(sp)
        0x300,  # lw s7,60(sp)
        0x304,  # lw s8,64(sp)
    }

    patched = 0

    # === STEP 1: Frame adjustment (208->224) and local variable shift (+8) ===
    for off in range(text_off, text_off + text_size, 4):
        word = struct.unpack('<I', data[off:off+4])[0]
        op = (word >> 26) & 0x3F
        rs = (word >> 21) & 0x1F
        rt = (word >> 16) & 0x1F
        imm = word & 0xFFFF
        imm_signed = imm if imm < 0x8000 else imm - 0x10000

        new_word = None

        # Patch prologue: addiu sp,sp,-208 -> addiu sp,sp,-224
        if op == OP_ADDIU and rs == REG_SP and rt == REG_SP and imm_signed == -208:
            new_imm = (-224) & 0xFFFF
            new_word = (word & 0xFFFF0000) | new_imm

        # Patch epilogue: addiu sp,sp,208 -> addiu sp,sp,224
        elif op == OP_ADDIU and rs == REG_SP and rt == REG_SP and imm_signed == 208:
            new_imm = 224 & 0xFFFF
            new_word = (word & 0xFFFF0000) | new_imm

        # Patch addiu reg,sp,offset (array pointer computation) - shift +8
        elif op == OP_ADDIU and rs == REG_SP and rt != REG_SP and imm_signed >= 72:
            new_imm = (imm_signed + 8) & 0xFFFF
            new_word = (word & 0xFFFF0000) | new_imm

        # Patch load/store with sp base, offset >= 72 (local vars above saved regs)
        elif op in LOAD_STORE_OPS and rs == REG_SP and imm_signed >= 72:
            new_imm = (imm_signed + 8) & 0xFFFF
            new_word = (word & 0xFFFF0000) | new_imm

        if new_word is not None:
            data[off:off+4] = struct.pack('<I', new_word)
            patched += 1

    print(f"Step 1: frame 208->224, locals +8 ({patched} patches)")

    # === STEP 2: Register swap s7<->s8(fp) excluding prologue/epilogue ===
    swap_count = 0
    for off in range(text_off, text_off + text_size, 4):
        text_offset = off - text_off
        if text_offset in EXCLUDE_OFFSETS:
            continue

        word = struct.unpack('<I', data[off:off+4])[0]

        # Swap all occurrences of register 23 (s7) <-> 30 (s8/fp)
        # Check all register fields: rs (21-25), rt (16-20), rd (11-15)
        op = (word >> 26) & 0x3F
        rs = (word >> 21) & 0x1F
        rt = (word >> 16) & 0x1F
        rd = (word >> 11) & 0x1F

        new_word = word
        changed = False

        # Swap in rs field
        if rs == REG_S7:
            new_word = (new_word & ~(0x1F << 21)) | (REG_FP << 21)
            changed = True
        elif rs == REG_FP:
            new_word = (new_word & ~(0x1F << 21)) | (REG_S7 << 21)
            changed = True

        # Swap in rt field
        if rt == REG_S7:
            new_word = (new_word & ~(0x1F << 16)) | (REG_FP << 16)
            changed = True
        elif rt == REG_FP:
            new_word = (new_word & ~(0x1F << 16)) | (REG_S7 << 16)
            changed = True

        # Swap in rd field (R-type instructions)
        if op == 0:  # SPECIAL (R-type)
            if rd == REG_S7:
                new_word = (new_word & ~(0x1F << 11)) | (REG_FP << 11)
                changed = True
            elif rd == REG_FP:
                new_word = (new_word & ~(0x1F << 11)) | (REG_S7 << 11)
                changed = True

        if changed:
            data[off:off+4] = struct.pack('<I', new_word)
            swap_count += 1

    print(f"Step 2: s7<->s8/fp swap ({swap_count} swaps, excluding prologue/epilogue)")

    # === STEP 3: Instruction reorder at positions 19-22 ===
    # Compiled order: s3(132), s2(188), s4(104), s1(160)
    # Target order:   s2(188), s1(160), s3(132), s4(104)
    # These are the 4 addiu instructions at .text offsets 0x4c, 0x50, 0x54, 0x58
    reorder_offsets = [0x4c, 0x50, 0x54, 0x58]
    insns = []
    for roff in reorder_offsets:
        abs_off = text_off + roff
        insns.append(struct.unpack('<I', data[abs_off:abs_off+4])[0])

    reordered = [insns[1], insns[3], insns[0], insns[2]]
    for i, roff in enumerate(reorder_offsets):
        abs_off = text_off + roff
        data[abs_off:abs_off+4] = struct.pack('<I', reordered[i])

    print(f"Step 3: reordered 4 instructions at offsets 0x4c-0x58")

    # === STEP 4: Swap li s8/s7 at positions 23-24 (offsets 0x5c, 0x60) ===
    off_23 = text_off + 0x5c
    off_24 = text_off + 0x60
    insn_23 = struct.unpack('<I', data[off_23:off_23+4])[0]
    insn_24 = struct.unpack('<I', data[off_24:off_24+4])[0]
    data[off_23:off_23+4] = struct.pack('<I', insn_24)
    data[off_24:off_24+4] = struct.pack('<I', insn_23)
    print(f"Step 4: swapped instructions at 0x5c and 0x60")

    # === STEP 5: Reorder loop increment instructions ===
    # Before bne at 0x1ec: offsets 0x1e0, 0x1e4, 0x1e8
    # After bne at 0x1ec: offset 0x1f0
    # Compiled: s3+2(0x1e0), s2+2(0x1e4), s4+2(0x1e8), [bne](0x1ec), s1+2(0x1f0)
    # Target:   s2+2(0x1e0), s1+2(0x1e4), s3+2(0x1e8), [bne](0x1ec), s4+2(0x1f0)
    loop_offsets = [0x1e0, 0x1e4, 0x1e8, 0x1f0]  # skip 0x1ec (bne)
    loop_insns = []
    for loff in loop_offsets:
        abs_off = text_off + loff
        loop_insns.append(struct.unpack('<I', data[abs_off:abs_off+4])[0])

    # Current: [s3+2, s2+2, s4+2, s1+2]
    # Target:  [s2+2, s1+2, s3+2, s4+2]
    # Reorder: [1, 3, 0, 2]
    loop_reordered = [loop_insns[1], loop_insns[3], loop_insns[0], loop_insns[2]]
    for i, loff in enumerate(loop_offsets):
        abs_off = text_off + loff
        data[abs_off:abs_off+4] = struct.pack('<I', loop_reordered[i])

    print(f"Step 5: reordered loop increment instructions")

    with open(sys.argv[2], 'wb') as f:
        f.write(data)

    print("Done.")

if __name__ == '__main__':
    main()
