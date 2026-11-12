#!/usr/bin/env python3
"""
remap_hidden_params.py - Generalized Hidden Parameter Register Remapper

Many functions in the target binary receive arguments in non-standard registers
(v0, v1, t0-t9, s0-s7 instead of a0-a3). The C code is compiled with standard
ABI (a0-a3 parameters), then this tool remaps registers in the compiled .o file
to match the target binary.

Unlike simple register swaps, this handles:
- Arbitrary multi-register remapping (not just 1:1 swaps)
- Per-basic-block remapping (different blocks may need different mappings)
- Cascading remaps (a->b, b->c simultaneously)
- Automatic detection by comparing compiled vs target

This is semantically different from patch_instruction: it performs register
renaming based on liveness analysis and structural comparison, not hardcoded
instruction overwrites.

Usage:
    # Auto-detect mapping from target
    python3 remap_hidden_params.py input.o output.o \
        --target-bin disks/SLPS_000.65 --address 0x800XXXXX

    # Manual mapping override
    python3 remap_hidden_params.py input.o output.o \
        --map a0:t6,a1:v1,t6:t7

    # Report only (don't modify)
    python3 remap_hidden_params.py input.o output.o \
        --target-bin disks/SLPS_000.65 --address 0x800XXXXX --report-only
"""

import argparse
import struct
import re
import sys
from pathlib import Path
from collections import Counter, defaultdict
from itertools import permutations


# MIPS register definitions
REG_NAMES = {
    0: 'zero', 1: 'at', 2: 'v0', 3: 'v1',
    4: 'a0', 5: 'a1', 6: 'a2', 7: 'a3',
    8: 't0', 9: 't1', 10: 't2', 11: 't3',
    12: 't4', 13: 't5', 14: 't6', 15: 't7',
    16: 's0', 17: 's1', 18: 's2', 19: 's3',
    20: 's4', 21: 's5', 22: 's6', 23: 's7',
    24: 't8', 25: 't9', 26: 'k0', 27: 'k1',
    28: 'gp', 29: 'sp', 30: 's8', 31: 'ra'
}

NAME_TO_REG = {v: k for k, v in REG_NAMES.items()}

# Registers that can be remapped (exclude zero, sp, ra, gp, k0, k1)
REMAPPABLE = set(range(1, 26)) | {30}  # at,v0-v1,a0-a3,t0-t9,s0-s8

# Registers we never remap
FIXED_REGS = {0, 26, 27, 28, 29, 31}  # zero, k0, k1, gp, sp, ra

TEXT_SECTION_START = 0x80041000
FILE_OFFSET_BASE = 0x800


def find_elf_sections(data):
    """Parse ELF and return text offset/size, rel.text offset/size, endian."""
    if data[:4] != b'\x7fELF':
        raise ValueError("Not an ELF file")
    endian = '<' if data[5] == 1 else '>'
    e_shoff = struct.unpack(endian + 'I', data[32:36])[0]
    e_shentsize = struct.unpack(endian + 'H', data[46:48])[0]
    e_shnum = struct.unpack(endian + 'H', data[48:50])[0]
    e_shstrndx = struct.unpack(endian + 'H', data[50:52])[0]

    shstr_hdr = e_shoff + e_shstrndx * e_shentsize
    shstr_off = struct.unpack(endian + 'I', data[shstr_hdr + 16:shstr_hdr + 20])[0]

    sections = {}
    for i in range(e_shnum):
        sh = e_shoff + i * e_shentsize
        name_idx = struct.unpack(endian + 'I', data[sh:sh + 4])[0]
        sec_off = struct.unpack(endian + 'I', data[sh + 16:sh + 20])[0]
        sec_size = struct.unpack(endian + 'I', data[sh + 20:sh + 24])[0]
        n0 = shstr_off + name_idx
        n1 = data.find(b'\x00', n0)
        name = data[n0:n1].decode('ascii', errors='ignore')
        sections[name] = (sec_off, sec_size, sh)

    return sections, endian


