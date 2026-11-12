#!/usr/bin/env python3
"""Reschedule basic block instructions to match a target binary.

This tool reorders instructions within basic blocks of a compiled .o file
to match the instruction ordering found in the target game binary, while
preserving data dependencies and fixing branch offsets.

Usage:
    python3 reschedule_basic_blocks.py compiled.o output.o \
        --target-exe SLPS_000.65 --address 0x8004D680

The tool:
1. Extracts .text from both compiled .o and target binary
2. Identifies basic block boundaries (branches/jumps)
3. For each basic block, builds a dependency DAG
4. Checks if target ordering is a valid topological sort
5. If valid, applies the reordering
6. Fixes branch offsets after any instruction movement
"""

import argparse
import struct
import sys
import os

# MIPS instruction field extraction
def get_op(insn):
    return (insn >> 26) & 0x3F

def get_rs(insn):
    return (insn >> 21) & 0x1F

def get_rt(insn):
    return (insn >> 16) & 0x1F

def get_rd(insn):
    return (insn >> 11) & 0x1F

def get_sa(insn):
    return (insn >> 6) & 0x1F

def get_funct(insn):
    return insn & 0x3F

def get_imm(insn):
    return insn & 0xFFFF

def get_imm_signed(insn):
    imm = insn & 0xFFFF
    return imm - 0x10000 if imm & 0x8000 else imm

def get_target(insn):
    return insn & 0x3FFFFFF

# Register names
REG_NAMES = [
    "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"
]

def reg_name(r):
    return REG_NAMES[r] if 0 <= r < 32 else f"r{r}"

def disasm_simple(insn):
    """Simple MIPS disassembler for debugging output."""
    op = get_op(insn)
    if insn == 0:
        return "nop"
    if op == 0:  # SPECIAL
        funct = get_funct(insn)
        rd = get_rd(insn)
        rs = get_rs(insn)
        rt = get_rt(insn)
        sa = get_sa(insn)
        if funct == 0x21:  # addu
            if rs == 0:
                return f"move {reg_name(rd)},{reg_name(rt)}"
            return f"addu {reg_name(rd)},{reg_name(rs)},{reg_name(rt)}"
        elif funct == 0x25:  # or
            if rt == 0:
                return f"move {reg_name(rd)},{reg_name(rs)}"
            return f"or {reg_name(rd)},{reg_name(rs)},{reg_name(rt)}"
        elif funct == 0x23:  # subu
            return f"subu {reg_name(rd)},{reg_name(rs)},{reg_name(rt)}"
        elif funct == 0x2a:  # slt
            return f"slt {reg_name(rd)},{reg_name(rs)},{reg_name(rt)}"
        elif funct == 0x00:  # sll
            return f"sll {reg_name(rd)},{reg_name(rt)},{sa}"
        elif funct == 0x08:  # jr
            return f"jr {reg_name(rs)}"
        elif funct == 0x09:  # jalr
            return f"jalr {reg_name(rs)}"
        return f"special_{funct:#x} {reg_name(rd)},{reg_name(rs)},{reg_name(rt)}"
    elif op == 0x09:  # addiu
        return f"addiu {reg_name(get_rt(insn))},{reg_name(get_rs(insn))},{get_imm_signed(insn)}"
    elif op == 0x0C:  # andi
        return f"andi {reg_name(get_rt(insn))},{reg_name(get_rs(insn))},{get_imm(insn):#x}"
    elif op == 0x0F:  # lui
        return f"lui {reg_name(get_rt(insn))},{get_imm(insn):#x}"
    elif op == 0x23:  # lw
        return f"lw {reg_name(get_rt(insn))},{get_imm_signed(insn)}({reg_name(get_rs(insn))})"
    elif op == 0x2B:  # sw
        return f"sw {reg_name(get_rt(insn))},{get_imm_signed(insn)}({reg_name(get_rs(insn))})"
    elif op == 0x04:  # beq
        return f"beq {reg_name(get_rs(insn))},{reg_name(get_rt(insn))},{get_imm_signed(insn)}"
    elif op == 0x05:  # bne
        return f"bne {reg_name(get_rs(insn))},{reg_name(get_rt(insn))},{get_imm_signed(insn)}"
    elif op == 0x03:  # jal
        return f"jal {get_target(insn):#x}"
    elif op == 0x02:  # j
        return f"j {get_target(insn):#x}"
    elif op == 0x01:  # REGIMM
        rt = get_rt(insn)
        if rt == 0:
            return f"bltz {reg_name(get_rs(insn))},{get_imm_signed(insn)}"
        elif rt == 1:
            return f"bgez {reg_name(get_rs(insn))},{get_imm_signed(insn)}"
        return f"regimm_{rt} {reg_name(get_rs(insn))},{get_imm_signed(insn)}"
    elif op == 0x06:  # blez
        return f"blez {reg_name(get_rs(insn))},{get_imm_signed(insn)}"
    elif op == 0x07:  # bgtz
        return f"bgtz {reg_name(get_rs(insn))},{get_imm_signed(insn)}"
    return f"op{op:#x} {insn:08x}"

