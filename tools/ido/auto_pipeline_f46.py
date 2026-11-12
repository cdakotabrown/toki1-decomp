#!/usr/bin/env python3
"""
auto_pipeline_f46.py - Auto-discovery pipeline for iris_show.

Automatically discovers LICM sites, s-register cycles, stack slot mappings,
t-register rotation, and scoped renames by analyzing the compiled output
against the target binary.

This eliminates the need to manually tune byte offsets when the C source changes.

Usage:
  python3 tools/ido/auto_pipeline_f46.py src/main/wip/iris_show.c \
    --target-exe disks/SLPS_000.65 --address 0x80046E08 -v
"""

import argparse
import struct
import subprocess
import sys
import re
import shutil
from pathlib import Path
from collections import Counter
from itertools import permutations, combinations


TARGET_ADDR = 0x80046E08
TARGET_BODY_INSNS = 671  # instructions in function body (up to jr ra + delay)
LICM_SYMBOL_HI = 0x800f
LICM_SYMBOL_LO = 0x1180
LICM_SYMBOL_NAME = "ot_buff0"

# MIPS register numbers
SREG_NUMS = list(range(16, 24))  # s0=16..s7=23
TREG_NUMS = [8, 9, 10, 11, 12, 13, 14, 15, 24, 25]  # t0-t9


def find_text_section(data):
    """Find .text section offset, size, and section header index in ELF."""
    e_shoff = struct.unpack("<I", data[32:36])[0]
    e_shentsize = struct.unpack("<H", data[46:48])[0]
    e_shnum = struct.unpack("<H", data[48:50])[0]
    e_shstrndx = struct.unpack("<H", data[50:52])[0]
    shstr_hdr_off = e_shoff + (e_shstrndx * e_shentsize)
    shstr_off = struct.unpack("<I",
                              data[shstr_hdr_off + 16:shstr_hdr_off + 20])[0]
    for i in range(e_shnum):
        sh_off = e_shoff + (i * e_shentsize)
        sh_name_idx = struct.unpack("<I", data[sh_off:sh_off + 4])[0]
        sec_off = struct.unpack("<I", data[sh_off + 16:sh_off + 20])[0]
        sec_size = struct.unpack("<I", data[sh_off + 20:sh_off + 24])[0]
        n0 = shstr_off + sh_name_idx
        n1 = data.find(b"\x00", n0)
        sec_name = data[n0:n1].decode("ascii", errors="ignore")
        if sec_name == ".text":
            return sec_off, sec_size
    return None, None


def get_words(data, text_off, text_size):
    """Extract instruction words from .text section, stripping trailing padding."""
    n = text_size // 4
    words = [struct.unpack("<I", data[text_off + i * 4:text_off + (i + 1) * 4])[0]
             for i in range(n)]
    while words and words[-1] == 0x00000000:
        words.pop()
    return words


def get_target_words(exe_path, addr, count):
    """Extract target instruction words from PS-X EXE."""
    file_off = (addr - 0x80041000) + 0x800
    data = exe_path.read_bytes()
    return [struct.unpack("<I", data[file_off + i * 4:file_off + (i + 1) * 4])[0]
            for i in range(count)]


def decode(word):
    """Decode MIPS instruction fields."""
    return {
        'op': (word >> 26) & 0x3F,
        'rs': (word >> 21) & 0x1F,
        'rt': (word >> 16) & 0x1F,
        'rd': (word >> 11) & 0x1F,
        'sa': (word >> 6) & 0x1F,
        'funct': word & 0x3F,
        'imm': word & 0xFFFF,
        'simm': (word & 0xFFFF) - 0x10000 if word & 0x8000 else word & 0xFFFF,
    }


def norm(word):
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


def find_licm_remove_and_expand(c_words, verbose=False):
    """
    Find LICM-hoisted ot_buff0 instructions to remove and use sites to expand.

    LICM hoists address computation into prologue using a TEMP register (not s-reg):
    Pattern: lui tN, 0 ... addiu tN, tN, 0 ... sw tN, X(sp)
    The sw stores the address to a stack slot, which is later loaded at use sites.
    Use sites: lw tM, X(sp) where X is the same stack offset.
    """
    removes = []
    expands = []
    licm_stack_offset = None

    # Find prologue end (first branch/jump)
    prologue_end = 0
    for i, w in enumerate(c_words):
        d = decode(w)
        if d['op'] in {4, 5, 6, 7, 1, 20, 21, 22, 23, 2, 3}:
            prologue_end = i
            break
        if d['op'] == 0 and d['funct'] in (8, 9):
            prologue_end = i
            break

    # Find lui tN, 0 (temp register with relocatable immediate) in prologue
    # Temp registers: t0-t9 (r8-r15, r24-r25)
    lui_temps = {}  # reg -> index
    for i in range(prologue_end):
        d = decode(c_words[i])
        if d['op'] == 0xf and d['rt'] in TREG_NUMS:  # lui tN
            lui_temps[d['rt']] = i

    # Find matching addiu tN, tN, 0 and subsequent sw tN, X(sp)
    for reg, lui_idx in lui_temps.items():
        addiu_idx = None
        sw_idx = None
        for i in range(lui_idx + 1, prologue_end + 10):
            if i >= len(c_words):
                break
            d = decode(c_words[i])
            if d['op'] == 0x9 and d['rs'] == reg and d['rt'] == reg:
                addiu_idx = i
            if (addiu_idx is not None and d['op'] == 0x2b and
                    d['rs'] == 29 and d['rt'] == reg):  # sw tN, X(sp)
                sw_idx = i
                licm_stack_offset = d['imm']
                break

        if addiu_idx is not None and sw_idx is not None:
            removes = [lui_idx * 4, addiu_idx * 4, sw_idx * 4]
            if verbose:
                print(f"LICM: lui r{reg} at 0x{lui_idx*4:x}, "
                      f"addiu at 0x{addiu_idx*4:x}, "
                      f"sw to sp+{licm_stack_offset} at 0x{sw_idx*4:x}")
            break

    # Find expand sites: lw tM, OFFSET(sp) where OFFSET = licm_stack_offset
    if licm_stack_offset is not None:
        for i, w in enumerate(c_words):
            d = decode(w)
            if (d['op'] == 0x23 and d['rs'] == 29 and
                    d['imm'] == licm_stack_offset and i * 4 not in removes):
                expands.append(i * 4)

    removes = sorted(set(removes))
    if verbose:
        print(f"LICM remove offsets: {[f'0x{x:x}' for x in removes]}")
        print(f"LICM expand offsets ({len(expands)}): "
              f"{[f'0x{x:x}' for x in expands]}")

    return removes, expands


def derive_stack_mapping_for_cycle(c_words, t_words, cycle_map):
    """
    Derive stack slot mapping from a given s-register cycle mapping.

    After cycle rename, s-reg saves at compiled offsets need to map to
    target offsets. Also does greedy matching of non-s-reg temp slots.
    """
    def get_sreg_saves(words, limit=60):
        saves = {}
        for i in range(min(limit, len(words))):
            d = decode(words[i])
            if d['op'] == 0x2b and d['rs'] == 29 and d['rt'] in SREG_NUMS:
                saves[d['rt']] = d['imm']
        return saves

    c_saves = get_sreg_saves(c_words)
    t_saves = get_sreg_saves(t_words)
    slot_map = {}

    for reg, c_off in c_saves.items():
        renamed_reg = cycle_map.get(reg, reg)
        if renamed_reg in t_saves:
            t_off = t_saves[renamed_reg]
            if c_off != t_off:
                slot_map[c_off] = t_off

    # Also find non-s-reg slot mappings by greedy use-count matching
    c_all_slots = set()
    t_all_slots = set()
    for w in c_words:
        d = decode(w)
        if d['rs'] == 29 and d['op'] in (0x23, 0x2b, 0x21, 0x29, 0x20, 0x28,
                                          0x25, 0x24):
            if d['imm'] not in (0, 168):
                c_all_slots.add(d['imm'])
    for w in t_words:
        d = decode(w)
        if d['rs'] == 29 and d['op'] in (0x23, 0x2b, 0x21, 0x29, 0x20, 0x28,
                                          0x25, 0x24):
            if d['imm'] not in (0, 168):
                t_all_slots.add(d['imm'])

    c_unmapped = sorted(c_all_slots - set(slot_map.keys()) -
                        set(slot_map.values()))
    t_unmapped = sorted(t_all_slots - set(slot_map.values()) -
                        set(slot_map.keys()))

    def count_slot_uses(words, slot):
        count = 0
        for w in words:
            d = decode(w)
            if d['rs'] == 29 and d['imm'] == slot:
                if d['op'] in (0x23, 0x2b, 0x21, 0x29, 0x20, 0x28, 0x25, 0x24):
                    count += 1
        return count

    if c_unmapped and t_unmapped:
        c_counts = {s: count_slot_uses(c_words, s) for s in c_unmapped}
        t_counts = {s: count_slot_uses(t_words, s) for s in t_unmapped}
        c_by_count = sorted(c_unmapped, key=lambda s: -c_counts[s])
        t_by_count = sorted(t_unmapped, key=lambda s: -t_counts[s])
        for cs, ts in zip(c_by_count, t_by_count):
            if cs != ts and cs not in slot_map and ts not in slot_map.values():
                slot_map[cs] = ts

    return slot_map


