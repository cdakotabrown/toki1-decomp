#!/usr/bin/env python3
"""
Custom script for _spu_interruptDMA - TARGETED transform.

The compiled IDO output loads _spu_RXX TWICE (for lhu base and sh base separately).
The target loads it ONCE (into v1) and uses it for both lhu and sh.
The target also uses different register names and has additional instructions.

Transforms applied:
1. Drop second _spu_RXX load (compiled idx 11+13); repurpose idx 11 for D_80131634 HI16
2. Remap registers: t7->v1 (struct ptr), t8->v0 (field), t9->v0, t0->t7 (fp hi->lo), t1->t7 (fp)  
3. Fix lhu/sh base register (t7->v1)
4. Fix sh base register (t0->v1)  
5. Insert andi v0,v0,0xffff after andi v0,v0,0xffcf
6. Replace beqz delay slot ori with nop; move ori to else-branch
7. Insert double-load of D_80131634 (lui t8 + lw t8) before jalr
"""

import struct
import sys

def read32(data, off):
    return struct.unpack_from('<I', data, off)[0]

def write32(data, off, val):
    struct.pack_into('<I', data, off, val)

def get_sections(elf_data):
    """Get all sections as list of (name, file_offset, size, sh_file_offset, index)."""
    e_shoff = struct.unpack_from('<I', elf_data, 0x20)[0]
    e_shnum = struct.unpack_from('<H', elf_data, 0x30)[0]
    e_shstrndx = struct.unpack_from('<H', elf_data, 0x32)[0]
    shstr_sh = e_shoff + e_shstrndx * 0x28
    shstr_off = struct.unpack_from('<I', elf_data, shstr_sh + 0x10)[0]
    sections = []
    for i in range(e_shnum):
        sh = e_shoff + i * 0x28
        nm_idx = struct.unpack_from('<I', elf_data, sh)[0]
        sh_type = struct.unpack_from('<I', elf_data, sh + 4)[0]
        sh_offset = struct.unpack_from('<I', elf_data, sh + 0x10)[0]
        sh_size = struct.unpack_from('<I', elf_data, sh + 0x14)[0]
        j = shstr_off + nm_idx
        name = b''
        while j < len(elf_data) and elf_data[j] != 0:
            name += bytes([elf_data[j]])
            j += 1
        sections.append((name.decode('ascii','replace'), sh_offset, sh_size, sh, i))
    return sections

def get_rs(i): return (i>>21)&0x1f
def get_rt(i): return (i>>16)&0x1f
def get_rd(i): return (i>>11)&0x1f
def set_rs(i,r): return (i&~(0x1f<<21))|((r&0x1f)<<21)
def set_rt(i,r): return (i&~(0x1f<<16))|((r&0x1f)<<16)

REG = {'v0':2,'v1':3,'a0':4,'a1':5,'t6':14,'t7':15,'t8':24,'t9':25,'t0':8,'t1':9,'ra':31,'sp':29}
V0,V1,T6,T7,T8,T9,T0,T1,A0,A1,RA,SP = (REG[k] for k in ['v0','v1','t6','t7','t8','t9','t0','t1','a0','a1','ra','sp'])

def remap_reg(insn, remap):
    """Apply register remapping to instruction (all fields)."""
    for old, new in remap.items():
        if get_rs(insn) == old: insn = set_rs(insn, new)
        if get_rt(insn) == old: insn = set_rt(insn, new)
    return insn

def andi(rt, rs, imm):
    return (0x0c << 26) | (rs << 21) | (rt << 16) | (imm & 0xffff)

def branch_off(insn):
    o = insn & 0xffff
    return o - 0x10000 if o & 0x8000 else o

def set_branch_off(insn, off):
    return (insn & 0xffff0000) | (off & 0xffff)

