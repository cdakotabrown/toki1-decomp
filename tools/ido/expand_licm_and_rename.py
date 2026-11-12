#!/usr/bin/env python3
"""
expand_licm_and_rename.py - Combined LICM expansion + cyclic register rename

Transforms a compiled .o file by:
1. Removing LICM-hoisted instructions from the prologue
2. Expanding stack loads of the hoisted value to inline lui+addiu pairs
3. Applying a cyclic register rename
4. Fixing all branch offsets affected by instruction insertion/removal

This is a purpose-built tool for matching functions where IDO's LICM
(loop-invariant code motion) hoists an address computation that the
target binary computes inline at each usage site.
"""

import argparse
import struct
import sys
import os
import copy

# MIPS instruction encoding helpers
def get_opcode(insn):
    return (insn >> 26) & 0x3F

def get_rs(insn):
    return (insn >> 21) & 0x1F

def get_rt(insn):
    return (insn >> 16) & 0x1F

def get_rd(insn):
    return (insn >> 11) & 0x1F

def get_funct(insn):
    return insn & 0x3F

def get_imm16(insn):
    """Get sign-extended 16-bit immediate"""
    v = insn & 0xFFFF
    if v & 0x8000:
        v -= 0x10000
    return v

def set_rs(insn, reg):
    return (insn & ~(0x1F << 21)) | ((reg & 0x1F) << 21)

def set_rt(insn, reg):
    return (insn & ~(0x1F << 16)) | ((reg & 0x1F) << 16)

def set_rd(insn, reg):
    return (insn & ~(0x1F << 11)) | ((reg & 0x1F) << 11)

def set_imm16(insn, imm):
    return (insn & 0xFFFF0000) | (imm & 0xFFFF)

def make_lui(rt, imm16):
    """LUI rt, imm16"""
    return (0x0F << 26) | ((rt & 0x1F) << 16) | (imm16 & 0xFFFF)

def make_addiu(rt, rs, imm16):
    """ADDIU rt, rs, imm16"""
    return (0x09 << 26) | ((rs & 0x1F) << 21) | ((rt & 0x1F) << 16) | (imm16 & 0xFFFF)

def is_branch(insn):
    """Check if instruction is a branch (PC-relative)"""
    op = get_opcode(insn)
    if op in (4, 5, 6, 7):  # BEQ, BNE, BLEZ, BGTZ
        return True
    if op == 1:  # REGIMM: BLTZ, BGEZ, BLTZAL, BGEZAL
        return True
    if op == 17:  # COP1: BC1F, BC1T
        rt = get_rt(insn)
        if rt in (0, 1, 2, 3):  # BC1F, BC1T, BC1FL, BC1TL
            return True
    return False

def get_branch_target_offset(insn):
    """Get branch target as instruction offset from next PC"""
    return get_imm16(insn)

def set_branch_offset(insn, offset):
    """Set branch offset (in instructions, signed)"""
    return set_imm16(insn, offset)