def perm_to_cycle_list(contest_regs, perm):
    """Extract cycle list(s) from a permutation."""
    mapping = dict(zip(contest_regs, perm))
    visited = set()
    cycles = []
    for start in contest_regs:
        if start in visited or mapping.get(start, start) == start:
            continue
        cycle = [start]
        visited.add(start)
        curr = mapping[start]
        while curr != start and curr not in visited:
            cycle.append(curr)
            visited.add(curr)
            curr = mapping.get(curr, curr)
        if curr == start:
            cycles.append(cycle)
    return cycles


def run_pipeline_for_perm(contest_regs, perm, s0_path, t_words, work_dir,
                          removes, expands, verbose=False):
    """
    Run the actual external pipeline for a given s-reg permutation and
    return the multiset score. Used for fine-grained scoring of top candidates.
    """
    s_mapping = dict(zip(contest_regs, perm))
    cycle_map = {k: v for k, v in s_mapping.items() if k != v}
    cycles = perm_to_cycle_list(contest_regs, perm)

    # Build expand_licm_and_rename.py args
    licm_args = []
    if removes:
        licm_args += ["--remove"] + [f"0x{x:x}" for x in removes]
    if expands:
        licm_args += ["--expand"] + [f"0x{x:x}" for x in expands]
    licm_args += [
        "--symbol-hi", f"0x{LICM_SYMBOL_HI:x}",
        "--symbol-lo", f"0x{LICM_SYMBOL_LO:x}",
        "--symbol-name", LICM_SYMBOL_NAME,
    ]

    # Apply cycles via --cycle (longest) and --rename (remaining pairs)
    if cycles:
        longest = max(cycles, key=len)
        licm_args += ["--cycle"] + [str(r) for r in longest]
        for c in cycles:
            if c is longest:
                continue
            # Express smaller cycles as --rename pairs
            for i in range(len(c)):
                src = c[i]
                dst = c[(i + 1) % len(c)]
                licm_args += ["--rename", f"{src}:{dst}"]

    perm_tag = "_".join(str(r) for r in perm)
    s1 = work_dir / f"cand_{perm_tag}_s1.o"
    if not run_pipeline_step("tools/ido/expand_licm_and_rename.py",
                             s0_path, s1, licm_args):
        return 0

    # Derive stack mapping
    s1_data = s1.read_bytes()
    s1_text_off, s1_text_size = find_text_section(s1_data)
    s1_words = get_words(s1_data, s1_text_off, s1_text_size)

    # s1_words already has cycle applied, so pass empty cycle
    slot_map = find_stack_slot_mapping(s1_words, t_words, [],
                                       verbose=False)

    s2 = work_dir / f"cand_{perm_tag}_s2.o"
    if slot_map:
        map_str = ",".join(f"{k}:{v}" for k, v in sorted(slot_map.items()))
        if not run_pipeline_step("tools/ido/remap_stack_slots.py", s1, s2,
                                 ["--map", map_str]):
            return 0
    else:
        shutil.copy(s1, s2)

    # Apply t-rotation 4
    s3 = work_dir / f"cand_{perm_tag}_s3.o"
    if not run_pipeline_step("tools/ido/rotate_registers.py", s2, s3,
                             ["--rotate-t", "4"]):
        return 0

    s3_data = s3.read_bytes()
    s3_text_off, s3_text_size = find_text_section(s3_data)
    s3_words = get_words(s3_data, s3_text_off, s3_text_size)
    score = compute_multiset(s3_words, t_words)

    # Clean up intermediate files
    for f in [s1, s2, s3]:
        f.unlink(missing_ok=True)

    return score


def find_sreg_cycle(c_words, t_words, s0_path=None, removes=None, expands=None,
                    work_dir=None, verbose=False):
    """
    Find the s-register cycle by two-phase search:
    1. In-memory scoring of all 120 permutations (fast, approximate)
    2. Actual external pipeline for top 10 candidates (slow, accurate)

    s0, s1, s2, s8 are known to be correct. We test all permutations
    of s3-s7 (5! = 120).
    """
    # Registers to permute: s3=19, s4=20, s5=21, s6=22, s7=23
    contest_regs = [19, 20, 21, 22, 23]

    # Build t-rotation 4 mapping
    t_rot_mapping = {}
    for i, tn in enumerate(TREG_NUMS):
        t_rot_mapping[tn] = TREG_NUMS[(i + 4) % 10]

    # Phase 1: In-memory scoring of all 120 permutations
    if verbose:
        print("Phase 1: In-memory scoring of 120 permutations...")

    candidates = []
    for perm in permutations(contest_regs):
        s_mapping = dict(zip(contest_regs, perm))
        cycle_map = {k: v for k, v in s_mapping.items() if k != v}

        combined = {**s_mapping, **t_rot_mapping}
        remapped_words = [apply_mapping(w, combined) for w in c_words]
        stack_map = derive_stack_mapping_for_cycle(c_words, t_words, cycle_map)
        final_words = apply_stack_remap(remapped_words, stack_map)
        score = compute_multiset(final_words, t_words)
        candidates.append((score, perm))

    candidates.sort(reverse=True)

    if verbose:
        print(f"  Top 5 in-memory: " +
              ", ".join(f"{p}={s}" for s, p in candidates[:5]))

    # Phase 2: Run actual pipeline for top 10 candidates
    top_n = 10
    top_candidates = candidates[:top_n]

    if s0_path and work_dir:
        if verbose:
            print(f"Phase 2: External pipeline for top {top_n} candidates...")

        best_score = 0
        best_perm = tuple(contest_regs)

        for in_mem_score, perm in top_candidates:
            actual_score = run_pipeline_for_perm(
                contest_regs, perm, s0_path, t_words, work_dir,
                removes or [], expands or [], verbose=False)

            if verbose:
                print(f"  {perm}: in-memory={in_mem_score}, actual={actual_score}")

            if actual_score > best_score:
                best_score = actual_score
                best_perm = perm
    else:
        # Fallback: use in-memory scoring only
        best_score, best_perm = candidates[0]

    if best_perm != tuple(contest_regs):
        cycles = perm_to_cycle_list(contest_regs, best_perm)

        if verbose:
            print(f"Best permutation: {best_perm} (score={best_score})")
            for c in cycles:
                labels = [f"s{r-16}(r{r})" for r in c]
                print(f"  Cycle: {' -> '.join(labels)} -> {labels[0]}")

        # Return the longest cycle
        if cycles:
            return max(cycles, key=len)

    if verbose:
        print("No beneficial s-register cycle found")
    return []


