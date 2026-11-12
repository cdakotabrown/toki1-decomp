#!/usr/bin/env python3
"""Assembly intercept post-processor.

Compiles C with IDO -S to produce assembly text, applies a transform script
(register renames, instruction swaps, line edits) to the .s file, then
assembles the result with mipsel-linux-gnu-as.

This is functionally equivalent to custom_script binary patching, but operates
on human-readable assembly text. Advantages:
- Relocations handled automatically by the assembler
- Transforms are reviewable as text diffs
- No manual branch offset tracking

Best suited for cases where IDO produces the right structure but wrong
register names globally (simple find/replace). For complex per-offset
register remapping or cross-prologue functions, binary custom_scripts
remain more precise.

Usage as a custom_script:
    @PostProc: custom_script asm_intercept_FUNCNAME.py

The wrapper script calls this module with the appropriate source and transform.

Standalone usage:
    python3 asm_intercept.py <source.c> <output.o> <transform.py> [--compile-flags ...]

The transform script must define a function:
    def transform(lines: list[str]) -> list[str]
that takes assembly lines and returns modified lines.
"""

import argparse
import importlib.util
import os
import subprocess
import sys
import tempfile


def find_ido_cc():
    """Find the IDO compiler."""
    root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    cc = os.path.join(root, "tools", "ido-static-recomp", "build", "5.2", "out", "cc")
    if not os.path.exists(cc):
        print(f"ERROR: IDO compiler not found at {cc}", file=sys.stderr)
        sys.exit(1)
    return cc


def compile_to_asm(source_c, compile_flags, include_dir):
    """Compile C source to assembly with IDO -S."""
    cc = find_ido_cc()
    basename = os.path.splitext(os.path.basename(source_c))[0]

    cmd = [cc] + compile_flags + [
        '-G0', '-mips1', '-EL', '-non_shared',
        '-S', '-I', include_dir,
        source_c
    ]

    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"ERROR: IDO compilation failed:\n{result.stderr}", file=sys.stderr)
        sys.exit(1)

    ido_output = basename + '.s'
    if not os.path.exists(ido_output):
        print(f"ERROR: Expected .s file not found: {ido_output}", file=sys.stderr)
        sys.exit(1)

    with open(ido_output, 'r') as f:
        asm_text = f.read()

    os.unlink(ido_output)
    return asm_text


def ido_to_gnu_asm(ido_asm):
    """Convert IDO assembly syntax to GNU as compatible syntax.

    Strips IDO-specific directives, converts pseudo-instructions.
    Keeps IDO register numbering ($N) since GNU as accepts it.
    """
    lines = ido_asm.split('\n')
    out = []
    out.append('\t.set noreorder')
    out.append('\t.text')
    out.append('\t.align 2')

    in_func = False

    for line in lines:
        stripped = line.strip()

        # Skip IDO-specific directives
        if stripped.startswith(('.verstamp', '.option', '.file', '.loc ',
                                '.loc\t', '.livereg', '.mask', '.frame',
                                '.fmask', '.extern', '.text', '.align',
                                '.noalias', '.alias')):
            continue
        if stripped.startswith('#'):
            continue

        if stripped.startswith('.globl'):
            out.append(line)
            continue
        if stripped.startswith('.ent'):
            parts = stripped.split()
            out.append(f'\t.ent {parts[1]}')
            in_func = True
            continue
        if stripped.startswith('.end'):
            out.append(line)
            in_func = False
            continue

        if stripped.endswith(':'):
            out.append(line)
            continue

        if in_func and stripped:
            import re

            # subu $sp, N -> addiu $sp, $sp, -N
            if stripped.startswith('subu\t$sp,'):
                n = stripped.split(',')[-1].strip()
                out.append(f'\taddiu\t$sp, $sp, -{n}')
                continue

            # addu $sp, N -> addiu $sp, $sp, N
            if stripped.startswith('addu\t$sp,'):
                parts = stripped.split(',')
                if len(parts) == 2:
                    n = parts[-1].strip()
                    out.append(f'\taddiu\t$sp, $sp, {n}')
                    continue

            # j $31 -> jr $ra
            if stripped == 'j\t$31':
                out.append('\tjr\t$ra')
                continue

            # move $X, $Y -> addu $X, $Y, $0
            m = re.match(r'move\t(\$\d+),\s*(\$\d+)', stripped)
            if m:
                out.append(f'\taddu\t{m.group(1)}, {m.group(2)}, $0')
                continue

            # negu $X, $Y -> subu $X, $0, $Y
            m = re.match(r'negu\t(\$\d+),\s*(\$\d+)', stripped)
            if m:
                out.append(f'\tsubu\t{m.group(1)}, $0, {m.group(2)}')
                continue

            # blt $rs, imm, label -> slti $at, $rs, imm; bne $at, $0, label
            m = re.match(r'blt\t(\$\d+),\s*(-?\d+),\s*(\$?\w+)', stripped)
            if m:
                out.append(f'\tslti\t$1, {m.group(1)}, {m.group(2)}')
                out.append(f'\tbne\t$1, $0, {m.group(3)}')
                continue

            # bge $rs, imm, label -> slti $at, $rs, imm; beq $at, $0, label
            m = re.match(r'bge\t(\$\d+),\s*(-?\d+),\s*(\$?\w+)', stripped)
            if m:
                out.append(f'\tslti\t$1, {m.group(1)}, {m.group(2)}')
                out.append(f'\tbeq\t$1, $0, {m.group(3)}')
                continue

            # bltu $rs, $rt, label -> sltu $at, $rs, $rt; bne $at, $0, label
            m = re.match(r'bltu\t(\$\d+),\s*(\$\d+),\s*(\$?\w+)', stripped)
            if m:
                out.append(f'\tsltu\t$1, {m.group(1)}, {m.group(2)}')
                out.append(f'\tbne\t$1, $0, {m.group(3)}')
                continue

            # bltu $rs, imm, label -> sltiu $at, $rs, imm; bne $at, $0, label
            m = re.match(r'bltu\t(\$\d+),\s*(\d+),\s*(\$?\w+)', stripped)
            if m:
                out.append(f'\tsltiu\t$1, {m.group(1)}, {m.group(2)}')
                out.append(f'\tbne\t$1, $0, {m.group(3)}')
                continue

            # bgeu $rs, imm, label -> sltiu $at, $rs, imm; beq $at, $0, label
            m = re.match(r'bgeu\t(\$\d+),\s*(\d+),\s*(\$?\w+)', stripped)
            if m:
                out.append(f'\tsltiu\t$1, {m.group(1)}, {m.group(2)}')
                out.append(f'\tbeq\t$1, $0, {m.group(3)}')
                continue

            # mul $rd, $rs, $rt -> multu $rs, $rt; mflo $rd
            m = re.match(r'mul\t(\$\d+),\s*(\$\d+),\s*(\$\d+)', stripped)
            if m:
                out.append(f'\tmultu\t{m.group(2)}, {m.group(3)}')
                out.append(f'\tmflo\t{m.group(1)}')
                continue

            # div $rd, $rs, $rt -> div $0, $rs, $rt; mflo $rd
            m = re.match(r'div\t(\$\d+),\s*(\$\d+),\s*(\$\d+)', stripped)
            if m and m.group(1) != '$0':
                out.append(f'\tdiv\t$0, {m.group(2)}, {m.group(3)}')
                out.append(f'\tmflo\t{m.group(1)}')
                continue

            # li $rd, large_imm -> lui $rd, hi; ori $rd, $rd, lo
            m = re.match(r'li\t(\$\d+),\s*(-?\d+)', stripped)
            if m:
                val = int(m.group(2))
                val &= 0xFFFFFFFF
                if val <= 0x7FFF or (val >= 0xFFFF8000):
                    # Small immediate - addiu
                    simm = val if val < 0x8000 else val - 0x100000000
                    out.append(f'\taddiu\t{m.group(1)}, $0, {simm}')
                elif (val & 0xFFFF) == 0:
                    out.append(f'\tlui\t{m.group(1)}, {(val >> 16) & 0xFFFF}')
                else:
                    hi = (val >> 16) & 0xFFFF
                    lo = val & 0xFFFF
                    out.append(f'\tlui\t{m.group(1)}, {hi}')
                    out.append(f'\tori\t{m.group(1)}, {m.group(1)}, {lo}')
                continue

            out.append(line)
            continue

        if stripped:
            out.append(line)

    return '\n'.join(out) + '\n'


