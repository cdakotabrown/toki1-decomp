#!/usr/bin/env python3
"""
Scan all ASM functions in the binary for cross-prologue switch dispatcher patterns.

Pattern A: Cross-prologue global load + switch
  - Instructions 0-1: lui REG, hi; lbu/lw REG, lo(REG)
  - Instruction 2: addiu sp, sp, negative (prologue)
  - Near instruction 3+: beqz REG or beq REG,zero
  - Later: li at,1 (0x24010001) and beq REG,at

Pattern B: Switch on v1 (no cross-prologue)
  - Instruction 0: addiu sp, sp, negative
  - Then: sw ra + beqz v1 / beq v1,zero
  - Later: li at,1 and beq v1,at

Pattern C: Switch on v1 with return value (move v0,v1)
  - Same as B but includes or v0,v1,zero (0x00601025)
"""

import json
import struct
import sys
import os

# MIPS register names
REG_NAMES = {
    0: "zero", 1: "at", 2: "v0", 3: "v1",
    4: "a0", 5: "a1", 6: "a2", 7: "a3",
    8: "t0", 9: "t1", 10: "t2", 11: "t3",
    12: "t4", 13: "t5", 14: "t6", 15: "t7",
    16: "s0", 17: "s1", 18: "s2", 19: "s3",
    20: "s4", 21: "s5", 22: "s6", 23: "s7",
    24: "t8", 25: "t9", 26: "k0", 27: "k1",
    28: "gp", 29: "sp", 30: "fp", 31: "ra",
}


def decode_insn(word):
    """Decode a MIPS instruction word into its fields."""
    opcode = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    rd = (word >> 11) & 0x1F
    shamt = (word >> 6) & 0x1F
    funct = word & 0x3F
    imm = word & 0xFFFF
    imm_signed = imm if imm < 0x8000 else imm - 0x10000
    target = word & 0x03FFFFFF
    return {
        'word': word,
        'opcode': opcode,
        'rs': rs, 'rt': rt, 'rd': rd,
        'shamt': shamt, 'funct': funct,
        'imm': imm, 'imm_signed': imm_signed,
        'target': target,
    }


def is_lui(insn, reg=None):
    """LUI rt, imm: opcode=0xF (15)"""
    if insn['opcode'] != 0xF:
        return False
    if reg is not None and insn['rt'] != reg:
        return False
    return True


def is_lbu(insn, rt=None, rs=None):
    """LBU rt, off(rs): opcode=0x24 (36)"""
    if insn['opcode'] != 0x24:
        return False
    if rt is not None and insn['rt'] != rt:
        return False
    if rs is not None and insn['rs'] != rs:
        return False
    return True


def is_lw(insn, rt=None, rs=None):
    """LW rt, off(rs): opcode=0x23 (35)"""
    if insn['opcode'] != 0x23:
        return False
    if rt is not None and insn['rt'] != rt:
        return False
    if rs is not None and insn['rs'] != rs:
        return False
    return True


def is_lh(insn, rt=None, rs=None):
    """LH rt, off(rs): opcode=0x21 (33)"""
    if insn['opcode'] != 0x21:
        return False
    if rt is not None and insn['rt'] != rt:
        return False
    if rs is not None and insn['rs'] != rs:
        return False
    return True


def is_lhu(insn, rt=None, rs=None):
    """LHU rt, off(rs): opcode=0x25 (37)"""
    if insn['opcode'] != 0x25:
        return False
    if rt is not None and insn['rt'] != rt:
        return False
    if rs is not None and insn['rs'] != rs:
        return False
    return True


def is_lb(insn, rt=None, rs=None):
    """LB rt, off(rs): opcode=0x20 (32)"""
    if insn['opcode'] != 0x20:
        return False
    if rt is not None and insn['rt'] != rt:
        return False
    if rs is not None and insn['rs'] != rs:
        return False
    return True


def is_load(insn, rt=None, rs=None):
    """Any load instruction: lw, lbu, lb, lh, lhu"""
    return (is_lw(insn, rt, rs) or is_lbu(insn, rt, rs) or
            is_lb(insn, rt, rs) or is_lh(insn, rt, rs) or is_lhu(insn, rt, rs))


