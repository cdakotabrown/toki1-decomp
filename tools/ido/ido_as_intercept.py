#!/usr/bin/env python3
"""IDO-native assembly interceptor.

Like asm_intercept.py but uses IDO's own assembler (as0 5.3 → as1 5.2)
instead of GNU as. This preserves IDO-native delay slot filling and
instruction scheduling while allowing text-level transforms on the .s output.

Pipeline:
1. Compile C with IDO 5.2 -S → .s text
2. Apply Python transform to .s text
3. Assemble with as0 (IDO 5.3) → binary intermediate .B
4. Assemble with as1 (IDO 5.2) → .o with IDO-native scheduling

Advantages over asm_intercept.py (which uses GNU as):
- IDO-native delay slot filling
- IDO-native load hazard scheduling
- IDO-native branch optimization
- Correct pipeline interlocking for MIPS R3000

Usage:
    python3 ido_as_intercept.py <source.c> <output.o> <transform.py> [--compile-flags ...]
"""

import argparse
import importlib.util
import os
import subprocess
import sys
import tempfile


def find_tools():
    """Find IDO compiler and assembler tools."""
    root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    cc = os.path.join(root, "tools", "ido-static-recomp", "build", "5.2", "out", "cc")
    as0 = os.path.join(root, "tools", "ido-static-recomp", "build", "5.3", "out", "as0")
    as1 = os.path.join(root, "tools", "ido-static-recomp", "build", "5.2", "out", "as1")

    for tool, path in [("cc", cc), ("as0", as0), ("as1", as1)]:
        if not os.path.exists(path):
            print(f"ERROR: {tool} not found at {path}", file=sys.stderr)
            sys.exit(1)

    return cc, as0, as1, root


def compile_to_asm(cc, source_c, compile_flags, include_dir):
    """Compile C to assembly text with IDO 5.2 -S."""
    with tempfile.TemporaryDirectory() as tmpdir:
        # IDO -S outputs to CWD with source basename
        base = os.path.splitext(os.path.basename(source_c))[0]
        asm_path = os.path.join(tmpdir, base + '.s')

        cmd = [cc] + compile_flags + ['-S', '-I', include_dir, source_c]
        result = subprocess.run(cmd, capture_output=True, text=True, cwd=tmpdir,
                              env=dict(os.environ))
        if result.returncode != 0:
            print(f"ERROR: IDO compilation failed:\n{result.stderr}", file=sys.stderr)
            sys.exit(1)

        with open(asm_path, 'r') as f:
            return f.read()


def assemble_ido_native(as0, as1, asm_text, output_o):
    """Assemble with as0 (5.3) → as1 (5.2) for IDO-native scheduling."""
    with tempfile.TemporaryDirectory() as tmpdir:
        asm_path = os.path.join(tmpdir, 'input.s')
        bin_path = os.path.join(tmpdir, 'input.B')

        with open(asm_path, 'w') as f:
            f.write(asm_text)

        # Step 1: as0 converts text .s to binary intermediate
        cmd_as0 = [as0, '-mips1', '-EL', asm_path, '-o', bin_path]
        result = subprocess.run(cmd_as0, capture_output=True, text=True)
        if result.returncode != 0:
            print(f"ERROR: as0 failed:\n{result.stderr}", file=sys.stderr)
            sys.exit(1)

        # Step 2: as1 assembles binary intermediate to .o
        cmd_as1 = [as1, '-mips1', '-EL', '-o', output_o, bin_path]
        result = subprocess.run(cmd_as1, capture_output=True, text=True)
        if result.returncode != 0:
            print(f"ERROR: as1 failed:\n{result.stderr}", file=sys.stderr)
            sys.exit(1)


def load_transform(transform_path):
    """Load transform module from path."""
    spec = importlib.util.spec_from_file_location("transform", transform_path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module.transform


def main():
    parser = argparse.ArgumentParser(description='IDO-native assembly interceptor')
    parser.add_argument('source', help='C source file')
    parser.add_argument('output', help='Output .o file')
    parser.add_argument('transform', help='Python transform script (must define transform(lines) -> lines)')
    parser.add_argument('--compile-flags', nargs='*', default=['-O2', '-G0', '-mips1', '-EL', '-non_shared'],
                       help='IDO compile flags')
    args = parser.parse_args()

    cc, as0, as1, root = find_tools()
    include_dir = os.path.join(root, 'include')

    # Step 1: Compile to assembly
    asm_text = compile_to_asm(cc, args.source, args.compile_flags, include_dir)

    # Step 2: Apply transform
    transform_fn = load_transform(args.transform)
    lines = asm_text.split('\n')
    transformed_lines = transform_fn(lines)
    transformed_asm = '\n'.join(transformed_lines)

    # Step 3: Assemble with IDO-native as0→as1
    assemble_ido_native(as0, as1, transformed_asm, args.output)

    print(f"[ido_as_intercept] Success: {args.source} → {args.output}")


if __name__ == '__main__':
    main()
