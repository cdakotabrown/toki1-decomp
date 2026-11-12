#!/usr/bin/env python3
"""Shift stack-relative offsets in .text section of MIPS ELF object file.

Finds all sw/lw/addiu instructions with $sp as base register and immediate
less than old_frame, then shifts them by (new_frame - old_frame).
"""
import struct
import sys
import argparse

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('input')
    parser.add_argument('output')
    parser.add_argument('--old', type=int, required=True, help='Old frame size')
    parser.add_argument('--new', type=int, required=True, help='New frame size')
    args = parser.parse_args()
    
    delta = args.new - args.old
    
    with open(args.input, 'rb') as f:
        data = bytearray(f.read())
    
    # Parse ELF to find .text
    e_shoff = struct.unpack('<I', data[32:36])[0]
    e_shnum = struct.unpack('<H', data[48:50])[0]
    e_shstrndx = struct.unpack('<H', data[50:52])[0]
    
    sections = []
    for i in range(e_shnum):
        off = e_shoff + i * 40
        sh = struct.unpack('<10I', data[off:off+40])
        sections.append(sh)
    
    strtab = sections[e_shstrndx]
    str_off = strtab[4]
    
    def get_name(idx):
        end = data.index(b'\x00', str_off + idx)
        return data[str_off + idx:end].decode()
    
    text_off = text_size = None
    for i, sh in enumerate(sections):
        if get_name(sh[0]) == '.text':
            text_off = sh[4]
            text_size = sh[5]
            break
    
    if text_off is None:
        print("No .text section found")
        sys.exit(1)
    
    patched = 0
    for j in range(0, text_size, 4):
        off = text_off + j
        insn = struct.unpack('<I', data[off:off+4])[0]
        
        op = (insn >> 26) & 0x3F
        rs = (insn >> 21) & 0x1F
        rt = (insn >> 16) & 0x1F
        imm = insn & 0xFFFF
        simm = imm if imm < 0x8000 else imm - 0x10000
        
        # Skip prologue/epilogue (addiu sp, sp, X)
        if op == 0x09 and rs == 29 and rt == 29:
            continue
        
        # Check for sw/lw/lh/lhu/sh/sb/lb/lbu with sp as base
        is_mem = op in (0x23, 0x2b, 0x21, 0x25, 0x29, 0x28, 0x20, 0x24)  # lw/sw/lh/lhu/sh/sb/lb/lbu
        is_addiu_sp = (op == 0x09 and rs == 29)
        
        if (is_mem and rs == 29) or is_addiu_sp:
            # Only shift offsets that are within old frame (local variables)
            if 0 <= simm < args.old:
                new_imm = simm + delta
                new_insn = (insn & 0xFFFF0000) | (new_imm & 0xFFFF)
                struct.pack_into('<I', data, off, new_insn)
                patched += 1
                print(f"  Offset 0x{j:04x}: sp+{simm} -> sp+{new_imm}")
    
    print(f"Patched {patched} instruction(s)")
    
    with open(args.output, 'wb') as f:
        f.write(data)

if __name__ == '__main__':
    main()