def is_branch(insn):
    """Check if instruction is a branch or jump."""
    op = get_op(insn)
    if op in (0x02, 0x03):  # j, jal
        return True
    if op in (0x04, 0x05, 0x06, 0x07):  # beq, bne, blez, bgtz
        return True
    if op == 0x01:  # REGIMM (bltz, bgez, etc.)
        return True
    if op == 0 and get_funct(insn) in (0x08, 0x09):  # jr, jalr
        return True
    return False

def is_store(insn):
    """Check if instruction is a memory store."""
    op = get_op(insn)
    return op in (0x28, 0x29, 0x2A, 0x2B, 0x2E)  # sb, sh, swl, sw, swr

def is_load(insn):
    """Check if instruction is a memory load."""
    op = get_op(insn)
    return op in (0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26)  # lb, lh, lwl, lw, lbu, lhu, lwr

def get_defs(insn):
    """Get registers defined (written) by instruction."""
    op = get_op(insn)
    if insn == 0:  # nop
        return set()
    if op == 0:  # SPECIAL
        funct = get_funct(insn)
        if funct in (0x08,):  # jr
            return set()
        if funct in (0x09,):  # jalr
            return {get_rd(insn), 31}  # rd and ra
        if funct in (0x10, 0x12):  # mfhi, mflo
            return {get_rd(insn)}
        if funct in (0x11, 0x13):  # mthi, mtlo
            return set()  # writes HI/LO, not GPR
        if funct in (0x18, 0x19, 0x1A, 0x1B):  # mult, multu, div, divu
            return set()  # writes HI/LO
        return {get_rd(insn)}
    if op == 0x03:  # jal
        return {31}  # ra
    if op in (0x04, 0x05, 0x06, 0x07, 0x01, 0x02):  # branches, j
        return set()
    if is_store(insn):
        return set()  # stores don't write registers
    # Most other instructions write rt
    return {get_rt(insn)}

def get_uses(insn):
    """Get registers read by instruction."""
    op = get_op(insn)
    if insn == 0:  # nop
        return set()
    if op == 0:  # SPECIAL
        funct = get_funct(insn)
        if funct in (0x00, 0x02, 0x03):  # sll, srl, sra
            return {get_rt(insn)}
        if funct in (0x04, 0x06, 0x07):  # sllv, srlv, srav
            return {get_rt(insn), get_rs(insn)}
        if funct in (0x08,):  # jr
            return {get_rs(insn)}
        if funct in (0x09,):  # jalr
            return {get_rs(insn)}
        return {get_rs(insn), get_rt(insn)}
    if op == 0x03:  # jal
        return set()
    if op == 0x02:  # j
        return set()
    if op in (0x04, 0x05):  # beq, bne
        return {get_rs(insn), get_rt(insn)}
    if op in (0x06, 0x07):  # blez, bgtz
        return {get_rs(insn)}
    if op == 0x01:  # REGIMM
        return {get_rs(insn)}
    if op == 0x0F:  # lui
        return set()
    if is_store(insn):
        return {get_rs(insn), get_rt(insn)}  # base reg + data reg
    if is_load(insn):
        return {get_rs(insn)}  # base reg
    # I-type: rs
    return {get_rs(insn)}

def instructions_match(a, b):
    """Compare two instructions, masking branch/jal targets."""
    if a == b:
        return True
    op_a = get_op(a)
    op_b = get_op(b)
    # JAL: compare only opcode
    if op_a == 3 and op_b == 3:
        return True
    # Branches: compare opcode+rs+rt (mask offset)
    if op_a in (4, 5, 6, 7, 1) and op_a == op_b:
        return (a & 0xFFFF0000) == (b & 0xFFFF0000)
    return False

