#!/usr/bin/env python3
"""Custom post-processing for near_click_check.

This function has a hidden t6 parameter (cross-prologue entry) and needs:
1. Frame size: 32 -> 56 bytes
2. Instruction 0x04: andi t6,a0,0x80 -> andi t7,t6,0x80 (rs:a0->t6, rt:t6->t7)
3. Instruction 0x08: beqz t6 -> beqz t7
4. Register shift in temp regs: t7->t8, t8->t9, t9->t0, t0->t1, t1->t2, t2->t3, t3->t4, t4->t5
   (applies selectively to the non-overlapping ranges)
5. Restructure jal area (0x98-0xc0):
   - compiled: sw v0,24(sp); sb v0,0(at); jal; andi a0,v0,0xff [delay]; andi t5,v0,0x80; beqz t5;
               lui t1; b; li v1,1; move v1,zero; lw t7,24(sp); sll t9,v1,3; sll t8,t7,4; addu t0,t8,t9; addiu t1,t1,0; addu v0,t0,t1
   - target:   sb v0,-2369(at); move a0,v0; jal; sw v0,44(sp) [delay]; lw a2,44(sp); andi t6,v0,0x80; beqz t6,0x8005b500;
               sll t8,a2,4 [delay]; b 0x8005b504; li v1,1 [delay]; move v1,zero;
               sll t0,v1,3; lui t2,0x800c; addiu t2,-24440; addu t1,t8,t0; addu v0,t1,t2
6. Trailing nop
"""
import struct
import sys


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} input.o output.o")
        sys.exit(1)

    with open(sys.argv[1], 'rb') as f:
        raw = bytearray(f.read())

    # Find ELF sections
    e_shoff = struct.unpack('<I', raw[0x20:0x24])[0]
    e_shentsize = struct.unpack('<H', raw[0x2e:0x30])[0]
    e_shnum = struct.unpack('<H', raw[0x30:0x32])[0]
    e_shstrndx = struct.unpack('<H', raw[0x32:0x34])[0]
    shstr_off = struct.unpack('<I', raw[e_shoff + e_shstrndx * e_shentsize + 0x10:
                                        e_shoff + e_shstrndx * e_shentsize + 0x14])[0]

    text_sh_idx = None
    text_off = None
    text_size = None
    rel_text_sh_idx = None
    rel_text_off = None
    rel_text_size = None

    for i in range(e_shnum):
        sh = e_shoff + i * e_shentsize
        name_off = struct.unpack('<I', raw[sh:sh+4])[0]
        name = b''
        j = shstr_off + name_off
        while raw[j] != 0:
            name += bytes([raw[j]])
            j += 1
        sh_off_field = struct.unpack('<I', raw[sh+0x10:sh+0x14])[0]
        sh_size_field = struct.unpack('<I', raw[sh+0x14:sh+0x18])[0]

        if name == b'.text':
            text_sh_idx = i
            text_off = sh_off_field
            text_size = sh_size_field
        elif name == b'.rel.text':
            rel_text_sh_idx = i
            rel_text_off = sh_off_field
            rel_text_size = sh_size_field

    if text_off is None:
        print("ERROR: .text section not found")
        sys.exit(1)

    compiled = []
    for i in range(text_size // 4):
        w = struct.unpack('<I', raw[text_off + i*4:text_off + i*4 + 4])[0]
        compiled.append(w)

    print(f"Compiled: {len(compiled)} instructions ({text_size} bytes)")

    relocs = []
    if rel_text_off is not None:
        n = rel_text_size // 8
        for i in range(n):
            r = rel_text_off + i * 8
            r_offset = struct.unpack('<I', raw[r:r+4])[0]
            r_info = struct.unpack('<I', raw[r+4:r+8])[0]
            r_sym = r_info >> 8
            r_type = r_info & 0xFF
            relocs.append([r_offset, r_sym, r_type])

    # Symbol table
    symtab_off = None
    symtab_size = None
    strtab_off = None
    for i in range(e_shnum):
        sh = e_shoff + i * e_shentsize
        sh_type = struct.unpack('<I', raw[sh+4:sh+8])[0]
        sh_link = struct.unpack('<I', raw[sh+0x18:sh+0x1C])[0]
        if sh_type == 2:
            symtab_off = struct.unpack('<I', raw[sh+0x10:sh+0x14])[0]
            symtab_size = struct.unpack('<I', raw[sh+0x14:sh+0x18])[0]
            strtab_sh = e_shoff + sh_link * e_shentsize
            strtab_off = struct.unpack('<I', raw[strtab_sh+0x10:strtab_sh+0x14])[0]
            break

    sym_name_to_idx = {}
    if symtab_off is not None:
        n_syms = symtab_size // 16
        for i in range(n_syms):
            s = symtab_off + i * 16
            name_off = struct.unpack('<I', raw[s:s+4])[0]
            name = b''
            j = strtab_off + name_off
            while raw[j] != 0:
                name += bytes([raw[j]])
                j += 1
            sym_name_to_idx[name.decode('ascii', errors='replace')] = i

    # Find symbol indices for functions called
    sym_51e88 = sym_name_to_idx.get('get_g_zyotai_s', None)
    sym_46560 = sym_name_to_idx.get('set_XAsector', None)
    sym_44980 = sym_name_to_idx.get('device_cmd_send', None)

    # Find symbol indices for global variables (for relocations)
    sym_ba088 = sym_name_to_idx.get('near_click_check_tbl0', None)
    sym_78f4 = sym_name_to_idx.get('mascot_cw_frame', None)
    sym_6ebf = sym_name_to_idx.get('now_girl', None)

    print(f"Symbols: get_g_zyotai_s={sym_51e88} set_XAsector={sym_46560} device_cmd_send={sym_44980}")
    print(f"  near_click_check_tbl0={sym_ba088} mascot_cw_frame={sym_78f4} now_girl={sym_6ebf}")

    R_MIPS_26 = 4
    R_MIPS_HI16 = 5
    R_MIPS_LO16 = 6

    def get_compiled_reloc(offset):
        for r in relocs:
            if r[0] == offset:
                return (r[1], r[2])
        return None

    # Register numbers
    Z = 0; AT = 1; V0 = 2; V1 = 3; A0 = 4; A1 = 5; A2 = 6; A3 = 7
    T0 = 8; T1 = 9; T2 = 10; T3 = 11; T4 = 12; T5 = 13; T6 = 14; T7 = 15
    T8 = 24; T9 = 25; SP = 29; RA = 31

    # Instruction encoders
    def andi(rt, rs, imm):
        return (0x0C << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)

    def addiu(rt, rs, imm):
        return (0x09 << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)

    def addu(rd, rs, rt):
        return (rs << 21) | (rt << 16) | (rd << 11) | 0x21

    def or_insn(rd, rs, rt):
        return (rs << 21) | (rt << 16) | (rd << 11) | 0x25

    def lw(rt, rs, imm):
        return (0x23 << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)

    def sw(rt, rs, imm):
        return (0x2B << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)

    def sb(rt, rs, imm):
        return (0x28 << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)

    def lbu(rt, rs, imm):
        return (0x24 << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)

    def lui(rt, imm):
        return (0x0F << 26) | (rt << 16) | (imm & 0xFFFF)

    def slt(rd, rs, rt):
        return (rs << 21) | (rt << 16) | (rd << 11) | 0x2A

    def sll(rd, rt, sa):
        return (rt << 16) | (rd << 11) | ((sa & 0x1F) << 6) | 0x00

    def jal(field=0):
        return (0x03 << 26) | (field & 0x3FFFFFF)

    def beqz(rs, offset):
        return (0x04 << 26) | (rs << 21) | (offset & 0xFFFF)

    def bnez(rs, offset):
        return (0x05 << 26) | (rs << 21) | (offset & 0xFFFF)

    def b(offset):
        return (0x04 << 26) | (offset & 0xFFFF)

    def li(rt, imm):
        if imm == 0:
            return or_insn(rt, Z, Z)
        else:
            return addiu(rt, Z, imm)

    def move(rd, rs):
        return or_insn(rd, rs, Z)

    def nop():
        return 0

    def subu(rd, rs, rt):
        return (rs << 21) | (rt << 16) | (rd << 11) | 0x23

    def sltiu(rt, rs, imm):
        return (0x0B << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)

    # Build new instruction list
    new_insns = []

    def add(word, reloc=None):
        new_insns.append((word, reloc))

    # --- Target has 67 instructions ---
    # Offset 0x00: addiu sp,sp,-56
    add(addiu(SP, SP, -56))

    # Offset 0x04: andi t7,t6,0x80  (hidden param t6, result in t7)
    add(andi(T7, T6, 0x80))

    # Offset 0x08: beqz t7, exit (exit is at instruction 62, offset=+0x3b from beqz+4)
    # Target exit at 0x8005b538 = 0x8005b440 + 0xF8
    # beqz is at 0x8005b448, delay slot at 0x8005b44c
    # branch offset = (0xF8 - 0x08 - 4) / 4 = (0xF8 - 0x0C) / 4 = 0xEC / 4 = 0x3B
    add(beqz(T7, 0x003B))

    # Offset 0x0C: sw ra,20(sp)  (delay slot)
    add(sw(RA, SP, 20))

    # Offset 0x10: lui v1,0x8012  (mascot_cw_x)
    add(lui(V1, 0), reloc=get_compiled_reloc(0x10))

    # Offset 0x14: lh v1,30978(v1)
    add(compiled[0x14 // 4], reloc=get_compiled_reloc(0x14))

    # Offset 0x18: lui v0,0x8012  (cursor_cw_x)
    add(compiled[0x18 // 4], reloc=get_compiled_reloc(0x18))

    # Offset 0x1C: lh v0,29142(v0)
    add(compiled[0x1c // 4], reloc=get_compiled_reloc(0x1c))

    # Offset 0x20: addiu t8,v1,12
    add(addiu(T8, V1, 12))

    # Offset 0x24: slt at,v0,t8
    add(slt(AT, V0, T8))

    # Offset 0x28: beqz at, exit
    # From 0x8005b468, delay slot 0x8005b46c
    # exit = 0x8005b538
    # offset = (0x8005b538 - 0x8005b46c) / 4 = 0xCC / 4 = 0x33
    add(beqz(AT, 0x0033))

    # Offset 0x2C: addiu t9,v1,-12  (delay slot of above beqz)
    add(addiu(T9, V1, -12))

    # Offset 0x30: slt at,t9,v0
    add(slt(AT, T9, V0))

    # Offset 0x34: beqz at, exit
    # From 0x8005b474, delay slot 0x8005b478
    # offset = (0x8005b538 - 0x8005b478) / 4 = 0xC0 / 4 = 0x30
    add(beqz(AT, 0x0030))

    # Offset 0x38: lui v0,0x8012  (delay slot, cursor_cw_y)
    add(compiled[0x38 // 4], reloc=get_compiled_reloc(0x38))

    # Offset 0x3C: lui v1,0x8012  (mascot_cw_y)
    add(compiled[0x3c // 4], reloc=get_compiled_reloc(0x3c))

    # Offset 0x40: lh v1,30982(v1)
    add(compiled[0x40 // 4], reloc=get_compiled_reloc(0x40))

    # Offset 0x44: lh v0,29146(v0)
    add(compiled[0x44 // 4], reloc=get_compiled_reloc(0x44))

    # Offset 0x48: addiu t0,v1,48
    add(addiu(T0, V1, 48))

    # Offset 0x4C: slt at,v0,t0
    add(slt(AT, V0, T0))

    # Offset 0x50: beqz at, exit
    # From 0x8005b490, delay slot 0x8005b494
    # offset = (0x8005b538 - 0x8005b494) / 4 = 0xA4 / 4 = 0x29
    add(beqz(AT, 0x0029))

    # Offset 0x54: slt at,v1,v0  (delay slot - same as compiled 0x54)
    add(slt(AT, V1, V0))

    # Offset 0x58: beqz at, exit
    # From 0x8005b498, delay slot 0x8005b49c
    # offset = (0x8005b538 - 0x8005b49c) / 4 = 0x9C / 4 = 0x27
    add(beqz(AT, 0x0027))

    # Offset 0x5C: lui t1,0x800f  (delay slot, pad_trig)
    add(lui(T1, 0), reloc=get_compiled_reloc(0x5c))

    # Offset 0x60: lw t1,-2328(t1)
    add(lw(T1, T1, -2328), reloc=get_compiled_reloc(0x60))

    # Offset 0x64: lui t3,0x800f  (frame_counter)
    add(lui(T3, 0), reloc=get_compiled_reloc(0x64))

    # Offset 0x68: andi t2,t1,0x20
    add(andi(T2, T1, 0x20))

    # Offset 0x6C: beqz t2, exit
    # From 0x8005b4ac, delay slot 0x8005b4b0
    # offset = (0x8005b538 - 0x8005b4b0) / 4 = 0x88 / 4 = 0x22
    add(beqz(T2, 0x0022))

    # Offset 0x70: lui t4,0x800c  (delay slot, near_click_check_tbl1)
    add(lui(T4, 0), reloc=get_compiled_reloc(0x70))

    # Offset 0x74: lw t3,-1960(t3)
    add(lw(T3, T3, -1960), reloc=get_compiled_reloc(0x74))

    # Offset 0x78: lh t4,-16656(t4)
    add(lw(T4, T4, -16656) & ~(0x3F << 26) | (0x21 << 26), reloc=get_compiled_reloc(0x78))
    # lh encoding: opcode=0x21
    # Actually lh: (0x21 << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)

    # Offset 0x7C: lui v0,0x8012  (mascot_cw_frame)
    add(lui(V0, 0), reloc=get_compiled_reloc(0x7c))

    # Offset 0x80: subu t5,t3,t4
    add(subu(T5, T3, T4))

    # Offset 0x84: sltiu at,t5,241
    add(sltiu(AT, T5, 241))

    # Offset 0x88: bnez at, exit  (if timer-threshold < 241, exit = "< 241 means too recent")
    # From 0x8005b4c8, delay slot 0x8005b4cc
    # exit = 0x8005b538
    # offset = (0x8005b538 - 0x8005b4cc) / 4 = 0x6C / 4 = 0x1B
    add(bnez(AT, 0x001B))

    # Offset 0x8C: nop (delay slot)
    add(nop())

    # Offset 0x90: lbu v0,30964(v0)  (mascot_cw_frame)
    add(compiled[0x90 // 4], reloc=get_compiled_reloc(0x90))

    # Offset 0x94: lui at,0x800f  (for now_girl)
    add(lui(AT, 0), reloc=get_compiled_reloc(0x94))

    # Offset 0x98: sb v0,-2369(at)  (now_girl = index)
    # Target: sb v0,-2369(at) = 0xa0220000 with reloc LO16 now_girl
    add(sb(V0, AT, -2369), reloc=get_compiled_reloc(0x94))

    # Wait - compiled has: lui at,0x0; sw v0,24(sp); sb v0,0(at)
    # Target has: lui at,0x800f; sb v0,-2369(at); move a0,v0
    # The reloc for sb should use the now_girl symbol (LO16)
    # But get_compiled_reloc(0x94) gives the HI16 reloc for lui at...
    # Actually the sb at 0x98 target should have LO16 reloc for now_girl
    # Let me reconsider and use the correct compiled relocs

    # Actually I realize I need to rethink the 0x94-0xb0 area.
    # Let me look at compiled vs target more carefully:
    # Compiled 0x94: lui at,0x0 (HI16 reloc for now_girl)
    # Compiled 0x98: sw v0,24(sp)  (spill a2 to stack)
    # Compiled 0x9c: sb v0,0(at) (LO16 reloc for now_girl)
    # Compiled 0xa0: jal get_g_zyotai_s
    # Compiled 0xa4: andi a0,v0,0xff (delay slot)

    # Target 0x94: lui at,0x800f (HI16 reloc for now_girl)
    # Target 0x98: sb v0,-2369(at) (LO16 reloc for now_girl)
    # Target 0x9c: move a0,v0
    # Target 0xa0: jal get_g_zyotai_s
    # Target 0xa4: sw v0,44(sp) (delay slot!)

    print("Warning: complex restructure at 0x94+ requires careful reloc handling")
    new_insns = []  # Reset and rebuild more carefully

    def add(word, reloc=None):
        new_insns.append((word, reloc))

    # --- REBUILD from scratch ---

    # Helper: lh instruction
    def lh(rt, rs, imm):
        return (0x21 << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)

    # Offset 0x00: addiu sp,sp,-56
    add(addiu(SP, SP, -56))

    # Offset 0x04: andi t7,t6,0x80
    add(andi(T7, T6, 0x80))

    # Offset 0x08: beqz t7, [exit at 0xF8]
    # From 0x8005b448+4=0x8005b44c, target 0x8005b538
    # offset = (0x8005b538 - 0x8005b44c) / 4 = 0xEC / 4 = 59 = 0x3B
    add(beqz(T7, 0x3B))

    # Offset 0x0C: sw ra,20(sp)
    add(sw(RA, SP, 20))

    # Offset 0x10: lui v1,0x8012  (mascot_cw_x HI16)
    add(lui(V1, 0), reloc=get_compiled_reloc(0x10))

    # Offset 0x14: lh v1,30978(v1)  (mascot_cw_x LO16)
    add(compiled[0x14 // 4], reloc=get_compiled_reloc(0x14))

    # Offset 0x18: lui v0,0x8012  (cursor_cw_x HI16)
    add(compiled[0x18 // 4], reloc=get_compiled_reloc(0x18))

    # Offset 0x1C: lh v0,29142(v0)  (cursor_cw_x LO16)
    add(compiled[0x1c // 4], reloc=get_compiled_reloc(0x1c))

    # Offset 0x20: addiu t8,v1,12
    add(addiu(T8, V1, 12))

    # Offset 0x24: slt at,v0,t8
    add(slt(AT, V0, T8))

    # Offset 0x28: beqz at, [exit at 0xF8]
    # From 0x8005b468+4=0x8005b46c, target 0x8005b538
    # offset = (0x8005b538 - 0x8005b46c) / 4 = 0xCC / 4 = 51 = 0x33
    add(beqz(AT, 0x33))

    # Offset 0x2C: addiu t9,v1,-12  (delay slot)
    add(addiu(T9, V1, -12))

    # Offset 0x30: slt at,t9,v0
    add(slt(AT, T9, V0))

    # Offset 0x34: beqz at, [exit at 0xF8]
    # From 0x8005b474+4=0x8005b478, target 0x8005b538
    # offset = (0x8005b538 - 0x8005b478) / 4 = 0xC0 / 4 = 48 = 0x30
    add(beqz(AT, 0x30))

    # Offset 0x38: lui v0,0x8012  (delay slot, cursor_cw_y HI16)
    add(lui(V0, 0), reloc=get_compiled_reloc(0x38))

    # Offset 0x3C: lui v1,0x8012  (mascot_cw_y HI16)
    add(lui(V1, 0), reloc=get_compiled_reloc(0x3c))

    # Offset 0x40: lh v1,30982(v1)  (mascot_cw_y LO16)
    add(compiled[0x40 // 4], reloc=get_compiled_reloc(0x40))

    # Offset 0x44: lh v0,29146(v0)  (cursor_cw_y LO16)
    add(compiled[0x44 // 4], reloc=get_compiled_reloc(0x44))

    # Offset 0x48: addiu t0,v1,48
    add(addiu(T0, V1, 48))

    # Offset 0x4C: slt at,v0,t0
    add(slt(AT, V0, T0))

    # Offset 0x50: beqz at, [exit at 0xF8]
    # From 0x8005b490+4=0x8005b494, target 0x8005b538
    # offset = (0x8005b538 - 0x8005b494) / 4 = 0xA4 / 4 = 41 = 0x29
    add(beqz(AT, 0x29))

    # Offset 0x54: slt at,v1,v0  (delay slot)
    add(slt(AT, V1, V0))

    # Offset 0x58: beqz at, [exit at 0xF8]
    # From 0x8005b498+4=0x8005b49c, target 0x8005b538
    # offset = (0x8005b538 - 0x8005b49c) / 4 = 0x9C / 4 = 39 = 0x27
    add(beqz(AT, 0x27))

    # Offset 0x5C: lui t1,0x800f  (delay slot, pad_trig HI16)
    add(lui(T1, 0), reloc=get_compiled_reloc(0x5c))

    # Offset 0x60: lw t1,-2328(t1)  (pad_trig LO16)
    add(lw(T1, T1, -2328), reloc=get_compiled_reloc(0x60))

    # Offset 0x64: lui t3,0x800f  (frame_counter HI16)
    add(lui(T3, 0), reloc=get_compiled_reloc(0x64))

    # Offset 0x68: andi t2,t1,0x20
    add(andi(T2, T1, 0x20))

    # Offset 0x6C: beqz t2, [exit at 0xF8]
    # From 0x8005b4ac+4=0x8005b4b0, target 0x8005b538
    # offset = (0x8005b538 - 0x8005b4b0) / 4 = 0x88 / 4 = 34 = 0x22
    add(beqz(T2, 0x22))

    # Offset 0x70: lui t4,0x800c  (delay slot, near_click_check_tbl1 HI16)
    add(lui(T4, 0), reloc=get_compiled_reloc(0x70))

    # Offset 0x74: lw t3,-1960(t3)  (frame_counter LO16)
    add(lw(T3, T3, -1960), reloc=get_compiled_reloc(0x74))

    # Offset 0x78: lh t4,-16656(t4)  (near_click_check_tbl1 LO16)
    add(lh(T4, T4, -16656), reloc=get_compiled_reloc(0x78))

    # Offset 0x7C: lui v0,0x8012  (mascot_cw_frame HI16)
    add(lui(V0, 0), reloc=get_compiled_reloc(0x7c))

    # Offset 0x80: subu t5,t3,t4
    add(subu(T5, T3, T4))

    # Offset 0x84: sltiu at,t5,241
    add(sltiu(AT, T5, 241))

    # Offset 0x88: bnez at, [exit at 0xF8]
    # From 0x8005b4c8+4=0x8005b4cc, target 0x8005b538
    # offset = (0x8005b538 - 0x8005b4cc) / 4 = 0x6C / 4 = 27 = 0x1B
    add(bnez(AT, 0x1B))

    # Offset 0x8C: nop  (delay slot)
    add(nop())

    # Offset 0x90: lbu v0,30964(v0)  (mascot_cw_frame LO16)
    add(compiled[0x90 // 4], reloc=get_compiled_reloc(0x90))

    # Offset 0x94: lui at,0x800f  (now_girl HI16)
    # compiled 0x94 has lui at reloc for now_girl
    add(lui(AT, 0), reloc=get_compiled_reloc(0x94))

    # Offset 0x98: sb v0,-2369(at)  (now_girl = index; LO16 reloc)
    # compiled 0x9c has sb v0,0(at) with LO16 reloc for now_girl
    add(sb(V0, AT, 0), reloc=get_compiled_reloc(0x9c))

    # Offset 0x9C: move a0,v0  (copy index to argument register)
    add(or_insn(A0, V0, Z))

    # Offset 0xA0: jal get_g_zyotai_s
    add(jal(), reloc=get_compiled_reloc(0xa0))

    # Offset 0xA4: sw v0,44(sp)  (delay slot: save original index to stack)
    # sp+44 = sp+0x2C
    add(sw(V0, SP, 44))

    # Offset 0xA8: lw a2,44(sp)  (reload saved index as a2)
    add(lw(A2, SP, 44))

    # Offset 0xAC: andi t6,v0,0x80  (test return value bit 7)
    add(andi(T6, V0, 0x80))

    # Offset 0xB0: beqz t6, [move v1,zero at 0xC0]
    # From 0x8005b4f0+4=0x8005b4f4, target 0x8005b500
    # offset = (0x8005b500 - 0x8005b4f4) / 4 = 0x0C / 4 = 3
    add(beqz(T6, 0x0003))

    # Offset 0xB4: sll t8,a2,4  (delay slot: a2 * 16, common for both branches)
    add(sll(T8, A2, 4))

    # Offset 0xB8: b [sll t0 at 0xC4]
    # From 0x8005b4f8+4=0x8005b4fc, target 0x8005b504
    # offset = (0x8005b504 - 0x8005b4fc) / 4 = 0x08 / 4 = 2
    add(b(0x0002))

    # Offset 0xBC: li v1,1  (delay slot: v1_bool = 1)
    add(li(V1, 1))

    # Offset 0xC0: move v1,zero  (v1_bool = 0, else branch)
    add(move(V1, Z))

    # Offset 0xC4: sll t0,v1,3  (v1_bool * 8)
    add(sll(T0, V1, 3))

    # Offset 0xC8: lui t2,0x800c  (near_click_check_tbl0 base HI16)
    add(lui(T2, 0), reloc=get_compiled_reloc(0xb0))

    # Offset 0xCC: addiu t2,t2,-24440  (near_click_check_tbl0 base LO16)
    add(addiu(T2, T2, -24440), reloc=get_compiled_reloc(0xd0))

    # Offset 0xD0: addu t1,t8,t0  (t8 + t0 = a2*16 + v1_bool*8)
    add(addu(T1, T8, T0))

    # Offset 0xD4: addu v0,t1,t2  (v0 = base + offset)
    add(addu(V0, T1, T2))

    # Offset 0xD8: lw a0,0(v0)  (entry->x)
    add(lw(A0, V0, 0))

    # Offset 0xDC: lw a1,4(v0)  (entry->y)
    add(lw(A1, V0, 4))

    # Offset 0xE0: jal set_XAsector
    add(jal(), reloc=get_compiled_reloc(0xe0))

    # Offset 0xE4: nop (delay slot)
    add(nop())

    # Offset 0xE8: jal device_cmd_send
    add(jal(), reloc=get_compiled_reloc(0xe8))

    # Offset 0xEC: li a0,768  (delay slot: a0 = 0x300)
    add(li(A0, 768))

    # Offset 0xF0: b [lw ra at 0xF8]
    # Wait, target has: b 0x8005b53c from 0x8005b530, delay slot li v0,1
    # From 0x8005b530+4=0x8005b534, target 0x8005b53c = 0x8005b440+0xFC
    # offset = (0x8005b53c - 0x8005b534) / 4 = 0x08 / 4 = 2
    add(b(0x0002))

    # Offset 0xF4: li v0,1  (delay slot: return value 1)
    add(li(V0, 1))

    # Offset 0xF8: move v0,zero  (exit: return 0)
    add(move(V0, Z))

    # Offset 0xFC: lw ra,20(sp)
    add(lw(RA, SP, 20))

    # Offset 0x100: addiu sp,sp,56
    add(addiu(SP, SP, 56))

    # Offset 0x104: jr ra
    add(0x03e00008)

    # Offset 0x108: nop
    add(nop())

    n_new = len(new_insns)
    print(f"New function: {n_new} instructions ({n_new*4} bytes)")

    expected = 67
    if n_new != expected:
        print(f"WARNING: Expected {expected} instructions, got {n_new}")

    # Fix up relocs: use compiled relocs where available
    # For near_click_check_tbl0, we need to find the right compiled relocs
    # Compiled 0xb0 has: lui t2,0x0 with HI16 reloc for near_click_check_tbl0
    # Compiled 0xd0 has: addiu t1,t1,0 (really addiu t2,t2,-24440) with LO16 reloc for near_click_check_tbl0
    # Wait, compiled 0xd0: addiu t1,t1,0 -- this is addiu(T1, T1, 0) with LO16 reloc
    # We want addiu(T2, T2, -24440) with LO16 reloc for near_click_check_tbl0

    # Build new ELF
    old_text_size = text_size
    new_text_size = n_new * 4
    size_delta = new_text_size - old_text_size

    old_rel_size = rel_text_size if rel_text_size else 0

    new_relocs_list = []
    new_text_bytes = bytearray()
    for i, (word, reloc) in enumerate(new_insns):
        new_text_bytes += struct.pack('<I', word)
        if reloc is not None:
            sym_idx, rtype = reloc
            if sym_idx is not None:
                new_relocs_list.append((i * 4, sym_idx, rtype))

    new_rel_content = bytearray()
    for r_offset, r_sym, r_type in new_relocs_list:
        r_info = (r_sym << 8) | r_type
        new_rel_content += struct.pack('<II', r_offset, r_info)

    new_rel_size = len(new_rel_content)
    print(f"New text: {len(new_text_bytes)} bytes")
    print(f"New relocations: {len(new_relocs_list)}")

    data = bytearray(raw)

    old_e_shoff = struct.unpack('<I', raw[0x20:0x24])[0]
    new_e_shoff = old_e_shoff + size_delta if old_e_shoff > text_off else old_e_shoff
    struct.pack_into('<I', data, 0x20, new_e_shoff)
    e_shoff = new_e_shoff

    for i in range(e_shnum):
        sh_i = old_e_shoff + i * e_shentsize
        sh_off_i = struct.unpack('<I', data[sh_i+0x10:sh_i+0x14])[0]
        if sh_off_i > text_off and i != text_sh_idx:
            new_sh_off = sh_off_i + size_delta
            struct.pack_into('<I', data, sh_i + 0x10, new_sh_off)

    before_text = bytes(data[:text_off])
    after_text = bytes(data[text_off + old_text_size:])
    data = bytearray(before_text + bytes(new_text_bytes) + after_text)

    sh = new_e_shoff + text_sh_idx * e_shentsize
    struct.pack_into('<I', data, sh + 0x14, new_text_size)

    if rel_text_sh_idx is not None:
        rel_sh = new_e_shoff + rel_text_sh_idx * e_shentsize
        new_rel_text_off = struct.unpack('<I', data[rel_sh+0x10:rel_sh+0x14])[0]

        old_rel_at = new_rel_text_off
        old_rel_end = old_rel_at + old_rel_size
        new_rel_delta = new_rel_size - old_rel_size

        before_rel = bytes(data[:old_rel_at])
        after_rel = bytes(data[old_rel_end:])
        data = bytearray(before_rel + bytes(new_rel_content) + after_rel)

        rel_sh = new_e_shoff + rel_text_sh_idx * e_shentsize
        struct.pack_into('<I', data, rel_sh + 0x14, new_rel_size)

        if new_rel_delta != 0:
            for i in range(e_shnum):
                sh_i = new_e_shoff + i * e_shentsize
                sh_off_i = struct.unpack('<I', data[sh_i+0x10:sh_i+0x14])[0]
                if sh_off_i > old_rel_at and i != rel_text_sh_idx:
                    struct.pack_into('<I', data, sh_i + 0x10, sh_off_i + new_rel_delta)

            cur_e_shoff = struct.unpack('<I', data[0x20:0x24])[0]
            if cur_e_shoff > old_rel_at:
                struct.pack_into('<I', data, 0x20, cur_e_shoff + new_rel_delta)

    with open(sys.argv[2], 'wb') as f:
        f.write(data)

    print(f"Written {len(data)} bytes to {sys.argv[2]}")
    print("Done.")


if __name__ == '__main__':
    main()
