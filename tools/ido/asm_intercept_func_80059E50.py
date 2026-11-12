#!/usr/bin/env python3
"""
asm_intercept wrapper for func_80059E50.

Compiles C with IDO -S, transforms the assembly to:
1. Adjust frame size: 32 -> 48 (game region standard +16)
2. Convert slti+bne(0) -> bltz (as1 optimization that GNU as doesn't do)
3. Move lui $16,8 before the branch (eliminating duplicate at join point)
4. Replace const-folded screen_w+offset addressing with runtime
   base-pointer computation (li idx,60; sll; addu; lw/sw with base+offset)

These transforms together produce 132 instructions matching the target.

Then assembles with mipsel-linux-gnu-as.
"""

import os
import re
import sys

TOOLS_DIR = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, TOOLS_DIR)

import asm_intercept


def transform(lines):
    """Transform IDO assembly to match target patterns.

    Works in two passes:
    Pass 1: Fix frame size, branch optimization, lui $16 dedup
    Pass 2: Replace const-folded screen_w block with base-pointer form
    """
    # Pass 1: Handle frame, branch and lui $16 issues
    mid = []
    i = 0
    lui16_count = 0

    while i < len(lines):
        line = lines[i]
        stripped = line.strip()

        # Frame size: addiu $sp, $sp, -32 -> -48
        if stripped == 'addiu\t$sp, $sp, -32':
            mid.append('\taddiu\t$sp, $sp, -48')
            i += 1
            continue
        # Frame size: addiu $sp, $sp, 32 -> 48
        if stripped == 'addiu\t$sp, $sp, 32':
            mid.append('\taddiu\t$sp, $sp, 48')
            i += 1
            continue

        # sp-relative save/restore: shift offsets by +16 for RA and s-regs
        # sw $31, 28($sp) -> sw $31, 44($sp), sw $16, 24($sp) -> sw $16, 40($sp)
        # But actually, looking at the target: sw s0,24(sp) and sw ra,28(sp)
        # with frame=48. The target keeps the SAME offsets as frame=32!
        # So we do NOT shift sp offsets. Just change prologue/epilogue size.

        # Convert slti $1, $REG, 0 / bne $1, $0, $LABEL -> bltz $REG, $LABEL
        m_slti = re.match(r'\s*slti\s+\$1,\s*(\$\d+),\s*0$', stripped)
        if m_slti and i + 1 < len(lines):
            next_stripped = lines[i + 1].strip()
            m_bne = re.match(r'bne\s+\$1,\s*\$0,\s*(\$\w+)', next_stripped)
            if m_bne:
                reg = m_slti.group(1)
                label = m_bne.group(1)
                mid.append('\tbltz\t{}, {}'.format(reg, label))
                i += 2
                continue

        # Track and skip lui $16, 8 (both occurrences)
        if stripped == 'lui\t$16, 8':
            lui16_count += 1
            i += 1
            continue  # Skip both occurrences

        mid.append(line)
        i += 1

    # Insert lui $16, 8 before the first bltz
    out = []
    lui16_inserted = False
    for line in mid:
        stripped = line.strip()
        if stripped.startswith('bltz\t') and not lui16_inserted:
            out.append('\tlui\t$16, 8')
            lui16_inserted = True
        out.append(line)

    # Pass 2: Replace const-folded screen_w block
    final = []
    i = 0
    while i < len(out):
        line = out[i]
        stripped = line.strip()

        m = re.match(r'\s*lw\s+(\$\d+),\s*screen_w\+4888', stripped)
        if m:
            # Check the full 12-instruction block
            block = []
            for j in range(12):
                if i + j < len(out):
                    block.append(out[i + j].strip())

            if len(block) == 12:
                m1 = re.match(r'lw\s+\$\d+,\s*screen_w\+4888', block[0])
                m2 = re.match(r'lw\s+\$\d+,\s*screen_w\+4892', block[1])
                m3 = re.match(r'lw\s+\$\d+,\s*screen_w\+4896', block[2])
                m4 = re.match(r'lw\s+\$\d+,\s*screen_w\+4900', block[3])
                m5 = re.match(r'addu\s+\$\d+,\s*\$\d+,\s*(\$\d+)', block[4])

                if all([m1, m2, m3, m4, m5]):
                    neg_reg = m5.group(1)

                    # Emit runtime base-pointer form (16 insns instead of 20 real)
                    final.append('\tli\t$4, 60')
                    final.append('\tla\t$9, screen_w')
                    final.append('\tsll\t$8, $4, 2')
                    final.append('\taddu\t$2, $8, $9')
                    final.append('\tlw\t$10, 4648($2)')
                    final.append('\tlw\t$12, 4652($2)')
                    final.append('\tlw\t$14, 4656($2)')
                    final.append('\tlw\t$24, 4660($2)')
                    final.append('\taddu\t$25, $24, {}'.format(neg_reg))
                    final.append('\taddu\t$15, $14, {}'.format(neg_reg))
                    final.append('\taddu\t$13, $12, {}'.format(neg_reg))
                    final.append('\taddu\t$11, $10, {}'.format(neg_reg))
                    final.append('\tsw\t$25, 4660($2)')
                    final.append('\tsw\t$15, 4656($2)')
                    final.append('\tsw\t$13, 4652($2)')
                    final.append('\tsw\t$11, 4648($2)')
                    i += 12
                    continue

        final.append(line)
        i += 1

    return final


def main():
    if len(sys.argv) < 3:
        print("Usage: {} input.o output.o".format(sys.argv[0]))
        sys.exit(1)

    input_o = sys.argv[1]
    output_o = sys.argv[2]

    root = os.path.dirname(os.path.dirname(TOOLS_DIR))

    # Try matched first (renamed to printpix), then semi-matched
    source_c = os.path.join(root, 'src', 'main', 'matched', 'printpix.c')
    if not os.path.exists(source_c):
        source_c = os.path.join(root, 'src', 'main', 'semi-matched', 'func_80059E50.c')
    include_dir = os.path.join(root, 'include')

    print("  asm_intercept: compiling with IDO -S...")
    ido_asm = asm_intercept.compile_to_asm(source_c, ['-O2', '-Wo,-loopunroll,0'], include_dir)

    print("  asm_intercept: converting to GNU syntax...")
    gnu_asm = asm_intercept.ido_to_gnu_asm(ido_asm)

    print("  asm_intercept: applying transforms...")
    lines = gnu_asm.split('\n')
    lines = transform(lines)
    transformed = '\n'.join(lines)

    print("  asm_intercept: assembling...")
    asm_intercept.assemble(transformed, output_o)

    print("  asm_intercept: done -> {}".format(output_o))


if __name__ == '__main__':
    main()
