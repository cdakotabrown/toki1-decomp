#!/usr/bin/env python3
"""Swap an address-load between lui+ori and lui+addiu forms.

For an absolute address A loaded into a register $rT, IDO can emit
either form depending on internal heuristics:

  ori   form: lui $rT, HI    ; ori   $rT, $rT, LO     (LO unsigned, 0..0xFFFF)
  addiu form: lui $rT, HI+1  ; addiu $rT, $rT, LO-0x10000  (LO signed)

The two forms yield the same final $rT value when LO >= 0x8000:
- ori:   $rT = HI*0x10000 + LO          = (HI+1)*0x10000 - (0x10000-LO)
- addiu: $rT = (HI+1)*0x10000 + (LO-0x10000)  (LO-0x10000 is negative)

This tool flips between the two forms at the given lui offset (the
following instruction at N+4 must be the matching ori or addiu using
the same destination register as the lui).

Usage:
  swap_lui_addr_form.py in.o out.o --offset 0x40 --to addiu
  swap_lui_addr_form.py in.o out.o --offset 0x40 --to ori

The `--to` selects target form; the tool detects the current form
automatically and noops if already in the target form.

PostProc annotation (function-relative offset):
  @PostProc: swap_lui_addr_form --offset 0x40 --to addiu
"""
import struct
import sys
import argparse


OP_LUI = 0x0F
OP_ORI = 0x0D
OP_ADDIU = 0x09


def find_text_section(data: bytearray):
    e_shoff = struct.unpack('<I', data[32:36])[0]
    e_shnum = struct.unpack('<H', data[48:50])[0]
    e_shstrndx = struct.unpack('<H', data[50:52])[0]
    sections = []
    for i in range(e_shnum):
        off = e_shoff + i * 40
        sections.append(struct.unpack('<10I', data[off:off+40]))
    str_off = sections[e_shstrndx][4]

    def get_name(idx):
        end = data.index(b'\x00', str_off + idx)
        return data[str_off + idx:end].decode()

    for sh in sections:
        if get_name(sh[0]) == '.text':
            return sh[4], sh[5]
    return None, None


def swap_form(data: bytearray, text_off: int, off: int, to_form: str) -> None:
    abs_off = text_off + off
    lui_word = struct.unpack('<I', data[abs_off:abs_off+4])[0]
    next_word = struct.unpack('<I', data[abs_off+4:abs_off+8])[0]

    op_lui = (lui_word >> 26) & 0x3f
    if op_lui != OP_LUI:
        raise ValueError(f"insn at 0x{off:x} is not LUI (op=0x{op_lui:02x})")
    rt_lui = (lui_word >> 16) & 0x1f
    hi = lui_word & 0xffff

    op_next = (next_word >> 26) & 0x3f
    rs_next = (next_word >> 21) & 0x1f
    rt_next = (next_word >> 16) & 0x1f
    imm_next = next_word & 0xffff

    if op_next not in (OP_ORI, OP_ADDIU):
        raise ValueError(
            f"insn at 0x{off+4:x} is not ORI or ADDIU (op=0x{op_next:02x})"
        )
    if rs_next != rt_lui or rt_next != rt_lui:
        raise ValueError(
            f"insn at 0x{off+4:x} doesn't reuse lui's dest reg "
            f"(lui rt=#{rt_lui}, next rs=#{rs_next} rt=#{rt_next})"
        )

    cur_form = 'ori' if op_next == OP_ORI else 'addiu'
    if cur_form == to_form:
        print(f"  Already in {to_form} form at 0x{off:x}; skip")
        return

    if to_form == 'addiu':
        # Convert ori (HI, LO≥0x8000) -> addiu (HI+1, LO-0x10000)
        if imm_next < 0x8000:
            raise ValueError(
                f"ORI imm 0x{imm_next:x} < 0x8000 — already representable in "
                f"addiu form without changing HI; would not match target"
            )
        new_hi = (hi + 1) & 0xffff
        new_imm = imm_next  # signed: 0xAE68 reads as -0x5198
        new_op = OP_ADDIU
    else:  # to_form == 'ori'
        # Convert addiu (HI, IMM<0) -> ori (HI-1, IMM+0x10000)
        simm = imm_next if imm_next < 0x8000 else imm_next - 0x10000
        if simm >= 0:
            if imm_next == 0:
                # Unrelocated object — immediate is placeholder 0.
                # Just swap the opcode; linker will fill in the real value.
                # For ori form with LO < 0x8000, HI stays the same.
                new_hi = hi
                new_imm = imm_next  # stays 0, linker fills in
                new_op = OP_ORI
            else:
                raise ValueError(
                    f"ADDIU imm {simm} >= 0 — already representable in "
                    f"ori form without changing HI; would not match target"
                )
        else:
            new_hi = (hi - 1) & 0xffff
            new_imm = (simm + 0x10000) & 0xffff
            new_op = OP_ORI

    new_lui = (lui_word & 0xffff0000) | new_hi
    new_next = (new_op << 26) | (rs_next << 21) | (rt_next << 16) | new_imm

    struct.pack_into('<I', data, abs_off, new_lui)
    struct.pack_into('<I', data, abs_off + 4, new_next)
    print(
        f"  Swapped {cur_form}->{to_form} at 0x{off:x}: "
        f"lui imm 0x{hi:04x}->0x{new_hi:04x}, "
        f"next 0x{imm_next:04x}->0x{new_imm:04x}"
    )


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                  formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('input', help='Input ELF object file')
    ap.add_argument('output', help='Output ELF object file')
    ap.add_argument('--offset', required=True,
                    help='Byte offset of LUI instruction in .text (hex)')
    ap.add_argument('--to', required=True, choices=('ori', 'addiu'),
                    help='Target form to convert TO')
    args = ap.parse_args()

    off = int(args.offset, 16)
    with open(args.input, 'rb') as f:
        data = bytearray(f.read())

    text_off, text_size = find_text_section(data)
    if text_off is None:
        print("No .text section found", file=sys.stderr)
        sys.exit(1)
    if off + 8 > text_size:
        print(f"offset 0x{off:x} pair extends past .text", file=sys.stderr)
        sys.exit(1)

    swap_form(data, text_off, off, args.to)

    with open(args.output, 'wb') as f:
        f.write(data)
    print(f"Wrote {args.output}")


if __name__ == '__main__':
    main()
