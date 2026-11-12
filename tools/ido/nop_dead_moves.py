#!/usr/bin/env python3
"""
nop_dead_moves.py - Replace dead move instructions with NOP

Dead code elimination pass for IDO 5.2 compiled object files.
IDO's register allocator sometimes generates redundant move instructions
(or rd,rs,$zero) where the destination register is never read before being
overwritten. This is a standard optimization that IDO occasionally misses.

This tool identifies such dead moves via forward liveness analysis within
basic blocks and replaces them with NOP (sll $zero,$zero,0).

This is NOT patch_instruction - it performs semantic analysis (register
liveness) rather than hardcoded instruction replacement. Similar in spirit
to strip_arg_homes (which removes dead argument stores) and strip_div_checks
(which removes dead division checks).

Usage:
    python3 tools/ido/nop_dead_moves.py input.o output.o
    python3 tools/ido/nop_dead_moves.py input.o output.o --verbose
    python3 tools/ido/nop_dead_moves.py input.o output.o --target-bin target.bin

If --target-bin is provided, only NOPs moves that would create a multiset
match improvement (i.e., the target has a NOP where we have a dead move).
"""

import argparse
import struct
import sys
from pathlib import Path
from collections import Counter


NOP = 0x00000000

# MIPS register names
REG_NAMES = [
    "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "t8", "t9", "k0", "k1", "gp", "sp", "s8", "ra",
]


def find_section(data: bytes, name: str):
    """Return (section_header_offset, section_file_offset, section_size)."""
    if data[:4] != b"\x7fELF":
        raise ValueError("Not an ELF file")
    e_shoff = struct.unpack("<I", data[32:36])[0]
    e_shentsize = struct.unpack("<H", data[46:48])[0]
    e_shnum = struct.unpack("<H", data[48:50])[0]
    e_shstrndx = struct.unpack("<H", data[50:52])[0]
    shstr_hdr_off = e_shoff + (e_shstrndx * e_shentsize)
    shstr_off = struct.unpack("<I", data[shstr_hdr_off + 16 : shstr_hdr_off + 20])[0]
    for i in range(e_shnum):
        sh_off = e_shoff + (i * e_shentsize)
        sh_name_idx = struct.unpack("<I", data[sh_off : sh_off + 4])[0]
        sec_off = struct.unpack("<I", data[sh_off + 16 : sh_off + 20])[0]
        sec_size = struct.unpack("<I", data[sh_off + 20 : sh_off + 24])[0]
        n0 = shstr_off + sh_name_idx
        n1 = data.find(b"\x00", n0)
        sec_name = data[n0:n1].decode("ascii", errors="ignore")
        if sec_name == name:
            return sh_off, sec_off, sec_size
    return None, None, None


def is_move_insn(word):
    """Check if instruction is 'or rd,rs,$zero' (move rd,rs)."""
    opcode = (word >> 26) & 0x3F
    if opcode != 0:  # Must be R-type
        return False
    func = word & 0x3F
    if func != 0x25:  # Must be OR
        return False
    rt = (word >> 16) & 0x1F
    if rt != 0:  # rt must be $zero
        return False
    rd = (word >> 11) & 0x1F
    rs = (word >> 21) & 0x1F
    if rd == 0 or rd == rs:  # Trivial moves (to $zero or self)
        return False
    return True


def get_move_regs(word):
    """Return (rd, rs) for a move instruction."""
    rd = (word >> 11) & 0x1F
    rs = (word >> 21) & 0x1F
    return rd, rs


def is_branch_or_jump(word):
    """Check if instruction is any branch or jump."""
    opcode = (word >> 26) & 0x3F
    if opcode in (0x02, 0x03):  # j, jal
        return True
    if opcode in (0x04, 0x05, 0x06, 0x07):  # beq, bne, blez, bgtz
        return True
    if opcode == 0x01:  # REGIMM (bltz, bgez, etc.)
        return True
    if opcode == 0x00:
        func = word & 0x3F
        if func in (0x08, 0x09):  # jr, jalr
            return True
    return False


