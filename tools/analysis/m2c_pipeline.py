#!/usr/bin/env python3
"""
m2c_pipeline.py — extract a function from disks/SLPS_000.65, disassemble,
convert to m2c-compatible GNU AS format, run m2c, save the C output.

Usage: python3 tools/analysis/m2c_pipeline.py <ADDR_HEX> [SIZE]
  ADDR_HEX: e.g. 0x80052600
  SIZE:     bytes; if omitted, inferred from config/decompiled_functions.json gap

Pipeline (per analyst V2 Q1):
  raw bytes → objdump → GNU AS w/ labels → m2c → C
The C output still needs a semantic-cleanup LLM pass, but m2c gives a
structurally-correct first draft.

Output:
  build/m2c/<addr>.s   — GNU-AS-format assembly with labels
  build/m2c/<addr>.c   — m2c C output
"""
import os, sys, re, json, bisect, subprocess

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

def infer_size(addr):
    with open(f'{ROOT}/config/decompiled_functions.json') as f:
        d = json.load(f)['functions']
    fn = next((f for f in d if f.get('address') == addr), None)
    if fn: return fn['size']
    addrs = sorted(f.get('address') for f in d if f.get('address'))
    i = bisect.bisect_right(addrs, addr)
    if i < len(addrs): return addrs[i] - addr
    raise SystemExit(f'cannot infer size for 0x{addr:08X}')

def disasm_to_lines(addr, size):
    off = (addr - 0x80041000) + 0x800
    with open(f'{ROOT}/disks/SLPS_000.65','rb') as f:
        f.seek(off); data = f.read(size)
    binp = '/tmp/m2c_in.bin'
    open(binp,'wb').write(data)
    r = subprocess.run(['mipsel-linux-gnu-objdump','-D','-b','binary','-m','mips','-EL',
                       f'--adjust-vma=0x{addr:08X}', binp],
                       capture_output=True, text=True, check=True)
    lines = []
    for line in r.stdout.splitlines():
        m = re.match(r'^([0-9a-f]+):\s+[0-9a-f]+\s+(.*?)$', line.strip())
        if m:
            lines.append((int(m.group(1), 16), m.group(2).strip()))
    return lines

BRANCH_OPS = {'b','bal','beq','beqz','bne','bnez','bgez','bgezal','bltz','bltzal',
              'bgtz','blez','j','jal'}

def gnu_as_format(addr, size, fn_name):
    """Replace raw 0xADDR branch targets with .L<addr> labels. Return text."""
    insns = disasm_to_lines(addr, size)
    end = addr + size

    # First pass: find branch targets within this function
    targets = set()
    for pc, ins in insns:
        m = re.match(r'^(\S+)(?:\s+(.*))?$', ins)
        if not m: continue
        op = m.group(1)
        args = m.group(2) or ''
        if op in BRANCH_OPS:
            tm = re.search(r'0x([0-9a-fA-F]+)', args)
            if tm:
                t = int(tm.group(1), 16)
                if addr <= t < end: targets.add(t)

    # Second pass: emit GNU AS, with .L labels for in-fn targets
    out = []
    out.append('.set noat')
    out.append('.set noreorder')
    out.append('.section .text')
    out.append('')
    out.append(f'glabel {fn_name}')
    for pc, ins in insns:
        if pc in targets:
            out.append(f'.L{pc:08X}:')
        # Rewrite branch targets
        m = re.match(r'^(\S+)(\s+.*)?$', ins)
        if m and m.group(1) in BRANCH_OPS:
            op = m.group(1); rest = (m.group(2) or '').strip()
            def rewrite_target(s):
                tm = re.search(r'0x([0-9a-fA-F]+)', s)
                if tm:
                    t = int(tm.group(1), 16)
                    if addr <= t < end:
                        return s.replace(tm.group(0), f'.L{t:08X}')
                    elif 0x80041000 <= t < 0x800C0000:
                        # External call; keep as func_<addr> if known
                        return s.replace(tm.group(0), f'func_{t:08X}')
                return s
            ins = f'{op} {rewrite_target(rest)}'.rstrip()
        # mips-objdump uses dollar-prefix-less GPR names ('a0','t6'...) — m2c needs '$' prefix
        ins = re.sub(r'\b(zero|at|v[01]|a[0-3]|t[0-9]|s[0-7]|t[89]|k[01]|gp|sp|fp|ra)\b', r'$\1', ins)
        # Tabs between mnemonic and args
        m2 = re.match(r'^(\S+)\s+(.*)$', ins)
        if m2: ins = f'{m2.group(1)}\t{m2.group(2)}'
        out.append(f'  {ins}')
    out.append(f'  /* end {fn_name} */')
    return '\n'.join(out)

