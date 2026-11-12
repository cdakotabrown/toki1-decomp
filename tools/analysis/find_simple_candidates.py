#!/usr/bin/env python3
"""
Scan the game binary for small ASM functions with simple cross-prologue guard patterns.

Reads config/level3_functions.json, extracts instruction bytes from the binary,
decodes MIPS I instructions, and identifies functions that match the cross-prologue
guard pattern (lui+load before prologue, branch, jal, jr ra).

Also identifies simple "call+return" patterns (prologue, save ra, jal, restore, jr ra).
"""

import json
import struct
import sys
from pathlib import Path

# Constants
BINARY_PATH = "disks/SLPS_000.65"
CONFIG_PATH = "config/level3_functions.json"
TEXT_START_ADDR = 0x80041000
FILE_OFFSET_BASE = 0x800
GAME_REGION_END = 0x80086000
LIB_REGION_END = 0x800C0000

# MIPS opcode constants
OP_SPECIAL = 0x00
OP_REGIMM = 0x01
OP_J = 0x02
OP_JAL = 0x03
OP_BEQ = 0x04
OP_BNE = 0x05
OP_BLEZ = 0x06
OP_BGTZ = 0x07
OP_ADDIU = 0x09
OP_SLTI = 0x0A
OP_SLTIU = 0x0B
OP_ANDI = 0x0C
OP_ORI = 0x0D
OP_LUI = 0x0F
OP_LB = 0x20
OP_LH = 0x21
OP_LW = 0x23
OP_LBU = 0x24
OP_LHU = 0x25
OP_SB = 0x28
OP_SH = 0x29
OP_SW = 0x2B

# SPECIAL function codes
FUNC_JR = 0x08
FUNC_JALR = 0x09
FUNC_SLL = 0x00
FUNC_SRL = 0x02
FUNC_SRA = 0x03
FUNC_ADDU = 0x21
FUNC_SUBU = 0x23
FUNC_AND = 0x24
FUNC_OR = 0x25
FUNC_NOR = 0x27
FUNC_SLT = 0x2A
FUNC_SLTU = 0x2B

# REGIMM rt codes
RT_BLTZ = 0x00
RT_BGEZ = 0x01

# Register names
REG_NAMES = [
    "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra",
]

# Register numbers
REG_SP = 29
REG_RA = 31
REG_T6 = 14
REG_T7 = 15
REG_T8 = 24
REG_V0 = 2
REG_V1 = 3
REG_A0 = 4
REG_AT = 1

IDO_TEMP_REGS = {REG_T6, REG_T7, REG_T8, REG_V0, REG_V1, REG_AT}


def decode_instruction(word):
    """Decode a 32-bit MIPS instruction into its fields."""
    op = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    rd = (word >> 11) & 0x1F
    shamt = (word >> 6) & 0x1F
    func = word & 0x3F
    imm = word & 0xFFFF
    imm_signed = imm if imm < 0x8000 else imm - 0x10000
    target = word & 0x03FFFFFF
    return {
        'word': word,
        'op': op,
        'rs': rs,
        'rt': rt,
        'rd': rd,
        'shamt': shamt,
        'func': func,
        'imm': imm,
        'imm_signed': imm_signed,
        'target': target,
    }