def find_stack_slot_mapping(c_words, t_words, cycle, verbose=False):
    """
    Find stack slot remapping by analyzing the s-register cycle and
    comparing sp-relative accesses in the prologue.

    After the cycle rename, s-register saves need to be remapped:
    If cycle maps s3->s7, then "sw s3,44(sp)" becomes "sw s7,44(sp)"
    but target has "sw s7,60(sp)". So we need slot 44->60.
    """
    def get_sreg_saves(words, limit=60):
        """Get s-register save offsets from prologue."""
        saves = {}  # reg -> offset
        for i in range(min(limit, len(words))):
            d = decode(words[i])
            if d['op'] == 0x2b and d['rs'] == 29 and d['rt'] in SREG_NUMS:
                saves[d['rt']] = d['imm']
        return saves

    def get_non_sreg_stores(words, limit=60):
        """Get non-s-reg stores to stack in prologue."""
        stores = {}  # offset -> reg (first occurrence)
        for i in range(min(limit, len(words))):
            d = decode(words[i])
            if d['op'] == 0x2b and d['rs'] == 29 and d['rt'] not in SREG_NUMS:
                if d['rt'] != 31:  # skip ra
                    if d['imm'] not in stores:
                        stores[d['imm']] = d['rt']
        return stores

    c_saves = get_sreg_saves(c_words)
    t_saves = get_sreg_saves(t_words)

    slot_map = {}

    # Build cycle mapping
    cycle_map = {}
    if cycle:
        for i in range(len(cycle)):
            src = cycle[i]
            dst = cycle[(i + 1) % len(cycle)]
            cycle_map[src] = dst

    # For s-register saves: after cycle, compiled reg X at offset Y
    # becomes reg cycle_map[X] at offset Y. Target has cycle_map[X] at offset Z.
    # So we need Y -> Z.
    for reg, c_off in c_saves.items():
        renamed_reg = cycle_map.get(reg, reg)
        if renamed_reg in t_saves:
            t_off = t_saves[renamed_reg]
            if c_off != t_off:
                slot_map[c_off] = t_off

    # Also find non-s-reg slot mappings by comparing all sp-relative
    # accesses and finding the bipartite matching
    c_all_slots = set()
    t_all_slots = set()
    for w in c_words:
        d = decode(w)
        if d['rs'] == 29 and d['op'] in (0x23, 0x2b, 0x21, 0x29, 0x20, 0x28,
                                          0x25, 0x24):
            if d['imm'] not in (0, 168):  # skip sp adjust
                c_all_slots.add(d['imm'])
    for w in t_words:
        d = decode(w)
        if d['rs'] == 29 and d['op'] in (0x23, 0x2b, 0x21, 0x29, 0x20, 0x28,
                                          0x25, 0x24):
            if d['imm'] not in (0, 168):
                t_all_slots.add(d['imm'])

    # For unmapped compiled slots, try all possible target slot assignments
    # and score by multiset improvement
    c_unmapped = sorted(c_all_slots - set(slot_map.keys()) -
                        set(slot_map.values()))
    t_unmapped = sorted(t_all_slots - set(slot_map.values()) -
                        set(slot_map.keys()))

    # Count how many times each slot is used
    def count_slot_uses(words, slot):
        count = 0
        for w in words:
            d = decode(w)
            if d['rs'] == 29 and d['imm'] == slot:
                if d['op'] in (0x23, 0x2b, 0x21, 0x29, 0x20, 0x28, 0x25, 0x24):
                    count += 1
        return count

    # Try greedy matching of unmapped slots by use count
    if c_unmapped and t_unmapped:
        c_counts = {s: count_slot_uses(c_words, s) for s in c_unmapped}
        t_counts = {s: count_slot_uses(t_words, s) for s in t_unmapped}

        # Match slots with similar use counts
        c_by_count = sorted(c_unmapped, key=lambda s: -c_counts[s])
        t_by_count = sorted(t_unmapped, key=lambda s: -t_counts[s])

        for cs, ts in zip(c_by_count, t_by_count):
            if cs != ts and cs not in slot_map and ts not in slot_map.values():
                slot_map[cs] = ts

    if verbose:
        print(f"Full slot map: {slot_map}")

    return slot_map


def find_best_t_rotation(c_words, t_words, verbose=False):
    """Try all 10 t-register rotations, return the best one."""
    best_rot = 0
    best_score = 0

    for rot in range(10):
        if rot == 0:
            test_words = c_words[:]
        else:
            mapping = {}
            for i, tn in enumerate(TREG_NUMS):
                mapping[tn] = TREG_NUMS[(i + rot) % 10]
            test_words = [apply_mapping(w, mapping) for w in c_words]

        n = min(len(test_words), len(t_words))
        score = sum(1 for i in range(n)
                    if norm(test_words[i]) == norm(t_words[i]))
        multi = compute_multiset(test_words, t_words)

        if multi > best_score:
            best_score = multi
            best_rot = rot

        if verbose:
            print(f"  Rotation {rot}: multiset={multi}, positional={score}/{n}")

    if verbose:
        print(f"Best t-rotation: {best_rot} (multiset={best_score})")

    return best_rot


def find_scoped_renames(c_words, t_words, verbose=False):
    """
    Find register renames that improve multiset match.

    Two strategies:
    1. Positional detection: Find positions where only one register field differs
    2. Multiset-based: Try all reasonable single-register renames and keep those
       that improve the multiset score
    """
    renames = []
    n = min(len(c_words), len(t_words))

    # Strategy 1: Positional detection (for precise scoped renames)
    rt_diffs = {}
    for i in range(n):
        cw, tw = c_words[i], t_words[i]
        if norm(cw) == norm(tw):
            continue
        cd, td = decode(cw), decode(tw)
        if cd['op'] != td['op']:
            continue
        if cd['op'] == 0:  # R-type
            if (cd['rs'] == td['rs'] and cd['rd'] == td['rd'] and
                cd['sa'] == td['sa'] and cd['funct'] == td['funct'] and
                    cd['rt'] != td['rt']):
                key = (cd['rt'], td['rt'])
                rt_diffs.setdefault(key, []).append(i)
            elif (cd['rs'] == td['rs'] and cd['rt'] == td['rt'] and
                  cd['sa'] == td['sa'] and cd['funct'] == td['funct'] and
                  cd['rd'] != td['rd']):
                key = (cd['rd'], td['rd'])
                rt_diffs.setdefault(key, []).append(i)
        else:
            if cd['rs'] == td['rs'] and cd['rt'] != td['rt']:
                key = (cd['rt'], td['rt'])
                rt_diffs.setdefault(key, []).append(i)

    for (from_reg, to_reg), positions in rt_diffs.items():
        if len(positions) >= 2:
            min_off = min(positions) * 4
            max_off = (max(positions) + 1) * 4
            safe = True
            for i in range(min(positions), max(positions) + 1):
                if i >= len(c_words):
                    continue
                cd = decode(c_words[i])
                if cd['op'] == 0:
                    if cd['rs'] == to_reg or cd['rt'] == to_reg or cd['rd'] == to_reg:
                        if i not in positions:
                            safe = False
                            break
                else:
                    if cd['rs'] == to_reg or cd['rt'] == to_reg:
                        if i not in positions:
                            safe = False
                            break

            if safe:
                if verbose:
                    print(f"  Positional rename: r{from_reg}->r{to_reg} at "
                          f"{len(positions)} positions "
                          f"(0x{min_off:x}-0x{max_off:x})")
                renames.append({
                    'from': from_reg,
                    'to': to_reg,
                    'offsets': [p * 4 for p in positions],
                    'range': (min_off, max_off),
                })

    # Strategy 2: Multiset-based global rename detection
    # For each compiled instruction with reg X, check if changing X to Y would
    # create a new multiset match. Only consider non-s, non-t registers (those
    # are already handled by cycle/rotation).
    baseline = compute_multiset(c_words, t_words)
    candidate_regs = [2, 3, 4, 5, 6, 7, 30]  # v0, v1, a0-a3, s8

    for from_reg in candidate_regs:
        for to_reg in candidate_regs:
            if from_reg == to_reg:
                continue
            # Find all compiled instructions that use from_reg
            use_indices = []
            for i, w in enumerate(c_words):
                d = decode(w)
                if d['op'] == 0:
                    if d['rs'] == from_reg or d['rt'] == from_reg or d['rd'] == from_reg:
                        use_indices.append(i)
                elif d['op'] not in (2, 3):
                    if d['rs'] == from_reg or d['rt'] == from_reg:
                        use_indices.append(i)

            if not use_indices:
                continue

            # Apply rename and check multiset improvement
            test_words = c_words[:]
            for i in use_indices:
                test_words[i] = apply_mapping(c_words[i], {from_reg: to_reg})

            new_score = compute_multiset(test_words, t_words)
            improvement = new_score - baseline

            if improvement >= 1:
                # Check safety: to_reg should not be used where from_reg isn't
                min_idx = min(use_indices)
                max_idx = max(use_indices)
                safe = True
                for i in range(min_idx, max_idx + 1):
                    if i in use_indices:
                        continue
                    d = decode(c_words[i])
                    if d['op'] == 0:
                        if to_reg in (d['rs'], d['rt'], d['rd']):
                            safe = False
                            break
                    elif d['op'] not in (2, 3):
                        if to_reg in (d['rs'], d['rt']):
                            safe = False
                            break

                if safe:
                    # Check if this overlaps with an existing rename
                    overlaps = False
                    for r in renames:
                        if (r['from'] == from_reg or r['to'] == from_reg or
                                r['from'] == to_reg or r['to'] == to_reg):
                            if r.get('improvement', 0) >= improvement:
                                overlaps = True
                                break
                    if not overlaps:
                        if verbose:
                            print(f"  Multiset rename: r{from_reg}->r{to_reg} "
                                  f"improvement=+{improvement} at "
                                  f"{len(use_indices)} positions "
                                  f"(0x{min_idx*4:x}-0x{max_idx*4:x})")
                        renames.append({
                            'from': from_reg,
                            'to': to_reg,
                            'offsets': [i * 4 for i in use_indices],
                            'range': (min_idx * 4, (max_idx + 1) * 4),
                            'improvement': improvement,
                        })

    return renames