class ELFRewriter:
    """Minimal ELF .o file rewriter for MIPS little-endian"""

    def __init__(self, data):
        self.data = bytearray(data)
        self.parse_elf()

    def parse_elf(self):
        # ELF header
        assert self.data[:4] == b'\x7fELF'
        self.ei_data = self.data[5]  # 1=LE, 2=BE
        assert self.ei_data == 1, "Expected little-endian ELF"

        self.e_shoff = struct.unpack_from('<I', self.data, 0x20)[0]
        self.e_shentsize = struct.unpack_from('<H', self.data, 0x2E)[0]
        self.e_shnum = struct.unpack_from('<H', self.data, 0x30)[0]
        self.e_shstrndx = struct.unpack_from('<H', self.data, 0x32)[0]

        # Parse section headers
        self.sections = []
        for i in range(self.e_shnum):
            off = self.e_shoff + i * self.e_shentsize
            sh = struct.unpack_from('<10I', self.data, off)
            self.sections.append({
                'name_idx': sh[0],
                'type': sh[1],
                'flags': sh[2],
                'addr': sh[3],
                'offset': sh[4],
                'size': sh[5],
                'link': sh[6],
                'info': sh[7],
                'addralign': sh[8],
                'entsize': sh[9],
                'index': i,
            })

        # Find .text section
        self.text_section = None
        self.rel_text_section = None
        for sec in self.sections:
            name = self.get_section_name(sec)
            if name == '.text':
                self.text_section = sec
            elif name == '.rel.text':
                self.rel_text_section = sec

        assert self.text_section is not None, "No .text section found"

    def get_section_name(self, sec):
        shstrtab = self.sections[self.e_shstrndx]
        start = shstrtab['offset'] + sec['name_idx']
        end = self.data.index(0, start)
        return self.data[start:end].decode('ascii')

    def get_text(self):
        sec = self.text_section
        return bytes(self.data[sec['offset']:sec['offset']+sec['size']])

    def get_instructions(self):
        text = self.get_text()
        n = len(text) // 4
        return list(struct.unpack(f'<{n}I', text))

    def get_relocations(self):
        """Get relocations for .text section"""
        if self.rel_text_section is None:
            return []
        sec = self.rel_text_section
        relocs = []
        for i in range(0, sec['size'], sec['entsize']):
            off = sec['offset'] + i
            r_offset, r_info = struct.unpack_from('<II', self.data, off)
            r_sym = r_info >> 8
            r_type = r_info & 0xFF
            relocs.append({
                'offset': r_offset,
                'sym': r_sym,
                'type': r_type,
                'index': i // sec['entsize'],
            })
        return relocs

    def write_text(self, instructions):
        """Write new instruction array to .text section"""
        new_text = struct.pack(f'<{len(instructions)}I', *instructions)
        old_size = self.text_section['size']
        new_size = len(new_text)
        size_delta = new_size - old_size

        sec = self.text_section
        old_offset = sec['offset']

        # Adjust section header table offset FIRST (before data modification)
        if size_delta != 0 and self.e_shoff > old_offset:
            self.e_shoff += size_delta
            struct.pack_into('<I', self.data, 0x20, self.e_shoff)

        # Replace .text data (this shifts everything after it)
        self.data[old_offset:old_offset+old_size] = new_text
        sec['size'] = new_size

        # Update .text section size in section header (using adjusted e_shoff)
        sec_header_off = self.e_shoff + sec['index'] * self.e_shentsize
        struct.pack_into('<I', self.data, sec_header_off + 20, new_size)

        # Adjust offsets of sections that come after .text
        if size_delta != 0:
            for s in self.sections:
                if s['index'] != sec['index'] and s['offset'] > old_offset:
                    s['offset'] += size_delta
                    s_header_off = self.e_shoff + s['index'] * self.e_shentsize
                    struct.pack_into('<I', self.data, s_header_off + 16, s['offset'])

    def write_relocations(self, relocs):
        """Write updated relocations"""
        if self.rel_text_section is None:
            return
        sec = self.rel_text_section
        old_size = sec['size']
        new_size = len(relocs) * sec['entsize']
        size_delta = new_size - old_size

        # Build new relocation data
        new_data = bytearray()
        for r in relocs:
            r_info = (r['sym'] << 8) | (r['type'] & 0xFF)
            new_data += struct.pack('<II', r['offset'], r_info)

        old_offset = sec['offset']

        # Adjust section header table offset FIRST
        if size_delta != 0 and self.e_shoff > old_offset:
            self.e_shoff += size_delta
            struct.pack_into('<I', self.data, 0x20, self.e_shoff)

        # Replace relocation data
        self.data[old_offset:old_offset+old_size] = new_data
        sec['size'] = new_size

        # Update section size in header (using adjusted e_shoff)
        sec_header_off = self.e_shoff + sec['index'] * self.e_shentsize
        struct.pack_into('<I', self.data, sec_header_off + 20, new_size)

        if size_delta != 0:
            for s in self.sections:
                if s['index'] != sec['index'] and s['offset'] > old_offset:
                    s['offset'] += size_delta
                    s_header_off = self.e_shoff + s['index'] * self.e_shentsize
                    struct.pack_into('<I', self.data, s_header_off + 16, s['offset'])

    def save(self, path):
        with open(path, 'wb') as f:
            f.write(self.data)