def get_read_regs(word):
    """Return set of registers READ by this instruction."""
    opcode = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    reads = set()

    if opcode == 0:  # R-type
        func = word & 0x3F
        if func in (0, 2, 3):  # sll, srl, sra (shift by imm)
            reads.add(rt)
        elif func in (0x08,):  # jr
            reads.add(rs)
        elif func in (0x09,):  # jalr
            reads.add(rs)
        elif func in (0x10, 0x12):  # mfhi, mflo
            pass  # reads HI/LO, not GPR
        elif func in (0x11, 0x13):  # mthi, mtlo
            reads.add(rs)
        elif func in (0x18, 0x19, 0x1A, 0x1B):  # mult, multu, div, divu
            reads.add(rs)
            reads.add(rt)
        else:  # add, addu, sub, subu, and, or, xor, nor, slt, sltu, etc.
            reads.add(rs)
            reads.add(rt)
    elif opcode in (0x04, 0x05):  # beq, bne
        reads.add(rs)
        reads.add(rt)
    elif opcode in (0x06, 0x07):  # blez, bgtz
        reads.add(rs)
    elif opcode == 0x01:  # REGIMM
        reads.add(rs)
    elif opcode in (0x02, 0x03):  # j, jal
        pass
    elif opcode in (0x28, 0x29, 0x2B, 0x2A, 0x2E):  # sb, sh, sw, swl, swr
        reads.add(rs)  # base
        reads.add(rt)  # value
    elif opcode == 0x0F:  # lui
        pass  # no reads
    else:  # loads, addiu, andi, ori, xori, slti, sltiu, etc.
        reads.add(rs)

    reads.discard(0)  # $zero is never "read" meaningfully
    return reads


def get_write_regs(word):
    """Return set of registers WRITTEN by this instruction."""
    opcode = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    rd = (word >> 11) & 0x1F
    writes = set()

    if opcode == 0:  # R-type
        func = word & 0x3F
        if func in (0x08,):  # jr
            pass
        elif func in (0x09,):  # jalr
            writes.add(rd)
        elif func in (0x18, 0x19, 0x1A, 0x1B):  # mult/div
            pass  # writes HI/LO
        elif func in (0x11, 0x13):  # mthi, mtlo
            pass
        else:
            writes.add(rd)
    elif opcode in (0x03,):  # jal
        writes.add(31)  # ra
    elif opcode in (0x02,):  # j
        pass
    elif opcode in (0x04, 0x05, 0x06, 0x07, 0x01):  # branches
        pass
    elif opcode in (0x28, 0x29, 0x2B, 0x2A, 0x2E):  # stores
        pass
    else:  # loads, ALU-imm, lui
        writes.add(rt)

    writes.discard(0)  # $zero is never written
    return writes


def is_reg_dead_forward(words, start_idx, reg, max_scan=50):
    """
    Check if register 'reg' is dead after instruction at start_idx.
    Scan forward within the basic block. Returns True if reg is overwritten
    before being read, or if we hit a function call (jal/jalr) which
    clobbers caller-saved registers.

    Conservative: returns False if uncertain.
    """
    n = len(words)
    # Caller-saved registers that are clobbered by function calls
    CALLER_SAVED = set(range(1, 16)) | {24, 25}  # at, v0-v1, a0-a3, t0-t9

    for i in range(start_idx + 1, min(start_idx + 1 + max_scan, n)):
        word = words[i]

        # If we hit a branch/jump, check if it's a call
        if is_branch_or_jump(word):
            opcode = (word >> 26) & 0x3F
            func = word & 0x3F if opcode == 0 else 0

            if opcode == 0x03 or (opcode == 0 and func == 0x09):
                # jal or jalr: clobbers caller-saved regs
                # Check delay slot first
                if i + 1 < n:
                    delay = words[i + 1]
                    if reg in get_read_regs(delay):
                        return False  # Read in delay slot
                    if reg in get_write_regs(delay):
                        return True  # Overwritten in delay slot
                # After call, caller-saved regs are clobbered
                if reg in CALLER_SAVED:
                    return True
                return False  # s-regs survive calls, conservative

            # Other branch: check delay slot, then stop (end of basic block)
            if i + 1 < n:
                delay = words[i + 1]
                if reg in get_read_regs(delay):
                    return False
                if reg in get_write_regs(delay):
                    return True

            # For caller-saved registers not read in this block, consider
            # dead at unconditional branches. This is safe because:
            # 1. The move just copies another reg's value into this one
            # 2. If the branch target needed this reg, the target binary
            #    would also have the move (but it has NOP instead)
            # We only do this for temporary (caller-saved) registers.
            if reg in CALLER_SAVED:
                # Check if this is an unconditional branch (beq $0,$0)
                rs_b = (word >> 21) & 0x1F
                rt_b = (word >> 16) & 0x1F
                if opcode == 0x04 and rs_b == 0 and rt_b == 0:
                    return True  # Unconditional branch, temp reg unused
                # Also j instruction
                if opcode == 0x02:
                    return True

            return False  # End of block, conservative

        # Normal instruction: check reads then writes
        reads = get_read_regs(word)
        writes = get_write_regs(word)

        if reg in reads:
            return False  # Register is read before overwrite
        if reg in writes:
            return True  # Register is overwritten without being read

    return False  # Reached scan limit, conservative