def find_text_section(data):
    """Find .text section in ELF."""
    if data[:4] != b'\x7fELF':
        return None, None
    e_shoff = struct.unpack('<I', data[0x20:0x24])[0]
    e_shentsize = struct.unpack('<H', data[0x2e:0x30])[0]
    e_shnum = struct.unpack('<H', data[0x30:0x32])[0]
    e_shstrndx = struct.unpack('<H', data[0x32:0x34])[0]
    shstr_hdr_off = e_shoff + e_shstrndx * e_shentsize
    shstr_off = struct.unpack('<I', data[shstr_hdr_off + 0x10:shstr_hdr_off + 0x14])[0]
    for i in range(e_shnum):
        sh_off = e_shoff + i * e_shentsize
        sh_name_idx = struct.unpack('<I', data[sh_off:sh_off+4])[0]
        sh_type = struct.unpack('<I', data[sh_off+4:sh_off+8])[0]
        name_end = data.find(b'\x00', shstr_off + sh_name_idx)
        name = data[shstr_off + sh_name_idx:name_end].decode('ascii', errors='ignore')
        if name == '.text' and sh_type == 1:
            text_off = struct.unpack('<I', data[sh_off + 0x10:sh_off + 0x14])[0]
            text_size = struct.unpack('<I', data[sh_off + 0x14:sh_off + 0x18])[0]
            return text_off, text_size
    return None, None

