#!/usr/bin/env python3
"""Custom post-processing for GsTMDfastTF3NL (0x800B42D0).

Transforms GCC 2.6.0 -O1 + fill-epilogue output to match target.

Transformations:
1. Prologue: move lui v1/lw v1 (libapi_startintrvsync_ptr_8cf4 load) before sw ra/sw s0
   GCC: [sw ra, sw s0, lui v1, lw v1] -> Target: [lui v1, lw v1, sw ra, sw s0]
2. Epilogue register: change li v0,1; sh v0 -> li v1,1; sh v1
3. Epilogue order: move lw ra/lw s0 before lui v0/addiu v0 (return value)
   GCC: [lui v0, addiu v0, lw ra, lw s0] -> Target: [lw ra, lw s0, lui v0, addiu v0]
"""
import struct
import sys


def read_u32(data, off):
    return struct.unpack('<I', data[off:off + 4])[0]


def write_u32(data, off, val):
    struct.pack_into('<I', data, off, val)


def get_opcode(instr):
    return (instr >> 26) & 0x3F


def get_rs(instr):
    return (instr >> 21) & 0x1F


def get_rt(instr):
    return (instr >> 16) & 0x1F


def set_rt(instr, rt):
    return (instr & ~(0x1F << 16)) | ((rt & 0x1F) << 16)


def find_section(data, name_bytes):
    """Find a section by name. Returns (data_offset, size, sh_offset)."""
    e_shoff = struct.unpack('<I', data[0x20:0x24])[0]
    e_shentsize = struct.unpack('<H', data[0x2e:0x30])[0]
    e_shnum = struct.unpack('<H', data[0x30:0x32])[0]
    e_shstrndx = struct.unpack('<H', data[0x32:0x34])[0]

    shstr_entry = e_shoff + e_shstrndx * e_shentsize
    shstr_off = struct.unpack('<I', data[shstr_entry + 0x10:shstr_entry + 0x14])[0]

    for i in range(e_shnum):
        sh = e_shoff + i * e_shentsize
        name_idx = struct.unpack('<I', data[sh:sh + 4])[0]
        j = shstr_off + name_idx
        sname = b''
        while data[j] != 0:
            sname += bytes([data[j]])
            j += 1
        if sname == name_bytes:
            offset = struct.unpack('<I', data[sh + 0x10:sh + 0x14])[0]
            size = struct.unpack('<I', data[sh + 0x14:sh + 0x18])[0]
            return offset, size, sh
    return None, None, None