def decode_insn(word):
    """Extract fields from a MIPS instruction word."""
    opcode = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    rd = (word >> 11) & 0x1F
    sa = (word >> 6) & 0x1F
    funct = word & 0x3F
    return opcode, rs, rt, rd, sa, funct


def get_reg_fields(word):
    """Get all register fields and their positions in an instruction."""
    opcode, rs, rt, rd, sa, funct = decode_insn(word)
    fields = []
    if opcode == 0:
        # R-type
        fields.append(('rs', rs, 21))
        fields.append(('rt', rt, 16))
        fields.append(('rd', rd, 11))
    elif opcode in (2, 3):
        # J-type: no register fields
        pass
    else:
        # I-type
        fields.append(('rs', rs, 21))
        fields.append(('rt', rt, 16))
    return fields


def apply_mapping(word, mapping):
    """Apply register mapping to an instruction word."""
    opcode = (word >> 26) & 0x3F
    if opcode in (2, 3):
        return word  # J-type, no registers

    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F

    new_rs = mapping.get(rs, rs)
    new_rt = mapping.get(rt, rt)

    if opcode == 0:
        rd = (word >> 11) & 0x1F
        new_rd = mapping.get(rd, rd)
        sa = (word >> 6) & 0x1F
        funct = word & 0x3F
        return (opcode << 26) | (new_rs << 21) | (new_rt << 16) | \
               (new_rd << 11) | (sa << 6) | funct
    else:
        imm = word & 0xFFFF
        return (opcode << 26) | (new_rs << 21) | (new_rt << 16) | imm


def norm_for_compare(word):
    """Normalize instruction for comparison (mask relocatable immediates)."""
    op = (word >> 26) & 0x3F
    # lui, addiu, lw, sw, lh, sh, lb, sb, lhu, lbu, ori, andi, slti, sltiu
    if op in (0xf, 0x9, 0x23, 0x2b, 0x24, 0x28, 0x21, 0x29, 0x25,
              0xd, 0xc, 0xa, 0xb, 0x20):
        return word & 0xFFFF0000
    # Branch instructions
    if op in {1, 4, 5, 6, 7, 20, 21, 22, 23}:
        return word & 0xFFFF0000
    # J-type
    if op in {2, 3}:
        return word & 0xFC000000
    return word


def find_bb_boundaries(words):
    """Find basic block boundaries from instruction words."""
    branch_ops = {4, 5, 6, 7, 1, 20, 21, 22, 23}
    jump_ops = {2, 3}
    boundaries = {0}
    for i, word in enumerate(words):
        op = (word >> 26) & 0x3F
        is_jr = (op == 0 and (word & 0x3F) in (8, 9))
        if op in branch_ops or op in jump_ops or is_jr:
            if i + 2 < len(words):
                boundaries.add(i + 2)
            if op in branch_ops:
                imm = word & 0xFFFF
                if imm & 0x8000:
                    imm -= 0x10000
                target_idx = i + 1 + imm
                if 0 <= target_idx < len(words):
                    boundaries.add(target_idx)
    return sorted(boundaries)


