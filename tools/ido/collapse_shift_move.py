#!/usr/bin/env python3
"""
collapse_shift_move.py - Collapse sra+move pairs into in-place shifts

IDO generates:
    sra $t6, $a0, N
    move $a0, $t6

This tool collapses it to:
    sra $a0, $a0, N
    (instruction removed)

Also patches RA offset if needed (e.g., 20 -> 16) and updates relocation offsets.

Usage:
    python3 collapse_shift_move.py input.o output.o
    python3 collapse_shift_move.py input.o output.o --ra-offset 20 16
"""

import argparse
import struct
import sys

# MIPS register numbers
REGS = {
    'zero': 0, 'at': 1, 'v0': 2, 'v1': 3,
    'a0': 4, 'a1': 5, 'a2': 6, 'a3': 7,
    't0': 8, 't1': 9, 't2': 10, 't3': 11,
    't4': 12, 't5': 13, 't6': 14, 't7': 15,
    's0': 16, 's1': 17, 's2': 18, 's3': 19,
    's4': 20, 's5': 21, 's6': 22, 's7': 23,
    't8': 24, 't9': 25, 'k0': 26, 'k1': 27,
    'gp': 28, 'sp': 29, 'fp': 30, 'ra': 31,
}

REG_NAMES = {v: k for k, v in REGS.items()}


def decode_r_type(instr):
    """Decode R-type instruction fields."""
    return {
        'opcode': (instr >> 26) & 0x3F,
        'rs': (instr >> 21) & 0x1F,
        'rt': (instr >> 16) & 0x1F,
        'rd': (instr >> 11) & 0x1F,
        'shamt': (instr >> 6) & 0x1F,
        'funct': instr & 0x3F,
    }


def is_sra(instr):
    """Check if instruction is SRA (shift right arithmetic)."""
    d = decode_r_type(instr)
    return d['opcode'] == 0 and d['funct'] == 3


def is_move(instr):
    """Check if instruction is MOVE (or $rd, $rs, $zero or addu $rd, $rs, $zero)."""
    d = decode_r_type(instr)
    if d['opcode'] != 0:
        return False
    # or $rd, $rs, $zero (funct=0x25) or addu $rd, $rs, $zero (funct=0x21)
    return d['funct'] in (0x25, 0x21) and d['rt'] == 0


def find_text_section(data):
    """Find .text section in ELF file."""
    if data[:4] != b'\x7fELF':
        return None, None, None

    e_shoff = struct.unpack('<I', data[32:36])[0]
    e_shentsize = struct.unpack('<H', data[46:48])[0]
    e_shnum = struct.unpack('<H', data[48:50])[0]
    e_shstrndx = struct.unpack('<H', data[50:52])[0]

    shstr_hdr = e_shoff + (e_shstrndx * e_shentsize)
    shstr_off = struct.unpack('<I', data[shstr_hdr + 16:shstr_hdr + 20])[0]

    for i in range(e_shnum):
        sh_off = e_shoff + (i * e_shentsize)
        sh_name_idx = struct.unpack('<I', data[sh_off:sh_off + 4])[0]
        sh_offset_val = struct.unpack('<I', data[sh_off + 16:sh_off + 20])[0]
        sh_size = struct.unpack('<I', data[sh_off + 20:sh_off + 24])[0]

        name_start = shstr_off + sh_name_idx
        name_end = data.find(b'\x00', name_start)
        section_name = data[name_start:name_end].decode('ascii')

        if section_name == '.text':
            return sh_off, sh_offset_val, sh_size

    return None, None, None