def format_instruction(insn, addr):
    """Format a decoded instruction as human-readable assembly."""
    op = insn['op']
    rs = insn['rs']
    rt = insn['rt']
    rd = insn['rd']
    func = insn['func']
    imm = insn['imm']
    imm_s = insn['imm_signed']

    if op == OP_SPECIAL:
        if func == FUNC_JR:
            return f"jr {REG_NAMES[rs]}"
        elif func == FUNC_JALR:
            return f"jalr {REG_NAMES[rs]}"
        elif func == FUNC_SLL and rd == 0 and rt == 0 and insn['shamt'] == 0:
            return "nop"
        elif func == FUNC_SLL:
            return f"sll {REG_NAMES[rd]}, {REG_NAMES[rt]}, {insn['shamt']}"
        elif func == FUNC_ADDU:
            if rt == 0:
                return f"move {REG_NAMES[rd]}, {REG_NAMES[rs]}  (addu)"
            return f"addu {REG_NAMES[rd]}, {REG_NAMES[rs]}, {REG_NAMES[rt]}"
        elif func == FUNC_OR:
            if rt == 0:
                return f"move {REG_NAMES[rd]}, {REG_NAMES[rs]}  (or)"
            return f"or {REG_NAMES[rd]}, {REG_NAMES[rs]}, {REG_NAMES[rt]}"
        elif func == FUNC_SUBU:
            return f"subu {REG_NAMES[rd]}, {REG_NAMES[rs]}, {REG_NAMES[rt]}"
        elif func == FUNC_AND:
            return f"and {REG_NAMES[rd]}, {REG_NAMES[rs]}, {REG_NAMES[rt]}"
        elif func == FUNC_NOR:
            return f"nor {REG_NAMES[rd]}, {REG_NAMES[rs]}, {REG_NAMES[rt]}"
        elif func == FUNC_SLT:
            return f"slt {REG_NAMES[rd]}, {REG_NAMES[rs]}, {REG_NAMES[rt]}"
        elif func == FUNC_SLTU:
            return f"sltu {REG_NAMES[rd]}, {REG_NAMES[rs]}, {REG_NAMES[rt]}"
        elif func == FUNC_SRL:
            return f"srl {REG_NAMES[rd]}, {REG_NAMES[rt]}, {insn['shamt']}"
        elif func == FUNC_SRA:
            return f"sra {REG_NAMES[rd]}, {REG_NAMES[rt]}, {insn['shamt']}"
        else:
            return f"special_{func:#04x} {REG_NAMES[rd]}, {REG_NAMES[rs]}, {REG_NAMES[rt]}"
    elif op == OP_REGIMM:
        branch_target = addr + 4 + (insn['imm_signed'] << 2)
        if rt == RT_BLTZ:
            return f"bltz {REG_NAMES[rs]}, 0x{branch_target:08X}"
        elif rt == RT_BGEZ:
            return f"bgez {REG_NAMES[rs]}, 0x{branch_target:08X}"
        else:
            return f"regimm_{rt:#04x} {REG_NAMES[rs]}, 0x{branch_target:08X}"
    elif op == OP_J:
        jtarget = (addr & 0xF0000000) | (insn['target'] << 2)
        return f"j 0x{jtarget:08X}"
    elif op == OP_JAL:
        jtarget = (addr & 0xF0000000) | (insn['target'] << 2)
        return f"jal 0x{jtarget:08X}"
    elif op == OP_BEQ:
        branch_target = addr + 4 + (insn['imm_signed'] << 2)
        if rt == 0:
            return f"beqz {REG_NAMES[rs]}, 0x{branch_target:08X}"
        return f"beq {REG_NAMES[rs]}, {REG_NAMES[rt]}, 0x{branch_target:08X}"
    elif op == OP_BNE:
        branch_target = addr + 4 + (insn['imm_signed'] << 2)
        if rt == 0:
            return f"bnez {REG_NAMES[rs]}, 0x{branch_target:08X}"
        return f"bne {REG_NAMES[rs]}, {REG_NAMES[rt]}, 0x{branch_target:08X}"
    elif op == OP_BLEZ:
        branch_target = addr + 4 + (insn['imm_signed'] << 2)
        return f"blez {REG_NAMES[rs]}, 0x{branch_target:08X}"
    elif op == OP_BGTZ:
        branch_target = addr + 4 + (insn['imm_signed'] << 2)
        return f"bgtz {REG_NAMES[rs]}, 0x{branch_target:08X}"
    elif op == OP_ADDIU:
        return f"addiu {REG_NAMES[rt]}, {REG_NAMES[rs]}, {imm_s}"
    elif op == OP_SLTI:
        return f"slti {REG_NAMES[rt]}, {REG_NAMES[rs]}, {imm_s}"
    elif op == OP_SLTIU:
        return f"sltiu {REG_NAMES[rt]}, {REG_NAMES[rs]}, {imm_s}"
    elif op == OP_ANDI:
        return f"andi {REG_NAMES[rt]}, {REG_NAMES[rs]}, 0x{imm:04X}"
    elif op == OP_ORI:
        return f"ori {REG_NAMES[rt]}, {REG_NAMES[rs]}, 0x{imm:04X}"
    elif op == OP_LUI:
        return f"lui {REG_NAMES[rt]}, 0x{imm:04X}"
    elif op == OP_LB:
        return f"lb {REG_NAMES[rt]}, {imm_s}({REG_NAMES[rs]})"
    elif op == OP_LH:
        return f"lh {REG_NAMES[rt]}, {imm_s}({REG_NAMES[rs]})"
    elif op == OP_LW:
        return f"lw {REG_NAMES[rt]}, {imm_s}({REG_NAMES[rs]})"
    elif op == OP_LBU:
        return f"lbu {REG_NAMES[rt]}, {imm_s}({REG_NAMES[rs]})"
    elif op == OP_LHU:
        return f"lhu {REG_NAMES[rt]}, {imm_s}({REG_NAMES[rs]})"
    elif op == OP_SB:
        return f"sb {REG_NAMES[rt]}, {imm_s}({REG_NAMES[rs]})"
    elif op == OP_SH:
        return f"sh {REG_NAMES[rt]}, {imm_s}({REG_NAMES[rs]})"
    elif op == OP_SW:
        return f"sw {REG_NAMES[rt]}, {imm_s}({REG_NAMES[rs]})"
    else:
        return f"op_{op:#04x} rt={REG_NAMES[rt]} rs={REG_NAMES[rs]} imm={imm:#06x}"