def is_addiu_sp(insn):
    """ADDIU sp, sp, imm: opcode=9, rs=29, rt=29"""
    return insn['opcode'] == 0x9 and insn['rs'] == 29 and insn['rt'] == 29


def is_beqz(insn, reg=None):
    """BEQ rs, zero, offset (beqz rs, offset): opcode=4, rt=0"""
    if insn['opcode'] != 0x4:
        return False
    if insn['rt'] != 0:
        return False
    if reg is not None and insn['rs'] != reg:
        return False
    return True


def is_bnez(insn, reg=None):
    """BNE rs, zero, offset (bnez rs, offset): opcode=5, rt=0"""
    if insn['opcode'] != 0x5:
        return False
    if insn['rt'] != 0:
        return False
    if reg is not None and insn['rs'] != reg:
        return False
    return True


def is_beq(insn, rs=None, rt=None):
    """BEQ rs, rt, offset: opcode=4"""
    if insn['opcode'] != 0x4:
        return False
    if rs is not None and insn['rs'] != rs:
        return False
    if rt is not None and insn['rt'] != rt:
        return False
    return True


def is_li_at(insn, value=None):
    """LI at, imm (= addiu at, zero, imm): opcode=9, rs=0, rt=1"""
    if insn['opcode'] != 0x9 or insn['rs'] != 0 or insn['rt'] != 1:
        return False
    if value is not None and insn['imm'] != value:
        return False
    return True


def is_ori_at(insn, value=None):
    """ORI at, zero, imm: opcode=0xD, rs=0, rt=1"""
    if insn['opcode'] != 0xD or insn['rs'] != 0 or insn['rt'] != 1:
        return False
    if value is not None and insn['imm'] != value:
        return False
    return True


def is_sw_ra(insn):
    """SW ra, off(sp): opcode=0x2B, rs=29, rt=31"""
    return insn['opcode'] == 0x2B and insn['rs'] == 29 and insn['rt'] == 31


def is_lw_ra(insn):
    """LW ra, off(sp): opcode=0x23, rs=29, rt=31"""
    return insn['opcode'] == 0x23 and insn['rs'] == 29 and insn['rt'] == 31


def is_jr_ra(insn):
    """JR ra: opcode=0, rs=31, funct=8"""
    return insn['opcode'] == 0 and insn['rs'] == 31 and insn['funct'] == 8


def is_jal(insn):
    """JAL target: opcode=3"""
    return insn['opcode'] == 3


def is_nop(insn):
    return insn['word'] == 0


def is_move_v0_v1(insn):
    """OR v0, v1, zero (move v0, v1): 0x00601025"""
    # or rd, rs, rt: opcode=0, funct=0x25
    # move v0,v1 = or v0, v1, zero
    if insn['opcode'] != 0:
        return False
    if insn['funct'] != 0x25:
        return False
    if insn['rd'] != 2 or insn['rs'] != 3 or insn['rt'] != 0:
        return False
    return True


def is_b_unconditional(insn):
    """B offset (= beq zero, zero, offset): opcode=4, rs=0, rt=0"""
    return insn['opcode'] == 4 and insn['rs'] == 0 and insn['rt'] == 0


def get_jal_targets(insns, func_addr):
    """Extract all JAL target addresses from instruction list."""
    targets = []
    for i, insn in enumerate(insns):
        if is_jal(insn):
            # JAL target field is in instruction words (26-bit)
            # Full target = (top 4 bits of PC) | (target << 2)
            # For PS1, top 4 bits are 0x8
            target_addr = (0x80000000) | (insn['target'] << 2)
            targets.append(target_addr)
    return targets