def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} input.o output.o")
        sys.exit(1)
    
    with open(sys.argv[1], 'rb') as f:
        elf = bytearray(f.read())
    
    sections = get_sections(elf)
    text_sec = next((s for s in sections if s[0]=='.text'), None)
    rtext_sec = next((s for s in sections if s[0]=='.rel.text'), None)
    
    _, t_off, t_size, t_sh, _ = text_sec
    
    # Read original 32 instructions
    O = [read32(elf, t_off + i*4) for i in range(t_size//4)]
    assert len(O) == 32, f"Expected 32 instructions, got {len(O)}"
    
    # Read original relocations
    orig_relocs = []
    if rtext_sec:
        _, r_off, r_size, r_sh, _ = rtext_sec
        for i in range(r_size // 8):
            ro = struct.unpack_from('<I', elf, r_off + i*8)[0]
            ri = struct.unpack_from('<I', elf, r_off + i*8 + 4)[0]
            orig_relocs.append((ro, ri))
    
    # Original reloc map: insn_idx -> (file_off, r_info)
    rmap = {ro//4: (ro, ri) for ro, ri in orig_relocs}
    
    # -------------------------------------------------------
    # Verify assumptions about compiled instruction sequence:
    # idx 9:  lui t7  [_spu_RXX HI16]
    # idx 10: lw  t7  [_spu_RXX LO16]
    # idx 11: lui t0  [_spu_RXX HI16 -- second load!]
    # idx 12: lhu t8, 426(t7)
    # idx 13: lw  t0  [_spu_RXX LO16]
    # idx 14: andi t9, t8, 0xffcf
    # idx 15: lui t1  [D_80131634 HI16]
    # idx 16: sh  t9, 426(t0)
    # idx 17: lw  t1  [D_80131634 LO16]
    # idx 18: lui a0, 0xf000
    # idx 19: beqz t1, +5
    # idx 20: ori a0, a0, 9   (beqz delay slot)
    # idx 21: jalr t1
    # idx 22: nop
    # idx 23: b +4
    # idx 24: lw ra, 20(sp)   (b delay slot)
    # idx 25: jal func_B59B0
    # idx 26: li a1, 32
    # idx 27: lw ra, 20(sp)
    # idx 28: addiu sp, sp, 24
    # idx 29: jr ra
    # idx 30: nop
    # idx 31: nop
    # -------------------------------------------------------
    
    # Get symbol indices from relocs
    # rmap[9][1] >> 8 = sym for _spu_RXX
    # rmap[15][1] >> 8 = sym for D_80131634
    sym_E3170 = rmap[9][1] >> 8
    r_type_HI16 = rmap[9][1] & 0xff   # 5 = R_MIPS_HI16
    r_type_LO16 = rmap[10][1] & 0xff  # 6 = R_MIPS_LO16
    sym_31634 = rmap[15][1] >> 8
    
    # -------------------------------------------------------
    # Build target 36-instruction sequence
    # -------------------------------------------------------
    # new[0..8] = O[0..8] (cross-prologue + calls) -- same
    # new[9]  = O[9] remap t7->v1  [_spu_RXX HI16]
    # new[10] = O[10] remap t7->v1 [_spu_RXX LO16]
    # new[11] = O[11] remap t0->t7 [D_80131634 HI16] ← change reloc from E3170 to 31634
    # new[12] = O[12] remap base t7->v1, t8->v0       (lhu v0, 426(v1))
    # new[13] = nop  (drop second _spu_RXX LO load)
    # new[14] = O[14] remap t9->v0, t8->v0            (andi v0,v0,0xffcf)
    # new[15] = andi v0,v0,0xffff  [INSERTED]
    # new[16] = O[16] remap t9->v0, t0->v1            (sh v0,426(v1))
    # new[17] = O[17] remap t1->t7  [D_80131634 LO16] ← reloc still sym_31634
    # new[18] = nop   (O[18] was lui a0 -- move to new[28])
    # new[19] = O[19] remap t1->t7, fix branch offset  (beqz t7, +8)
    # new[20] = nop   (O[20] was ori a0,a0,9 delay slot -- move to new[29])
    # new[21] = O[11] remap t0->t8  [D_80131634 HI16]  ← same sym_31634
    # new[22] = O[17] remap t1->t8  [D_80131634 LO16]  ← same sym_31634
    # new[23] = nop  [INSERTED]
    # new[24] = O[21] remap t1->t8  (jalr t8)
    # new[25] = O[22]  (nop)
    # new[26] = O[23] fix branch offset  (b +6)
    # new[27] = O[24]  (lw ra, 20(sp))
    # new[28] = O[18]  (lui a0, 0xf000) -- moved from new[18]
    # new[29] = O[20]  (ori a0, a0, 9)  -- moved from delay slot new[20]
    # new[30] = O[25]  (jal func_B59B0)
    # new[31] = O[26]  (li a1, 32)
    # new[32] = O[27]  (lw ra, 20(sp))
    # new[33] = O[28]  (addiu sp, sp, 24)
    # new[34] = O[29]  (jr ra)
    # new[35] = O[30]  (nop)
    
    # new relocs (by new instruction index):
    # new[0]:  libspu_spu_interruptdma_var_bd20 HI16 = rmap[0]
    # new[1]:  libspu_spu_interruptdma_var_bd20 LO16 = rmap[1]
    # new[5]:  func_800956F0   = rmap[5]
    # new[7]:  func_800956F0   = rmap[7]
    # new[9]:  _spu_RXX HI16 = rmap[9] (_spu_RXX sym)
    # new[10]: _spu_RXX LO16 = rmap[10]
    # new[11]: D_80131634 HI16 = (sym_31634 << 8) | r_type_HI16
    # new[17]: D_80131634 LO16 = (sym_31634 << 8) | r_type_LO16
    # new[21]: D_80131634 HI16 = (sym_31634 << 8) | r_type_HI16
    # new[22]: D_80131634 LO16 = (sym_31634 << 8) | r_type_LO16
    # new[30]: func_B59B0      = rmap[25]
    
    def mk_reloc(sym, rtype):
        return (sym << 8) | rtype
    
    r_31634_HI = mk_reloc(sym_31634, r_type_HI16)
    r_31634_LO = mk_reloc(sym_31634, r_type_LO16)
    
    # Build instruction + reloc list
    new_seq = []  # list of (insn, reloc_info_or_None)
    
    def add(insn, reloc=None):
        new_seq.append((insn, reloc))
    
    def copy(idx, remap=None, reloc_override=None):
        ins = O[idx]
        if remap:
            ins = remap_reg(ins, remap)
        reloc = reloc_override if reloc_override is not None else (rmap[idx][1] if idx in rmap else None)
        add(ins, reloc)
    
    # new[0..8]
    for i in range(9):
        copy(i)
    
    # new[9] = lui v1, _spu_RXX
    copy(9, {T7: V1})  # reloc = _spu_RXX HI16
    
    # new[10] = lw v1, _spu_RXX
    copy(10, {T7: V1})  # reloc = _spu_RXX LO16
    
    # new[11] = lui t7, D_80131634 (repurpose compiled[11] slot with new reloc)
    ins11 = remap_reg(O[11], {T0: T7})
    add(ins11, r_31634_HI)
    
    # new[12] = lhu v0, 426(v1)
    copy(12, {T8: V0, T7: V1})  # no reloc on this insn
    
    # new[13] = nop (drop lw t0 = _spu_RXX second load)
    add(0, None)
    
    # new[14] = andi v0, v0, 0xffcf
    copy(14, {T9: V0, T8: V0})  # no reloc
    
    # new[15] = andi v0, v0, 0xffff [INSERTED]
    add(andi(V0, V0, 0xffff), None)
    
    # new[16] = sh v0, 426(v1)
    copy(16, {T9: V0, T0: V1})  # no reloc
    
    # new[17] = lw t7, D_80131634 (O[17] was lw t1 - remap + fix reloc)
    ins17 = remap_reg(O[17], {T1: T7})
    add(ins17, r_31634_LO)
    
    # new[18] = nop (O[18] was lui a0 -- moved to new[28])
    add(0, None)
    
    # new[19] = beqz t7, +8 (O[19] was beqz t1, +5)
    # From new[19], +8 words forward = new[27+1] = new[28] = lui a0
    # Actually: branch offset in MIPS = # of instructions from NEXT insn
    # new[19] + offset*4 = target; next insn = new[20]
    # We want to branch to new[28]: offset = 28 - 20 = 8 ✓
    ins19 = set_rs(O[19], T7)   # BEQ rs, rt, offset: rs is the test register
    ins19 = set_branch_off(ins19, 8)
    add(ins19, None)
    
    # new[20] = nop (O[20] was ori a0,a0,9 in delay slot -- moved to new[29])
    add(0, None)
    
    # new[21] = lui t8, D_80131634 HI [INSERTED reload]
    ins21 = remap_reg(O[11], {T0: T8})  # copy compiled[11] with t0->t8
    add(ins21, r_31634_HI)
    
    # new[22] = lw t8, D_80131634 LO [INSERTED reload]
    ins22 = remap_reg(O[17], {T1: T8})
    add(ins22, r_31634_LO)
    
    # new[23] = nop [INSERTED]
    add(0, None)
    
    # new[24] = jalr t8 (O[21] was jalr t1)
    copy(21, {T1: T8})  # no reloc on jalr
    
    # new[25] = nop (O[22])
    copy(22)
    
    # new[26] = b +6 (O[23] was b +4)
    # From new[26], +6 words = new[33] = addiu sp
    # new[26] -> next = new[27], target = new[33]: 33-27 = 6 ✓
    ins26 = set_branch_off(O[23], 6)
    add(ins26, None)
    
    # new[27] = lw ra, 20(sp) (O[24])
    copy(24)
    
    # new[28] = lui a0, 0xf000 (O[18] moved here)
    copy(18)
    
    # new[29] = ori a0, a0, 9 (O[20] moved here)
    copy(20)
    
    # new[30] = jal func_B59B0 (O[25])
    copy(25)
    
    # new[31] = li a1, 32 (O[26])
    copy(26)
    
    # new[32] = lw ra, 20(sp) (O[27])
    copy(27)
    
    # new[33] = addiu sp, sp, 24 (O[28])
    copy(28)
    
    # new[34] = jr ra (O[29])
    copy(29)
    
    # new[35] = nop (O[30])
    copy(30)
    
    assert len(new_seq) == 36, f"Expected 36, got {len(new_seq)}"
    
    # Build new .text bytes
    new_text = bytearray()
    for ins, _ in new_seq:
        new_text += struct.pack('<I', ins)
    
    # Build new relocation list
    new_relocs = []
    for idx, (ins, rel) in enumerate(new_seq):
        if rel is not None:
            new_relocs.append((idx * 4, rel))
    
    # Expand ELF .text section by 16 bytes (4 inserted instructions)
    delta = len(new_text) - t_size  # should be +16
    assert delta == 16, f"Expected +16 bytes delta, got {delta}"

    insert_pos = t_off + t_size  # end of old .text
    new_elf = bytearray(elf[:t_off] + new_text + elf[insert_pos:])

    # Update all section headers with offsets >= insert_pos (do this FIRST)
    e_shoff_orig = struct.unpack_from('<I', new_elf, 0x20)[0]
    e_shnum = struct.unpack_from('<H', new_elf, 0x30)[0]

    # Compute updated e_shoff
    e_shoff_new = e_shoff_orig + delta if e_shoff_orig >= insert_pos else e_shoff_orig

    # Update section header table offsets
    for i in range(e_shnum):
        sh = e_shoff_orig + i * 0x28
        sh_type = struct.unpack_from('<I', new_elf, sh + 4)[0]
        sh_offset = struct.unpack_from('<I', new_elf, sh + 0x10)[0]
        if sh_offset >= insert_pos and sh_type != 0:
            struct.pack_into('<I', new_elf, sh + 0x10, sh_offset + delta)

    # Update e_shoff in ELF header
    if e_shoff_orig >= insert_pos:
        struct.pack_into('<I', new_elf, 0x20, e_shoff_new)

    # Now update .text section size using the NEW section header table location
    # t_sh was the original file offset of .text's sh; it's now shifted if >= insert_pos
    t_sh_new = t_sh + delta if t_sh >= insert_pos else t_sh
    struct.pack_into('<I', new_elf, t_sh_new + 0x14, len(new_text))
    
    # Update symbol sizes: .text section symbol and _spu_interruptDMA FUNC symbol
    new_sections2 = get_sections(new_elf)
    sym_sec = next((s for s in new_sections2 if s[0]=='.symtab'), None)
    if sym_sec:
        _, sym_off, sym_size, sym_sh, _ = sym_sec
        num_syms = sym_size // 16
        for i in range(num_syms):
            sym_sh_ndx = struct.unpack_from('<H', new_elf, sym_off + i*16 + 14)[0]
            sym_st_size = struct.unpack_from('<I', new_elf, sym_off + i*16 + 8)[0]
            # Update SECTION symbol for .text (size = text section size)
            st_info = new_elf[sym_off + i*16 + 12]
            st_type = st_info & 0xf
            st_bind = (st_info >> 4) & 0xf
            if st_type == 3 and sym_sh_ndx == 4:  # STT_SECTION for .text
                struct.pack_into('<I', new_elf, sym_off + i*16 + 8, len(new_text))
            elif st_type == 2 and sym_sh_ndx == 4:  # STT_FUNC in .text
                struct.pack_into('<I', new_elf, sym_off + i*16 + 8, len(new_text) - 4)

    # Rewrite .rel.text with new relocs
    new_sections = get_sections(new_elf)
    rtext_new = next((s for s in new_sections if s[0]=='.rel.text'), None)
    if rtext_new:
        _, rn_off, rn_size, rn_sh, _ = rtext_new
        # Write new relocs
        for i, (ro, ri) in enumerate(new_relocs):
            struct.pack_into('<I', new_elf, rn_off + i*8, ro)
            struct.pack_into('<I', new_elf, rn_off + i*8 + 4, ri)
        # Update .rel.text size
        struct.pack_into('<I', new_elf, rn_sh + 0x14, len(new_relocs) * 8)
        # Zero out any extra old reloc bytes
        old_reloc_bytes = rn_size
        new_reloc_bytes = len(new_relocs) * 8
        if new_reloc_bytes < old_reloc_bytes:
            for i in range(new_reloc_bytes, old_reloc_bytes):
                new_elf[rn_off + i] = 0
    
    with open(sys.argv[2], 'wb') as f:
        f.write(new_elf)
    
    print(f"Transform complete: {len(O)} -> {len(new_seq)} instructions, {t_size} -> {len(new_text)} bytes")

if __name__ == '__main__':
    main()