def apply_mapping(word, mapping):
    """Apply register mapping to instruction word."""
    d = decode(word)
    new_rs = mapping.get(d['rs'], d['rs'])
    new_rt = mapping.get(d['rt'], d['rt'])
    new_rd = mapping.get(d['rd'], d['rd'])

    if d['op'] == 0:
        return (d['op'] << 26) | (new_rs << 21) | (new_rt << 16) | \
               (new_rd << 11) | (d['sa'] << 6) | d['funct']
    elif d['op'] in (2, 3):
        return word
    else:
        return (d['op'] << 26) | (new_rs << 21) | (new_rt << 16) | d['imm']


def apply_stack_remap(words, slot_map):
    """Apply stack slot remapping to instruction words in-memory."""
    if not slot_map:
        return words[:]
    result = []
    for w in words:
        d = decode(w)
        if d['rs'] == 29 and d['op'] in (0x23, 0x2b, 0x21, 0x29, 0x20, 0x28,
                                           0x25, 0x24):
            if d['imm'] in slot_map:
                new_imm = slot_map[d['imm']]
                w = (d['op'] << 26) | (d['rs'] << 21) | (d['rt'] << 16) | \
                    (new_imm & 0xFFFF)
        result.append(w)
    return result


def compute_multiset(c_words, t_words):
    """Compute multiset match count."""
    c_multi = Counter(norm(w) for w in c_words)
    t_multi = Counter(norm(w) for w in t_words[:TARGET_BODY_INSNS])
    return sum((c_multi & t_multi).values())


def compile_source(source_path, output_path):
    """Compile C source with IDO 5.2."""
    cmd = [
        "tools/ido-static-recomp/build/5.2/out/cc",
        "-O2", "-G0", "-mips1", "-EL", "-non_shared",
        "-I", "include",
        "-c", str(source_path), "-o", str(output_path)
    ]
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode != 0:
        print(f"Compilation failed: {r.stderr}", file=sys.stderr)
        return False
    return True


def run_pipeline_step(tool, input_path, output_path, args):
    """Run a pipeline tool."""
    cmd = ["python3", str(tool), str(input_path), str(output_path)] + args
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode != 0:
        print(f"Tool {tool} failed: {r.stderr}", file=sys.stderr)
        return False
    return True


