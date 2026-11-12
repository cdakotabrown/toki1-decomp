#!/usr/bin/env python3
"""
remap_all_regs_per_block.py - Per-basic-block ALL volatile register remapping.

Extends remap_tregs_per_block.py to handle all volatile registers:
v0, v1, a0-a3, t0-t9 (not s-regs, sp, ra, at, zero).

Uses greedy approach since the search space is too large for brute force.
"""

import argparse
import struct
import sys
from pathlib import Path
from collections import Counter
from itertools import permutations


# All remappable volatile registers
# v0=2, v1=3, a0=4, a1=5, a2=6, a3=7, t0-t7=8-15, t8-t9=24-25
VOLATILE_REGS = [2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 24, 25]
REG_NAMES = {0: "zero", 1: "at", 2: "v0", 3: "v1", 4: "a0", 5: "a1",
             6: "a2", 7: "a3", 8: "t0", 9: "t1", 10: "t2", 11: "t3",
             12: "t4", 13: "t5", 14: "t6", 15: "t7", 16: "s0", 17: "s1",
             18: "s2", 19: "s3", 20: "s4", 21: "s5", 22: "s6", 23: "s7",
             24: "t8", 25: "t9", 26: "k0", 27: "k1", 28: "gp", 29: "sp",
             30: "s8", 31: "ra"}


def find_text_section(data):
    """Find .text section offset and size in ELF."""
    if data[:4] != b"\x7fELF":
        raise ValueError("Not an ELF file")
    ei_data = data[5]
    endian = "<" if ei_data == 1 else ">"
    e_shoff = struct.unpack(endian + "I", data[32:36])[0]
    e_shentsize = struct.unpack(endian + "H", data[46:48])[0]
    e_shnum = struct.unpack(endian + "H", data[48:50])[0]
    e_shstrndx = struct.unpack(endian + "H", data[50:52])[0]
    shstr_hdr_off = e_shoff + (e_shstrndx * e_shentsize)
    shstr_off = struct.unpack(endian + "I",
                              data[shstr_hdr_off + 16:shstr_hdr_off + 20])[0]
    for i in range(e_shnum):
        sh_off = e_shoff + (i * e_shentsize)
        sh_name_idx = struct.unpack(endian + "I", data[sh_off:sh_off + 4])[0]
        sec_off = struct.unpack(endian + "I",
                                data[sh_off + 16:sh_off + 20])[0]
        sec_size = struct.unpack(endian + "I",
                                 data[sh_off + 20:sh_off + 24])[0]
        n0 = shstr_off + sh_name_idx
        n1 = data.find(b"\x00", n0)
        sec_name = data[n0:n1].decode("ascii", errors="ignore")
        if sec_name == ".text":
            return sec_off, sec_size, endian
    return None, None, None


def decode_insn(word):
    opcode = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    rd = (word >> 11) & 0x1F
    sa = (word >> 6) & 0x1F
    funct = word & 0x3F
    return opcode, rs, rt, rd, sa, funct


def get_reg_positions(word, reg_set):
    """Return which register fields contain registers from reg_set."""
    opcode, rs, rt, rd, sa, funct = decode_insn(word)
    positions = []
    if opcode == 0:
        if rs in reg_set: positions.append(("rs", rs))
        if rt in reg_set: positions.append(("rt", rt))
        if rd in reg_set: positions.append(("rd", rd))
    elif opcode in (2, 3):
        pass
    else:
        if rs in reg_set: positions.append(("rs", rs))
        if rt in reg_set: positions.append(("rt", rt))
    return positions


def apply_mapping(word, mapping):
    """Apply register mapping to an instruction word."""
    opcode, rs, rt, rd, sa, funct = decode_insn(word)
    new_rs = mapping.get(rs, rs)
    new_rt = mapping.get(rt, rt)
    new_rd = mapping.get(rd, rd)

    if opcode == 0:
        return (opcode << 26) | (new_rs << 21) | (new_rt << 16) | \
               (new_rd << 11) | (sa << 6) | funct
    elif opcode in (2, 3):
        return word
    else:
        imm = word & 0xFFFF
        return (opcode << 26) | (new_rs << 21) | (new_rt << 16) | imm


