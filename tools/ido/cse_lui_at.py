#!/usr/bin/env python3
"""
CSE (Common Subexpression Elimination) for lui at instructions.

IDO 5.2 generates a separate `lui at, %hi(sym)` for each global store,
even when multiple stores share the same high 16-bit address. This tool
removes redundant `lui at` instructions and compacts the code.

The transform is REMOVAL-ONLY: it deletes a compiler-emitted `lui $at`
(and its HI16 relocation), compacts the text and fixes up the remaining
relocation offsets / symbol sizes.  It never writes a hardcoded
instruction word, never changes an instruction's operation and never
inserts anything.  (See CLAUDE.md "PostProc Policy".)

Works on little-endian ELF object files (IDO with -EL flag).

Modes
-----
default (no scoping flag)
    Legacy behaviour: keep the first `lui $at`, drop every later one that
    either references the same symbol or carries any HI16 relocation
    ("optimistic", all-or-nothing).  Unchanged for existing users.

--same-symbol-only
    Symbol-scoped CSE.  Walks the text in order tracking which symbol the
    live `$at` currently holds the %hi of, and drops a `lui $at` only when
    the immediately preceding *kept* `lui $at` targets the SAME symbol and
    nothing in between clobbers `$at` (e.g. an `slti $at`) or crosses a
    branch / branch target.  This is what the target does when it shares
    one `lui $at` inside one symbol/object group and re-emits a fresh one
    for the next group.

--offsets a,b,c
    Range/site-scoped CSE.  Removes ONLY the `lui $at` instructions living
    at the given .text byte offsets (of the input object).  Use this when
    the shared group spans *different* symbols that happen to sit in the
    same 64K page (their relative addresses are not knowable from the
    object file, so the tool cannot infer the grouping itself).  Combines
    with --same-symbol-only, which then acts as an extra filter.

--nop-in-place
    Same redundancy selection and the same removal-only guarantee, but
    instead of compacting the text the removed `lui $at` slot is left as
    the `nop` the assembler would have left there (its HI16 relocation is
    dropped with it).  Nothing shifts: no branch re-targeting, no
    relocation offset fixups, no symbol-size changes.  Use when the
    TARGET keeps the slot as a load-use-hazard `nop` (the original as1
    shared `$at` across %hi-identical store macros and had nothing else
    to fill the slot with), so instruction count must be preserved.
    Strictly more conservative than the compacting mode.

Use `--list` (or -v) to print every `lui $at` with its .text offset and
symbol name, which is how you pick the offsets.

Usage:
    python3 cse_lui_at.py input.o output.o [--verbose]
    python3 cse_lui_at.py input.o output.o --same-symbol-only
    python3 cse_lui_at.py input.o output.o --offsets 0x2c,0x48
    python3 cse_lui_at.py input.o --list
"""

import sys
import re
import struct
import argparse


# MIPS instruction encoding
def decode_insn(word):
    """Decode a MIPS instruction word."""
    op = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    rd = (word >> 11) & 0x1F
    imm = word & 0xFFFF
    return op, rs, rt, rd, imm


def is_lui_at(word):
    """Check if instruction is lui at,X (at = $1)."""
    op, rs, rt, rd, imm = decode_insn(word)
    return op == 0x0F and rs == 0 and rt == 1  # lui $at, imm


def uses_at(word):
    """Check if instruction uses $at as a source register (rs or base)."""
    op, rs, rt, rd, imm = decode_insn(word)
    # sw/sh/sb/lw/lh/lb use rs as base register
    if op in (0x2B, 0x29, 0x28, 0x23, 0x21, 0x20, 0x24, 0x25):  # sw,sh,sb,lw,lhu,lb,lbu,lh
        return rs == 1
    # Also check R-type and I-type that use rs
    if op == 0x00:  # R-type: check rs and rt
        return rs == 1 or rt == 1
    # I-type: check rs
    return rs == 1


