#!/usr/bin/env python3
"""
block_reorder.py - Basic-block instruction reorderer for MIPS ELF .o files.

Reorders instructions within basic blocks of a compiled .o file to match
a target binary, respecting data dependencies and MIPS R3000 constraints.

Usage:
  python3 tools/postproc/block_reorder.py compiled.o output.o \
    --target-bin disks/SLPS_000.65 --address 0x800XXXXX [-v]
"""

import argparse, struct, sys
from pathlib import Path

TEXT_SECTION_START = 0x80041000
FILE_OFFSET_BASE = 0x800
BRANCH_OPS = {1, 4, 5, 6, 7, 20, 21, 22, 23}
JUMP_OPS = {2, 3}
LOAD_OPS = {0x20, 0x21, 0x23, 0x24, 0x25}
STORE_OPS = {0x28, 0x29, 0x2B}


def find_elf_section(data, name_want):
    """Find a named section's (offset, size) in an ELF file."""
    if data[:4] != b"\x7fELF":
        raise ValueError("Not an ELF file")
    e = "<" if data[5] == 1 else ">"
    shoff = struct.unpack(e + "I", data[32:36])[0]
    shentsz = struct.unpack(e + "H", data[46:48])[0]
    shnum = struct.unpack(e + "H", data[48:50])[0]
    shstrndx = struct.unpack(e + "H", data[50:52])[0]
    shstr_hdr = shoff + shstrndx * shentsz
    shstr_off = struct.unpack(e + "I", data[shstr_hdr + 16:shstr_hdr + 20])[0]
    for i in range(shnum):
        sh = shoff + i * shentsz
        nidx = struct.unpack(e + "I", data[sh:sh + 4])[0]
        soff = struct.unpack(e + "I", data[sh + 16:sh + 20])[0]
        ssz = struct.unpack(e + "I", data[sh + 20:sh + 24])[0]
        n0 = shstr_off + nidx
        name = data[n0:data.find(b"\x00", n0)].decode("ascii", errors="ignore")
        if name == name_want:
            return soff, ssz
    return None, None


def decode_insn(w):
    return (w >> 26) & 0x3F, (w >> 21) & 0x1F, (w >> 16) & 0x1F, \
           (w >> 11) & 0x1F, (w >> 6) & 0x1F, w & 0x3F


def insn_reads_writes(word):
    """Return (reads, writes) sets for a MIPS instruction."""
    op, rs, rt, rd, sa, fn = decode_insn(word)
    R, W = set(), set()
    if op == 0:
        if fn == 8:          R.add(rs)                    # jr
        elif fn == 9:        R.add(rs); W.add(rd or 31)   # jalr
        elif fn == 0x10:     W.add(rd); R.add('hi')       # mfhi
        elif fn == 0x12:     W.add(rd); R.add('lo')       # mflo
        elif fn == 0x11:     R.add(rs); W.add('hi')       # mthi
        elif fn == 0x13:     R.add(rs); W.add('lo')       # mtlo
        elif fn in (0x18, 0x19, 0x1A, 0x1B):              # mult/div
            R.update((rs, rt)); W.update(('hi', 'lo'))
        elif fn in (0, 2, 3): R.add(rt); W.add(rd)        # sll/srl/sra
        elif fn not in (0xC, 0xD):                         # normal R-type
            R.update((rs, rt)); W.add(rd)
    elif op in JUMP_OPS:
        if op == 3: W.add(31)                              # jal
    elif op in BRANCH_OPS:
        R.add(rs)
        if op in (4, 5): R.add(rt)                         # beq/bne
    elif op == 0x0F: W.add(rt)                              # lui
    elif op in LOAD_OPS:  R.add(rs); R.add('mem'); W.add(rt)
    elif op in STORE_OPS: R.update((rs, rt)); W.add('mem')
    else:                 R.add(rs); W.add(rt)              # I-type ALU
    R.discard(0); W.discard(0)
    return R, W


def is_branch(word):
    op = (word >> 26) & 0x3F
    return op in BRANCH_OPS or op in JUMP_OPS or \
           (op == 0 and (word & 0x3F) in (8, 9))


def find_bb_boundaries(words):
    bounds = {0}
    for i, w in enumerate(words):
        if is_branch(w):
            if i + 2 < len(words):
                bounds.add(i + 2)
            op = (w >> 26) & 0x3F
            if op in BRANCH_OPS:
                imm = w & 0xFFFF
                if imm & 0x8000: imm -= 0x10000
                t = i + 1 + imm
                if 0 <= t < len(words): bounds.add(t)
    return sorted(bounds)


