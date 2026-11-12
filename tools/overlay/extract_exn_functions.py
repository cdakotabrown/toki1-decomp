#!/usr/bin/env python3
"""Extract and disassemble functions from EXN overlay modules.

Scans EXN binary for function boundaries (prologue/epilogue patterns),
outputs individual .s assembly files for each function.

Usage:
    python3 tools/overlay/extract_exn_functions.py pc/assets/overlays/OPTION.EXN asm/overlay/OPTION/
"""
import struct
import sys
import os
from pathlib import Path

LOAD_ADDR = 0x80132000  # All overlays load here


def find_functions(data, max_offset=None):
    """Find function boundaries by scanning for prologues and jr ra."""
    if max_offset is None:
        max_offset = len(data) - 4  # exclude checksum

    functions = []
    in_function = False
    func_start = 0

    for i in range(0, max_offset, 4):
        insn = struct.unpack_from('<I', data, i)[0]
        op = (insn >> 26) & 0x3F
        rs = (insn >> 21) & 0x1F
        rt = (insn >> 16) & 0x1F

        # Detect function prologue: addiu sp, sp, -N
        if op == 0x09 and rs == 29 and rt == 29:
            imm = insn & 0xFFFF
            if imm >= 0x8000:  # negative = stack allocation
                if in_function and i > func_start + 8:
                    # End previous function
                    functions.append((func_start, i - func_start))
                func_start = i
                in_function = True

        # Detect function epilogue: jr ra
        if insn == 0x03E00008 and in_function:
            # Function ends after the delay slot
            func_end = i + 8
            if func_end <= max_offset:
                functions.append((func_start, func_end - func_start))
                in_function = False

    return functions


def find_gap_functions(data, prologue_functions, max_offset):
    """Detect functions missed by prologue-based scanning.

    Two kinds recovered here:

    1. **Leaf / init routines** — prologue-less functions whose body is a
       sequence of lui/ori/sw (or any non-prologue instructions) terminated
       by `jr ra` + delay slot. These are common at the very start of every
       overlay (dispatch-table initializers that populate 0x80155E__).
    2. **Cross-prologue alt-entries** — a few instructions that IDO hoisted
       above the real `addiu sp` prologue. The gap immediately precedes an
       extracted prologue-bearing function and contains no `jr ra`. The alt
       entry covers [gap_start, extracted_func_start).

    Returns list of (offset, size, kind) where kind is 'leaf' or
    'cross-prologue'. Pure-data orphans (no terminator, no abutting prologue)
    are skipped.
    """
    prologue_functions = sorted(prologue_functions)
    prologue_starts = {off for off, _ in prologue_functions}

    # Build gap list from 0 to max_offset
    gaps = []
    prev_end = 0
    for off, size in prologue_functions:
        if prev_end < off:
            gaps.append((prev_end, off))
        prev_end = off + size
    if prev_end < max_offset:
        gaps.append((prev_end, max_offset))

    new_funcs = []
    for gs, ge in gaps:
        if gs >= ge:
            continue
        # Skip leading all-zero padding (runs of nop before real code start)
        # Only skip if the entire leading run is zero — helps avoid emitting
        # pure-zero "functions".
        cur = gs
        while cur < ge and struct.unpack_from('<I', data, cur)[0] == 0:
            cur += 4
        if cur >= ge:
            continue
        func_start = cur
        i = cur
        while i < ge:
            insn = struct.unpack_from('<I', data, i)[0]
            if insn == 0x03E00008:  # jr ra
                func_end = min(i + 8, ge)  # include delay slot
                size = func_end - func_start
                # Only emit if function contains at least one meaningful
                # instruction besides nop/jr. Reject empty padding regions.
                has_real_insn = False
                for j in range(func_start, func_end, 4):
                    w = struct.unpack_from('<I', data, j)[0]
                    if w != 0 and w != 0x03E00008:
                        has_real_insn = True
                        break
                if has_real_insn:
                    new_funcs.append((func_start, size, 'leaf'))
                # Skip trailing zero padding before next function
                next_start = func_end
                while next_start < ge and struct.unpack_from('<I', data, next_start)[0] == 0:
                    next_start += 4
                func_start = next_start
                i = next_start
                continue
            i += 4

        # Tail handling: if cur_start < ge and the gap abuts an extracted
        # prologue-bearing function on the right, this is a cross-prologue
        # alt-entry. Otherwise it is data/orphan and skipped.
        if func_start < ge and ge in prologue_starts:
            tail_size = ge - func_start
            # Reject tails that are all zero
            has_real = False
            for j in range(func_start, ge, 4):
                if struct.unpack_from('<I', data, j)[0] != 0:
                    has_real = True
                    break
            if has_real:
                new_funcs.append((func_start, tail_size, 'cross-prologue'))

    return new_funcs


