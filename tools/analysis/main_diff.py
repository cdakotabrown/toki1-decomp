#!/usr/bin/env python3
"""Harness-accurate target-vs-compiled diff for one MAIN-binary function.

Compiles the C exactly as test_single_match.py does (same header directives,
@CompileFlags, @CompileEnv and full PostProc chain) and prints the retail
words next to the compiled words.

    python3 tools/analysis/main_diff.py FUNC 0xADDR NWORDS path/to/file.c [--all]

Default output lists only RELOC-MASKED mismatches: on lui/jal and on
I-type memory/addiu words only opcode+register fields are compared, the
same over-masking the harness applies, so what is printed is real codegen
difference.  --all prints every word (unmasked) with a marker column.
Unlike dump_main_diff.py this handles GCC files and every directive the
build understands.  (2026-09-04; grew out of the scratchpad hdiff/mdiff.)
"""
import sys, re, struct, subprocess, os, tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, 'tools/build'))
sys.path.insert(0, os.path.join(ROOT, 'tools/analysis'))


def main():
    args = [a for a in sys.argv[1:] if not a.startswith('--')]
    show_all = '--all' in sys.argv
    if len(args) != 4:
        print(__doc__); sys.exit(2)
    fn, addr, n, src = args[0], int(args[1], 16), int(args[2]), args[3]
    import build_level3_c as b, test_single_match as t
    os.chdir(ROOT)
    pp = t.parse_postproc_header(src); fl = t.parse_compile_flags(src); comp = t.parse_compiler(src)
    os.environ.update(t.parse_compile_env(src) or {})
    r = b.compile_c_function(src, fn, addr, postproc_steps=pp, extra_flags=fl, compiler=comp)
    data = r[0] if isinstance(r, tuple) else r
    cw = [struct.unpack_from('<I', data, i)[0] for i in range(0, len(data), 4)]
    img = open(os.path.join(ROOT, 'disks/SLPS_000.65'), 'rb').read()
    off = addr - 0x80041000 + 0x800
    tw = [struct.unpack_from('<I', img, off + i * 4)[0] for i in range(n)]

    def dis(ws, base):
        with tempfile.NamedTemporaryFile(suffix='.bin', delete=False) as f:
            f.write(b''.join(struct.pack('<I', w) for w in ws)); path = f.name
        try:
            o = subprocess.run(['mipsel-linux-gnu-objdump', '-D', '-z', '-b', 'binary', '-m', 'mips:3000', '-EL',
                                '--adjust-vma=%#x' % base, path], capture_output=True, text=True).stdout
        finally:
            os.unlink(path)
        return [l.split('\t', 1)[1].strip() for l in o.splitlines() if re.match(r'^\s*[0-9a-f]+:', l)]

    def mask(w):
        op = w >> 26
        if op == 3:
            return w & 0xfc000000
        if op in (0xf, 0x9, 0x20, 0x21, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2b):
            return w & 0xffff0000
        return w

    td, cd = dis(tw, addr), dis(cw, addr)
    bad = [i for i in range(min(len(tw), len(cw))) if mask(tw[i]) != mask(cw[i])]
    print(f'target={len(tw)} compiled={len(cw)} masked-mismatch={len(bad)}: {bad}')
    rows = range(max(len(td), len(cd))) if show_all else bad
    for i in rows:
        a = td[i] if i < len(td) else ''; c = cd[i] if i < len(cd) else ''
        m = '' if (i < len(tw) and i < len(cw) and tw[i] == cw[i]) else '  <<'
        print(f'{i:3d} 0x{i*4:03x} {a:40s} | {c}{m if show_all else ""}')


if __name__ == '__main__':
    main()