def extract_case_calls(insns, func_addr, switch_reg):
    """
    Extract case->jal mapping from a switch dispatcher.
    Returns list of (case_value, jal_target_addr) tuples.
    """
    cases = []
    # Track comparison values
    # Pattern: li at, N / beq switch_reg, at, offset -> case N
    # Also: beqz switch_reg -> case 0

    n_insns = len(insns)
    for i in range(n_insns):
        insn = insns[i]

        # beqz switch_reg -> case 0
        if is_beqz(insn, switch_reg):
            # The branch target contains the case 0 code
            # Look for jal in the branch target area
            branch_offset = insn['imm_signed']
            branch_target_idx = i + 1 + branch_offset
            # Find jal near branch target
            if 0 <= branch_target_idx < n_insns:
                for j in range(branch_target_idx, min(branch_target_idx + 8, n_insns)):
                    if is_jal(insns[j]):
                        target_addr = 0x80000000 | (insns[j]['target'] << 2)
                        cases.append((0, target_addr))
                        break
            # Also check delay slot and following instructions
            # Sometimes the jal is right after (fallthrough for != 0 case)

        # li at, N followed by beq switch_reg, at
        if is_li_at(insn) or is_ori_at(insn):
            case_val = insn['imm']
            # Check next non-nop instruction for beq
            for j in range(i + 1, min(i + 3, n_insns)):
                if is_beq(insns[j], rs=switch_reg, rt=1) or is_beq(insns[j], rs=1, rt=switch_reg):
                    branch_offset = insns[j]['imm_signed']
                    branch_target_idx = j + 1 + branch_offset
                    if 0 <= branch_target_idx < n_insns:
                        for k in range(branch_target_idx, min(branch_target_idx + 8, n_insns)):
                            if is_jal(insns[k]):
                                target_addr = 0x80000000 | (insns[k]['target'] << 2)
                                cases.append((case_val, target_addr))
                                break
                    break

    # Deduplicate
    seen = set()
    unique_cases = []
    for c in cases:
        if c not in seen:
            seen.add(c)
            unique_cases.append(c)

    return sorted(unique_cases, key=lambda x: x[0])


def find_switch_comparisons(insns, switch_reg, start_idx, end_idx):
    """
    Find all li at,N / beq switch_reg,at patterns in a range.
    Returns the number of case comparisons found.
    """
    count = 0
    for i in range(start_idx, min(end_idx, len(insns))):
        if is_li_at(insns[i]) or is_ori_at(insns[i]):
            # Check next few instructions for beq with switch_reg and at
            for j in range(i + 1, min(i + 3, len(insns))):
                if (is_beq(insns[j], rs=switch_reg, rt=1) or
                    is_beq(insns[j], rs=1, rt=switch_reg)):
                    count += 1
                    break
    return count