def read_function_bytes(binary_data, addr, size):
    """Read function bytes from binary data."""
    file_offset = (addr - TEXT_START_ADDR) + FILE_OFFSET_BASE
    if file_offset < 0 or file_offset + size > len(binary_data):
        return None
    return binary_data[file_offset:file_offset + size]


def decode_function(binary_data, addr, size):
    """Decode all instructions in a function."""
    func_bytes = read_function_bytes(binary_data, addr, size)
    if func_bytes is None:
        return []
    instructions = []
    for i in range(0, len(func_bytes), 4):
        if i + 4 > len(func_bytes):
            break
        word = struct.unpack('<I', func_bytes[i:i+4])[0]
        insn = decode_instruction(word)
        insn['addr'] = addr + i
        insn['offset'] = i
        instructions.append(insn)
    return instructions


def is_cross_prologue_guard(instructions):
    """
    Check if function matches cross-prologue guard pattern:
    1. Starts with lui rx (where rx is an IDO temp register)
    2. Followed by load (lbu/lw/lhu/lb/lh) using same register
    3. Then addiu sp, sp, -XX (prologue)
    4. Contains a branch (beq/bne/beqz/bnez/bgez/bltz) shortly after
    5. Has a jal somewhere
    6. Ends with jr ra (+ delay slot nop)
    """
    if len(instructions) < 5:
        return False, None

    insn0 = instructions[0]
    insn1 = instructions[1]

    # Check: starts with lui rx (IDO temp reg)
    if insn0['op'] != OP_LUI:
        return False, None
    lui_reg = insn0['rt']
    if lui_reg not in IDO_TEMP_REGS:
        return False, None

    # Check: second instruction is a load using the lui register
    load_ops = {OP_LBU, OP_LW, OP_LHU, OP_LB, OP_LH}
    if insn1['op'] not in load_ops:
        return False, None
    if insn1['rs'] != lui_reg:
        return False, None

    # Check: find addiu sp, sp, -XX (prologue) within first 5 instructions
    prologue_idx = None
    for i in range(2, min(5, len(instructions))):
        insn = instructions[i]
        if insn['op'] == OP_ADDIU and insn['rt'] == REG_SP and insn['rs'] == REG_SP and insn['imm_signed'] < 0:
            prologue_idx = i
            break
    if prologue_idx is None:
        return False, None

    # Check: has a branch within first 8 instructions
    has_branch = False
    branch_ops = {OP_BEQ, OP_BNE, OP_BLEZ, OP_BGTZ}
    for i in range(prologue_idx, min(prologue_idx + 6, len(instructions))):
        insn = instructions[i]
        if insn['op'] in branch_ops or insn['op'] == OP_REGIMM:
            has_branch = True
            break
    if not has_branch:
        return False, None

    # Check: has jal somewhere
    has_jal = any(insn['op'] == OP_JAL for insn in instructions)

    # Check: ends with jr ra (possibly followed by nop in delay slot)
    has_jr_ra = False
    for i in range(len(instructions) - 1, max(len(instructions) - 3, -1), -1):
        insn = instructions[i]
        if insn['op'] == OP_SPECIAL and insn['func'] == FUNC_JR and insn['rs'] == REG_RA:
            has_jr_ra = True
            break

    if not has_jr_ra:
        return False, None

    frame_size = -instructions[prologue_idx]['imm_signed']
    info = {
        'pattern': 'cross_prologue_guard',
        'lui_reg': REG_NAMES[lui_reg],
        'load_type': format_instruction(insn1, insn1.get('addr', 0)).split()[0],
        'frame_size': frame_size,
        'has_jal': has_jal,
        'num_jals': sum(1 for insn in instructions if insn['op'] == OP_JAL),
    }
    return True, info