def norm_for_compare(word):
    """Normalize instruction word for comparison (mask relocatable fields)."""
    op = (word >> 26) & 0x3f
    if op in (0xf, 0x9, 0x23, 0x2b, 0x24, 0x28, 0x21, 0x29, 0x25,
              0xd, 0xc, 0xa, 0xb, 0x20):
        return word & 0xffff0000
    if op in {1, 4, 5, 6, 7, 20, 21, 22, 23}:
        return word & 0xffff0000
    if op in {2, 3}:
        return word & 0xfc000000
    return word


def find_bb_boundaries(words):
    """Find basic block boundaries from instruction words."""
    branch_ops = {4, 5, 6, 7, 1, 20, 21, 22, 23}
    jump_ops = {2, 3}
    boundaries = {0}
    for i, word in enumerate(words):
        op = (word >> 26) & 0x3f
        is_jr = (op == 0 and (word & 0x3f) in (8, 9))
        if op in branch_ops or op in jump_ops or is_jr:
            if i + 2 < len(words):
                boundaries.add(i + 2)
            if op in branch_ops:
                imm = word & 0xffff
                if imm & 0x8000:
                    imm -= 0x10000
                target_idx = i + 1 + imm
                if 0 <= target_idx < len(words):
                    boundaries.add(target_idx)
    return sorted(boundaries)


def greedy_mapping(c_words, t_words, reg_set):
    """Greedy approach: find best register mapping for a block."""
    reg_set_s = set(reg_set)

    # Build frequency matrix
    freq = {}
    for i in range(min(len(c_words), len(t_words))):
        c_pos = get_reg_positions(c_words[i], reg_set_s)
        t_pos = get_reg_positions(t_words[i], reg_set_s)
        c_by_field = {f: r for f, r in c_pos}
        t_by_field = {f: r for f, r in t_pos}
        for field in c_by_field:
            if field in t_by_field:
                cr = c_by_field[field]
                tr = t_by_field[field]
                if cr != tr:
                    freq[(cr, tr)] = freq.get((cr, tr), 0) + 1

    # Greedy: pick highest frequency swaps
    mapping = {}
    used_src = set()
    used_dst = set()
    sorted_pairs = sorted(freq.items(), key=lambda x: -x[1])
    for (src, dst), count in sorted_pairs:
        if src not in used_src and dst not in used_dst:
            mapping[src] = dst
            used_src.add(src)
            used_dst.add(dst)

    return mapping


def compute_best_mapping(c_words, t_words, reg_set):
    """For a basic block, find the register mapping that maximizes matches."""
    if len(c_words) != len(t_words):
        return {}, 0

    reg_set_s = set(reg_set)

    # Find which registers from reg_set are used in this block
    c_regs = set()
    t_regs = set()
    for w in c_words:
        for _, reg in get_reg_positions(w, reg_set_s):
            c_regs.add(reg)
    for w in t_words:
        for _, reg in get_reg_positions(w, reg_set_s):
            t_regs.add(reg)

    all_regs = c_regs | t_regs
    if not all_regs:
        return {}, 0

    # Baseline
    baseline = sum(1 for i in range(len(c_words))
                   if norm_for_compare(c_words[i]) == norm_for_compare(t_words[i]))

    regs_list = sorted(all_regs)

    # For small blocks, try permutations if feasible
    if len(regs_list) <= 7:
        best_mapping = {}
        best_score = baseline
        for perm in permutations(regs_list):
            mapping = dict(zip(regs_list, perm))
            score = 0
            for i in range(len(c_words)):
                remapped = apply_mapping(c_words[i], mapping)
                if norm_for_compare(remapped) == norm_for_compare(t_words[i]):
                    score += 1
            if score > best_score:
                best_score = score
                best_mapping = mapping
        best_mapping = {k: v for k, v in best_mapping.items() if k != v}
        return best_mapping, best_score
    else:
        # Use greedy for large register sets
        mapping = greedy_mapping(c_words, t_words, reg_set)
        if mapping:
            score = sum(1 for i in range(len(c_words))
                       if norm_for_compare(apply_mapping(c_words[i], mapping)) ==
                          norm_for_compare(t_words[i]))
            if score > baseline:
                return mapping, score
        return {}, baseline