def analyze_function(binary_data, func_info, c_covered_ranges):
    """Analyze a single function for switch patterns."""
    name = func_info['name']
    addr = int(func_info['address'], 16)
    size = func_info['size']

    if size < 24:  # Too small for any switch pattern
        return None

    file_offset = (addr - 0x80041000) + 0x800
    if file_offset < 0 or file_offset + size > len(binary_data):
        return None

    func_bytes = binary_data[file_offset:file_offset + size]
    n_words = size // 4
    if n_words < 6:
        return None

    insns = []
    for i in range(n_words):
        word = struct.unpack_from('<I', func_bytes, i * 4)[0]
        insns.append(decode_insn(word))

    # Check if already covered by C
    already_covered = False
    for (cstart, cend) in c_covered_ranges:
        if addr >= cstart and addr + size <= cend:
            already_covered = True
            break

    result = None

    # === PATTERN A: Cross-prologue global load + switch ===
    # Instructions 0-1: lui REG, hi + load REG, lo(REG)
    # Instruction 2: addiu sp, sp, negative
    if n_words >= 8 and is_lui(insns[0]):
        lui_reg = insns[0]['rt']
        lui_hi = insns[0]['imm']

        # Check instruction 1 is a load from the same register
        if is_load(insns[1], rt=lui_reg, rs=lui_reg):
            load_lo = insns[1]['imm']
            global_addr = (lui_hi << 16) + (load_lo if load_lo < 0x8000 else load_lo - 0x10000)

            # Load type
            load_types = {0x20: 'lb', 0x21: 'lh', 0x23: 'lw', 0x24: 'lbu', 0x25: 'lhu'}
            load_type = load_types.get(insns[1]['opcode'], 'unknown')

            # Check instruction 2 is prologue
            if is_addiu_sp(insns[2]) and insns[2]['imm_signed'] < 0:
                frame_size = -insns[2]['imm_signed']
                switch_reg = lui_reg

                # Look for beqz/bnez of switch_reg in first ~8 instructions
                has_branch = False
                for idx in range(3, min(10, n_words)):
                    if is_beqz(insns[idx], switch_reg) or is_bnez(insns[idx], switch_reg):
                        has_branch = True
                        break
                    if is_beq(insns[idx], rs=switch_reg) or is_beq(insns[idx], rt=switch_reg):
                        has_branch = True
                        break

                if has_branch:
                    # Look for li at, N + beq switch_reg, at pattern
                    n_cases = find_switch_comparisons(insns, switch_reg, 3, n_words)

                    # Need at least the beqz (case 0) pattern
                    # Count total jals
                    jal_targets = get_jal_targets(insns, addr)

                    if n_cases >= 1 or (has_branch and len(jal_targets) >= 2):
                        case_calls = extract_case_calls(insns, addr, switch_reg)

                        result = {
                            'name': name,
                            'address': f'0x{addr:08X}',
                            'size': size,
                            'pattern': 'A',
                            'pattern_description': 'Cross-prologue global load + switch',
                            'global_address': f'0x{global_addr & 0xFFFFFFFF:08X}',
                            'load_type': load_type,
                            'switch_register': REG_NAMES.get(switch_reg, f'r{switch_reg}'),
                            'frame_size': frame_size,
                            'num_cases': n_cases + 1,  # +1 for the beqz case 0
                            'jal_targets': [f'0x{t:08X}' for t in jal_targets],
                            'case_calls': [(c, f'0x{t:08X}') for c, t in case_calls],
                            'already_covered_by_c': already_covered,
                            'region': func_info.get('region', 'unknown'),
                        }

    if result is not None:
        return result

    # === PATTERN B & C: Switch on v1 ===
    # Instruction 0: addiu sp, sp, negative
    if is_addiu_sp(insns[0]) and insns[0]['imm_signed'] < 0:
        frame_size = -insns[0]['imm_signed']
        switch_reg = 3  # v1

        # Look for beqz v1 or beq v1,zero in first ~10 instructions
        has_v1_branch = False
        v1_branch_idx = -1
        for idx in range(1, min(12, n_words)):
            if is_beqz(insns[idx], 3):  # beqz v1
                has_v1_branch = True
                v1_branch_idx = idx
                break
            if is_beq(insns[idx]) and ((insns[idx]['rs'] == 3 and insns[idx]['rt'] == 0) or
                                        (insns[idx]['rs'] == 0 and insns[idx]['rt'] == 3)):
                has_v1_branch = True
                v1_branch_idx = idx
                break

        if not has_v1_branch:
            # Also check for bnez v1
            for idx in range(1, min(12, n_words)):
                if is_bnez(insns[idx], 3):
                    has_v1_branch = True
                    v1_branch_idx = idx
                    break

        if has_v1_branch:
            # Check for sw ra near prologue
            has_sw_ra = False
            for idx in range(1, min(6, n_words)):
                if is_sw_ra(insns[idx]):
                    has_sw_ra = True
                    break

            # Look for li at, N + beq v1, at pattern
            n_cases = find_switch_comparisons(insns, 3, v1_branch_idx, n_words)

            jal_targets = get_jal_targets(insns, addr)

            if n_cases >= 1 and has_sw_ra:
                # Check for Pattern C: move v0, v1
                has_move_v0_v1 = False
                for idx in range(n_words):
                    if is_move_v0_v1(insns[idx]):
                        has_move_v0_v1 = True
                        break

                pattern = 'C' if has_move_v0_v1 else 'B'

                case_calls = extract_case_calls(insns, addr, 3)

                result = {
                    'name': name,
                    'address': f'0x{addr:08X}',
                    'size': size,
                    'pattern': pattern,
                    'pattern_description': ('Switch on v1 with return value (move v0,v1)'
                                           if pattern == 'C' else 'Switch on v1'),
                    'global_address': None,
                    'load_type': None,
                    'switch_register': 'v1',
                    'frame_size': frame_size,
                    'num_cases': n_cases + 1,
                    'jal_targets': [f'0x{t:08X}' for t in jal_targets],
                    'case_calls': [(c, f'0x{t:08X}') for c, t in case_calls],
                    'already_covered_by_c': already_covered,
                    'region': func_info.get('region', 'unknown'),
                }

    if result is not None:
        return result

    # === PATTERN A variant: lui+load not necessarily same register ===
    # Sometimes the load uses a different dest register than lui
    # e.g., lui t6, hi; lbu t7, lo(t6) - but this is unusual for switch vars
    # Skip this for now - focus on standard patterns

    # === PATTERN B variant: Switch on a0 (after move from v1 or direct) ===
    # Check if the function starts with prologue and switches on a0
    if is_addiu_sp(insns[0]) and insns[0]['imm_signed'] < 0:
        frame_size = -insns[0]['imm_signed']

        for switch_reg in [4, 5]:  # a0, a1
            has_branch = False
            branch_idx = -1
            for idx in range(1, min(12, n_words)):
                if is_beqz(insns[idx], switch_reg) or is_bnez(insns[idx], switch_reg):
                    has_branch = True
                    branch_idx = idx
                    break
                if is_beq(insns[idx]) and ((insns[idx]['rs'] == switch_reg and insns[idx]['rt'] == 0) or
                                            (insns[idx]['rs'] == 0 and insns[idx]['rt'] == switch_reg)):
                    has_branch = True
                    branch_idx = idx
                    break

            if has_branch:
                has_sw_ra = False
                for idx in range(1, min(6, n_words)):
                    if is_sw_ra(insns[idx]):
                        has_sw_ra = True
                        break

                n_cases = find_switch_comparisons(insns, switch_reg, branch_idx, n_words)
                jal_targets = get_jal_targets(insns, addr)

                if n_cases >= 1 and has_sw_ra and len(jal_targets) >= 2:
                    case_calls = extract_case_calls(insns, addr, switch_reg)

                    result = {
                        'name': name,
                        'address': f'0x{addr:08X}',
                        'size': size,
                        'pattern': 'B',
                        'pattern_description': f'Switch on {REG_NAMES[switch_reg]}',
                        'global_address': None,
                        'load_type': None,
                        'switch_register': REG_NAMES[switch_reg],
                        'frame_size': frame_size,
                        'num_cases': n_cases + 1,
                        'jal_targets': [f'0x{t:08X}' for t in jal_targets],
                        'case_calls': [(c, f'0x{t:08X}') for c, t in case_calls],
                        'already_covered_by_c': already_covered,
                        'region': func_info.get('region', 'unknown'),
                    }
                    break

    return result