_REGS = ['zero','at','v0','v1','a0','a1','a2','a3',
         't0','t1','t2','t3','t4','t5','t6','t7',
         's0','s1','s2','s3','s4','s5','s6','s7',
         't8','t9','k0','k1','gp','sp','fp','ra']

# SPECIAL (op=0) -- selected by `funct` field. Three-register R-type unless noted.
_SPECIAL_R = {
    0x20: 'add',  0x21: 'addu', 0x22: 'sub',  0x23: 'subu',
    0x24: 'and',  0x25: 'or',   0x26: 'xor',  0x27: 'nor',
    0x2A: 'slt',  0x2B: 'sltu',
}
# Shift instructions (rd, rt, sa)
_SPECIAL_SHIFT = {0x00: 'sll', 0x02: 'srl', 0x03: 'sra'}
# Variable-shift (rd, rt, rs)
_SPECIAL_VSHIFT = {0x04: 'sllv', 0x06: 'srlv', 0x07: 'srav'}
# Multiply / divide (rs, rt; result in HI/LO)
_SPECIAL_MULDIV = {0x18: 'mult', 0x19: 'multu', 0x1A: 'div', 0x1B: 'divu'}
# HI/LO move
_SPECIAL_HILO_TO = {0x10: 'mfhi', 0x12: 'mflo'}        # rd
_SPECIAL_HILO_FROM = {0x11: 'mthi', 0x13: 'mtlo'}      # rs

# REGIMM (op=1) -- selected by `rt` field
_REGIMM = {0x00: 'bltz', 0x01: 'bgez', 0x10: 'bltzal', 0x11: 'bgezal'}

# Direct opcodes -- I-type (rt, rs, imm)
_OP_IMM = {
    0x08: 'addi', 0x09: 'addiu',
    0x0A: 'slti', 0x0B: 'sltiu',
    0x0C: 'andi', 0x0D: 'ori', 0x0E: 'xori',
}
# Memory I-type (rt, simm(rs))
_OP_MEM = {
    0x20: 'lb',  0x21: 'lh',  0x22: 'lwl', 0x23: 'lw',
    0x24: 'lbu', 0x25: 'lhu', 0x26: 'lwr',
    0x28: 'sb',  0x29: 'sh',  0x2A: 'swl', 0x2B: 'sw',  0x2E: 'swr',
}
# Coprocessor load/store (cop_reg, simm(rs))
_OP_COP_MEM = {
    0x30: 'lwc0', 0x31: 'lwc1', 0x32: 'lwc2', 0x33: 'lwc3',
    0x38: 'swc0', 0x39: 'swc1', 0x3A: 'swc2', 0x3B: 'swc3',
}