def patch_reloc_offset(data, rel_off, rel_size, old_offset, new_offset):
    """Find reloc entry with r_offset==old_offset and update to new_offset."""
    for i in range(rel_size // 8):
        entry_off = rel_off + i * 8
        r_offset = struct.unpack('<I', data[entry_off:entry_off + 4])[0]
        if r_offset == old_offset:
            struct.pack_into('<I', data, entry_off, new_offset)
            return True
    return False


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} input.o output.o")
        sys.exit(1)

    with open(sys.argv[1], 'rb') as f:
        data = bytearray(f.read())

    text_off, text_size, text_sh = find_section(data, b'.text')
    if text_off is None:
        print("ERROR: .text not found", file=sys.stderr)
        sys.exit(1)

    n = text_size // 4
    print(f"Input: {n} instructions ({text_size} bytes)")

    def get(i):
        return read_u32(data, text_off + i * 4)

    def put(i, val):
        write_u32(data, text_off + i * 4, val)

    # Expected GCC -O1 + fill-epilogue layout (29 instructions):
    # idx  0 (0x00): addiu sp,sp,-24    (0x27bdffe8)
    # idx  1 (0x04): sw ra,20(sp)       (0xafbf0014)
    # idx  2 (0x08): sw s0,16(sp)       (0xafb00010)
    # idx  3 (0x0c): lui v1,0x0         [HI16 libapi_startintrvsync_ptr_8cf4]
    # idx  4 (0x10): lw v1,0(v1)        [LO16 libapi_startintrvsync_ptr_8cf4]
    # idx  5 (0x14): li v0,263          (0x24020107)
    # idx  6 (0x18): sw v0,0(v1)        (0xac620000)
    # idx  7 (0x1c): jal ChangeClearPAD  [reloc]
    # idx  8 (0x20): move a0,zero       (0x00002021)
    # idx  9 (0x24): li a0,3            (0x24040003)
    # idx 10 (0x28): jal ChangeClearRCnt  [reloc]
    # idx 11 (0x2c): move a1,zero       (0x00002821)
    # idx 12 (0x30): lui s0,0x0         [HI16 libapi_setintrvsync_var_8cf8]
    # idx 13 (0x34): addiu s0,s0,0      [LO16 libapi_setintrvsync_var_8cf8]
    # idx 14 (0x38): move a0,s0         (0x02002021)
    # idx 15 (0x3c): jal vsync_intr_table_clear  [reloc]
    # idx 16 (0x40): li a1,2            (0x24050002)
    # idx 17 (0x44): lui a1,0x0         [HI16 D_800B437C]
    # idx 18 (0x48): addiu a1,a1,0      [LO16 D_800B437C]
    # idx 19 (0x4c): jal InterruptCallback  [reloc]
    # idx 20 (0x50): move a0,zero       (0x00002021)
    # idx 21 (0x54): li v0,1            (0x24020001)
    # idx 22 (0x58): sh v0,0(s0)        (0xa6020000)
    # idx 23 (0x5c): lui v0,0x0         [HI16 D_800B43C4]
    # idx 24 (0x60): addiu v0,v0,0      [LO16 D_800B43C4]
    # idx 25 (0x64): lw ra,20(sp)       (0x8fbf0014)
    # idx 26 (0x68): lw s0,16(sp)       (0x8fb00010)
    # idx 27 (0x6c): jr ra              (0x03e00008)
    # idx 28 (0x70): addiu sp,sp,24     (0x27bd0018)

    # Verify key instructions
    assert get(0) == 0x27bdffe8, f"idx 0: expected addiu sp,-24, got {get(0):#010x}"
    assert get(1) == 0xafbf0014, f"idx 1: expected sw ra,20(sp), got {get(1):#010x}"
    assert get(2) == 0xafb00010, f"idx 2: expected sw s0,16(sp), got {get(2):#010x}"
    assert get_opcode(get(3)) == 0x0F and get_rt(get(3)) == 3, \
        f"idx 3: expected lui v1, got {get(3):#010x}"
    assert get_opcode(get(4)) == 0x23 and get_rs(get(4)) == 3, \
        f"idx 4: expected lw v1, got {get(4):#010x}"
    assert get(5) == 0x24020107, f"idx 5: expected li v0,263, got {get(5):#010x}"
    assert get(21) == 0x24020001, f"idx 21: expected li v0,1, got {get(21):#010x}"
    assert get(22) == 0xa6020000, f"idx 22: expected sh v0,0(s0), got {get(22):#010x}"
    assert get(25) == 0x8fbf0014, f"idx 25: expected lw ra, got {get(25):#010x}"
    assert get(26) == 0x8fb00010, f"idx 26: expected lw s0, got {get(26):#010x}"
    assert get(27) == 0x03e00008, f"idx 27: expected jr ra, got {get(27):#010x}"
    assert get(28) == 0x27bd0018, f"idx 28: expected addiu sp,24, got {get(28):#010x}"

    # Find .rel.text
    rel_off, rel_size, _ = find_section(data, b'.rel.text')

    # ========================================================================
    # FIX 1: Prologue reorder
    # GCC:    [sw ra(1), sw s0(2), lui v1(3), lw v1(4)]
    # Target: [lui v1(1), lw v1(2), sw ra(3), sw s0(4)]
    # ========================================================================
    sw_ra = get(1)   # 0xafbf0014
    sw_s0 = get(2)   # 0xafb00010
    lui_v1 = get(3)  # lui v1,X [reloc at 0x0c]
    lw_v1 = get(4)   # lw v1,Y(v1) [reloc at 0x10]

    put(1, lui_v1)   # lui v1 -> offset 0x04
    put(2, lw_v1)    # lw v1 -> offset 0x08
    put(3, sw_ra)    # sw ra -> offset 0x0c
    put(4, sw_s0)    # sw s0 -> offset 0x10

    # Update relocations: lui moved 0x0c->0x04, lw moved 0x10->0x08
    if rel_off:
        patch_reloc_offset(data, rel_off, rel_size, 0x0c, 0x04)
        patch_reloc_offset(data, rel_off, rel_size, 0x10, 0x08)

    print("Fix 1: Prologue reorder done")

    # ========================================================================
    # FIX 2: Epilogue register swap (v0 -> v1 for constant 1)
    # GCC:    li v0,1 (0x24020001); sh v0,0(s0) (0xa6020000)
    # Target: li v1,1 (0x24030001); sh v1,0(s0) (0xa6030000)
    # ========================================================================
    li_v0 = get(21)  # li v0,1 = addiu v0,zero,1
    sh_v0 = get(22)  # sh v0,0(s0)

    # Change rt from v0(2) to v1(3)
    put(21, set_rt(li_v0, 3))   # li v1,1
    put(22, set_rt(sh_v0, 3))   # sh v1,0(s0)

    print("Fix 2: Epilogue register v0->v1 done")

    # ========================================================================
    # FIX 3: Epilogue reorder
    # GCC:    [lui v0(23), addiu v0(24), lw ra(25), lw s0(26)]
    # Target: [lw ra(23), lw s0(24), lui v0(25), addiu v0(26)]
    # ========================================================================
    lui_v0 = get(23)   # lui v0,X [reloc at 0x5c]
    addiu_v0 = get(24) # addiu v0,v0,Y [reloc at 0x60]
    lw_ra = get(25)    # lw ra,20(sp)
    lw_s0 = get(26)    # lw s0,16(sp)

    put(23, lw_ra)     # lw ra -> offset 0x5c
    put(24, lw_s0)     # lw s0 -> offset 0x60
    put(25, lui_v0)    # lui v0 -> offset 0x64
    put(26, addiu_v0)  # addiu v0 -> offset 0x68

    # Update relocations: lui moved 0x5c->0x64, addiu moved 0x60->0x68
    if rel_off:
        patch_reloc_offset(data, rel_off, rel_size, 0x5c, 0x64)
        patch_reloc_offset(data, rel_off, rel_size, 0x60, 0x68)

    print("Fix 3: Epilogue reorder done")

    with open(sys.argv[2], 'wb') as f:
        f.write(bytes(data))

    print(f"Done: {sys.argv[2]}")


if __name__ == '__main__':
    main()