def sets_at(word):
    """Check if instruction sets $at (as destination)."""
    op, rs, rt, rd, imm = decode_insn(word)
    if op == 0x0F:  # lui
        return rt == 1
    if op == 0x00:  # R-type
        return rd == 1
    # I-type: rt is destination for most
    if op not in (0x2B, 0x29, 0x28, 0x04, 0x05, 0x06, 0x07, 0x01):  # not stores or branches
        return rt == 1
    return False


# ELF parsing (little-endian)
def read_u16(data, off):
    return struct.unpack('<H', data[off:off+2])[0]

def read_u32(data, off):
    return struct.unpack('<I', data[off:off+4])[0]

def write_u16(data, off, val):
    struct.pack_into('<H', data, off, val)

def write_u32(data, off, val):
    struct.pack_into('<I', data, off, val)


def parse_elf_le(data):
    """Parse little-endian ELF32 headers."""
    assert data[:4] == b'\x7fELF', "Not an ELF file"
    assert data[4] == 1, "Not 32-bit"
    assert data[5] == 1, "Not little-endian"

    e_shoff = read_u32(data, 32)
    e_shentsize = read_u16(data, 46)
    e_shnum = read_u16(data, 48)
    e_shstrndx = read_u16(data, 50)

    sections = []
    for i in range(e_shnum):
        off = e_shoff + i * e_shentsize
        sh = {
            'index': i,
            'sh_name': read_u32(data, off),
            'sh_type': read_u32(data, off + 4),
            'sh_flags': read_u32(data, off + 8),
            'sh_addr': read_u32(data, off + 12),
            'sh_offset': read_u32(data, off + 16),
            'sh_size': read_u32(data, off + 20),
            'sh_link': read_u32(data, off + 24),
            'sh_info': read_u32(data, off + 28),
            'sh_addralign': read_u32(data, off + 32),
            'sh_entsize': read_u32(data, off + 36),
        }
        sections.append(sh)

    # Resolve section names
    if e_shstrndx < len(sections):
        strtab = sections[e_shstrndx]
        strtab_data = data[strtab['sh_offset']:strtab['sh_offset'] + strtab['sh_size']]
        for sh in sections:
            end = strtab_data.find(b'\x00', sh['sh_name'])
            if end >= 0:
                sh['name'] = strtab_data[sh['sh_name']:end].decode('ascii', errors='replace')
            else:
                sh['name'] = ''
    else:
        for sh in sections:
            sh['name'] = ''

    return sections


def get_relocations(data, sections):
    """Get relocations for .text section."""
    relocs = []
    for sh in sections:
        if sh['sh_type'] == 9 and sh['name'] == '.rel.text':  # SHT_REL
            num = sh['sh_size'] // 8
            for i in range(num):
                off = sh['sh_offset'] + i * 8
                r_offset = read_u32(data, off)
                r_info = read_u32(data, off + 4)
                relocs.append({
                    'offset': r_offset,
                    'info': r_info,
                    'type': r_info & 0xFF,
                    'sym': r_info >> 8,
                    'file_offset': off,
                })
    return relocs


def get_reloc_symbol(relocs, text_offset):
    """Get the symbol index for a relocation at a given text offset."""
    for r in relocs:
        if r['offset'] == text_offset:
            return r['sym'], r['type']
    return None, None


def get_symbol_names(data, sections):
    """Return a list of symbol names indexed by symbol table index."""
    symtab = None
    for sh in sections:
        if sh['name'] == '.symtab':
            symtab = sh
            break
    if not symtab:
        return []
    strtab = None
    for sh in sections:
        if sh['index'] == symtab['sh_link']:
            strtab = sh
            break
    if not strtab:
        return []
    sdata = data[strtab['sh_offset']:strtab['sh_offset'] + strtab['sh_size']]
    names = []
    num = symtab['sh_size'] // 16
    for i in range(num):
        off = symtab['sh_offset'] + i * 16
        st_name = read_u32(data, off)
        end = sdata.find(b'\x00', st_name)
        if end < 0:
            names.append('')
        else:
            names.append(sdata[st_name:end].decode('ascii', errors='replace'))
    return names


