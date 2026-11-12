#!/usr/bin/env python3
"""
decode_jump_tables.py — find functions whose body contains a `jr <reg>` switch
dispatch via a .rodata table, decode the table, and report the targets.

Usage: python3 tools/analysis/decode_jump_tables.py [--all|--stubs]
  --all   : sweep every known main-binary function
  --stubs : sweep only the active semi-stubs from the audit (default)

Pattern detected:
    sltiu at, idx, BOUND        ; bounds check (optional)
    sll   t6, idx, 2            ; idx * 4
    lui   at, HI
    addu  at, at, t6
    lw    t6, LO(at)            ; load function pointer from table
    jr    t6                    ; indirect call

Output: config/jump_tables_in_<scope>.json
"""
import json, re, subprocess, bisect, os, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

def disasm_addr(addr, size):
    off = (addr - 0x80041000) + 0x800
    with open(f'{ROOT}/disks/SLPS_000.65','rb') as f:
        f.seek(off); data = f.read(size)
    open('/tmp/dis.bin','wb').write(data)
    r = subprocess.run(['mipsel-linux-gnu-objdump','-D','-b','binary','-m','mips','-EL',
                       f'--adjust-vma=0x{addr:08X}','/tmp/dis.bin'],
                       capture_output=True, text=True, timeout=10)
    return r.stdout

INSN = re.compile(r'^([0-9a-f]+):\s+[0-9a-f]+\s+(\S+)\s*(.*)$')

def find_jump_table(disasm_text, fn_start, fn_end):
    parsed = []
    for line in disasm_text.splitlines():
        m = INSN.match(line)
        if m:
            pc = int(m.group(1), 16)
            if pc >= fn_end: break
            parsed.append((pc, m.group(2), m.group(3).strip()))
    for i, (pc, op, args) in enumerate(parsed):
        if op == 'jr' and re.match(r'^t[0-9]$', args):
            jr_reg = args
            for j in range(i-1, max(-1, i-20), -1):
                pc2, op2, args2 = parsed[j]
                m_lw = re.match(rf'^{jr_reg},(-?\d+)\(at\)', args2)
                if op2 == 'lw' and m_lw:
                    offset = int(m_lw.group(1))
                    for k in range(j-1, max(-1, j-15), -1):
                        pc3, op3, args3 = parsed[k]
                        if op3 == 'lui':
                            m_lui = re.match(r'^at,0x([0-9a-fA-F]+)$', args3)
                            if m_lui:
                                hi = int(m_lui.group(1), 16)
                                table_addr = (hi << 16) + offset
                                bound = None
                                for kk in range(j-1, max(-1, j-30), -1):
                                    pc4, op4, args4 = parsed[kk]
                                    if op4 == 'sltiu':
                                        m_sl = re.match(r'^\w+,(\w+),(?:0x)?([0-9a-fA-F]+)', args4)
                                        if m_sl:
                                            bound = int(m_sl.group(2), 16)
                                            break
                                return (table_addr, pc, jr_reg, bound)
                    break
    return None

def read_table(addr, max_entries=64):
    off = (addr - 0x80041000) + 0x800
    if off < 0: return []
    with open(f'{ROOT}/disks/SLPS_000.65','rb') as f:
        f.seek(off); data = f.read(max_entries * 4)
    entries = []
    for i in range(0, len(data), 4):
        if i+4 > len(data): break
        word = int.from_bytes(data[i:i+4], 'little')
        if not (0x80041000 <= word < 0x800C0000):
            break
        entries.append(word)
    return entries

def main():
    scope = '--stubs'
    if len(sys.argv) > 1: scope = sys.argv[1]

    with open(f'{ROOT}/config/decompiled_functions.json') as f: dd = json.load(f)
    fn_size = {fn['address']: fn['size'] for fn in dd['functions'] if fn.get('address')}

    if scope == '--all':
        targets = sorted(fn_size.keys())
    else:
        with open(f'{ROOT}/config/main_function_audit_by_addr.json') as f: aud = json.load(f)
        targets = sorted([int(a,16) for a in aud['active_semi_stubs']])

    results = []
    for addr in targets:
        size = fn_size.get(addr, 0)
        if size < 32: continue
        try:
            da = disasm_addr(addr, size)
        except: continue
        jt = find_jump_table(da, addr, addr + size)
        if not jt: continue
        table_addr, jr_pc, jr_reg, bound = jt
        n = bound if bound and bound < 64 else 64
        entries = read_table(table_addr, n)
        internal = all(addr <= e < addr+size for e in entries) if entries else False
        results.append({
            'addr': f'0x{addr:08X}',
            'size': size,
            'table_addr': f'0x{table_addr:08X}',
            'bound': bound,
            'entries': len(entries),
            'unique': len(set(entries)),
            'internal': internal,
            'targets': [f'0x{e:08X}' for e in entries],
        })

    out = f'config/jump_tables_in_{scope.lstrip("-")}.json'
    with open(f'{ROOT}/{out}','w') as f: json.dump(results, f, indent=2)
    print(f'Found {len(results)} jump-table dispatchers in {scope}')
    print(f'Wrote {out}')
    for r in results:
        kind = 'INTERNAL' if r['internal'] else 'external'
        print(f'  {r["addr"]} size={r["size"]:5d} table={r["table_addr"]} bound={r["bound"]} entries={r["entries"]} uniq={r["unique"]} {kind}')

if __name__ == '__main__':
    main()