def get_blocks(words, bounds):
    return [(bounds[i], bounds[i+1] if i+1 < len(bounds) else len(words))
            for i in range(len(bounds))]


def norm(word):
    op = (word >> 26) & 0x3F
    if op in (0xF, 0x9, 0x23, 0x2B, 0x24, 0x28, 0x21, 0x29, 0x25,
              0xD, 0xC, 0xA, 0xB, 0x20):
        return word & 0xFFFF0000
    if op in BRANCH_OPS: return word & 0xFFFF0000
    if op in JUMP_OPS:   return word & 0xFC000000
    return word


def score_match(cw, tw):
    if norm(cw) == norm(tw): return 3
    co, crs, crt, crd, _, cfn = decode_insn(cw)
    to, trs, trt, trd, _, tfn = decode_insn(tw)
    if co == to:
        if co == 0:
            if cfn == tfn and crs == trs and crt == trt and crd == trd: return 2
            if cfn == tfn: return 1
        else:
            if crs == trs and crt == trt: return 2
            return 1
    return 0


def build_deps(words):
    n = len(words)
    deps = {i: set() for i in range(n)}
    rw = [insn_reads_writes(w) for w in words]
    for i in range(n):
        ri, wi = rw[i]
        for j in range(i + 1, n):
            rj, wj = rw[j]
            if (wi & rj) or (ri & wj) or (wi & wj):
                deps[j].add(i)
    return deps


def topo_sort_biased(deps, desired, n):
    in_deg = {i: 0 for i in range(n)}
    for i in range(n):
        for p in deps[i]:
            if p < n: in_deg[i] += 1
    dpos = {idx: pos for pos, idx in enumerate(desired)}
    result, avail, remaining = [], set(), set(range(n))
    for i in range(n):
        if in_deg[i] == 0: avail.add(i)
    while avail:
        best = min(avail, key=lambda x: dpos.get(x, n))
        avail.remove(best); result.append(best); remaining.discard(best)
        for i in list(remaining):
            if best in deps[i]:
                in_deg[i] -= 1
                if in_deg[i] == 0: avail.add(i)
    return result if len(result) == n else None


def check_load_use(words, order):
    for pos in range(len(order) - 1):
        w = words[order[pos]]
        if (w >> 26) & 0x3F in LOAD_OPS:
            rt = (w >> 16) & 0x1F
            if rt and rt in insn_reads_writes(words[order[pos + 1]])[0]:
                return False
    return True


def fix_load_use(words, order, deps):
    order = list(order)
    for _ in range(len(order) * 2):
        ok = True
        for pos in range(len(order) - 1):
            w = words[order[pos]]
            if (w >> 26) & 0x3F not in LOAD_OPS: continue
            rt = (w >> 16) & 0x1F
            if not rt: continue
            if rt not in insn_reads_writes(words[order[pos + 1]])[0]: continue
            ok = False
            swapped = False
            for sp in range(pos + 2, len(order)):
                if rt not in insn_reads_writes(words[order[sp]])[0]:
                    trial = list(order)
                    trial[pos + 1], trial[sp] = trial[sp], trial[pos + 1]
                    if all(all(order.index(p) < order.index(i)
                               for p in deps[i] if p < len(words))
                           for i in (trial[pos + 1], trial[sp])
                           if i in deps):
                        order = trial; swapped = True; break
            if not swapped: return None
        if ok: break
    return order if check_load_use(words, order) else None


def find_best_reorder(c_block, t_block):
    n = len(c_block)
    if n <= 1 or n != len(t_block): return None

    # Identify fixed branch+delay at end
    fixed = 0
    if n >= 2 and is_branch(c_block[-2]): fixed = 2
    elif n >= 1 and is_branch(c_block[-1]): fixed = 1
    rn = n - fixed
    if rn <= 1: return None

    rwords = c_block[:rn]
    deps = build_deps(rwords)

    # Greedy match each target position to best compiled instruction
    used, mapping = set(), [None] * rn
    for tp in range(rn):
        best_i, best_s = None, -1
        for ci in range(rn):
            if ci in used:
                continue
            s = score_match(rwords[ci], t_block[tp])
            if s > best_s: best_s = s; best_i = ci
        mapping[tp] = best_i; used.add(best_i)

    if mapping == list(range(rn)): return None

    # Validate / fix dependencies
    pos_of = {idx: p for p, idx in enumerate(mapping)}
    valid = all(pos_of.get(p, -1) < pos_of[i]
                for i in range(rn) for p in deps[i] if p < rn)
    if not valid:
        mapping = topo_sort_biased(deps, mapping, rn)
        if mapping is None: return None

    if not check_load_use(rwords, mapping):
        mapping = fix_load_use(rwords, mapping, deps)
        if mapping is None: return None

    full = mapping + list(range(rn, n))
    old_m = sum(1 for i in range(n) if norm(c_block[i]) == norm(t_block[i]))
    new_m = sum(1 for i in range(n)
                if norm(c_block[full[i]]) == norm(t_block[i]))
    return full if new_m > old_m else None


