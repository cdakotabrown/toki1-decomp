#!/usr/bin/env python3
"""Undo delay-slot fill optimization for jal/jr instructions.

Unlike conditional branches (handled by `unfill_branch_delay.py`), jal
targets are absolute (26-bit immediate << 2) and jr is register-indirect,
so no target field needs to be adjusted when removing a filled delay
slot.

Use case:
    IDO emits:    jal X ; sb $tN, off($at)
    Target wants: jal X ; nop ; sb $tN, off($at)

Algorithm:
    1. Verify the instruction at --offset is JAL (op=0x03) or JR
       (op=0x00, special funct=0x08).
    2. Read the delay slot instruction (at offset+4).
    3. Search forward up to --max-search instructions for a NOP.
    4. Swap the delay-slot instruction with that NOP. Relocations
       targeting either offset are also swapped so they follow their
       instructions.
    5. If no NOP is found, abort with an error (no instruction may be
       silently lost — policy: instruction-shape-preserving only).

POLICY:
    - ALLOWED: swap two instructions, NOP-out an existing slot via swap.
    - BANNED: hardcoded byte patches, .text replacement, instruction
      insertion that shifts subsequent code.

Usage:
    unfill_jal_delay.py in.o out.o --offset 0x2c [--max-search 8]
"""
import struct
import sys
import argparse


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

    text_sh = None
    rel_sh = None
    for sh in sections:
        name = get_name(sh[0])
        if name == '.text':
            text_sh = sh
        elif name == '.rel.text':
            rel_sh = sh
    return text_sh, rel_sh


def is_jal_or_jr(insn: int) -> str:
    """Return 'jal', 'jr', or '' if not a jump-with-delay-slot."""
    op = (insn >> 26) & 0x3F
    if op == 0x03:
        return 'jal'
    # jalr (op=0, funct=0x09) also has a delay slot; include for completeness
    if op == 0x00:
        funct = insn & 0x3F
        if funct == 0x08:
            return 'jr'
        if funct == 0x09:
            return 'jalr'
    # j (op=0x02) also has a delay slot
    if op == 0x02:
        return 'j'
    return ''


def main():
    ap = argparse.ArgumentParser(
        description='Undo jal/jr delay-slot fill in MIPS ELF',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__,
    )
    ap.add_argument('input', help='Input ELF object file')
    ap.add_argument('output', help='Output ELF object file')
    ap.add_argument('--offset', required=True,
                    help='Offset of jal/jr instruction (hex)')
    ap.add_argument('--max-search', type=int, default=8,
                    help='Max instructions forward to search for a NOP '
                         '(default: 8)')
    args = ap.parse_args()

    jal_off = int(args.offset, 16) if args.offset.startswith('0x') \
        else int(args.offset)
    max_search = args.max_search

    with open(args.input, 'rb') as f:
        data = bytearray(f.read())

    if data[:4] != b'\x7fELF':
        print("Error: not a valid ELF file", file=sys.stderr)
        sys.exit(1)

    text_sh, rel_sh = find_text_section(data)
    if text_sh is None:
        print("No .text section found", file=sys.stderr)
        sys.exit(1)
    text_off = text_sh[4]
    text_size = text_sh[5]

    if jal_off < 0 or jal_off + 8 > text_size:
        print(f"Error: offset 0x{jal_off:x} out of range "
              f"(text size: 0x{text_size:x})", file=sys.stderr)
        sys.exit(1)

    abs_jal = text_off + jal_off
    jal_insn = struct.unpack('<I', data[abs_jal:abs_jal+4])[0]
    kind = is_jal_or_jr(jal_insn)
    if not kind:
        op = (jal_insn >> 26) & 0x3F
        funct = jal_insn & 0x3F
        print(f"Error: instruction at offset 0x{jal_off:x} "
              f"(op=0x{op:02x}, funct=0x{funct:02x}) is not "
              f"jal/jr/jalr/j", file=sys.stderr)
        sys.exit(1)

    delay_off = jal_off + 4
    abs_delay = text_off + delay_off
    delay_insn = struct.unpack('<I', data[abs_delay:abs_delay+4])[0]

    if delay_insn == 0x00000000:
        print(f"Note: delay slot at 0x{delay_off:x} is already NOP — "
              f"nothing to do.")
        # Still write output (idempotent)
        with open(args.output, 'wb') as f:
            f.write(data)
        return

    # Search forward for a NOP
    target_off = None
    for i in range(1, max_search + 1):
        candidate_off = delay_off + i * 4
        if candidate_off + 4 > text_size:
            break
        abs_cand = text_off + candidate_off
        cand_insn = struct.unpack('<I', data[abs_cand:abs_cand+4])[0]
        if cand_insn == 0x00000000:
            target_off = candidate_off
            break

    if target_off is None:
        print(f"Error: no NOP found within {max_search} instructions "
              f"after delay slot at 0x{delay_off:x}. Cannot relocate "
              f"instruction without losing it.", file=sys.stderr)
        sys.exit(1)

    abs_target = text_off + target_off

    # Swap delay-slot insn with the NOP
    data[abs_delay:abs_delay+4] = struct.pack('<I', 0x00000000)
    data[abs_target:abs_target+4] = struct.pack('<I', delay_insn)

    # Swap relocations targeting these offsets so they follow the insns
    relocs_swapped = 0
    if rel_sh is not None:
        rel_offset = rel_sh[4]
        rel_size = rel_sh[5]
        num_rels = rel_size // 8
        for i in range(num_rels):
            ro = rel_offset + i * 8
            r_off = struct.unpack('<I', data[ro:ro+4])[0]
            if r_off == delay_off:
                struct.pack_into('<I', data, ro, target_off)
                relocs_swapped += 1
            elif r_off == target_off:
                struct.pack_into('<I', data, ro, delay_off)
                relocs_swapped += 1

    print(f"  {kind} at 0x{jal_off:04x}: moved delay-slot insn "
          f"0x{delay_insn:08x} from 0x{delay_off:04x} to "
          f"0x{target_off:04x} (NOP'd delay slot)")
    if relocs_swapped:
        print(f"  ({relocs_swapped} relocation(s) swapped)")

    with open(args.output, 'wb') as f:
        f.write(data)
    print("Success")


if __name__ == '__main__':
    main()