def read_instructions(data, offset, size):
    """Read instructions from binary data."""
    insns = []
    for i in range(size // 4):
        insns.append(struct.unpack('<I', data[offset + i*4:offset + i*4 + 4])[0])
    return insns

def get_target_instructions(exe_path, addr, count):
    """Extract instructions from PS1 executable."""
    with open(exe_path, 'rb') as f:
        header = f.read(0x800)
        text_addr = struct.unpack('<I', header[0x18:0x1C])[0]
        offset = (addr - text_addr) + 0x800
        f.seek(offset)
        data = f.read(count * 4)
    return read_instructions(data, 0, count * 4)

def find_instruction_mapping(compiled, target):
    """Find which compiled instruction matches each target instruction.

    Returns a list where mapping[target_idx] = compiled_idx, or -1 if no match.
    """
    n_compiled = len(compiled)
    n_target = len(target)
    used = [False] * n_compiled
    mapping = [-1] * n_target

    for ti in range(n_target):
        for ci in range(n_compiled):
            if not used[ci] and instructions_match(target[ti], compiled[ci]):
                mapping[ti] = ci
                used[ci] = True
                break

    return mapping

def find_basic_blocks(insns):
    """Find basic block boundaries.

    Returns list of (start, end) tuples where end is exclusive.
    A basic block ends after a branch instruction's delay slot.
    """
    blocks = []
    start = 0
    i = 0
    n = len(insns)

    while i < n:
        if is_branch(insns[i]):
            # Branch at i, delay slot at i+1
            end = min(i + 2, n)
            blocks.append((start, end))
            start = end
            i = end
        else:
            i += 1

    if start < n:
        blocks.append((start, n))

    return blocks

def check_dependency(insns, i, j):
    """Check if instruction j depends on instruction i (i must come before j).

    Returns True if there's a data dependency (RAW, WAR, or WAW).
    """
    defs_i = get_defs(insns[i])
    uses_i = get_uses(insns[i])
    defs_j = get_defs(insns[j])
    uses_j = get_uses(insns[j])

    # Remove $zero (r0) - always 0, no real dependency
    defs_i.discard(0)
    uses_i.discard(0)
    defs_j.discard(0)
    uses_j.discard(0)

    # RAW: j reads something i writes
    if defs_i & uses_j:
        return True
    # WAW: j writes something i writes
    if defs_i & defs_j:
        return True
    # WAR: j writes something i reads
    if uses_i & defs_j:
        return True
    # Memory dependencies: store after load or store after store to same base
    if (is_store(insns[i]) or is_load(insns[i])) and (is_store(insns[j]) or is_load(insns[j])):
        # Conservative: if same base register AND one is a store, add dependency
        base_i = get_rs(insns[i])
        base_j = get_rs(insns[j])
        if base_i == base_j and (is_store(insns[i]) or is_store(insns[j])):
            # Only if offsets might alias
            off_i = get_imm_signed(insns[i])
            off_j = get_imm_signed(insns[j])
            if off_i == off_j:
                return True
    return False

def validate_reordering(insns, order):
    """Check if a given ordering respects all data dependencies.

    insns: original instruction list
    order: new ordering (order[new_pos] = old_pos)

    Returns (valid, violations) where violations is list of dependency issues.
    """
    n = len(insns)
    # Build position map: pos_of[old_idx] = new_idx
    pos_of = [0] * n
    for new_idx, old_idx in enumerate(order):
        pos_of[old_idx] = new_idx

    violations = []
    for i in range(n):
        for j in range(i + 1, n):
            if check_dependency(insns, i, j):
                # i must come before j in original
                if pos_of[i] > pos_of[j]:
                    violations.append(
                        f"  Dep violation: insn {i} ({disasm_simple(insns[i])}) must come before "
                        f"insn {j} ({disasm_simple(insns[j])}), "
                        f"but new positions are {pos_of[i]} > {pos_of[j]}"
                    )

    return len(violations) == 0, violations

def compute_branch_offset(insn, old_pos, new_pos, block_size_change=0):
    """Recompute branch offset after instruction has moved.

    For relative branches: offset = (target_addr - (pc+4)) / 4
    If the branch moved but its target didn't (or vice versa), adjust.
    """
    op = get_op(insn)
    if op in (0x04, 0x05, 0x06, 0x07, 0x01):  # relative branches
        old_offset = get_imm_signed(insn)
        # old_target_pos = old_pos + 1 + old_offset (relative to delay slot)
        # new branch is at new_pos, so new_offset = old_target_pos - (new_pos + 1)
        # But if target is outside the block, we need the absolute position
        adjustment = old_pos - new_pos
        new_offset = old_offset + adjustment
        new_insn = (insn & 0xFFFF0000) | (new_offset & 0xFFFF)
        return new_insn
    return insn

def reschedule_block(compiled_insns, target_insns, block_start, block_end, verbose=False):
    """Attempt to reschedule a basic block to match target ordering.

    Returns (new_insns, success) where new_insns is the reordered block.
    """
    block_compiled = compiled_insns[block_start:block_end]
    block_target = target_insns[block_start:block_end]
    n = len(block_compiled)

    if n == 0:
        return [], True

    # Check if already matching
    all_match = all(instructions_match(block_compiled[i], block_target[i]) for i in range(n))
    if all_match:
        return list(block_compiled), True

    # Find mapping: for each target position, which compiled instruction goes there
    mapping = find_instruction_mapping(block_compiled, block_target)

    if -1 in mapping:
        unmapped = [i for i, m in enumerate(mapping) if m == -1]
        if verbose:
            print(f"  Cannot map target instructions at positions: {unmapped}")
            for i in unmapped:
                print(f"    target[{i}]: {block_target[i]:08x} {disasm_simple(block_target[i])}")
        return list(block_compiled), False

    # mapping[target_idx] = compiled_idx
    # This gives us the reordering: new position target_idx should have compiled instruction mapping[target_idx]
    order = mapping  # order[new_pos] = old_pos

    # Validate the reordering respects dependencies
    valid, violations = validate_reordering(block_compiled, order)

    if not valid:
        if verbose:
            print(f"  Reordering violates {len(violations)} dependencies:")
            for v in violations[:5]:
                print(v)
        return list(block_compiled), False

    # Apply reordering
    new_block = [0] * n
    for new_pos in range(n):
        old_pos = order[new_pos]
        insn = block_compiled[old_pos]

        # Fix branch offsets if the instruction moved
        if is_branch(insn) and old_pos != new_pos:
            insn = compute_branch_offset(insn, block_start + old_pos, block_start + new_pos)

        new_block[new_pos] = insn

    return new_block, True

def main():
    parser = argparse.ArgumentParser(description="Reschedule basic blocks to match target binary")
    parser.add_argument("input", help="Compiled .o file")
    parser.add_argument("output", help="Output .o file")
    parser.add_argument("--target-exe", required=True, help="Target PS1 executable")
    parser.add_argument("--address", required=True, help="Function address in target (hex)")
    parser.add_argument("--verbose", "-v", action="store_true", help="Verbose output")
    parser.add_argument("--dry-run", "-n", action="store_true", help="Don't write output")
    args = parser.parse_args()

    addr = int(args.address, 16)

    # Read compiled .o
    with open(args.input, 'rb') as f:
        obj_data = bytearray(f.read())

    text_off, text_size = find_text_section(obj_data)
    if text_off is None:
        print("Error: Cannot find .text section in compiled .o", file=sys.stderr)
        return 1

    compiled = read_instructions(obj_data, text_off, text_size)
    n_compiled = len(compiled)

    # Read target
    target = get_target_instructions(args.target_exe, addr, n_compiled)
    n_target = len(target)

    if args.verbose:
        print(f"Compiled: {n_compiled} instructions")
        print(f"Target:   {n_target} instructions")

    # Initial match
    initial_matches = sum(1 for i in range(min(n_compiled, n_target))
                         if instructions_match(compiled[i], target[i]))
    initial_pct = initial_matches / max(n_compiled, n_target) * 100
    print(f"Initial match: {initial_matches}/{max(n_compiled, n_target)} ({initial_pct:.1f}%)")

    if n_compiled != n_target:
        print(f"Warning: instruction count mismatch ({n_compiled} vs {n_target})")
        print("Cannot reschedule with different instruction counts")
        return 1

    # Find basic blocks in compiled code
    blocks = find_basic_blocks(compiled)
    if args.verbose:
        print(f"Found {len(blocks)} basic blocks:")
        for i, (s, e) in enumerate(blocks):
            print(f"  BB{i}: insns {s}-{e-1} ({e-s} insns)")

    # Try to reschedule each block
    new_insns = list(compiled)
    blocks_fixed = 0
    blocks_already_ok = 0
    blocks_failed = 0

    for bi, (bs, be) in enumerate(blocks):
        block_compiled = compiled[bs:be]
        block_target = target[bs:be]

        # Check if block already matches
        all_match = all(instructions_match(block_compiled[i], block_target[i])
                       for i in range(len(block_compiled)))
        if all_match:
            blocks_already_ok += 1
            continue

        if args.verbose:
            print(f"\nBB{bi} (insns {bs}-{be-1}): needs rescheduling")
            print("  Compiled:")
            for i, insn in enumerate(block_compiled):
                marker = " *" if not instructions_match(insn, block_target[i]) else ""
                print(f"    {bs+i:2d}: {insn:08x} {disasm_simple(insn)}{marker}")
            print("  Target:")
            for i, insn in enumerate(block_target):
                print(f"    {bs+i:2d}: {insn:08x} {disasm_simple(insn)}")

        new_block, success = reschedule_block(compiled, target, bs, be, verbose=args.verbose)

        if success:
            # Verify the rescheduled block matches target
            match_count = sum(1 for i in range(len(new_block))
                            if instructions_match(new_block[i], block_target[i]))
            if match_count == len(new_block):
                for i in range(len(new_block)):
                    new_insns[bs + i] = new_block[i]
                blocks_fixed += 1
                if args.verbose:
                    print(f"  -> Rescheduled successfully ({match_count}/{len(new_block)} match)")
            else:
                blocks_failed += 1
                if args.verbose:
                    print(f"  -> Rescheduled but only {match_count}/{len(new_block)} match")
        else:
            blocks_failed += 1

    # Final match
    final_matches = sum(1 for i in range(n_compiled)
                       if instructions_match(new_insns[i], target[i]))
    final_pct = final_matches / n_compiled * 100

    print(f"\nBlocks: {blocks_already_ok} already OK, {blocks_fixed} fixed, {blocks_failed} failed")
    print(f"Final match: {final_matches}/{n_compiled} ({final_pct:.1f}%)")

    if final_pct > initial_pct:
        print(f"Improvement: {initial_pct:.1f}% -> {final_pct:.1f}%")

    # Write output
    if not args.dry_run and final_matches > initial_matches:
        for i in range(n_compiled):
            struct.pack_into('<I', obj_data, text_off + i * 4, new_insns[i])

        with open(args.output, 'wb') as f:
            f.write(obj_data)
        print(f"Written to {args.output}")
    elif args.dry_run:
        print("(dry run - no output written)")
    else:
        print("No improvement - not writing output")

    return 0 if final_pct >= 100 else 1

if __name__ == "__main__":
    sys.exit(main())