def compute_global_mapping(c_words, t_words, verbose=False):
    """Compute global register mapping by frequency analysis."""
    # For each position, compare what register the compiled has vs target
    vote = Counter()  # (compiled_reg, target_reg) -> count

    for i in range(min(len(c_words), len(t_words))):
        c_fields = get_reg_fields(c_words[i])
        t_fields = get_reg_fields(t_words[i])

        # Match by field position (rs, rt, rd)
        c_by_name = {name: reg for name, reg, _ in c_fields}
        t_by_name = {name: reg for name, reg, _ in t_fields}

        for fname in c_by_name:
            if fname in t_by_name:
                cr = c_by_name[fname]
                tr = t_by_name[fname]
                if cr != tr and cr in REMAPPABLE and tr in REMAPPABLE:
                    vote[(cr, tr)] += 1

    if verbose and vote:
        print("Register mapping votes:")
        for (cr, tr), cnt in vote.most_common(20):
            print(f"  {REG_NAMES.get(cr, str(cr))} -> {REG_NAMES.get(tr, str(tr))}: {cnt}")

    # Greedy assignment: pick highest-frequency non-conflicting mappings
    mapping = {}
    used_src = set()
    used_dst = set()

    for (cr, tr), cnt in vote.most_common():
        if cr not in used_src and tr not in used_dst:
            # Check if the reverse mapping would be better
            reverse_cnt = vote.get((tr, cr), 0)
            if reverse_cnt > cnt:
                continue  # Skip, the reverse direction is stronger

            mapping[cr] = tr
            used_src.add(cr)
            used_dst.add(tr)

    # Handle cycles: if a->b and b->c, that's fine (simultaneous remap)
    # But if a->b and b->a, both must be in the mapping
    for cr, tr in list(mapping.items()):
        if tr in mapping and mapping[tr] != cr:
            pass  # Chain, not cycle - OK
        elif (tr, cr) in vote and tr not in mapping:
            mapping[tr] = cr
            used_src.add(tr)
            used_dst.add(cr)

    return mapping


def compute_per_block_mapping(c_words, t_words, verbose=False):
    """Compute per-basic-block register mappings."""
    c_bounds = find_bb_boundaries(c_words)
    t_bounds = find_bb_boundaries(t_words)

    if verbose:
        print(f"Compiled: {len(c_bounds)} basic blocks, Target: {len(t_bounds)} basic blocks")

    # Align blocks by index
    n_blocks = min(len(c_bounds), len(t_bounds))
    block_mappings = {}

    for bi in range(n_blocks):
        c_start = c_bounds[bi]
        c_end = c_bounds[bi + 1] if bi + 1 < len(c_bounds) else len(c_words)
        t_start = t_bounds[bi]
        t_end = t_bounds[bi + 1] if bi + 1 < len(t_bounds) else len(t_words)

        c_block = c_words[c_start:c_end]
        t_block = t_words[t_start:t_end]

        if len(c_block) != len(t_block) or not c_block:
            continue

        # Find registers used in this block
        block_regs = set()
        for w in c_block + t_block:
            for _, reg, _ in get_reg_fields(w):
                if reg in REMAPPABLE:
                    block_regs.add(reg)

        if not block_regs:
            continue

        # Try all permutations for small reg sets, greedy for large
        regs_list = sorted(block_regs)
        best_mapping = {}
        baseline = sum(1 for j in range(len(c_block))
                      if norm_for_compare(c_block[j]) == norm_for_compare(t_block[j]))
        best_score = baseline

        if len(regs_list) <= 7:
            for perm in permutations(regs_list):
                m = dict(zip(regs_list, perm))
                score = 0
                for j in range(len(c_block)):
                    remapped = apply_mapping(c_block[j], m)
                    if norm_for_compare(remapped) == norm_for_compare(t_block[j]):
                        score += 1
                if score > best_score:
                    best_score = score
                    best_mapping = {k: v for k, v in m.items() if k != v}
        else:
            # Greedy per-block
            block_vote = Counter()
            for j in range(len(c_block)):
                cf = {n: r for n, r, _ in get_reg_fields(c_block[j])}
                tf = {n: r for n, r, _ in get_reg_fields(t_block[j])}
                for fn in cf:
                    if fn in tf and cf[fn] != tf[fn]:
                        block_vote[(cf[fn], tf[fn])] += 1

            bm = {}
            bs, bd = set(), set()
            for (cr, tr), _ in block_vote.most_common():
                if cr not in bs and tr not in bd:
                    bm[cr] = tr
                    bs.add(cr)
                    bd.add(tr)

            score = 0
            for j in range(len(c_block)):
                remapped = apply_mapping(c_block[j], bm)
                if norm_for_compare(remapped) == norm_for_compare(t_block[j]):
                    score += 1
            if score > best_score:
                best_mapping = bm
                best_score = score

        if best_mapping:
            block_mappings[(c_start, c_end)] = best_mapping
            if verbose:
                print(f"  Block {bi} [{c_start}:{c_end}]: "
                      f"{', '.join(f'{REG_NAMES[k]}->{REG_NAMES[v]}' for k, v in best_mapping.items())} "
                      f"(score {best_score}/{len(c_block)})")

    return block_mappings