def is_branch_or_jump(word):
    """True for MIPS branches / jumps (control-flow boundary)."""
    op = (word >> 26) & 0x3F
    if op in (0x02, 0x03):          # j, jal
        return True
    if op in (0x01, 0x04, 0x05, 0x06, 0x07):   # regimm, beq, bne, blez, bgtz
        return True
    if op in (0x14, 0x15, 0x16, 0x17):         # beql, bnel, blezl, bgtzl
        return True
    if op == 0x00:
        funct = word & 0x3F
        if funct in (0x08, 0x09):   # jr, jalr
            return True
    return False


def branch_targets(insns):
    """Indices that are the target of a branch inside this text section."""
    targets = set()
    for i, word in enumerate(insns):
        op = (word >> 26) & 0x3F
        if op in (0x01, 0x04, 0x05, 0x06, 0x07, 0x14, 0x15, 0x16, 0x17):
            imm = word & 0xFFFF
            if imm & 0x8000:
                imm -= 0x10000
            targets.add(i + 1 + imm)
    return targets


def select_removals_scoped(insns, lui_at_info, same_symbol_only, offsets,
                           symnames, verbose):
    """
    Pick which `lui $at` to remove under the scoped rules.

    same_symbol_only: a lui is redundant only if the last *kept* lui $at
    holds the %hi of the SAME symbol and $at has not been clobbered and no
    control-flow boundary was crossed in between.

    offsets: whitelist of .text byte offsets; only these may be removed.
    """
    offset_set = set(offsets) if offsets else None
    targets = branch_targets(insns)
    lui_idx = {info['index']: info for info in lui_at_info}

    to_remove = []
    live_sym = None       # symbol index whose %hi is currently in $at
    live_type = None

    for i, word in enumerate(insns):
        if i in targets:
            # control-flow join: cannot assume $at survived
            live_sym = None
        info = lui_idx.get(i)
        if info is not None:
            eligible = True
            reason = ''
            if offset_set is not None and (i * 4) not in offset_set:
                eligible = False
                reason = 'not in --offsets'
            elif info['rtype'] != 5:
                eligible = False
                reason = 'no HI16 relocation'
            elif live_sym is None:
                eligible = False
                reason = 'no live preceding lui $at'
            elif same_symbol_only and info['sym'] != live_sym:
                eligible = False
                reason = 'different symbol than live lui $at'

            if eligible:
                to_remove.append(i)
                if verbose:
                    name = symnames[info['sym']] if info['sym'] is not None \
                        and info['sym'] < len(symnames) else '?'
                    print(f"  remove insn[{i}] (.text+0x{i*4:x}) "
                          f"lui at, %hi({name})")
                # $at keeps the previously loaded %hi -> live_sym unchanged
            else:
                if verbose and offset_set is not None and (i * 4) in offset_set:
                    print(f"  WARNING: .text+0x{i*4:x} kept: {reason}")
                elif verbose:
                    name = symnames[info['sym']] if info['sym'] is not None \
                        and info['sym'] < len(symnames) else '?'
                    print(f"  keep   insn[{i}] (.text+0x{i*4:x}) "
                          f"lui at, %hi({name}) [{reason}]")
                live_sym = info['sym']
                live_type = info['rtype']
            continue

        if sets_at(word):
            live_sym = None
        if is_branch_or_jump(word):
            # be conservative: the delay slot and the fallthrough may be
            # reached from elsewhere
            live_sym = None

    if offset_set is not None:
        missed = sorted(offset_set - set(i * 4 for i in to_remove))
        for off in missed:
            print(f"WARNING: no lui $at removed at .text+0x{off:x}",
                  file=sys.stderr)

    return to_remove