def process(input_path, output_path, ra_old=None, ra_new=None, verbose=False):
    """Collapse sra+move pairs and optionally patch RA offset."""
    with open(input_path, 'rb') as f:
        data = bytearray(f.read())

    text_sh_offset, text_offset, text_size = find_text_section(data)
    if text_offset is None:
        print("Error: .text section not found", file=sys.stderr)
        return False

    # Read instructions
    instrs = []
    for i in range(0, text_size, 4):
        w = struct.unpack('<I', data[text_offset + i:text_offset + i + 4])[0]
        instrs.append(w)

    if verbose:
        print(f"Original: {len(instrs)} instructions, {text_size} bytes")

    # Find sra+move pairs to collapse
    # Pattern: sra $tmp, $arg, N ... move $arg, $tmp
    # The move may not be immediately after the sra
    collapse_pairs = []  # (sra_idx, move_idx, arg_reg, tmp_reg, shamt)

    for i, instr in enumerate(instrs):
        if not is_sra(instr):
            continue
        d = decode_r_type(instr)
        tmp_reg = d['rd']
        arg_reg = d['rt']
        shamt = d['shamt']

        # Don't collapse if source == dest (already in-place)
        if tmp_reg == arg_reg:
            continue

        # Look for a subsequent move $arg, $tmp
        for j in range(i + 1, len(instrs)):
            if is_move(instrs[j]):
                md = decode_r_type(instrs[j])
                if md['rd'] == arg_reg and md['rs'] == tmp_reg:
                    collapse_pairs.append((i, j, arg_reg, tmp_reg, shamt))
                    break
            # Stop if tmp_reg is used as source before the move
            jd = decode_r_type(instrs[j])
            if jd['opcode'] == 0:
                if jd['rs'] == tmp_reg or jd['rt'] == tmp_reg:
                    if not is_move(instrs[j]):
                        break

    if verbose:
        for sra_idx, move_idx, arg_reg, tmp_reg, shamt in collapse_pairs:
            print(f"  Collapse: sra ${REG_NAMES.get(tmp_reg, str(tmp_reg))}, "
                  f"${REG_NAMES.get(arg_reg, str(arg_reg))}, {shamt} (idx {sra_idx}) + "
                  f"move ${REG_NAMES.get(arg_reg, str(arg_reg))}, "
                  f"${REG_NAMES.get(tmp_reg, str(tmp_reg))} (idx {move_idx})")

    if not collapse_pairs:
        print("No sra+move pairs found to collapse")
        # Still write output (might just need RA patch)
    else:
        # Build new instruction list
        remove_indices = set()
        modify = {}  # idx -> new_instr

        for sra_idx, move_idx, arg_reg, tmp_reg, shamt in collapse_pairs:
            # Change sra dest from tmp to arg
            new_sra = (instrs[sra_idx] & ~(0x1F << 11)) | (arg_reg << 11)
            modify[sra_idx] = new_sra
            remove_indices.add(move_idx)

        # Apply modifications
        for idx, new_instr in modify.items():
            instrs[idx] = new_instr

        # Remove collapsed moves (in reverse order to preserve indices)
        for idx in sorted(remove_indices, reverse=True):
            instrs.pop(idx)

    # Apply RA offset patch if requested
    if ra_old is not None and ra_new is not None:
        sw_ra_old = 0xAFBF0000 | (ra_old & 0xFFFF)
        lw_ra_old = 0x8FBF0000 | (ra_old & 0xFFFF)
        sw_ra_new = 0xAFBF0000 | (ra_new & 0xFFFF)
        lw_ra_new = 0x8FBF0000 | (ra_new & 0xFFFF)

        for i, instr in enumerate(instrs):
            if instr == sw_ra_old:
                instrs[i] = sw_ra_new
                if verbose:
                    print(f"  Patched sw ra: {ra_old} -> {ra_new} (idx {i})")
            elif instr == lw_ra_old:
                instrs[i] = lw_ra_new
                if verbose:
                    print(f"  Patched lw ra: {ra_old} -> {ra_new} (idx {i})")

    new_text_size = len(instrs) * 4

    if verbose:
        print(f"New: {len(instrs)} instructions, {new_text_size} bytes")

    # Build offset mapping for relocations (old byte offset -> new byte offset)
    # We need to track which original instruction indices map to which new indices
    if collapse_pairs:
        remove_indices_sorted = sorted(set(idx for _, idx, _, _, _ in collapse_pairs))
        # Create mapping from old instruction index to new instruction index
        offset_map = {}
        new_idx = 0
        for old_idx in range(text_size // 4):
            if old_idx in set(idx for _, idx, _, _, _ in collapse_pairs):
                continue  # This instruction was removed
            offset_map[old_idx * 4] = new_idx * 4
            new_idx += 1
    else:
        offset_map = {i: i for i in range(0, text_size, 4)}

    # Write new .text
    new_text = b''
    for w in instrs:
        new_text += struct.pack('<I', w)

    # Pad to original size (or shrink)
    if new_text_size < text_size:
        data[text_offset:text_offset + new_text_size] = new_text
        # Zero out remaining bytes
        data[text_offset + new_text_size:text_offset + text_size] = b'\x00' * (text_size - new_text_size)
    else:
        data[text_offset:text_offset + text_size] = new_text[:text_size]

    # Update .text section size
    struct.pack_into('<I', data, text_sh_offset + 20, new_text_size)

    # Update symbol table sizes
    e_shoff = struct.unpack('<I', data[32:36])[0]
    e_shentsize = struct.unpack('<H', data[46:48])[0]
    e_shnum = struct.unpack('<H', data[48:50])[0]

    for i in range(e_shnum):
        sh_off = e_shoff + (i * e_shentsize)
        sh_type = struct.unpack('<I', data[sh_off + 4:sh_off + 8])[0]

        if sh_type == 2:  # SHT_SYMTAB
            symtab_offset = struct.unpack('<I', data[sh_off + 16:sh_off + 20])[0]
            symtab_size = struct.unpack('<I', data[sh_off + 20:sh_off + 24])[0]
            sym_entsize = struct.unpack('<I', data[sh_off + 36:sh_off + 40])[0]

            for j in range(0, symtab_size, sym_entsize):
                sym_off = symtab_offset + j
                sym_size = struct.unpack('<I', data[sym_off + 8:sym_off + 12])[0]
                sym_info = data[sym_off + 12]

                if sym_size == text_size and (sym_info >> 4) == 1:
                    if verbose:
                        print(f"  Updated symbol size: {sym_size} -> {new_text_size}")
                    struct.pack_into('<I', data, sym_off + 8, new_text_size)
            break

    # Update relocations
    for i in range(e_shnum):
        sh_off = e_shoff + (i * e_shentsize)
        sh_type = struct.unpack('<I', data[sh_off + 4:sh_off + 8])[0]

        if sh_type in (4, 9):  # SHT_RELA or SHT_REL
            rel_offset = struct.unpack('<I', data[sh_off + 16:sh_off + 20])[0]
            rel_size = struct.unpack('<I', data[sh_off + 20:sh_off + 24])[0]
            rel_entsize = struct.unpack('<I', data[sh_off + 36:sh_off + 40])[0]

            for j in range(0, rel_size, rel_entsize):
                rel_off = rel_offset + j
                r_offset = struct.unpack('<I', data[rel_off:rel_off + 4])[0]

                if r_offset in offset_map:
                    new_r_offset = offset_map[r_offset]
                    if new_r_offset != r_offset:
                        if verbose:
                            print(f"  Patched reloc: 0x{r_offset:X} -> 0x{new_r_offset:X}")
                        struct.pack_into('<I', data, rel_off, new_r_offset)

    with open(output_path, 'wb') as f:
        f.write(data)

    removed = (text_size - new_text_size) // 4
    print(f"Collapsed {len(collapse_pairs)} sra+move pairs, "
          f"removed {removed} instructions "
          f"({text_size} -> {new_text_size} bytes)")
    return True


def main():
    parser = argparse.ArgumentParser(
        description='Collapse sra+move pairs into in-place shifts')
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output ELF object file')
    parser.add_argument('--ra-offset', nargs=2, type=int, metavar=('OLD', 'NEW'),
                        help='Patch RA save/restore offset (e.g., --ra-offset 20 16)')
    parser.add_argument('-v', '--verbose', action='store_true',
                        help='Verbose output')

    args = parser.parse_args()

    ra_old = args.ra_offset[0] if args.ra_offset else None
    ra_new = args.ra_offset[1] if args.ra_offset else None

    success = process(args.input, args.output,
                      ra_old=ra_old, ra_new=ra_new,
                      verbose=args.verbose)
    sys.exit(0 if success else 1)


if __name__ == '__main__':
    main()