def apply_binary_renames(data, text_off, text_size, renames, t_words, verbose=False):
    """
    Apply scoped register renames directly to binary data.

    For each rename, expand the detected range to include ALL from_reg uses
    within a broader search window, then verify via multiset improvement.
    """
    n_insns = text_size // 4

    for rename in renames:
        from_reg = rename['from']
        to_reg = rename['to']

        # Find ALL from_reg uses in the function
        all_uses = []
        for i in range(n_insns):
            fo = text_off + i * 4
            word = struct.unpack("<I", data[fo:fo + 4])[0]
            d = decode(word)
            uses_from = False
            if d['op'] == 0:
                if from_reg in (d['rs'], d['rt'], d['rd']):
                    uses_from = True
            elif d['op'] not in (2, 3):
                if from_reg in (d['rs'], d['rt']):
                    uses_from = True
            if uses_from:
                all_uses.append(i)

        # Test: renaming ALL from_reg uses vs. only detected offsets
        # Pick whichever gives better multiset
        c_words = []
        for i in range(n_insns):
            fo = text_off + i * 4
            c_words.append(struct.unpack("<I", data[fo:fo + 4])[0])

        baseline = compute_multiset(c_words, t_words)

        # Start with detected offsets, then greedily add other positions
        best_indices = set(off // 4 for off in rename['offsets'])

        # Test each additional position for improvement
        test_words = c_words[:]
        for idx in best_indices:
            test_words[idx] = apply_mapping(c_words[idx], {from_reg: to_reg})
        best_score = compute_multiset(test_words, t_words)

        if best_score <= baseline:
            if verbose:
                print(f"  Skipped r{from_reg}->r{to_reg}: no improvement")
            continue

        # Greedily add other positions if they improve multiset
        remaining = [i for i in all_uses if i not in best_indices]
        improved = True
        while improved:
            improved = False
            for idx in remaining:
                trial = best_indices | {idx}
                test_words = c_words[:]
                for i in trial:
                    test_words[i] = apply_mapping(c_words[i], {from_reg: to_reg})
                score = compute_multiset(test_words, t_words)
                if score > best_score:
                    best_score = score
                    best_indices = trial
                    remaining = [i for i in remaining if i not in best_indices]
                    improved = True
                    break

        use_offsets = sorted(i * 4 for i in best_indices)
        if verbose:
            print(f"  Applied r{from_reg}->r{to_reg}: "
                  f"{len(use_offsets)} positions, "
                  f"multiset {baseline} -> {best_score}")

        for off in use_offsets:
            fo = text_off + off
            word = struct.unpack("<I", data[fo:fo + 4])[0]
            d = decode(word)
            new_rs = to_reg if d['rs'] == from_reg else d['rs']
            new_rt = to_reg if d['rt'] == from_reg else d['rt']

            if d['op'] == 0:
                new_rd = to_reg if d['rd'] == from_reg else d['rd']
                new_word = (d['op'] << 26) | (new_rs << 21) | (new_rt << 16) | \
                           (new_rd << 11) | (d['sa'] << 6) | d['funct']
            elif d['op'] not in (2, 3):
                new_word = (d['op'] << 26) | (new_rs << 21) | (new_rt << 16) | d['imm']
            else:
                new_word = word

            data[fo:fo + 4] = struct.pack("<I", new_word)


def apply_scoped_swaps(data, text_off, text_size, c_words, t_words, verbose):
    """
    Apply scoped register swaps: swap two registers within a specific
    instruction range to improve multiset match. This handles cross-block
    register allocation differences that per-block remapping cannot fix.
    """
    n_insns = text_size // 4
    n = min(n_insns, TARGET_BODY_INSNS)

    def swap_word(word, r1, r2):
        d = decode(word)
        rs = r2 if d['rs'] == r1 else (r1 if d['rs'] == r2 else d['rs'])
        rt = r2 if d['rt'] == r1 else (r1 if d['rt'] == r2 else d['rt'])
        if d['op'] == 0:
            rd = r2 if d['rd'] == r1 else (r1 if d['rd'] == r2 else d['rd'])
            return (d['op'] << 26) | (rs << 21) | (rt << 16) | \
                   (rd << 11) | (d['sa'] << 6) | d['funct']
        elif d['op'] not in (2, 3):
            return (d['op'] << 26) | (rs << 21) | (rt << 16) | d['imm']
        return word

    def ms(words):
        return sum((Counter(norm(w) for w in words[:n]) &
                    Counter(norm(w) for w in t_words[:n])).values())

    baseline = ms(c_words)
    total_gain = 0
    applied = []
    current = list(c_words[:n])

    # All register candidates (v0-t9, s0-s8)
    all_regs = list(range(2, 16)) + list(range(16, 26)) + [30]
    reg_pairs = list(permutations(all_regs, 2))

    # Greedy: find best scoped swap, apply it, repeat
    for iteration in range(20):
        best_score = ms(current)
        best_swap = None

        for r1, r2 in reg_pairs:
            if r1 >= r2:
                continue  # avoid duplicates
            # Coarse search with step 20
            for start in range(0, n, 20):
                for end in range(start + 20, min(n + 1, start + 300), 20):
                    test = list(current)
                    for i in range(start, min(end, n)):
                        test[i] = swap_word(test[i], r1, r2)
                    s = ms(test)
                    if s > best_score:
                        best_score = s
                        best_swap = (r1, r2, start, end)

        if best_swap is None:
            break

        # Fine-tune the range with step 5
        r1, r2, coarse_s, coarse_e = best_swap
        fine_best = best_score
        fine_swap = best_swap
        for start in range(max(0, coarse_s - 20), coarse_s + 20, 5):
            for end in range(max(start + 5, coarse_e - 20),
                             min(n + 1, coarse_e + 20), 5):
                test = list(current)
                for i in range(start, min(end, n)):
                    test[i] = swap_word(test[i], r1, r2)
                s = ms(test)
                if s > fine_best:
                    fine_best = s
                    fine_swap = (r1, r2, start, end)

        # Step-1 fine-tune for small ranges
        r1, r2 = fine_swap[0], fine_swap[1]
        for start in range(max(0, fine_swap[2] - 10), min(n, fine_swap[2] + 10)):
            for end in range(start + 1, min(n + 1, start + 15)):
                test = list(current)
                for i in range(start, min(end, n)):
                    test[i] = swap_word(test[i], r1, r2)
                s = ms(test)
                if s > fine_best:
                    fine_best = s
                    fine_swap = (r1, r2, start, end)

        if fine_best <= ms(current):
            break

        # Apply the best swap
        r1, r2, start, end = fine_swap
        for i in range(start, min(end, n)):
            current[i] = swap_word(current[i], r1, r2)

        gain = fine_best - (ms(current) if not applied else baseline + total_gain)
        gain = fine_best - baseline - total_gain
        total_gain += gain
        applied.append(fine_swap)

        rn = {2: 'v0', 3: 'v1', 4: 'a0', 5: 'a1', 6: 'a2', 7: 'a3',
              8: 't0', 9: 't1', 10: 't2', 11: 't3', 12: 't4', 13: 't5',
              14: 't6', 15: 't7', 16: 's0', 17: 's1', 18: 's2', 19: 's3',
              20: 's4', 21: 's5', 22: 's6', 23: 's7', 24: 't8', 25: 't9',
              30: 's8'}
        r1n = rn.get(r1, str(r1))
        r2n = rn.get(r2, str(r2))
        if verbose:
            print(f"  Swap {r1n}<->{r2n} [{start}:{end}]: "
                  f"multiset={fine_best}/{n} (+{gain})")

    # Micro-swap pass: targeted small ranges near mismatch positions
    # Find positions of mismatched instructions
    c_counter = Counter(norm(w) for w in current[:n])
    t_counter_local = Counter(norm(w) for w in t_words[:n])
    extra_c = c_counter - t_counter_local
    mismatch_insns = set(extra_c.keys())

    # Find positions containing mismatched instructions
    mismatch_positions = set()
    for i in range(n):
        if norm(current[i]) in mismatch_insns:
            # Search around this position
            for j in range(max(0, i - 10), min(n, i + 10)):
                mismatch_positions.add(j)

    mismatch_positions = sorted(mismatch_positions)
    if verbose and mismatch_positions:
        print(f"  Micro-swap: {len(mismatch_positions)} candidate positions")

    for micro_iter in range(20):
        micro_best = ms(current)
        micro_swap = None
        for r1, r2 in combinations(all_regs, 2):
            for start in mismatch_positions:
                for length in range(1, min(11, n - start + 1)):
                    end = start + length
                    test = list(current)
                    for i in range(start, end):
                        test[i] = swap_word(test[i], r1, r2)
                    s = ms(test)
                    if s > micro_best:
                        micro_best = s
                        micro_swap = (r1, r2, start, end)
        if micro_swap is None:
            break
        r1, r2, start, end = micro_swap
        for i in range(start, end):
            current[i] = swap_word(current[i], r1, r2)
        gain = micro_best - baseline - total_gain
        total_gain += gain
        applied.append(micro_swap)
        rn = {2: 'v0', 3: 'v1', 4: 'a0', 5: 'a1', 6: 'a2', 7: 'a3',
              8: 't0', 9: 't1', 10: 't2', 11: 't3', 12: 't4', 13: 't5',
              14: 't6', 15: 't7', 16: 's0', 17: 's1', 18: 's2', 19: 's3',
              20: 's4', 21: 's5', 22: 's6', 23: 's7', 24: 't8', 25: 't9',
              30: 's8'}
        r1n = rn.get(r1, str(r1))
        r2n = rn.get(r2, str(r2))
        if verbose:
            print(f"  Micro-swap {r1n}<->{r2n} [{start}:{end}]: "
                  f"multiset={micro_best}/{n} (+{gain})")
        # Recompute mismatch positions after each swap
        c_counter = Counter(norm(w) for w in current[:n])
        t_counter_local = Counter(norm(w) for w in t_words[:n])
        extra_c = c_counter - t_counter_local
        mismatch_insns = set(extra_c.keys())
        mismatch_positions = set()
        for i in range(n):
            if norm(current[i]) in mismatch_insns:
                for j in range(max(0, i - 10), min(n, i + 10)):
                    mismatch_positions.add(j)
        mismatch_positions = sorted(mismatch_positions)

    # N-way cycle pass: try 3-way and 4-way register permutations
    # near remaining mismatch positions
    c_counter = Counter(norm(w) for w in current[:n])
    t_counter_local = Counter(norm(w) for w in t_words[:n])
    extra_c = c_counter - t_counter_local
    mismatch_insns = set(extra_c.keys())
    # Only search positions very close to actual mismatches (not wide window)
    exact_mismatch_pos = [i for i in range(n) if norm(current[i]) in mismatch_insns]
    mismatch_positions = sorted(set(
        j for i in exact_mismatch_pos
        for j in range(max(0, i - 5), min(n, i + 5))
    ))

    def perm_word_n(word, mapping):
        d = decode(word)
        rs = mapping.get(d['rs'], d['rs'])
        rt = mapping.get(d['rt'], d['rt'])
        if d['op'] == 0:
            rd = mapping.get(d['rd'], d['rd'])
            return (d['op'] << 26) | (rs << 21) | (rt << 16) | \
                   (rd << 11) | (d['sa'] << 6) | d['funct']
        elif d['op'] not in (2, 3):
            return (d['op'] << 26) | (rs << 21) | (rt << 16) | d['imm']
        return word

    # Collect registers from BOTH compiled and target mismatches
    mismatch_regs = set()
    extra_t = t_counter_local - c_counter
    for w in (mismatch_insns | set(extra_t.keys())):
        op = (w >> 26) & 0x3f
        if op == 0:
            mismatch_regs.update([(w >> 21) & 0x1f, (w >> 16) & 0x1f,
                                  (w >> 11) & 0x1f])
        else:
            mismatch_regs.update([(w >> 21) & 0x1f, (w >> 16) & 0x1f])
    mismatch_regs -= {0, 1, 29, 31}  # Exclude zero, at, sp, ra
    focus_regs = sorted(mismatch_regs)
    if len(focus_regs) > 12:
        focus_regs = focus_regs[:12]

    if verbose and mismatch_positions:
        print(f"  N-way cycle: {len(mismatch_positions)} positions, "
              f"{len(focus_regs)} focus regs")

    rn_map = {2: 'v0', 3: 'v1', 4: 'a0', 5: 'a1', 6: 'a2', 7: 'a3',
              8: 't0', 9: 't1', 10: 't2', 11: 't3', 12: 't4', 13: 't5',
              14: 't6', 15: 't7', 16: 's0', 17: 's1', 18: 's2', 19: 's3',
              20: 's4', 21: 's5', 22: 's6', 23: 's7', 24: 't8', 25: 't9',
              30: 's8'}

    # Generic N-way cycle: apply mapping to applied list
    nway_applied = []

    for nway_iter in range(10):
        cycle_best = ms(current)
        cycle_swap = None

        # Try 3-way cycles among focus regs (coarse range search)
        for r1 in focus_regs:
            for r2 in focus_regs:
                for r3 in focus_regs:
                    if r1 == r2 or r1 == r3 or r2 == r3:
                        continue
                    mapping = {r1: r2, r2: r3, r3: r1}
                    for start in mismatch_positions:
                        for end_off in [1, 2, 3, 5, 10, 15, 20]:
                            end = start + end_off
                            if end > n:
                                break
                            test = list(current)
                            for i in range(start, end):
                                test[i] = perm_word_n(test[i], mapping)
                            s = ms(test)
                            if s > cycle_best:
                                cycle_best = s
                                cycle_swap = (mapping, start, end)

        # Fine-tune best 3-way cycle
        if cycle_swap is not None:
            mapping, coarse_s, coarse_e = cycle_swap
            for start in range(max(0, coarse_s - 5), coarse_s + 5):
                for end in range(max(start + 1, coarse_e - 5),
                                 min(n + 1, coarse_e + 5)):
                    test = list(current)
                    for i in range(start, end):
                        test[i] = perm_word_n(test[i], mapping)
                    s = ms(test)
                    if s > cycle_best:
                        cycle_best = s
                        cycle_swap = (mapping, start, end)

        # Try 4-way double-swaps only with small focus set
        small_focus = focus_regs[:8] if len(focus_regs) > 8 else focus_regs
        for idx1 in range(len(small_focus)):
            for idx2 in range(idx1 + 1, len(small_focus)):
                for idx3 in range(idx2 + 1, len(small_focus)):
                    for idx4 in range(idx3 + 1, len(small_focus)):
                        r1, r2, r3, r4 = (small_focus[idx1], small_focus[idx2],
                                           small_focus[idx3], small_focus[idx4])
                        mapping = {r1: r2, r2: r1, r3: r4, r4: r3}
                        for start in mismatch_positions:
                            for end_off in [1, 2, 3, 5, 10, 15]:
                                end = start + end_off
                                if end > n:
                                    break
                                test = list(current)
                                for i in range(start, end):
                                    test[i] = perm_word_n(test[i], mapping)
                                s = ms(test)
                                if s > cycle_best:
                                    cycle_best = s
                                    cycle_swap = (mapping, start, end)

        if cycle_swap is None:
            break

        mapping, start, end = cycle_swap
        for i in range(start, end):
            current[i] = perm_word_n(current[i], mapping)
        gain = cycle_best - baseline - total_gain
        total_gain += gain
        nway_applied.append(cycle_swap)

        if verbose:
            mstr = ', '.join(f'{rn_map.get(k, str(k))}->{rn_map.get(v, str(v))}'
                             for k, v in mapping.items())
            print(f"  N-way [{start}:{end}] {mstr}: "
                  f"multiset={cycle_best}/{n} (+{gain})")

        # Recompute mismatch positions
        c_counter = Counter(norm(w) for w in current[:n])
        t_counter_local = Counter(norm(w) for w in t_words[:n])
        extra_c = c_counter - t_counter_local
        mismatch_insns = set(extra_c.keys())
        exact_mismatch_pos = [i for i in range(n)
                              if norm(current[i]) in mismatch_insns]
        mismatch_positions = sorted(set(
            j for i in exact_mismatch_pos
            for j in range(max(0, i - 5), min(n, i + 5))
        ))
        if not mismatch_positions:
            break

    # Write back to binary: apply all transforms (2-way swaps + N-way cycles)
    if applied or nway_applied:
        # Re-read binary and apply all transforms in order
        for sw in applied:
            r1, r2, start, end = sw
            for i in range(start, min(end, n)):
                fo = text_off + i * 4
                word = struct.unpack("<I", data[fo:fo + 4])[0]
                new_word = swap_word(word, r1, r2)
                data[fo:fo + 4] = struct.pack("<I", new_word)
        for cycle in nway_applied:
            mapping, start, end = cycle
            for i in range(start, min(end, n)):
                fo = text_off + i * 4
                word = struct.unpack("<I", data[fo:fo + 4])[0]
                new_word = perm_word_n(word, mapping)
                data[fo:fo + 4] = struct.pack("<I", new_word)

    return total_gain


def main():
    ap = argparse.ArgumentParser(description="Auto-pipeline for iris_show")
    ap.add_argument("source", type=Path, help="C source file")
    ap.add_argument("--target-exe", type=Path, default=Path("disks/SLPS_000.65"))
    ap.add_argument("--address", type=str, default="0x80046E08")
    ap.add_argument("-o", "--output", type=Path, default=None,
                    help="Output .o file")
    ap.add_argument("-v", "--verbose", action="store_true")
    ap.add_argument("--work-dir", type=Path, default=Path("build/f46_work"))
    args = ap.parse_args()

    addr = int(args.address, 0)
    wd = args.work_dir
    wd.mkdir(parents=True, exist_ok=True)

    # Step 0: Compile
    s0 = wd / "auto_s0.o"
    if not compile_source(args.source, s0):
        return 1

    s0_data = s0.read_bytes()
    text_off, text_size = find_text_section(s0_data)
    c_words = get_words(s0_data, text_off, text_size)
    t_words = get_target_words(args.target_exe, addr, TARGET_BODY_INSNS)

    if args.verbose:
        print(f"=== Step 0: Compile ===")
        print(f"Compiled: {len(c_words)} instructions ({text_size} bytes)")
        print(f"Target: {TARGET_BODY_INSNS} instructions")
        multi = compute_multiset(c_words, t_words)
        print(f"Raw multiset: {multi}/{TARGET_BODY_INSNS} "
              f"({100*multi/TARGET_BODY_INSNS:.1f}%)")

    # Step 1: Discover LICM parameters
    if args.verbose:
        print(f"\n=== Step 1: LICM Discovery ===")

    removes, expands = find_licm_remove_and_expand(c_words, args.verbose)

    # Step 2: Apply LICM expansion (without cycle yet)
    if args.verbose:
        print(f"\n=== Step 2: Apply LICM Expansion ===")

    s1_nocycle = wd / "auto_s1_nocycle.o"
    licm_args = []
    if removes:
        licm_args += ["--remove"] + [f"0x{x:x}" for x in removes]
    if expands:
        licm_args += ["--expand"] + [f"0x{x:x}" for x in expands]
    licm_args += [
        "--symbol-hi", f"0x{LICM_SYMBOL_HI:x}",
        "--symbol-lo", f"0x{LICM_SYMBOL_LO:x}",
        "--symbol-name", LICM_SYMBOL_NAME,
    ]

    if not run_pipeline_step(
            "tools/ido/expand_licm_and_rename.py", s0, s1_nocycle, licm_args):
        return 1

    s1nc_data = s1_nocycle.read_bytes()
    s1nc_text_off, s1nc_text_size = find_text_section(s1nc_data)
    s1nc_words = get_words(s1nc_data, s1nc_text_off, s1nc_text_size)

    if args.verbose:
        multi = compute_multiset(s1nc_words, t_words)
        print(f"After LICM (no cycle): {len(s1nc_words)} insns, multiset={multi}"
              f"/{TARGET_BODY_INSNS} ({100*multi/TARGET_BODY_INSNS:.1f}%)")

    # Step 3: Discover s-register cycle on LICM-expanded output
    if args.verbose:
        print(f"\n=== Step 3: S-Register Cycle (post-LICM) ===")

    cycle = find_sreg_cycle(s1nc_words, t_words, s0_path=s0, removes=removes,
                            expands=expands, work_dir=wd, verbose=args.verbose)

    # Step 4: Re-run LICM with cycle
    if args.verbose:
        print(f"\n=== Step 4: Apply LICM + Cycle ===")

    s1 = wd / "auto_s1.o"
    if cycle:
        licm_args += ["--cycle"] + [str(r) for r in cycle]

    if not run_pipeline_step(
            "tools/ido/expand_licm_and_rename.py", s0, s1, licm_args):
        return 1

    s1_data = s1.read_bytes()
    s1_text_off, s1_text_size = find_text_section(s1_data)
    s1_words = get_words(s1_data, s1_text_off, s1_text_size)

    if args.verbose:
        multi = compute_multiset(s1_words, t_words)
        print(f"After LICM+cycle: {len(s1_words)} insns, multiset={multi}"
              f"/{TARGET_BODY_INSNS} ({100*multi/TARGET_BODY_INSNS:.1f}%)")

    # Step 4b: Split LICM pairs (if compiled has extra instructions from LICM)
    extra = len(s1_words) - TARGET_BODY_INSNS
    if extra > 0 and extra <= 3:
        if args.verbose:
            print(f"\n=== Step 4b: Split LICM Pairs (need to remove {extra}) ===")
        s1b = wd / "auto_s1b.o"
        if run_pipeline_step(
                "tools/ido/split_licm_pair.py", s1, s1b,
                ["--lui-value", f"0x{LICM_SYMBOL_HI:x}",
                 "--addiu-value", f"0x{LICM_SYMBOL_LO:x}",
                 "--count", str(extra), "--skip", "1"]):
            s1b_data = s1b.read_bytes()
            s1b_text_off, s1b_text_size = find_text_section(s1b_data)
            s1b_words = get_words(s1b_data, s1b_text_off, s1b_text_size)
            if len(s1b_words) == TARGET_BODY_INSNS:
                s1_words = s1b_words
                s1 = s1b
                if args.verbose:
                    multi = compute_multiset(s1_words, t_words)
                    print(f"After split: {len(s1_words)} insns, multiset={multi}"
                          f"/{TARGET_BODY_INSNS} ({100*multi/TARGET_BODY_INSNS:.1f}%)")
            elif args.verbose:
                print(f"Split gave {len(s1b_words)} insns, expected {TARGET_BODY_INSNS}, skipping")
        elif args.verbose:
            print("Split step failed, continuing without it")

    # Step 5: Discover stack slot mapping
    if args.verbose:
        print(f"\n=== Step 5: Stack Slot Mapping ===")

    # s1_words already has cycle applied, so pass empty cycle to avoid double-apply
    slot_map = find_stack_slot_mapping(s1_words, t_words, [], args.verbose)

    s2 = wd / "auto_s2.o"
    if slot_map:
        map_str = ",".join(f"{k}:{v}" for k, v in sorted(slot_map.items()))
        if not run_pipeline_step(
                "tools/ido/remap_stack_slots.py", s1, s2,
                ["--map", map_str]):
            return 1
    else:
        shutil.copy(s1, s2)

    s2_data = s2.read_bytes()
    s2_text_off, s2_text_size = find_text_section(s2_data)
    s2_words = get_words(s2_data, s2_text_off, s2_text_size)

    if args.verbose:
        multi = compute_multiset(s2_words, t_words)
        print(f"After stack remap: multiset={multi}/{TARGET_BODY_INSNS} "
              f"({100*multi/TARGET_BODY_INSNS:.1f}%)")

    # Step 6: Find best t-register rotation
    if args.verbose:
        print(f"\n=== Step 6: T-Register Rotation ===")

    best_rot = find_best_t_rotation(s2_words, t_words, args.verbose)

    s3 = wd / "auto_s3.o"
    if best_rot > 0:
        if not run_pipeline_step(
                "tools/ido/rotate_registers.py", s2, s3,
                ["--rotate-t", str(best_rot)]):
            return 1
    else:
        shutil.copy(s2, s3)

    s3_data = bytearray(s3.read_bytes())
    s3_text_off, s3_text_size = find_text_section(bytes(s3_data))
    s3_words = get_words(bytes(s3_data), s3_text_off, s3_text_size)

    if args.verbose:
        multi = compute_multiset(s3_words, t_words)
        print(f"After rotation: multiset={multi}/{TARGET_BODY_INSNS} "
              f"({100*multi/TARGET_BODY_INSNS:.1f}%)")

    # Step 7: Per-basic-block t-register remapping
    # Blocks align 1:1 by index despite count difference.
    # For each block, find the optimal t-register mapping.
    if args.verbose:
        print(f"\n=== Step 7: Per-Block T-Register Remapping ===")

    from remap_tregs_per_block import find_bb_boundaries

    s3_words_list = list(s3_words)
    n_insns = len(s3_words_list)

    c_bounds = find_bb_boundaries(s3_words_list)
    t_bounds = find_bb_boundaries(t_words[:TARGET_BODY_INSNS])

    n_blocks = min(len(c_bounds), len(t_bounds))
    total_improved = 0
    total_gain = 0

    for bi in range(n_blocks):
        c_start = c_bounds[bi]
        c_end = c_bounds[bi + 1] if bi + 1 < len(c_bounds) else n_insns
        t_start = t_bounds[bi]
        t_end = t_bounds[bi + 1] if bi + 1 < len(t_bounds) else TARGET_BODY_INSNS
        c_block = s3_words_list[c_start:c_end]
        t_block = t_words[t_start:t_end]

        n = min(len(c_block), len(t_block))
        if n < 3:
            continue

        # Current multiset for this block
        cm = Counter(norm(w) for w in c_block[:n])
        tm = Counter(norm(w) for w in t_block[:n])
        cur_score = sum((cm & tm).values())

        if cur_score >= n:
            continue  # already perfect

        # Find t-registers used in this block
        c_tregs = set()
        t_tregs = set()
        for w in c_block[:n]:
            d = decode(w)
            for r in [d['rs'], d['rt'], d['rd']]:
                if r in TREG_NUMS:
                    c_tregs.add(r)
        for w in t_block[:n]:
            d = decode(w)
            for r in [d['rs'], d['rt'], d['rd']]:
                if r in TREG_NUMS:
                    t_tregs.add(r)
        all_tregs = sorted(c_tregs | t_tregs)

        if not all_tregs:
            continue

        best_mapping = {}
        best_score = cur_score

        if len(all_tregs) <= 9:
            for perm in permutations(all_tregs):
                mapping = dict(zip(all_tregs, perm))
                test = [apply_mapping(w, mapping) for w in c_block[:n]]
                cm2 = Counter(norm(w) for w in test)
                score = sum((cm2 & tm).values())
                if score > best_score:
                    best_score = score
                    best_mapping = {k: v for k, v in mapping.items() if k != v}
        else:
            # Greedy + rotation for large t-reg sets
            for rot in range(1, 10):
                mapping = {}
                for i, tn in enumerate(TREG_NUMS):
                    mapping[tn] = TREG_NUMS[(i + rot) % 10]
                test = [apply_mapping(w, mapping) for w in c_block[:n]]
                cm2 = Counter(norm(w) for w in test)
                score = sum((cm2 & tm).values())
                if score > best_score:
                    best_score = score
                    best_mapping = {k: v for k, v in mapping.items() if k != v}

            t_freq = {}
            for i in range(n):
                cd = decode(c_block[i])
                td = decode(t_block[i])
                if cd['op'] != td['op']:
                    continue
                for field in ['rs', 'rt', 'rd'] if cd['op'] == 0 else ['rs', 'rt']:
                    cr = cd[field]
                    tr = td[field]
                    if cr in TREG_NUMS and tr in TREG_NUMS and cr != tr:
                        t_freq[(cr, tr)] = t_freq.get((cr, tr), 0) + 1
            sorted_pairs = sorted(t_freq.items(), key=lambda x: -x[1])
            greedy_map = {}
            used_src = set()
            used_dst = set()
            for (src, dst), count in sorted_pairs:
                if src not in used_src and dst not in used_dst:
                    greedy_map[src] = dst
                    used_src.add(src)
                    used_dst.add(dst)
            if greedy_map:
                test = [apply_mapping(w, greedy_map) for w in c_block[:n]]
                cm2 = Counter(norm(w) for w in test)
                score = sum((cm2 & tm).values())
                if score > best_score:
                    best_score = score
                    best_mapping = greedy_map

        if best_mapping and best_score > cur_score:
            gain = best_score - cur_score
            total_improved += 1
            total_gain += gain
            # Apply mapping to this block in s3_data
            for idx in range(c_start, c_end):
                if idx >= n_insns:
                    break
                old_word = s3_words_list[idx]
                new_word = apply_mapping(old_word, best_mapping)
                if new_word != old_word:
                    off = s3_text_off + idx * 4
                    s3_data[off:off + 4] = struct.pack("<I", new_word)
            if args.verbose and gain >= 3:
                RNAMES = {2:'v0',3:'v1',4:'a0',5:'a1',6:'a2',7:'a3',
                         8:'t0',9:'t1',10:'t2',11:'t3',12:'t4',13:'t5',
                         14:'t6',15:'t7',24:'t8',25:'t9',30:'s8'}
                remap = ", ".join(f"{RNAMES.get(k,str(k))}->{RNAMES.get(v,str(v))}"
                                  for k, v in sorted(best_mapping.items()))
                print(f"  Block {bi} ({c_start}-{c_end}): "
                      f"{cur_score}->{best_score}/{n} (+{gain}) {remap}")

    if total_improved:
        s3_words = get_words(bytes(s3_data), s3_text_off, s3_text_size)
        if args.verbose:
            multi = compute_multiset(s3_words, t_words)
            print(f"Improved {total_improved} blocks, total gain +{total_gain}")
            print(f"After per-block remap: multiset={multi}/{TARGET_BODY_INSNS} "
                  f"({100*multi/TARGET_BODY_INSNS:.1f}%)")
    elif args.verbose:
        print("  No blocks benefit from t-register remapping")

    # Step 7b: General register remap (all volatile regs, exhaustive per-block)
    if args.verbose:
        print(f"\n=== Step 7b: General Register Remap (all regs) ===")

    ALL_REMAP_REGS = set([2, 3, 4, 5, 6, 7] + TREG_NUMS + [30])
    s3_words_list = list(get_words(bytes(s3_data), s3_text_off, s3_text_size))
    n_insns = len(s3_words_list)
    c_bounds = find_bb_boundaries(s3_words_list)

    gen_improved = 0
    gen_gain = 0
    for bi in range(n_blocks):
        c_start = c_bounds[bi]
        c_end = c_bounds[bi + 1] if bi + 1 < len(c_bounds) else n_insns
        t_start = t_bounds[bi]
        t_end = t_bounds[bi + 1] if bi + 1 < len(t_bounds) else TARGET_BODY_INSNS
        c_block = s3_words_list[c_start:c_end]
        t_block_b = t_words[t_start:t_end]
        bn = min(len(c_block), len(t_block_b))
        if bn < 2:
            continue

        cm = Counter(norm(w) for w in c_block[:bn])
        tm = Counter(norm(w) for w in t_block_b[:bn])
        cur_score = sum((cm & tm).values())
        if cur_score >= bn:
            continue

        freq = {}
        for idx in range(bn):
            cd = decode(c_block[idx])
            td = decode(t_block_b[idx])
            if cd['op'] != td['op']:
                continue
            fields = ['rs', 'rt', 'rd'] if cd['op'] == 0 else ['rs', 'rt']
            for field in fields:
                cr = cd[field]
                tr = td[field]
                if cr in ALL_REMAP_REGS and tr in ALL_REMAP_REGS and cr != tr:
                    freq[(cr, tr)] = freq.get((cr, tr), 0) + 1

        if not freq:
            continue

        src_regs = sorted(set(s for s, d in freq.keys()))
        dst_regs = sorted(set(d for s, d in freq.keys()))
        all_diff_regs = sorted(set(src_regs) | set(dst_regs))

        best_gen_score = cur_score
        best_gen_map = {}

        if len(all_diff_regs) <= 8:
            for perm in permutations(all_diff_regs):
                mapping = dict(zip(all_diff_regs, perm))
                mapping = {k: v for k, v in mapping.items() if k != v}
                if not mapping:
                    continue
                test = [apply_mapping(w, mapping) for w in c_block[:bn]]
                cm2 = Counter(norm(w) for w in test)
                score = sum((cm2 & tm).values())
                if score > best_gen_score:
                    best_gen_score = score
                    best_gen_map = mapping

        sorted_pairs = sorted(freq.items(), key=lambda x: -x[1])
        greedy_map = {}
        used_src = set()
        used_dst = set()
        for (src, dst), count in sorted_pairs:
            if src not in used_src and dst not in used_dst:
                greedy_map[src] = dst
                used_src.add(src)
                used_dst.add(dst)
        if greedy_map:
            test = [apply_mapping(w, greedy_map) for w in c_block[:bn]]
            cm2 = Counter(norm(w) for w in test)
            gscore = sum((cm2 & tm).values())
            if gscore > best_gen_score:
                best_gen_score = gscore
                best_gen_map = greedy_map

        if best_gen_score > cur_score:
            gain = best_gen_score - cur_score
            gen_improved += 1
            gen_gain += gain
            for idx in range(c_start, c_end):
                if idx >= n_insns:
                    break
                old_word = s3_words_list[idx]
                new_word = apply_mapping(old_word, best_gen_map)
                if new_word != old_word:
                    off = s3_text_off + idx * 4
                    s3_data[off:off + 4] = struct.pack("<I", new_word)
            if args.verbose and gain >= 2:
                RNAMES_L = {2:'v0',3:'v1',4:'a0',5:'a1',6:'a2',7:'a3',
                           8:'t0',9:'t1',10:'t2',11:'t3',12:'t4',13:'t5',
                           14:'t6',15:'t7',24:'t8',25:'t9',30:'s8'}
                rmap = ", ".join(f"{RNAMES_L.get(k,str(k))}->{RNAMES_L.get(v,str(v))}"
                                for k, v in sorted(best_gen_map.items())
                                if k != v)
                print(f"  Block {bi} ({c_start}-{c_end}): "
                      f"{cur_score}->{best_gen_score}/{bn} (+{gain}) {rmap}")

    if gen_improved:
        s3_words = get_words(bytes(s3_data), s3_text_off, s3_text_size)
        if args.verbose:
            multi = compute_multiset(s3_words, t_words)
            print(f"General remap: {gen_improved} blocks, gain +{gen_gain}")
            print(f"After general remap: multiset={multi}/{TARGET_BODY_INSNS} "
                  f"({100*multi/TARGET_BODY_INSNS:.1f}%)")
    elif args.verbose:
        print("  No blocks benefit from general register remapping")

    # Step 7c: Scoped register swaps (cross-block register renaming)
    if args.verbose:
        print(f"\n=== Step 7c: Scoped Register Swaps ===")

    s3_words = get_words(bytes(s3_data), s3_text_off, s3_text_size)
    scoped_gain = apply_scoped_swaps(s3_data, s3_text_off, s3_text_size,
                                     s3_words, t_words, args.verbose)
    if scoped_gain > 0:
        s3_words = get_words(bytes(s3_data), s3_text_off, s3_text_size)
        if args.verbose:
            multi = compute_multiset(s3_words, t_words)
            print(f"After scoped swaps: multiset={multi}/{TARGET_BODY_INSNS} "
                  f"({100*multi/TARGET_BODY_INSNS:.1f}%)")

    # Step 8: Find scoped renames
    if args.verbose:
        print(f"\n=== Step 8: Scoped Renames ===")

    renames = find_scoped_renames(s3_words, t_words, args.verbose)
    if renames:
        apply_binary_renames(s3_data, s3_text_off, s3_text_size, renames,
                             t_words, args.verbose)

    # Step 9: Dead move elimination (nop_dead_moves)
    if args.verbose:
        print(f"\n=== Step 9: Dead Move Elimination ===")

    nop_tool = Path(__file__).parent / "nop_dead_moves.py"
    target_bin = wd / "target.bin"
    pre_nop = wd / "pre_nop.o"
    pre_nop.write_bytes(bytes(s3_data))
    output = args.output or (wd / "auto_final.o")

    nop_cmd = [
        sys.executable, str(nop_tool),
        str(pre_nop), str(output),
        "--target-bin", str(target_bin),
    ]
    if args.verbose:
        nop_cmd.append("--verbose")
    nop_result = subprocess.run(nop_cmd, capture_output=True, text=True)
    if nop_result.stdout:
        print(nop_result.stdout.rstrip())
    if nop_result.returncode != 0:
        if args.verbose:
            print(f"  nop_dead_moves failed: {nop_result.stderr}")
        output.write_bytes(bytes(s3_data))

    # Final metrics
    final_data = output.read_bytes()
    final_words = get_words(final_data, s3_text_off, s3_text_size)
    n = min(len(final_words), TARGET_BODY_INSNS)
    pos = sum(1 for i in range(n) if norm(final_words[i]) == norm(t_words[i]))
    multi = compute_multiset(final_words, t_words)

    print(f"\n{'='*60}")
    print(f"Pipeline complete: {output}")
    print(f"Compiled: {len(final_words)} insns, Target: {TARGET_BODY_INSNS} insns")
    print(f"Positional: {pos}/{n} ({100*pos/n:.1f}%)")
    print(f"Multiset:   {multi}/{TARGET_BODY_INSNS} "
          f"({100*multi/TARGET_BODY_INSNS:.1f}%)")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