def rename_register_in_insn(insn, rename_map):
    """Apply register rename to a single instruction"""
    op = get_opcode(insn)

    if op == 0:  # R-type (SPECIAL)
        funct = get_funct(insn)
        rs = get_rs(insn)
        rt = get_rt(insn)
        rd = get_rd(insn)
        new_rs = rename_map.get(rs, rs)
        new_rt = rename_map.get(rt, rt)
        new_rd = rename_map.get(rd, rd)
        insn = set_rs(insn, new_rs)
        insn = set_rt(insn, new_rt)
        insn = set_rd(insn, new_rd)
    elif op in (2, 3):  # J, JAL - no register fields
        pass
    elif op == 1:  # REGIMM
        rs = get_rs(insn)
        insn = set_rs(insn, rename_map.get(rs, rs))
    elif op == 17:  # COP1
        # FP instructions - rs field is sub-opcode, rename rt
        rt = get_rt(insn)
        insn = set_rt(insn, rename_map.get(rt, rt))
    else:  # I-type
        rs = get_rs(insn)
        rt = get_rt(insn)
        insn = set_rs(insn, rename_map.get(rs, rs))
        insn = set_rt(insn, rename_map.get(rt, rt))

    return insn


REG_NAME_TO_NUM = {
    'zero': 0,
    'at': 1,
    'v0': 2,
    'v1': 3,
    'a0': 4,
    'a1': 5,
    'a2': 6,
    'a3': 7,
    't0': 8,
    't1': 9,
    't2': 10,
    't3': 11,
    't4': 12,
    't5': 13,
    't6': 14,
    't7': 15,
    's0': 16,
    's1': 17,
    's2': 18,
    's3': 19,
    's4': 20,
    's5': 21,
    's6': 22,
    's7': 23,
    't8': 24,
    't9': 25,
    'k0': 26,
    'k1': 27,
    'gp': 28,
    'sp': 29,
    's8': 30,
    'fp': 30,
    'ra': 31,
}


def parse_reg(reg_text):
    text = reg_text.strip().lower()
    if text.startswith('$'):
        text = text[1:]
    if text.isdigit():
        reg = int(text, 10)
        if 0 <= reg <= 31:
            return reg
        raise ValueError(f"Register number out of range: {reg_text}")
    if text in REG_NAME_TO_NUM:
        return REG_NAME_TO_NUM[text]
    raise ValueError(f"Unknown register name: {reg_text}")


def apply_stack_slot_remap(instructions, stack_slot_map, offset_filter=None):
    """
    Remap immediates of lw/sw instructions that reference sp-relative stack slots.

    Only patches:
    - lw/sw (op 35/43)
    - base register = sp ($29)
    - target register in stack_slot_map
    - if offset_filter is provided, current offset must be in offset_filter
    """
    patched = 0
    filtered = set(offset_filter) if offset_filter else None
    for i, insn in enumerate(instructions):
        op = get_opcode(insn)
        if op not in (35, 43):  # LW / SW
            continue
        if get_rs(insn) != 29:  # base must be sp
            continue
        rt = get_rt(insn)
        if rt not in stack_slot_map:
            continue
        cur_off = get_imm16(insn)
        if filtered is not None and cur_off not in filtered:
            continue
        new_off = stack_slot_map[rt]
        if cur_off != new_off:
            instructions[i] = set_imm16(insn, new_off)
            patched += 1
    return patched


def build_offset_map(old_count, removals, expansions):
    """
    Build mapping from old instruction index to new instruction index.

    removals: set of old indices to remove
    expansions: dict of old_index -> extra_count (how many instructions to INSERT after)

    Returns: dict mapping old_index -> new_index (or None if removed)
    """
    offset_map = {}
    new_idx = 0
    for old_idx in range(old_count):
        if old_idx in removals:
            offset_map[old_idx] = None
        else:
            offset_map[old_idx] = new_idx
            new_idx += 1
            # If this instruction has expansions, account for inserted instructions
            if old_idx in expansions:
                new_idx += expansions[old_idx]
    return offset_map


def fix_branches(instructions, offset_map, old_count):
    """Fix branch offsets after instruction reordering"""
    new_insns = list(instructions)
    for new_idx, insn in enumerate(new_insns):
        if is_branch(insn):
            # Find the original index for this instruction
            old_idx = None
            for oi, ni in offset_map.items():
                if ni == new_idx:
                    old_idx = oi
                    break
            if old_idx is None:
                continue

            # Branch target: old_idx + 1 + branch_offset (in instructions)
            branch_offset = get_branch_target_offset(insn)
            old_target = old_idx + 1 + branch_offset

            # Map old target to new target
            if old_target in offset_map and offset_map[old_target] is not None:
                new_target = offset_map[old_target]
            else:
                # Target was removed or out of range - find nearest valid
                # Search forward from old_target
                for t in range(old_target, old_count + 1):
                    if t in offset_map and offset_map[t] is not None:
                        new_target = offset_map[t]
                        break
                else:
                    print(f"WARNING: Cannot map branch target {old_target} for insn at old_idx {old_idx}")
                    continue

            new_branch_offset = new_target - (new_idx + 1)
            new_insns[new_idx] = set_branch_offset(insn, new_branch_offset)

    return new_insns