def main():
    ap = argparse.ArgumentParser(
        description="Reorder instructions within basic blocks to match target")
    ap.add_argument("input", type=Path)
    ap.add_argument("output", type=Path)
    ap.add_argument("--target-bin", type=Path, required=True)
    ap.add_argument("--address", type=str, required=True)
    ap.add_argument("-v", "--verbose", action="store_true")
    args = ap.parse_args()

    addr = int(args.address, 0)
    file_off = (addr - TEXT_SECTION_START) + FILE_OFFSET_BASE

    data = bytearray(args.input.read_bytes())
    toff, tsz = find_elf_section(data, ".text")
    if toff is None:
        print("Error: .text not found", file=sys.stderr); return 1

    n_insns = tsz // 4
    c_words = [struct.unpack("<I", data[toff+i*4:toff+i*4+4])[0]
               for i in range(n_insns)]

    tgt = args.target_bin.read_bytes()
    if file_off + n_insns * 4 > len(tgt):
        print("Error: address out of range", file=sys.stderr); return 1
    t_words = [struct.unpack("<I", tgt[file_off+i*4:file_off+i*4+4])[0]
               for i in range(n_insns)]

    c_bounds = find_bb_boundaries(c_words)
    t_bounds = find_bb_boundaries(t_words)
    c_blocks = get_blocks(c_words, c_bounds)
    t_blocks = get_blocks(t_words, t_bounds)

    if args.verbose:
        print(f"Compiled: {n_insns} insns, {len(c_blocks)} blocks")
        print(f"Target:   {n_insns} insns, {len(t_blocks)} blocks")

    if len(c_blocks) != len(t_blocks):
        print(f"Warning: block count mismatch ({len(c_blocks)} vs "
              f"{len(t_blocks)}), using compiled boundaries", file=sys.stderr)
        t_blocks = c_blocks

    # Load relocations
    roff, rsz = find_elf_section(data, ".rel.text")
    relocs = {}
    if roff is not None and rsz:
        for i in range(rsz // 8):
            ro = roff + i * 8
            relocs[struct.unpack("<I", data[ro:ro+4])[0]] = i

    # Process blocks
    total_reordered = 0
    off_map = {}

    for bi in range(len(c_blocks)):
        cs, ce = c_blocks[bi]
        ts, te = t_blocks[bi]
        cb = c_words[cs:ce]
        tb = t_words[ts:te]

        if len(cb) != len(tb):
            for j in range(len(cb)):
                off_map[( cs + j) * 4] = (cs + j) * 4
            continue

        reorder = find_best_reorder(cb, tb)
        if reorder is None:
            for j in range(len(cb)):
                off_map[(cs + j) * 4] = (cs + j) * 4
            continue

        new_block = [cb[reorder[i]] for i in range(len(cb))]
        for j in range(len(new_block)):
            c_words[cs + j] = new_block[j]
            off_map[(cs + reorder[j]) * 4] = (cs + j) * 4

        total_reordered += 1
        if args.verbose:
            old_m = sum(1 for j in range(len(cb))
                        if norm(cb[j]) == norm(tb[j]))
            new_m = sum(1 for j in range(len(new_block))
                        if norm(new_block[j]) == norm(tb[j]))
            print(f"Block {bi} ({cs}-{ce}): {old_m}->{new_m}/{len(cb)}")

    # Write updated .text
    for i in range(n_insns):
        struct.pack_into("<I", data, toff + i * 4, c_words[i])

    # Update relocations
    if roff is not None and rsz:
        for old_off, rel_idx in relocs.items():
            if old_off in off_map and off_map[old_off] != old_off:
                ro = roff + rel_idx * 8
                struct.pack_into("<I", data, ro, off_map[old_off])
                if args.verbose:
                    print(f"Reloc 0x{old_off:x} -> 0x{off_map[old_off]:x}")

    args.output.write_bytes(bytes(data))

    if args.verbose or total_reordered > 0:
        pm = sum(1 for i in range(n_insns)
                 if norm(c_words[i]) == norm(t_words[i]))
        print(f"Reordered {total_reordered} blocks, "
              f"positional: {pm}/{n_insns} ({100*pm/n_insns:.1f}%)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
