#!/usr/bin/env python3
"""Custom post-processing for func_80086D60.

GCC 2.6.0 -O2 produces correct instruction sequence but with different
prologue ordering. Identical structure to func_80086DF4 (sibling pair).

Compiled prologue (offsets 0x00-0x18):
  0x00: addiu sp,sp,-32
  0x04: lui v0,0x10         (counter init high)
  0x08: sw v0,16(sp)        (counter init store)
  0x0c: lui v0,HI16(libpress_mdec_in_ptr_0990)   [RELOC HI16]
  0x10: lw v0,LO16(libpress_mdec_in_ptr_0990)(v0) [RELOC LO16]
  0x14: sw ra,24(sp)
  0x18: lw v0,0(v0)         (deref pointer)

Target prologue (offsets 0x00-0x18):
  0x00: lui v1,HI16(libpress_mdec_in_ptr_0990)   [cross-prologue, stripped later]
  0x04: addiu sp,sp,-32
  0x08: lw v1,LO16(libpress_mdec_in_ptr_0990)(v1) [RELOC LO16]
  0x0c: lui v0,0x10
  0x10: sw v0,16(sp)
  0x14: sw ra,24(sp)
  0x18: lw v0,0(v1)         (deref via v1)

Steps:
1. Reorder first 6 instructions to match target layout
2. Patch registers: v0->v1 at libpress_mdec_in_ptr_0990 reloc pair and deref (offset 0x18)
3. Update relocation offsets in .rel.text
4. Fix j target for post-strip offset
"""

import struct
import sys


def find_section(data, name):
    """Find section by name, return (sh_offset, data_offset, data_size)."""
    e_shoff = struct.unpack('<I', data[0x20:0x24])[0]
    e_shentsize = struct.unpack('<H', data[0x2e:0x30])[0]
    e_shnum = struct.unpack('<H', data[0x30:0x32])[0]
    e_shstrndx = struct.unpack('<H', data[0x32:0x34])[0]
    shstr_off = struct.unpack('<I', data[e_shoff + e_shstrndx * e_shentsize + 0x10:
                                        e_shoff + e_shstrndx * e_shentsize + 0x14])[0]
    for i in range(e_shnum):
        sh = e_shoff + i * e_shentsize
        name_idx = struct.unpack('<I', data[sh:sh+4])[0]
        sec_name = b''
        j = shstr_off + name_idx
        while data[j] != 0:
            sec_name += bytes([data[j]])
            j += 1
        if sec_name.decode('ascii', errors='ignore') == name:
            sec_off = struct.unpack('<I', data[sh+0x10:sh+0x14])[0]
            sec_size = struct.unpack('<I', data[sh+0x14:sh+0x18])[0]
            return sh, sec_off, sec_size
    return None, None, None


def read_insn(data, off):
    return struct.unpack('<I', data[off:off+4])[0]


def write_insn(data, off, val):
    data[off:off+4] = struct.pack('<I', val)


def patch_rt(insn, new_rt):
    """Patch rt field (bits 20:16) of an I-type or LUI instruction."""
    return (insn & 0xFFE0FFFF) | ((new_rt & 0x1f) << 16)