def is_simple_call_return(instructions):
    """
    Check if function is a simple call+return pattern:
    1. addiu sp, sp, -XX (prologue)
    2. sw ra, XX(sp)
    3. 0-3 jal calls
    4. lw ra, XX(sp)
    5. jr ra
    6. addiu sp, sp, XX (epilogue in delay slot)
    """
    if len(instructions) < 5:
        return False, None

    insn0 = instructions[0]

    # Check: starts with addiu sp, sp, -XX
    if insn0['op'] != OP_ADDIU or insn0['rt'] != REG_SP or insn0['rs'] != REG_SP:
        return False, None
    if insn0['imm_signed'] >= 0:
        return False, None

    # Check: sw ra early
    has_sw_ra = False
    for i in range(1, min(4, len(instructions))):
        insn = instructions[i]
        if insn['op'] == OP_SW and insn['rt'] == REG_RA and insn['rs'] == REG_SP:
            has_sw_ra = True
            break
    if not has_sw_ra:
        return False, None

    # Check: ends with jr ra
    has_jr_ra = False
    for i in range(len(instructions) - 1, max(len(instructions) - 3, -1), -1):
        insn = instructions[i]
        if insn['op'] == OP_SPECIAL and insn['func'] == FUNC_JR and insn['rs'] == REG_RA:
            has_jr_ra = True
            break
    if not has_jr_ra:
        return False, None

    # Count jals
    num_jals = sum(1 for insn in instructions if insn['op'] == OP_JAL)

    # Count s-register saves (sw s0-s7 to stack)
    s_reg_saves = 0
    for insn in instructions:
        if insn['op'] == OP_SW and insn['rs'] == REG_SP and 16 <= insn['rt'] <= 23:
            s_reg_saves += 1

    frame_size = -insn0['imm_signed']
    info = {
        'pattern': 'simple_call_return',
        'frame_size': frame_size,
        'num_jals': num_jals,
        's_reg_saves': s_reg_saves,
    }
    return True, info


def is_leaf_function(instructions):
    """
    Check if function is a simple leaf function (no calls, no stack frame or small frame):
    - No jal instructions
    - Possibly no stack frame at all
    - Just some computation and jr ra
    """
    if len(instructions) < 3:
        return False, None

    # No jals
    has_jal = any(insn['op'] == OP_JAL for insn in instructions)
    if has_jal:
        return False, None

    # Ends with jr ra
    has_jr_ra = False
    for i in range(len(instructions) - 1, max(len(instructions) - 3, -1), -1):
        insn = instructions[i]
        if insn['op'] == OP_SPECIAL and insn['func'] == FUNC_JR and insn['rs'] == REG_RA:
            has_jr_ra = True
            break
    if not has_jr_ra:
        return False, None

    # Check for stack frame
    has_frame = False
    frame_size = 0
    for insn in instructions:
        if insn['op'] == OP_ADDIU and insn['rt'] == REG_SP and insn['rs'] == REG_SP and insn['imm_signed'] < 0:
            has_frame = True
            frame_size = -insn['imm_signed']
            break

    info = {
        'pattern': 'leaf_function',
        'has_frame': has_frame,
        'frame_size': frame_size,
    }
    return True, info


