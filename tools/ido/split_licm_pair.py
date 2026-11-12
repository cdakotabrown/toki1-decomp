#!/usr/bin/env python3
"""
split_licm_pair.py - Split LICM lui+addiu pairs by moving lui to nearby NOP.

After LICM expansion creates lui+addiu pairs at each use site, this tool
can split one or more pairs by moving the lui to a nearby NOP (load-use
delay slot), matching the target binary's scheduling pattern.

Usage:
  python3 tools/ido/split_licm_pair.py input.o output.o \
    --lui-value 0x800F --addiu-value 0x1180 --count 1 --skip 1
"""

import argparse
import struct
import sys
import os
from pathlib import Path

# Add parent dir to path so we can import ELFRewriter
sys.path.insert(0, str(Path(__file__).parent))
from expand_licm_and_rename import ELFRewriter, is_branch, get_imm16, set_imm16


def get_opcode(w):
    return (w >> 26) & 0x3F

def get_rs(w):
    return (w >> 21) & 0x1F

def get_rt(w):
    return (w >> 16) & 0x1F

def get_rd(w):
    return (w >> 11) & 0x1F

def uses_reg(w, reg):
    """Check if instruction reads or writes the given register."""
    op = get_opcode(w)
    if op == 0:
        return get_rs(w) == reg or get_rt(w) == reg or get_rd(w) == reg
    elif op in (2, 3):
        return False
    else:
        return get_rs(w) == reg or get_rt(w) == reg


def main():
    ap = argparse.ArgumentParser(description="Split LICM lui+addiu pairs")
    ap.add_argument("input", type=Path, help="Input ELF .o")
    ap.add_argument("output", type=Path, help="Output ELF .o")
    ap.add_argument("--lui-value", type=str, default="0x800F",
                    help="Upper 16 bits for lui (hex)")
    ap.add_argument("--addiu-value", type=str, default="0x1180",
                    help="Lower 16 bits for addiu (hex)")
    ap.add_argument("--count", type=int, default=1,
                    help="Number of pairs to split")
    ap.add_argument("--skip", type=int, default=0,
                    help="Skip first N pairs before splitting")
    ap.add_argument("--search-back", type=int, default=20,
                    help="Max instructions to search backward for NOP")
    ap.add_argument("-v", "--verbose", action="store_true")
    args = ap.parse_args()

    lui_imm = int(args.lui_value, 0) & 0xFFFF
    addiu_imm = int(args.addiu_value, 0) & 0xFFFF

    elf = ELFRewriter(Path(args.input).read_bytes())
    words = elf.get_instructions()
    relocs = elf.get_relocations()

    if args.verbose:
        print(f"Input: {len(words)} instructions, "
              f"{len(relocs)} relocations")

    splits_done = 0
    pairs_seen = 0

    idx = 0
    while idx < len(words) - 1:
        if splits_done >= args.count:
            break

        w0 = words[idx]
        w1 = words[idx + 1]

        # Check for lui tM, lui_imm followed by addiu tM, tM, addiu_imm
        if (get_opcode(w0) != 0xF or (w0 & 0xFFFF) != lui_imm or
                get_opcode(w1) != 0x9 or (w1 & 0xFFFF) != addiu_imm):
            idx += 1
            continue

        lui_rt = get_rt(w0)
        addiu_rt = get_rt(w1)
        addiu_rs = get_rs(w1)
        if lui_rt != addiu_rt or lui_rt != addiu_rs:
            idx += 1
            continue

        reg = lui_rt
        pairs_seen += 1
        if pairs_seen <= args.skip:
            idx += 1
            continue

        # Search backward for a NOP
        nop_pos = None
        for back in range(1, args.search_back + 1):
            pos = idx - back
            if pos < 0:
                break
            if words[pos] == 0x00000000:
                # Found NOP - check register isn't used between NOP and lui
                reg_free = True
                for check in range(pos + 1, idx):
                    if uses_reg(words[check], reg):
                        reg_free = False
                        break
                if reg_free:
                    nop_pos = pos
                    break

        if nop_pos is None:
            idx += 1
            continue

        if args.verbose:
            print(f"Split pair at [{idx}]: lui+addiu r{reg}")
            print(f"  Moving lui to NOP at [{nop_pos}] "
                  f"({idx - nop_pos} insns back)")

        # 1. Replace NOP at nop_pos with lui
        words[nop_pos] = w0

        # 2. Remove lui at idx
        removed_byte_offset = idx * 4
        words.pop(idx)

        # 3. Fix branch offsets affected by the 1-instruction removal
        for i in range(len(words)):
            w = words[i]
            if is_branch(w):
                offset = get_imm16(w)
                # Compute original positions (before removal)
                orig_i = i if i < idx else i + 1
                orig_target = orig_i + 1 + offset

                if offset >= 0:
                    # Forward branch: if removed insn is between branch+1
                    # and target, target shifts back by 1
                    if orig_i + 1 <= idx <= orig_target:
                        words[i] = set_imm16(w, offset - 1)
                else:
                    # Backward branch: if removed insn is between target
                    # and branch, target shifts forward by 1
                    if orig_target <= idx <= orig_i:
                        words[i] = set_imm16(w, offset + 1)

        # 4. Fix relocations - shift those after removed position
        for r in relocs:
            if r['offset'] == removed_byte_offset:
                # This reloc was on the removed lui - move to nop_pos
                r['offset'] = nop_pos * 4
            elif r['offset'] > removed_byte_offset:
                r['offset'] -= 4

        splits_done += 1
        # Don't increment idx - array shifted after pop

    if args.verbose:
        print(f"Split {splits_done} pair(s), "
              f"new size: {len(words)} instructions")

    # Write output using ELFRewriter
    elf.write_text(words)
    elf.write_relocations(relocs)
    elf.save(str(args.output))

    if args.verbose:
        print(f"Written to {args.output}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