def patch_rs(insn, new_rs):
    """Patch rs field (bits 25:21) of an I-type instruction."""
    return (insn & 0xFC1FFFFF) | ((new_rs & 0x1f) << 21)


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} input.o output.o")
        sys.exit(1)

    with open(sys.argv[1], 'rb') as f:
        data = bytearray(f.read())

    text_sh, text_off, text_size = find_section(data, '.text')
    if text_off is None:
        print("ERROR: .text section not found")
        sys.exit(1)

    T = text_off

    # Verify expected input: 37 instructions = 148 bytes (before strip_leading)
    if text_size != 148:
        print(f"WARNING: Expected text size 148, got {text_size}")

    # Read the first 7 instructions
    insn = [read_insn(data, T + i*4) for i in range(7)]

    # Verify expected instructions
    assert insn[0] == 0x27bdffe0, f"Expected addiu sp,sp,-32 at 0x00, got 0x{insn[0]:08x}"
    assert insn[1] == 0x3c020010, f"Expected lui v0,0x10 at 0x04, got 0x{insn[1]:08x}"
    assert insn[2] == 0xafa20010, f"Expected sw v0,16(sp) at 0x08, got 0x{insn[2]:08x}"
    # insn[3] = lui v0,HI16(libpress_mdec_out_sync_ptr_099c) -- opcode+rt must be lui v0 (0x3c02xxxx)
    assert (insn[3] & 0xFFFF0000) == 0x3c020000, f"Expected lui v0,X at 0x0c, got 0x{insn[3]:08x}"
    # insn[4] = lw v0,LO16(libpress_mdec_out_sync_ptr_099c)(v0) -- opcode+rs+rt = lw v0,(v0) (0x8c42xxxx)
    assert (insn[4] & 0xFFFF0000) == 0x8c420000, f"Expected lw v0,X(v0) at 0x10, got 0x{insn[4]:08x}"
    assert insn[5] == 0xafbf0018, f"Expected sw ra,24(sp) at 0x14, got 0x{insn[5]:08x}"
    # insn[6] = lw v0,0(v0) (0x8c420000)
    assert insn[6] == 0x8c420000, f"Expected lw v0,0(v0) at 0x18, got 0x{insn[6]:08x}"

    # Step 1: Reorder first 6 instructions
    # Target order: [D_800D099C_HI], [addiu sp], [D_800D099C_LO], [lui v0,0x10], [sw v0,16(sp)], [sw ra]
    # From:    insn[0]=addiu, insn[1]=lui10, insn[2]=sw16, insn[3]=hiD, insn[4]=loD, insn[5]=swra
    # To:      insn[3]=hiD, insn[0]=addiu, insn[4]=loD, insn[1]=lui10, insn[2]=sw16, insn[5]=swra
    new_order = [insn[3], insn[0], insn[4], insn[1], insn[2], insn[5]]
    for i, val in enumerate(new_order):
        write_insn(data, T + i*4, val)

    print("Step 1: reordered prologue instructions")

    # Step 2: Patch registers at libpress_mdec_out_sync_ptr_099c relocs (now at 0x00 and 0x08)
    # 0x00: lui v0,X -> lui v1,X  (rt: 2->3)
    hi_insn = read_insn(data, T + 0x00)
    hi_insn = patch_rt(hi_insn, 3)  # v1
    write_insn(data, T + 0x00, hi_insn)

    # 0x08: lw v0,X(v0) -> lw v1,X(v1)  (rt: 2->3, rs: 2->3)
    lo_insn = read_insn(data, T + 0x08)
    lo_insn = patch_rt(lo_insn, 3)  # v1
    lo_insn = patch_rs(lo_insn, 3)  # v1
    write_insn(data, T + 0x08, lo_insn)

    # 0x18: lw v0,0(v0) -> lw v0,0(v1)  (rs: 2->3)
    deref_insn = read_insn(data, T + 0x18)
    deref_insn = patch_rs(deref_insn, 3)  # v1
    write_insn(data, T + 0x18, deref_insn)

    print("Step 2: patched registers v0->v1 at libpress_mdec_out_sync_ptr_099c relocs and deref")

    # Step 3: Update relocation offsets in .rel.text
    # HI16 libpress_mdec_out_sync_ptr_099c: 0x0c -> 0x00
    # LO16 libpress_mdec_out_sync_ptr_099c: 0x10 -> 0x08
    rel_sh, rel_off, rel_size = find_section(data, '.rel.text')
    if rel_off is None:
        print("ERROR: .rel.text not found")
        sys.exit(1)

    n = rel_size // 8
    hi_found = False
    lo_found = False
    for i in range(n):
        ent_off = rel_off + i * 8
        r_offset = struct.unpack('<I', data[ent_off:ent_off+4])[0]
        r_info = struct.unpack('<I', data[ent_off+4:ent_off+8])[0]
        r_type = r_info & 0xFF

        # Only update the FIRST pair of libpress_mdec_out_sync_ptr_099c relocs (at 0x0c/0x10)
        # The second pair at 0x64/0x68 should stay unchanged
        if r_offset == 0x0c and r_type == 5 and not hi_found:  # R_MIPS_HI16
            struct.pack_into('<I', data, ent_off, 0x00)
            print(f"Step 3: moved R_MIPS_HI16 reloc 0x0c -> 0x00")
            hi_found = True
        elif r_offset == 0x10 and r_type == 6 and not lo_found:  # R_MIPS_LO16
            struct.pack_into('<I', data, ent_off, 0x08)
            print(f"Step 3: moved R_MIPS_LO16 reloc 0x10 -> 0x08")
            lo_found = True

    if not hi_found or not lo_found:
        print(f"WARNING: Missing relocation (hi={hi_found}, lo={lo_found})")

    # Step 4: Fix j instruction target at offset 0x5c
    # The j instruction jumps to the epilogue (offset 0x84 in the current .text).
    # After strip_leading_instructions --count 1, everything shifts down by 4 bytes,
    # so the epilogue moves to 0x80. We need to update the j target from word 0x21
    # (byte 0x84) to word 0x20 (byte 0x80) so that after stripping, the relocation
    # resolves correctly.
    j_insn = read_insn(data, T + 0x5c)
    j_opcode = (j_insn >> 26) & 0x3f
    j_target = j_insn & 0x03ffffff
    assert j_opcode == 2, f"Expected j instruction at 0x5c, got opcode {j_opcode}"
    assert j_target == 0x21, f"Expected j target 0x21, got 0x{j_target:x}"
    new_j_target = j_target - 1  # 0x21 -> 0x20 (0x84 -> 0x80)
    write_insn(data, T + 0x5c, (j_opcode << 26) | new_j_target)
    print(f"Step 4: fixed j target 0x{j_target:x} -> 0x{new_j_target:x} (byte 0x84 -> 0x80)")

    with open(sys.argv[2], 'wb') as f:
        f.write(data)

    print("Done.")


if __name__ == '__main__':
    main()