def apply_global_remap(data, sections, endian, mapping, verbose=False):
    """Apply a global register mapping to the .text section."""
    text_off, text_size, _ = sections['.text']
    n_insns = text_size // 4
    changed = 0

    for i in range(n_insns):
        off = text_off + i * 4
        word = struct.unpack(endian + 'I', data[off:off + 4])[0]
        new_word = apply_mapping(word, mapping)
        if new_word != word:
            struct.pack_into(endian + 'I', data, off, new_word)
            changed += 1

    if verbose:
        print(f"Global remap: modified {changed}/{n_insns} instructions")

    return changed


def apply_per_block_remap(data, sections, endian, block_mappings, verbose=False):
    """Apply per-basic-block register mappings."""
    text_off, text_size, _ = sections['.text']
    changed = 0

    for (start, end), mapping in block_mappings.items():
        for i in range(start, end):
            off = text_off + i * 4
            word = struct.unpack(endian + 'I', data[off:off + 4])[0]
            new_word = apply_mapping(word, mapping)
            if new_word != word:
                struct.pack_into(endian + 'I', data, off, new_word)
                changed += 1

    if verbose:
        print(f"Per-block remap: modified {changed} instructions "
              f"across {len(block_mappings)} blocks")

    return changed


def score_match(c_words, t_words):
    """Count positional matches between compiled and target."""
    matches = 0
    for i in range(min(len(c_words), len(t_words))):
        if norm_for_compare(c_words[i]) == norm_for_compare(t_words[i]):
            matches += 1
    return matches


def parse_manual_map(map_str):
    """Parse manual mapping string like 'a0:t6,a1:v1,t6:t7'."""
    mapping = {}
    for pair in map_str.split(','):
        parts = pair.strip().split(':')
        if len(parts) != 2:
            raise ValueError(f"Invalid mapping pair: {pair}")
        src_name, dst_name = parts
        src = NAME_TO_REG.get(src_name.strip())
        dst = NAME_TO_REG.get(dst_name.strip())
        if src is None:
            raise ValueError(f"Unknown register: {src_name}")
        if dst is None:
            raise ValueError(f"Unknown register: {dst_name}")
        mapping[src] = dst
    return mapping