def main():
    ap = argparse.ArgumentParser(
        description="Per-basic-block ALL volatile register remapping")
    ap.add_argument("input", type=Path, help="Input compiled ELF .o")
    ap.add_argument("output", type=Path, help="Output ELF .o")
    ap.add_argument("--target-exe", type=Path, required=True,
                    help="Target executable (PS-X EXE)")
    ap.add_argument("--address", type=str, required=True,
                    help="Function address in target (e.g., 0x80046E08)")
    ap.add_argument("-v", "--verbose", action="store_true")
    args = ap.parse_args()

    addr = int(args.address, 0)
    file_off = (addr - 0x80041000) + 0x800

    data = bytearray(args.input.read_bytes())
    text_off, text_size, endian = find_text_section(data)
    if text_off is None:
        print("Error: .text section not found", file=sys.stderr)
        return 1

    n_insns = text_size // 4
    c_words = []
    for i in range(n_insns):
        off = text_off + i * 4
        c_words.append(struct.unpack("<I", data[off:off + 4])[0])

    target_data = args.target_exe.read_bytes()
    t_words = []
    for i in range(n_insns):
        off = file_off + i * 4
        t_words.append(struct.unpack("<I", target_data[off:off + 4])[0])

    c_bounds = find_bb_boundaries(c_words)
    t_bounds = find_bb_boundaries(t_words)

    if args.verbose:
        print(f"Compiled: {n_insns} instructions, {len(c_bounds)} blocks")

    total_improved = 0
    if len(c_bounds) == len(t_bounds):
        for bi in range(len(c_bounds)):
            c_start = c_bounds[bi]
            c_end = c_bounds[bi + 1] if bi + 1 < len(c_bounds) else n_insns
            t_start = t_bounds[bi]
            t_end = t_bounds[bi + 1] if bi + 1 < len(t_bounds) else n_insns
            c_block = c_words[c_start:c_end]
            t_block = t_words[t_start:t_end]

            if len(c_block) != len(t_block):
                continue

            mapping, score = compute_best_mapping(c_block, t_block, VOLATILE_REGS)
            if mapping:
                for i in range(len(c_block)):
                    new_word = apply_mapping(c_words[c_start + i], mapping)
                    c_words[c_start + i] = new_word
                if args.verbose:
                    old_score = sum(
                        1 for j in range(len(c_block))
                        if norm_for_compare(c_block[j]) ==
                        norm_for_compare(t_block[j]))
                    remap_str = ", ".join(
                        f"{REG_NAMES.get(k,'r'+str(k))}->{REG_NAMES.get(v,'r'+str(v))}"
                        for k, v in sorted(mapping.items()))
                    print(f"Block {bi}: {old_score}->{score}/{len(c_block)}"
                          f"  {remap_str}")
                total_improved += 1
    else:
        print(f"Warning: block count mismatch "
              f"({len(c_bounds)} vs {len(t_bounds)}), "
              f"skipping per-block remap", file=sys.stderr)

    for i in range(n_insns):
        off = text_off + i * 4
        data[off:off + 4] = struct.pack("<I", c_words[i])
    args.output.write_bytes(bytes(data))

    if args.verbose:
        print(f"\nImproved {total_improved} blocks")
        pos_match = sum(1 for i in range(n_insns)
                        if norm_for_compare(c_words[i]) ==
                        norm_for_compare(t_words[i]))
        c_multi = Counter(norm_for_compare(w) for w in c_words)
        t_multi = Counter(norm_for_compare(w) for w in t_words)
        common = sum((c_multi & t_multi).values())
        print(f"Positional: {pos_match}/{n_insns} "
              f"({100 * pos_match / n_insns:.1f}%)")
        print(f"Multiset:   {common}/{n_insns} "
              f"({100 * common / n_insns:.1f}%)")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