def main():
    # Load config
    with open(CONFIG_PATH) as f:
        config = json.load(f)

    # Load binary
    with open(BINARY_PATH, 'rb') as f:
        binary_data = f.read()

    print(f"Binary size: {len(binary_data)} bytes")
    print(f"Total functions in config: {len(config['functions'])}")

    # Filter to ASM functions only
    asm_funcs = [f for f in config['functions'] if f['source'].endswith('.s')]
    print(f"ASM functions: {len(asm_funcs)}")

    # Separate by region
    game_asm = [f for f in asm_funcs if int(f['address'], 16) < GAME_REGION_END]
    lib_asm = [f for f in asm_funcs if int(f['address'], 16) >= GAME_REGION_END]
    print(f"Game region ASM: {len(game_asm)}")
    print(f"Library region ASM: {len(lib_asm)}")

    # Focus on small functions (up to 160 bytes = 40 instructions)
    MAX_SIZE = 160
    MIN_SIZE = 28  # 7 instructions minimum
    small_game = [f for f in game_asm if MIN_SIZE <= f['size'] <= MAX_SIZE]
    small_lib = [f for f in lib_asm if MIN_SIZE <= f['size'] <= MAX_SIZE]
    print(f"\nSmall game ASM ({MIN_SIZE}-{MAX_SIZE} bytes): {len(small_game)}")
    print(f"Small library ASM ({MIN_SIZE}-{MAX_SIZE} bytes): {len(small_lib)}")

    # Scan all ASM functions (not just small ones) for patterns
    candidates = []

    for func_info in asm_funcs:
        addr = int(func_info['address'], 16)
        size = func_info['size']
        name = func_info['name']
        region = "game" if addr < GAME_REGION_END else "library"

        # Only consider functions up to 160 bytes
        if size > MAX_SIZE or size < MIN_SIZE:
            continue

        instructions = decode_function(binary_data, addr, size)
        if not instructions:
            continue

        num_insns = len(instructions)

        # Try cross-prologue guard pattern
        is_cp, cp_info = is_cross_prologue_guard(instructions)
        if is_cp:
            candidates.append({
                'name': name,
                'address': addr,
                'size': size,
                'num_insns': num_insns,
                'region': region,
                'info': cp_info,
                'instructions': instructions,
                'priority': 1,  # highest priority
            })
            continue

        # Try simple call+return
        is_cr, cr_info = is_simple_call_return(instructions)
        if is_cr and cr_info['num_jals'] <= 3 and cr_info['s_reg_saves'] <= 2:
            candidates.append({
                'name': name,
                'address': addr,
                'size': size,
                'num_insns': num_insns,
                'region': region,
                'info': cr_info,
                'instructions': instructions,
                'priority': 2,
            })
            continue

        # Try leaf functions
        is_leaf, leaf_info = is_leaf_function(instructions)
        if is_leaf:
            candidates.append({
                'name': name,
                'address': addr,
                'size': size,
                'num_insns': num_insns,
                'region': region,
                'info': leaf_info,
                'instructions': instructions,
                'priority': 3,
            })
            continue

    # Sort by priority then size
    candidates.sort(key=lambda c: (c['priority'], c['size']))

    # Report
    print(f"\n{'='*80}")
    print(f"CANDIDATES FOUND: {len(candidates)}")
    print(f"  Cross-prologue guard: {sum(1 for c in candidates if c['info']['pattern'] == 'cross_prologue_guard')}")
    print(f"  Simple call+return:   {sum(1 for c in candidates if c['info']['pattern'] == 'simple_call_return')}")
    print(f"  Leaf function:        {sum(1 for c in candidates if c['info']['pattern'] == 'leaf_function')}")
    print(f"{'='*80}")

    # Show top 20 candidates
    print(f"\nTOP 20 CANDIDATES (sorted by priority then size):")
    print(f"{'='*80}")

    for i, cand in enumerate(candidates[:20]):
        info = cand['info']
        print(f"\n--- Candidate #{i+1}: {cand['name']} ---")
        print(f"  Address: 0x{cand['address']:08X}  Size: {cand['size']} bytes ({cand['num_insns']} insns)  Region: {cand['region']}")
        print(f"  Pattern: {info['pattern']}")
        if info['pattern'] == 'cross_prologue_guard':
            print(f"  LUI reg: {info['lui_reg']}, Load type: {info['load_type']}, Frame: {info['frame_size']}, JALs: {info['num_jals']}")
        elif info['pattern'] == 'simple_call_return':
            print(f"  Frame: {info['frame_size']}, JALs: {info['num_jals']}, S-reg saves: {info['s_reg_saves']}")
        elif info['pattern'] == 'leaf_function':
            print(f"  Has frame: {info['has_frame']}, Frame: {info['frame_size']}")

        print(f"  Instructions:")
        for insn in cand['instructions']:
            asm_str = format_instruction(insn, insn['addr'])
            print(f"    0x{insn['addr']:08X}:  {insn['word']:08X}  {asm_str}")

    # Summary table of ALL candidates
    print(f"\n{'='*80}")
    print(f"ALL {len(candidates)} CANDIDATES SUMMARY:")
    print(f"{'='*80}")
    print(f"{'#':>3} {'Name':<24} {'Addr':>12} {'Size':>5} {'Insns':>5} {'Region':>7} {'Pattern':<24} {'Details'}")
    print(f"{'-'*3} {'-'*24} {'-'*12} {'-'*5} {'-'*5} {'-'*7} {'-'*24} {'-'*30}")
    for i, cand in enumerate(candidates):
        info = cand['info']
        details = ""
        if info['pattern'] == 'cross_prologue_guard':
            details = f"lui={info['lui_reg']} load={info['load_type']} frame={info['frame_size']} jals={info['num_jals']}"
        elif info['pattern'] == 'simple_call_return':
            details = f"frame={info['frame_size']} jals={info['num_jals']} sregs={info['s_reg_saves']}"
        elif info['pattern'] == 'leaf_function':
            details = f"frame={info['frame_size']}"
        print(f"{i+1:>3} {cand['name']:<24} 0x{cand['address']:08X} {cand['size']:>5} {cand['num_insns']:>5} {cand['region']:>7} {info['pattern']:<24} {details}")


if __name__ == '__main__':
    main()