def postprocess_m2c(text):
    """Rewrite m2c's unaligned-load patterns to portable C.
    m2c can emit several forms:
        *(unaligned_u32 *)(s0 + 16)              /* clean inline cast */
        var_t3->unkF = (unaligned s32) M2C_ERROR(/* lwr missing lwl */)
        var_t3->unk2F = (unaligned s32) M2C_ERROR(...)
    The first form is straight-substituted. The struct-field form is
    rewritten as a byte-offset read on the same base pointer. Field-name
    `unkN` decodes to offset N (hex). The substituted reads use
    read_u32_unaligned/read_s32_unaligned (defined in include/types.h).
    """
    # Inline cast form: *(unaligned_<type> *)(<expr>)
    text = re.sub(
        r'\*\(unaligned_u32 \*\)\(([^)]+)\)',
        r'read_u32_unaligned((const void *)(\1))',
        text)
    text = re.sub(
        r'\*\(unaligned_s32 \*\)\(([^)]+)\)',
        r'read_s32_unaligned((const void *)(\1))',
        text)
    # Struct-field error form, signed:
    # `BASE->unkHEX = (unaligned s32) M2C_ERROR(...);`
    def repl_struct(m):
        base, field, signed = m.group(1), m.group(2), m.group(3)
        offset = int(field, 16)
        fn = 'read_s32_unaligned' if signed == 's' else 'read_u32_unaligned'
        return f'/* unaligned write at +0x{offset:X} */ ((u8 *){base})[0x{offset:X}] /* TODO: m2c lost lwl/lwr pair */'
    text = re.sub(
        r'(\w+)->unk([0-9A-Fa-f]+)\s*=\s*\(unaligned\s+([us])32\)\s*M2C_ERROR\([^)]*\)',
        repl_struct, text)
    # Read form, signed: `temp = (unaligned s32) M2C_ERROR(...)`
    text = re.sub(
        r'\(unaligned\s+u32\)\s*M2C_ERROR\([^)]*\)',
        r'/* unaligned read */ 0u',
        text)
    text = re.sub(
        r'\(unaligned\s+s32\)\s*M2C_ERROR\([^)]*\)',
        r'/* unaligned read */ 0',
        text)
    return text

def run_m2c(asm_path, out_c_path, target='mips-ido-c'):
    r = subprocess.run(['python3', f'{ROOT}/build/m2c/m2c.py',
                        '-t', target, asm_path],
                       capture_output=True, text=True)
    out = postprocess_m2c(r.stdout)
    open(out_c_path, 'w').write(out)
    return r.returncode, r.stderr

def main():
    if len(sys.argv) < 2:
        print(__doc__); sys.exit(1)
    addr = int(sys.argv[1], 16)
    size = int(sys.argv[2]) if len(sys.argv) > 2 else infer_size(addr)
    fn_name = f'func_{addr:08X}'

    out_dir = f'{ROOT}/build/m2c'
    os.makedirs(out_dir, exist_ok=True)
    asm_path = f'{out_dir}/{fn_name}.s'
    c_path = f'{out_dir}/{fn_name}.c'

    text = gnu_as_format(addr, size, fn_name)
    open(asm_path, 'w').write(text)
    print(f'Wrote {asm_path} ({len(text.splitlines())} lines)')

    rc, err = run_m2c(asm_path, c_path)
    if rc != 0 and err.strip():
        print(f'm2c stderr:\n{err}')
    print(f'Wrote {c_path}')
    print(f'\nFirst 30 lines of C:\n')
    with open(c_path) as f:
        for i, line in enumerate(f):
            if i >= 30: break
            print(f'  {line}', end='')

if __name__ == '__main__':
    main()