def _fmt_insn(insn, iaddr):
    """Format a single 32-bit MIPS-I instruction. Falls back to .word for
    encodings that no current PSX game-code path emits (CP0 control,
    coprocessor branches, MIPS-II+ extensions)."""
    if insn == 0:
        return '    nop'

    op = (insn >> 26) & 0x3F
    rs = (insn >> 21) & 0x1F
    rt = (insn >> 16) & 0x1F
    rd = (insn >> 11) & 0x1F
    sa = (insn >> 6) & 0x1F
    imm = insn & 0xFFFF
    simm = imm - 0x10000 if imm >= 0x8000 else imm
    funct = insn & 0x3F
    R = _REGS

    # ---- SPECIAL (op = 0) -- R-type / shift / mult-div / jr / jalr ----
    if op == 0x00:
        if funct in _SPECIAL_R:
            return f'    {_SPECIAL_R[funct]:<7s} ${R[rd]}, ${R[rs]}, ${R[rt]}'
        if funct in _SPECIAL_SHIFT:
            return f'    {_SPECIAL_SHIFT[funct]:<7s} ${R[rd]}, ${R[rt]}, {sa}'
        if funct in _SPECIAL_VSHIFT:
            return f'    {_SPECIAL_VSHIFT[funct]:<7s} ${R[rd]}, ${R[rt]}, ${R[rs]}'
        if funct in _SPECIAL_MULDIV:
            return f'    {_SPECIAL_MULDIV[funct]:<7s} ${R[rs]}, ${R[rt]}'
        if funct in _SPECIAL_HILO_TO:
            return f'    {_SPECIAL_HILO_TO[funct]:<7s} ${R[rd]}'
        if funct in _SPECIAL_HILO_FROM:
            return f'    {_SPECIAL_HILO_FROM[funct]:<7s} ${R[rs]}'
        if funct == 0x08:                        # JR rs
            return f'    jr      ${R[rs]}'
        if funct == 0x09:                        # JALR rd, rs (rd defaults to $ra)
            if rd == 31:
                return f'    jalr    ${R[rs]}'
            return f'    jalr    ${R[rd]}, ${R[rs]}'
        if funct == 0x0C:
            return f'    syscall 0x{(insn >> 6) & 0xFFFFF:X}'
        if funct == 0x0D:
            return f'    break   0x{(insn >> 6) & 0xFFFFF:X}'
        if funct == 0x34:                        # TEQ rs, rt
            return f'    teq     ${R[rs]}, ${R[rt]}'
        return f'    .word   0x{insn:08X}'       # unhandled SPECIAL funct

    # ---- REGIMM (op = 1) -- selected by rt ----
    if op == 0x01:
        if rt in _REGIMM:
            target = iaddr + 4 + simm * 4
            return f'    {_REGIMM[rt]:<7s} ${R[rs]}, 0x{target:08X}'
        return f'    .word   0x{insn:08X}'

    # ---- J / JAL ----
    if op == 0x02:
        target = (iaddr & 0xF0000000) | ((insn & 0x3FFFFFF) << 2)
        return f'    j       0x{target:08X}'
    if op == 0x03:
        target = (iaddr & 0xF0000000) | ((insn & 0x3FFFFFF) << 2)
        return f'    jal     0x{target:08X}'

    # ---- BEQ / BNE / BLEZ / BGTZ ----
    if op == 0x04:
        target = iaddr + 4 + simm * 4
        return f'    beq     ${R[rs]}, ${R[rt]}, 0x{target:08X}'
    if op == 0x05:
        target = iaddr + 4 + simm * 4
        return f'    bne     ${R[rs]}, ${R[rt]}, 0x{target:08X}'
    if op == 0x06:
        target = iaddr + 4 + simm * 4
        return f'    blez    ${R[rs]}, 0x{target:08X}'
    if op == 0x07:
        target = iaddr + 4 + simm * 4
        return f'    bgtz    ${R[rs]}, 0x{target:08X}'

    # ---- I-type immediate ALU ----
    if op in _OP_IMM:
        mn = _OP_IMM[op]
        # Logical immediates render as hex; arithmetic as decimal.
        if op in (0x0C, 0x0D, 0x0E):
            return f'    {mn:<7s} ${R[rt]}, ${R[rs]}, 0x{imm:04X}'
        return f'    {mn:<7s} ${R[rt]}, ${R[rs]}, {simm}'

    # ---- LUI ----
    if op == 0x0F:
        return f'    lui     ${R[rt]}, 0x{imm:04X}'

    # ---- COP0/COP1/COP2 instructions (op 0x10/0x11/0x12) ----
    # Game code on PS1 uses GTE (COP2) heavily. Render as a generic cop2
    # form; full GTE mnemonic decoding isn't required for our matching work.
    if op == 0x12:                                # COP2 (GTE)
        return f'    .word   0x{insn:08X}      # cop2/GTE'
    if op in (0x10, 0x11):                        # COP0 / COP1 (no FPU on R3000A)
        return f'    .word   0x{insn:08X}      # cop{op - 0x10}'

    # ---- Memory I-type ----
    if op in _OP_MEM:
        return f'    {_OP_MEM[op]:<7s} ${R[rt]}, {simm}(${R[rs]})'

    # ---- Coprocessor load/store ----
    if op in _OP_COP_MEM:
        # rt is the coprocessor register; we render as $cN_rt for clarity.
        cop = (op >> 1) & 0x3 if op >= 0x30 else 0
        return f'    {_OP_COP_MEM[op]:<7s} $c{cop}_{rt}, {simm}(${R[rs]})'

    return f'    .word   0x{insn:08X}'