def main():
    parser = argparse.ArgumentParser(description='LICM expansion + cyclic register rename')
    parser.add_argument('input', help='Input .o file')
    parser.add_argument('output', help='Output .o file')
    parser.add_argument('--remove', nargs='+', type=lambda x: int(x, 0),
                        help='Byte offsets of instructions to remove from prologue')
    parser.add_argument('--expand', nargs='+', type=lambda x: int(x, 0),
                        help='Byte offsets of stack-load instructions to expand to inline lui+addiu')
    parser.add_argument('--symbol-hi', type=lambda x: int(x, 0), default=0,
                        help='HI16 value for inline expansion (e.g., 0x800f)')
    parser.add_argument('--symbol-lo', type=lambda x: int(x, 0), default=0,
                        help='LO16 value for inline expansion (e.g., 0x1180)')
    parser.add_argument('--symbol-name', type=str, default=None,
                        help='Symbol name for relocations (e.g., ot_buff0)')
    parser.add_argument('--rename', nargs='+', type=str, default=[],
                        help='Register rename pairs: FROM:TO (e.g., 18:23 19:18 ...)')
    parser.add_argument('--cycle', nargs='+', type=int, default=[],
                        help='Cyclic register rename: list of register numbers forming a cycle')
    parser.add_argument('--stack-slot', type=str, default='',
                        help='Comma-separated stack remap pairs REG:OFFSET (e.g., s2:40,s3:44)')
    parser.add_argument('--stack-slot-filter', type=str, default='',
                        help='Comma-separated current offsets to patch (e.g., 40,44,48,52,60)')
    parser.add_argument('-v', '--verbose', action='store_true')
    args = parser.parse_args()

    with open(args.input, 'rb') as f:
        elf_data = f.read()

    elf = ELFRewriter(elf_data)
    old_insns = elf.get_instructions()
    old_count = len(old_insns)
    relocs = elf.get_relocations()

    if args.verbose:
        print(f"Input: {len(old_insns)} instructions ({len(old_insns)*4} bytes)")
        print(f"Relocations: {len(relocs)}")

    # Build rename map
    rename_map = {}
    if args.cycle:
        # Cyclic rename: [a, b, c, d, e] means a->b, b->c, c->d, d->e, e->a
        cycle = args.cycle
        for i in range(len(cycle)):
            src = cycle[i]
            dst = cycle[(i + 1) % len(cycle)]
            rename_map[src] = dst
        if args.verbose:
            print(f"Cyclic rename: {' -> '.join(f'${r}' for r in cycle)} -> ${cycle[0]}")
    for pair in args.rename:
        src, dst = pair.split(':')
        rename_map[int(src)] = int(dst)
    if rename_map and args.verbose:
        print(f"Rename map: {rename_map}")

    stack_slot_map = {}
    if args.stack_slot:
        for pair in (item.strip() for item in args.stack_slot.split(',')):
            if not pair:
                continue
            if ':' not in pair:
                print(f"ERROR: invalid --stack-slot pair '{pair}', expected REG:OFFSET")
                sys.exit(1)
            reg_text, off_text = pair.split(':', 1)
            try:
                reg = parse_reg(reg_text)
            except ValueError as e:
                print(f"ERROR: {e}")
                sys.exit(1)
            try:
                off = int(off_text, 0)
            except ValueError:
                print(f"ERROR: invalid stack offset in --stack-slot '{pair}'")
                sys.exit(1)
            stack_slot_map[reg] = off
        if args.verbose:
            human = ", ".join(f"${reg}->{off}" for reg, off in sorted(stack_slot_map.items()))
            print(f"Stack-slot remap: {human}")

    stack_slot_filter = []
    if args.stack_slot_filter:
        for item in (part.strip() for part in args.stack_slot_filter.split(',')):
            if not item:
                continue
            try:
                stack_slot_filter.append(int(item, 0))
            except ValueError:
                print(f"ERROR: invalid --stack-slot-filter value '{item}'")
                sys.exit(1)
        if args.verbose:
            filt = ", ".join(str(x) for x in stack_slot_filter)
            print(f"Stack-slot filter: {filt}")

    # Convert byte offsets to instruction indices
    removals = set()
    if args.remove:
        for off in args.remove:
            idx = off // 4
            if idx >= old_count:
                print(f"ERROR: removal offset 0x{off:x} (idx {idx}) out of range")
                sys.exit(1)
            removals.add(idx)
            if args.verbose:
                print(f"Remove: idx {idx} (0x{off:x}): {old_insns[idx]:08x}")

    expansions = {}  # idx -> number of extra instructions to insert
    expand_indices = []
    if args.expand:
        for off in args.expand:
            idx = off // 4
            if idx >= old_count:
                print(f"ERROR: expansion offset 0x{off:x} (idx {idx}) out of range")
                sys.exit(1)
            expansions[idx] = 1  # Insert 1 extra instruction (lw becomes lui+addiu)
            expand_indices.append(idx)
            if args.verbose:
                print(f"Expand: idx {idx} (0x{off:x}): {old_insns[idx]:08x}")

    # Build offset map
    offset_map = build_offset_map(old_count, removals, expansions)

    # Build new instruction array
    new_insns = []
    new_relocs = []

    # Track which symbol index to use for new relocations
    symbol_idx = None
    if args.symbol_name:
        # Find the symbol in existing relocations
        for r in relocs:
            # We'll use the same sym index as the existing ot_buff0 relocs
            if r['offset'] in [off for off in (args.remove or [])]:
                symbol_idx = r['sym']
                break

    for old_idx in range(old_count):
        if old_idx in removals:
            continue

        new_idx = offset_map[old_idx]
        insn = old_insns[old_idx]

        if old_idx in expansions:
            # Replace lw with lui+addiu pair
            target_reg = get_rt(insn)  # The register being loaded
            lui_insn = make_lui(target_reg, args.symbol_hi)
            addiu_insn = make_addiu(target_reg, target_reg, args.symbol_lo)
            new_insns.append(lui_insn)
            new_insns.append(addiu_insn)

            # Add relocations for the new lui+addiu pair
            if symbol_idx is not None:
                new_relocs.append({
                    'offset': new_idx * 4,
                    'sym': symbol_idx,
                    'type': 5,  # R_MIPS_HI16
                })
                new_relocs.append({
                    'offset': (new_idx + 1) * 4,
                    'sym': symbol_idx,
                    'type': 6,  # R_MIPS_LO16
                })
        else:
            new_insns.append(insn)

    # Fix branches
    new_insns = fix_branches(new_insns, offset_map, old_count)

    # Update existing relocations (adjust offsets, skip removed ones)
    removed_offsets = set(idx * 4 for idx in removals)
    expanded_offsets = set(idx * 4 for idx in expansions.keys())

    for r in relocs:
        old_off = r['offset']
        old_idx = old_off // 4

        if old_idx in removals:
            continue  # Skip relocations for removed instructions
        if old_idx in expansions:
            continue  # Skip old relocations for expanded instructions (replaced by new ones)

        if old_idx in offset_map and offset_map[old_idx] is not None:
            new_off = offset_map[old_idx] * 4
            new_relocs.append({
                'offset': new_off,
                'sym': r['sym'],
                'type': r['type'],
            })

    # Apply register rename
    if rename_map:
        for i in range(len(new_insns)):
            new_insns[i] = rename_register_in_insn(new_insns[i], rename_map)

    stack_slot_patch_count = 0
    if stack_slot_map:
        stack_slot_patch_count = apply_stack_slot_remap(
            new_insns,
            stack_slot_map,
            offset_filter=stack_slot_filter,
        )
        if args.verbose:
            print(f"Stack-slot patches: {stack_slot_patch_count}")

    if args.verbose:
        print(f"Output: {len(new_insns)} instructions ({len(new_insns)*4} bytes)")
        print(f"Output relocations: {len(new_relocs)}")
        delta = len(new_insns) - old_count
        print(f"Delta: {delta:+d} instructions ({delta*4:+d} bytes)")

    # Sort relocations by offset
    new_relocs.sort(key=lambda r: r['offset'])

    # Write output
    elf.write_text(new_insns)
    elf.write_relocations(new_relocs)
    elf.save(args.output)

    print(f"Wrote {args.output}: {len(new_insns)*4} bytes, {len(new_relocs)} relocations")


if __name__ == '__main__':
    main()