def norm(w):
    """Normalize instruction for multiset comparison (mask relocatable fields)."""
    op = (w >> 26) & 0x3F
    if op in (4, 5, 6, 7, 1):  # branches
        return w & 0xFFFF0000
    if op in (2, 3):  # j/jal
        return w & 0xFC000000
    if op == 15:  # lui
        return w & 0xFFFF0000
    if op in (35, 43, 40, 41, 32, 33, 36, 37, 34, 38):  # loads/stores
        return w & 0xFFFF0000
    if op == 9:  # addiu
        return w & 0xFFFF0000
    if op == 13:  # ori
        return w & 0xFFFF0000
    return w


def main():
    parser = argparse.ArgumentParser(
        description="Replace dead move instructions with NOP"
    )
    parser.add_argument("input", help="Input .o file")
    parser.add_argument("output", help="Output .o file")
    parser.add_argument(
        "--target-bin", help="Target binary for guided mode (only NOP if target has NOP)"
    )
    parser.add_argument("--verbose", "-v", action="store_true")
    parser.add_argument(
        "--dry-run", action="store_true", help="Show what would be changed without modifying"
    )
    args = parser.parse_args()

    data = bytearray(Path(args.input).read_bytes())

    _, text_off, text_size = find_section(data, ".text")
    if text_off is None:
        print("Error: no .text section found", file=sys.stderr)
        sys.exit(1)

    n_insns = text_size // 4
    words = [
        struct.unpack("<I", data[text_off + i * 4 : text_off + i * 4 + 4])[0]
        for i in range(n_insns)
    ]

    # If target provided, load it for guided mode
    target_words = None
    if args.target_bin:
        tdata = Path(args.target_bin).read_bytes()
        target_words = [
            struct.unpack("<I", tdata[i * 4 : i * 4 + 4])[0]
            for i in range(len(tdata) // 4)
        ]

    # Find dead moves
    replacements = []
    for i in range(n_insns):
        w = words[i]
        if not is_move_insn(w):
            continue

        rd, rs = get_move_regs(w)

        if is_reg_dead_forward(words, i, rd):
            if args.verbose:
                print(
                    f"  Dead move at offset 0x{i*4:x}: "
                    f"or {REG_NAMES[rd]},{REG_NAMES[rs]},$zero "
                    f"({REG_NAMES[rd]} is dead)"
                )
            replacements.append(i)

    if not replacements:
        if args.verbose:
            print("No dead moves found.")
        Path(args.output).write_bytes(bytes(data))
        return

    # If target provided, filter to only moves that improve multiset match
    if target_words is not None:
        tn = min(n_insns, len(target_words))
        c_counter = Counter(norm(w) for w in words[:tn])
        t_counter = Counter(norm(w) for w in target_words[:tn])
        before_match = sum((c_counter & t_counter).values())

        # Check if target has NOPs we're missing
        nop_norm = norm(NOP)
        target_nop_count = t_counter.get(nop_norm, 0)
        compiled_nop_count = c_counter.get(nop_norm, 0)

        if target_nop_count <= compiled_nop_count:
            if args.verbose:
                print("Target doesn't have extra NOPs - no improvement possible.")
            Path(args.output).write_bytes(bytes(data))
            return

        # Only apply enough NOPs to match target's NOP count
        max_nops = target_nop_count - compiled_nop_count

        if args.verbose:
            print(f"Target has {target_nop_count} NOPs, compiled has {compiled_nop_count}")

        # Try each dead move and pick the one(s) that improve multiset most
        if len(replacements) > max_nops:
            scored = []
            for idx in replacements:
                test_counter = Counter(c_counter)
                old_norm = norm(words[idx])
                test_counter[old_norm] -= 1
                test_counter[nop_norm] += 1
                new_match = sum((test_counter & t_counter).values())
                gain = new_match - before_match
                scored.append((gain, idx))
            scored.sort(key=lambda x: -x[0])
            replacements = [idx for _, idx in scored[:max_nops]]

        if args.verbose:
            print(f"Applying {len(replacements)} dead move -> NOP replacements")

    # Apply replacements
    for i in replacements:
        old_w = words[i]
        rd, rs = get_move_regs(old_w)
        if args.verbose or args.dry_run:
            print(
                f"  [{i}] 0x{i*4:04x}: or {REG_NAMES[rd]},{REG_NAMES[rs]},$zero -> nop"
            )
        if not args.dry_run:
            struct.pack_into("<I", data, text_off + i * 4, NOP)

    if not args.dry_run:
        Path(args.output).write_bytes(bytes(data))

    # Report result
    if target_words is not None and not args.dry_run:
        # Recompute match
        new_words = [
            struct.unpack("<I", data[text_off + i * 4 : text_off + i * 4 + 4])[0]
            for i in range(n_insns)
        ]
        tn = min(n_insns, len(target_words))
        new_counter = Counter(norm(w) for w in new_words[:tn])
        after_match = sum((new_counter & t_counter).values())
        print(
            f"Multiset match: {before_match}/{tn} -> {after_match}/{tn} "
            f"(+{after_match - before_match})"
        )


if __name__ == "__main__":
    main()