def main():
    config_path = 'config/level3_functions.json'
    binary_path = 'disks/SLPS_000.65'

    with open(config_path) as f:
        config = json.load(f)

    with open(binary_path, 'rb') as f:
        binary_data = f.read()

    functions = config['functions']

    # Get ASM-only functions
    asm_funcs = [fn for fn in functions if fn['source'].startswith('asm/')]

    # Build C-covered ranges for overlap check
    c_funcs = [fn for fn in functions if fn['source'].startswith('src/')]
    c_covered_ranges = []
    for fn in c_funcs:
        addr = int(fn['address'], 16)
        c_covered_ranges.append((addr, addr + fn['size']))

    print(f"Scanning {len(asm_funcs)} ASM functions...", file=sys.stderr)

    results = []
    for fn in asm_funcs:
        if fn['size'] == 0:
            continue
        match = analyze_function(binary_data, fn, c_covered_ranges)
        if match is not None:
            results.append(match)

    # Sort by address
    results.sort(key=lambda x: int(x['address'], 16))

    # Summary
    pattern_counts = {}
    for r in results:
        p = r['pattern']
        pattern_counts[p] = pattern_counts.get(p, 0) + 1

    summary = {
        'total_matches': len(results),
        'pattern_counts': pattern_counts,
        'already_covered': sum(1 for r in results if r['already_covered_by_c']),
        'uncovered': sum(1 for r in results if not r['already_covered_by_c']),
    }

    output = {
        'summary': summary,
        'functions': results,
    }

    print(json.dumps(output, indent=2))

    # Also print a human-readable summary to stderr
    print(f"\n=== RESULTS ===", file=sys.stderr)
    print(f"Total matching functions: {len(results)}", file=sys.stderr)
    for p, count in sorted(pattern_counts.items()):
        descs = {'A': 'Cross-prologue global load + switch',
                 'B': 'Switch on register (no cross-prologue)',
                 'C': 'Switch on v1 with move v0,v1'}
        print(f"  Pattern {p} ({descs.get(p, '?')}): {count}", file=sys.stderr)
    print(f"Already covered by C: {summary['already_covered']}", file=sys.stderr)
    print(f"Uncovered (conversion candidates): {summary['uncovered']}", file=sys.stderr)


if __name__ == '__main__':
    main()