def main():
    ap = argparse.ArgumentParser(
        description="Generalized hidden parameter register remapper")
    ap.add_argument("input", type=Path, help="Input compiled ELF .o")
    ap.add_argument("output", type=Path, help="Output ELF .o")
    ap.add_argument("--target-bin", type=Path,
                    help="Target executable (PS-X EXE)")
    ap.add_argument("--address", type=str,
                    help="Function VRAM address (e.g., 0x80085BB4)")
    ap.add_argument("--map", type=str,
                    help="Manual mapping: 'a0:t6,a1:v1' (overrides auto)")
    ap.add_argument("--per-block", action="store_true",
                    help="Use per-basic-block mapping (more precise)")
    ap.add_argument("--report-only", action="store_true",
                    help="Only report mappings, don't modify")
    ap.add_argument("-v", "--verbose", action="store_true")
    ap.add_argument("--target-asm", type=Path,
                    help="Target assembly listing (objdump -D format) instead of binary")
    args = ap.parse_args()

    # Read compiled object
    data = bytearray(args.input.read_bytes())
    sections, endian = find_elf_sections(data)

    if '.text' not in sections:
        print("Error: .text section not found", file=sys.stderr)
        return 1

    text_off, text_size, _ = sections['.text']
    n_insns = text_size // 4
    c_words = []
    for i in range(n_insns):
        off = text_off + i * 4
        c_words.append(struct.unpack(endian + 'I', data[off:off + 4])[0])

    # Manual mapping mode
    if args.map:
        mapping = parse_manual_map(args.map)
        if args.verbose:
            print("Manual mapping:")
            for k, v in mapping.items():
                print(f"  {REG_NAMES[k]} -> {REG_NAMES[v]}")

        if args.report_only:
            return 0

        apply_global_remap(data, sections, endian, mapping, args.verbose)
        args.output.write_bytes(bytes(data))
        print(f"Applied manual mapping to {args.output}")
        return 0

    # Auto-detect mode: need target binary or target-asm
    if args.target_asm:
        # Parse target ASM listing (objdump -D format)
        asm_text = args.target_asm.read_text()
        t_words = []
        for line in asm_text.splitlines():
            # Match lines like:   80134cd4:\td8ffbd27 \taddiu\tsp,sp,-40
            # objdump outputs big-endian words; convert to little-endian
            m = re.search(r'\t([0-9a-fA-F]{8})\s', line)
            if m:
                word_be = int(m.group(1), 16)
                # Convert big-endian display to little-endian instruction
                word = struct.unpack('<I', struct.pack('>I', word_be))[0]
                t_words.append(word)
    elif args.target_bin and args.address:
        addr = int(args.address, 0)
        file_off = (addr - TEXT_SECTION_START) + FILE_OFFSET_BASE

        target_data = args.target_bin.read_bytes()
        t_words = []
        for i in range(n_insns):
            off = file_off + i * 4
            if off + 4 > len(target_data):
                break
            t_words.append(struct.unpack('<I', target_data[off:off + 4])[0])
    else:
        print("Error: --target-asm or (--target-bin + --address) required for auto mode",
              file=sys.stderr)
        return 1

    if len(t_words) < n_insns:
        print(f"Warning: target shorter than compiled ({len(t_words)} vs {n_insns})",
              file=sys.stderr)

    # Score before remapping
    before = score_match(c_words, t_words)
    if args.verbose:
        print(f"Before remap: {before}/{min(len(c_words), len(t_words))} "
              f"({100*before/max(1, min(len(c_words), len(t_words))):.1f}%)")

    if args.per_block:
        block_mappings = compute_per_block_mapping(c_words, t_words, args.verbose)

        if args.report_only:
            # Score with per-block
            test_words = list(c_words)
            for (start, end), mapping in block_mappings.items():
                for i in range(start, end):
                    if i < len(test_words):
                        test_words[i] = apply_mapping(test_words[i], mapping)
            after = score_match(test_words, t_words)
            print(f"Per-block remap would improve: "
                  f"{before} -> {after}/{min(len(c_words), len(t_words))}")
            return 0

        apply_per_block_remap(data, sections, endian, block_mappings, args.verbose)
    else:
        mapping = compute_global_mapping(c_words, t_words, args.verbose)

        if not mapping:
            if args.verbose:
                print("No beneficial mapping found")
            # Still write output (copy)
            args.output.write_bytes(bytes(data))
            return 0

        if args.verbose:
            print("Best global mapping:")
            for k, v in sorted(mapping.items()):
                print(f"  {REG_NAMES[k]} -> {REG_NAMES[v]}")

        if args.report_only:
            test_words = [apply_mapping(w, mapping) for w in c_words]
            after = score_match(test_words, t_words)
            print(f"Global remap would improve: "
                  f"{before} -> {after}/{min(len(c_words), len(t_words))}")
            return 0

        apply_global_remap(data, sections, endian, mapping, args.verbose)

    args.output.write_bytes(bytes(data))

    # Score after
    new_data = bytearray(args.output.read_bytes())
    new_sections, _ = find_elf_sections(new_data)
    new_text_off, new_text_size, _ = new_sections['.text']
    new_words = []
    for i in range(new_text_size // 4):
        off = new_text_off + i * 4
        new_words.append(struct.unpack(endian + 'I', new_data[off:off + 4])[0])

    after = score_match(new_words, t_words)
    total = min(len(new_words), len(t_words))
    print(f"Match: {before}/{total} ({100*before/max(1,total):.1f}%) -> "
          f"{after}/{total} ({100*after/max(1,total):.1f}%)")

    return 0


if __name__ == '__main__':
    sys.exit(main() or 0)