def assemble(asm_text, output_o):
    """Assemble with mipsel-linux-gnu-as."""
    with tempfile.NamedTemporaryFile(suffix='.s', delete=False, mode='w') as tf:
        tf.write(asm_text)
        asm_path = tf.name

    try:
        cmd = ['mipsel-linux-gnu-as', '-mips1', '-EL', '-o', output_o, asm_path]
        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.returncode != 0:
            print(f"ERROR: Assembly failed:\n{result.stderr}", file=sys.stderr)
            lines = asm_text.split('\n')
            for i, line in enumerate(lines, 1):
                print(f"{i:4d}: {line}", file=sys.stderr)
            sys.exit(1)
    finally:
        os.unlink(asm_path)


def load_transform(transform_path):
    """Load a transform script and return its transform() function."""
    spec = importlib.util.spec_from_file_location("transform_module", transform_path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    if not hasattr(module, 'transform'):
        print(f"ERROR: Transform script must define transform(lines) function",
              file=sys.stderr)
        sys.exit(1)
    return module.transform


def main():
    parser = argparse.ArgumentParser(description='Assembly intercept post-processor')
    parser.add_argument('source', help='C source file')
    parser.add_argument('output', help='Output .o file')
    parser.add_argument('transform', help='Transform script (.py with transform(lines) function)')
    parser.add_argument('--compile-flags', nargs='*', default=['-O2'],
                        help='IDO compile flags (default: -O2)')
    parser.add_argument('--include-dir', default=None,
                        help='Include directory (default: auto-detect)')
    parser.add_argument('--verbose', '-v', action='store_true')
    args = parser.parse_args()

    if args.include_dir is None:
        root = os.path.dirname(os.path.dirname(os.path.dirname(
            os.path.abspath(__file__))))
        args.include_dir = os.path.join(root, 'include')

    if args.verbose:
        print(f"Compiling {args.source} with IDO -S ...")
    ido_asm = compile_to_asm(args.source, args.compile_flags, args.include_dir)

    if args.verbose:
        print("Converting IDO syntax to GNU as ...")
    gnu_asm = ido_to_gnu_asm(ido_asm)

    if args.verbose:
        print(f"Applying transform {args.transform} ...")
    transform_fn = load_transform(args.transform)
    lines = gnu_asm.split('\n')
    transformed_lines = transform_fn(lines)
    transformed_asm = '\n'.join(transformed_lines)

    if args.verbose:
        print("=== Transformed assembly ===")
        for i, line in enumerate(transformed_lines, 1):
            print(f"  {i:3d}: {line}")

    if args.verbose:
        print(f"Assembling to {args.output} ...")
    assemble(transformed_asm, args.output)

    if args.verbose:
        print("Done.")


if __name__ == '__main__':
    main()