def disassemble_function(data, offset, size, base_addr):
    """MIPS-I disassembly of a function body. See `_fmt_insn` for opcode
    coverage. Anything that survives as `.word` is genuinely outside the
    standard PSX game-code instruction set."""
    lines = []
    addr = base_addr + offset
    for i in range(0, size, 4):
        if offset + i + 4 > len(data):
            break
        insn = struct.unpack_from('<I', data, offset + i)[0]
        lines.append(_fmt_insn(insn, addr + i))
    return lines


def extract_overlay(exn_path, output_dir):
    """Extract all functions from an EXN overlay."""
    with open(exn_path, 'rb') as f:
        data = f.read()

    overlay_name = Path(exn_path).stem
    os.makedirs(output_dir, exist_ok=True)

    # Find data boundary (exclude trailing zeros + checksum)
    data_end = len(data) - 4
    while data_end > 0 and struct.unpack_from('<I', data, data_end - 4)[0] == 0:
        data_end -= 4

    print(f'{overlay_name}: {len(data)} bytes, code+data to 0x{data_end:X}')

    # Find functions (prologue-based)
    functions = find_functions(data, data_end)
    print(f'  Found {len(functions)} prologue-bearing functions')

    # Second pass: gap functions (leaf/init + cross-prologue alt-entries)
    gap_functions = find_gap_functions(data, functions, data_end)
    leaf_count = sum(1 for _,_,k in gap_functions if k == 'leaf')
    cross_count = sum(1 for _,_,k in gap_functions if k == 'cross-prologue')
    print(f'  Found {leaf_count} prologue-less leaf/init functions')
    print(f'  Found {cross_count} cross-prologue alt-entries')

    # Merge: promote gap functions into the unified list with kind tag
    all_funcs = [(off, size, 'prologue') for off, size in functions]
    all_funcs.extend(gap_functions)
    all_funcs.sort(key=lambda x: x[0])

    # Write each function as .s file
    for idx, (offset, size, kind) in enumerate(all_funcs):
        addr = LOAD_ADDR + offset
        func_name = f'func_{addr:08X}'

        kind_comment = {
            'prologue': 'standard function (addiu sp prologue + jr ra)',
            'leaf': 'prologue-less leaf/init routine (no stack frame)',
            'cross-prologue': 'cross-prologue alt-entry (loads hoisted above addiu sp)',
        }[kind]

        asm_lines = [
            f'.set noreorder',
            f'.set noat',
            f'',
            f'.text',
            f'.globl {func_name}',
            f'',
            f'/* Function: {func_name}',
            f' * Overlay: {overlay_name}',
            f' * Address: 0x{addr:08X}',
            f' * Size: {size} bytes ({size // 4} instructions)',
            f' * Kind: {kind} — {kind_comment}',
            f' */',
            f'{func_name}:',
        ]
        asm_lines.extend(disassemble_function(data, offset, size, LOAD_ADDR))

        out_path = os.path.join(output_dir, f'{func_name}.s')
        with open(out_path, 'w') as f:
            f.write('\n'.join(asm_lines) + '\n')

    # Write summary
    summary_path = os.path.join(output_dir, 'SUMMARY.md')
    with open(summary_path, 'w') as f:
        f.write(f'# {overlay_name} Overlay Functions\n\n')
        f.write(f'Total: {len(all_funcs)} functions, {data_end} bytes\n')
        f.write(f'(prologue: {len(functions)}, leaf: {leaf_count}, cross-prologue: {cross_count})\n\n')
        f.write(f'| # | Address | Size | Instructions | Kind |\n')
        f.write(f'|---|---------|------|--------------|------|\n')
        for idx, (offset, size, kind) in enumerate(all_funcs):
            addr = LOAD_ADDR + offset
            f.write(f'| {idx} | 0x{addr:08X} | {size} | {size//4} | {kind} |\n')

    print(f'  Written to {output_dir}/')
    return len(all_funcs)


def main():
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <EXN_FILE> <OUTPUT_DIR>")
        print(f"       {sys.argv[0]} --all <OVERLAY_DIR> <OUTPUT_BASE>")
        sys.exit(1)

    if sys.argv[1] == '--all':
        overlay_dir = sys.argv[2]
        output_base = sys.argv[3]
        total = 0
        for exn in sorted(os.listdir(overlay_dir)):
            if not exn.endswith('.EXN'):
                continue
            name = Path(exn).stem
            count = extract_overlay(
                os.path.join(overlay_dir, exn),
                os.path.join(output_base, name))
            total += count
        print(f'\nTotal: {total} functions extracted')
    else:
        extract_overlay(sys.argv[1], sys.argv[2])


if __name__ == '__main__':
    main()