def cse_lui_at(data, verbose=False, same_symbol_only=False, offsets=None,
               list_only=False, fix_branches=None, nop_in_place=False):
    """
    Remove redundant lui at instructions from .text section.

    Strategy:
    1. Find all lui at instructions
    2. Group consecutive ones that load the same symbol (via relocation)
    3. Keep the first, remove the rest
    4. For removed lui at: adjust the following sw/lw to use the offset
       from the first lui at's symbol
    5. Compact code and fix relocations
    """
    data = bytearray(data)
    sections = parse_elf_le(data)

    text_sh = None
    for sh in sections:
        if sh['name'] == '.text':
            text_sh = sh
            break

    if not text_sh:
        raise ValueError("No .text section")

    text_off = text_sh['sh_offset']
    text_size = text_sh['sh_size']
    num_insns = text_size // 4

    relocs = get_relocations(data, sections)

    # Read all instructions
    insns = []
    for i in range(num_insns):
        word = read_u32(data, text_off + i * 4)
        insns.append(word)

    if verbose:
        print(f"Text section: {num_insns} instructions")

    # Find all lui at instructions and their relocation symbols
    lui_at_info = []
    for i, word in enumerate(insns):
        if is_lui_at(word):
            sym, rtype = get_reloc_symbol(relocs, i * 4)
            lui_at_info.append({
                'index': i,
                'word': word,
                'sym': sym,
                'rtype': rtype,
            })

    symnames = get_symbol_names(data, sections)

    def symname(info):
        if info['sym'] is not None and info['sym'] < len(symnames):
            return symnames[info['sym']]
        return '?'

    if verbose or list_only:
        print(f"Found {len(lui_at_info)} lui at instructions")
        for info in lui_at_info:
            print(f"  insn[{info['index']}] .text+0x{info['index']*4:x}: "
                  f"lui at, %hi({symname(info)}) "
                  f"(sym={info['sym']}, rtype={info['rtype']})")

    if list_only:
        return bytes(data)

    if len(lui_at_info) <= 1:
        if verbose:
            print("Nothing to CSE")
        return bytes(data)

    scoped = same_symbol_only or offsets
    if scoped:
        to_remove = select_removals_scoped(
            insns, lui_at_info, same_symbol_only, offsets, symnames, verbose)
    else:
        # Legacy (all-or-nothing) behaviour: keep the first lui at, drop
        # every later one referencing the same symbol or carrying a HI16.
        first_sym = lui_at_info[0]['sym']
        to_remove = []
        for info in lui_at_info[1:]:
            if info['sym'] == first_sym:
                to_remove.append(info['index'])
            elif info['rtype'] == 5:  # R_MIPS_HI16 - might be same page
                # Remove it optimistically (same struct)
                to_remove.append(info['index'])

    if not to_remove:
        if verbose:
            print("No redundant lui at found")
        return bytes(data)

    if verbose:
        print(f"Removing {len(to_remove)} redundant lui at instructions at indices: {to_remove}")

    if nop_in_place:
        # Leave a nop in each removed slot and drop only its relocation.
        # Text size, every other relocation offset, branch displacements
        # and symbol sizes are untouched.
        removed_set = set(to_remove)
        for i in to_remove:
            write_u32(data, text_off + i * 4, 0x00000000)
        rel_sh = None
        for sh in sections:
            if sh['name'] == '.rel.text':
                rel_sh = sh
                break
        if rel_sh:
            kept = [(r['offset'], r['info']) for r in relocs
                    if (r['offset'] // 4) not in removed_set]
            e_shoff = read_u32(data, 32)
            e_shentsize = read_u16(data, 46)
            for i, (r_off, r_info) in enumerate(kept):
                file_off = rel_sh['sh_offset'] + i * 8
                write_u32(data, file_off, r_off)
                write_u32(data, file_off + 4, r_info)
            for i in range(len(kept) * 8, rel_sh['sh_size']):
                data[rel_sh['sh_offset'] + i] = 0
            rel_hdr_off = e_shoff + rel_sh['index'] * e_shentsize
            write_u32(data, rel_hdr_off + 20, len(kept) * 8)
        if verbose:
            print(f"nop-in-place: {len(to_remove)} slot(s) left as nop, "
                  f"HI16 reloc(s) dropped")
        return bytes(data)

    # Now we need to:
    # 1. Remove the instructions at to_remove indices
    # 2. Compact the remaining instructions
    # 3. Update relocations

    # Build the new instruction list and track index mapping
    new_insns = []
    old_to_new = {}  # maps old index -> new index
    removed_set = set(to_remove)

    for i, word in enumerate(insns):
        if i in removed_set:
            continue
        old_to_new[i] = len(new_insns)
        new_insns.append(word)

    if verbose:
        print(f"Compacted: {len(insns)} -> {len(new_insns)} instructions")

    # Re-target PC-relative branches that span a removed instruction.
    # This only rewrites the imm16 displacement field of the compiler's own
    # branch instruction (same opcode, same registers) so that it still
    # points at the same instruction after compaction -- without it the
    # removal would silently corrupt control flow.
    if fix_branches is None:
        fix_branches = bool(scoped)
    if fix_branches:
        def next_kept(idx):
            while idx not in old_to_new and idx <= len(insns):
                idx += 1
            return old_to_new.get(idx, len(new_insns))

        reloc_offsets = set(r['offset'] for r in relocs)
        for old_i, word in enumerate(insns):
            if old_i in removed_set:
                continue
            op = (word >> 26) & 0x3F
            if op not in (0x01, 0x04, 0x05, 0x06, 0x07,
                          0x14, 0x15, 0x16, 0x17):
                continue
            if old_i * 4 in reloc_offsets:
                continue
            imm = word & 0xFFFF
            simm = imm - 0x10000 if imm & 0x8000 else imm
            old_target = old_i + 1 + simm
            new_i = old_to_new[old_i]
            new_target = next_kept(old_target)
            new_simm = new_target - (new_i + 1)
            if new_simm == simm:
                continue
            if not (-0x8000 <= new_simm <= 0x7FFF):
                print(f"WARNING: branch at .text+0x{old_i*4:x} cannot be "
                      f"re-targeted after CSE", file=sys.stderr)
                continue
            new_insns[new_i] = (word & 0xFFFF0000) | (new_simm & 0xFFFF)
            if verbose:
                print(f"  branch at insn[{new_i}]: {simm} -> {new_simm}")

    # Update text section in the ELF data
    new_text_size = len(new_insns) * 4
    for i, word in enumerate(new_insns):
        write_u32(data, text_off + i * 4, word)

    # Zero out the remaining bytes
    for i in range(len(new_insns) * 4, text_size):
        data[text_off + i] = 0

    # Update text section size in section header
    e_shoff = read_u32(data, 32)
    e_shentsize = read_u16(data, 46)
    for sh in sections:
        if sh['name'] == '.text':
            sh_hdr_off = e_shoff + sh['index'] * e_shentsize
            write_u32(data, sh_hdr_off + 20, new_text_size)  # sh_size
            break

    # Update relocations: remove relocs for removed instructions,
    # adjust offsets for remaining ones
    rel_sh = None
    for sh in sections:
        if sh['name'] == '.rel.text':
            rel_sh = sh
            break

    if rel_sh:
        new_relocs = []
        for r in relocs:
            old_insn_idx = r['offset'] // 4
            if old_insn_idx in removed_set:
                # Skip relocation for removed instruction
                continue
            if old_insn_idx in old_to_new:
                new_offset = old_to_new[old_insn_idx] * 4
            else:
                new_offset = r['offset']
            new_relocs.append((new_offset, r['info']))

        # Write new relocations
        new_rel_size = len(new_relocs) * 8
        for i, (r_off, r_info) in enumerate(new_relocs):
            file_off = rel_sh['sh_offset'] + i * 8
            write_u32(data, file_off, r_off)
            write_u32(data, file_off + 4, r_info)

        # Zero out remaining relocation entries
        for i in range(len(new_relocs) * 8, rel_sh['sh_size']):
            data[rel_sh['sh_offset'] + i] = 0

        # Update rel section size
        rel_hdr_off = e_shoff + rel_sh['index'] * e_shentsize
        write_u32(data, rel_hdr_off + 20, new_rel_size)

    # Update symbol sizes
    symtab_sh = None
    for sh in sections:
        if sh['name'] == '.symtab':
            symtab_sh = sh
            break

    if symtab_sh:
        text_idx = text_sh['index']
        num_syms = symtab_sh['sh_size'] // 16
        for i in range(num_syms):
            sym_off = symtab_sh['sh_offset'] + i * 16
            st_shndx = read_u16(data, sym_off + 14)
            if st_shndx == text_idx:
                st_value = read_u32(data, sym_off + 4)
                st_size = read_u32(data, sym_off + 8)
                if st_size > 0:
                    # Adjust value and size
                    old_start_idx = st_value // 4
                    if old_start_idx in old_to_new:
                        new_value = old_to_new[old_start_idx] * 4
                    else:
                        new_value = st_value
                    # Count how many removed insns were within this symbol
                    removed_count = sum(1 for r in to_remove
                                       if st_value <= r * 4 < st_value + st_size)
                    new_size = st_size - removed_count * 4
                    write_u32(data, sym_off + 4, new_value)
                    write_u32(data, sym_off + 8, new_size)

    return bytes(data)


def main():
    parser = argparse.ArgumentParser(
        description='CSE lui at instructions in IDO-compiled object files')
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', nargs='?',
                        help='Output ELF file (omit with --list)')
    parser.add_argument('-v', '--verbose', action='store_true')
    parser.add_argument('--same-symbol-only', action='store_true',
                        help='only CSE lui $at pairs that reference the SAME '
                             'symbol, with no $at clobber or control-flow '
                             'boundary in between')
    parser.add_argument('--offsets',
                        help='comma/space separated .text byte offsets of the '
                             'lui $at instructions to remove (whitelist)')
    parser.add_argument('--fix-branches', dest='fix_branches',
                        action='store_true', default=None,
                        help='re-target PC-relative branches that span a '
                             'removed lui (default: on in scoped mode)')
    parser.add_argument('--no-fix-branches', dest='fix_branches',
                        action='store_false',
                        help='leave branch displacements untouched')
    parser.add_argument('--nop-in-place', action='store_true',
                        help='leave a nop in the removed slot (drop only '
                             'the HI16 reloc); preserves instruction count')
    parser.add_argument('--list', action='store_true',
                        help='list every lui $at with its .text offset and '
                             'symbol, then exit without writing')

    args = parser.parse_args()

    offsets = None
    if args.offsets:
        offsets = []
        for tok in re.split(r'[\s,]+', args.offsets.strip()):
            if tok:
                offsets.append(int(tok, 16) if tok.lower().startswith('0x')
                               else int(tok, 0))

    with open(args.input, 'rb') as f:
        data = f.read()

    if args.list:
        cse_lui_at(data, verbose=args.verbose, list_only=True)
        return

    if not args.output:
        parser.error('output is required unless --list is given')

    result = cse_lui_at(data, args.verbose,
                        same_symbol_only=args.same_symbol_only,
                        offsets=offsets, fix_branches=args.fix_branches,
                        nop_in_place=args.nop_in_place)

    with open(args.output, 'wb') as f:
        f.write(result)

    # Report
    old_text = 0
    new_text = 0
    for sh in parse_elf_le(data):
        if sh['name'] == '.text':
            old_text = sh['sh_size']
    for sh in parse_elf_le(result):
        if sh['name'] == '.text':
            new_text = sh['sh_size']

    removed = old_text - new_text
    if removed > 0:
        print(f"CSE'd lui at: {old_text} -> {new_text} bytes (removed {removed} bytes / {removed // 4} instructions)")
    else:
        print("No changes made")


if __name__ == '__main__':
    main()
