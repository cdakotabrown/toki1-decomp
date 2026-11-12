#!/usr/bin/env python3
"""
recover_struct_layout.py — reconstruct a C struct layout from PS1 binary
accesses to a global address. Per analyst V3 D2 recipe.

Usage:
  python3 tools/analysis/recover_struct_layout.py <BASE_ADDR_HEX>

  e.g. python3 tools/analysis/recover_struct_layout.py 0x800EE760

Approach:
  1. Disassemble every known function in disks/SLPS_000.65 with Capstone.
  2. Locate `lui REG, HI; addiu REG, REG, LO` (or `lui+lw`) pairs that
     materialize the target base address into a register.
  3. Taint-track the destination register through subsequent instructions
     in the same basic block.
  4. On every load/store using the tainted register as base + offset,
     record (offset, access_width, signed/unsigned).
  5. Cluster results across all functions; emit a candidate C struct.

Output:
  config/struct_layout_<addr>.json — raw offset map (per-fn provenance)
  build/structs/struct_<addr>.h    — candidate C struct definition

Run with the venv's python: .venv/bin/python3
"""
import os, sys, json, bisect
from collections import defaultdict, Counter

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, f'{ROOT}/.venv/lib/python3.12/site-packages')

import capstone

CS = capstone.Cs(capstone.CS_ARCH_MIPS, capstone.CS_MODE_MIPS32 | capstone.CS_MODE_LITTLE_ENDIAN)
CS.detail = True

LOADS  = {'lb','lbu','lh','lhu','lw','lwl','lwr'}
STORES = {'sb','sh','sw','swl','swr'}
WIDTH  = {'lb':1,'lbu':1,'sb':1,
          'lh':2,'lhu':2,'sh':2,
          'lw':4,'lwl':4,'lwr':4,'sw':4,'swl':4,'swr':4}
SIGNED = {'lb':True,'lh':True,'lw':True,
          'lbu':False,'lhu':False,'sb':None,'sh':None,'sw':None,
          'lwl':False,'lwr':False,'swl':None,'swr':None}

def load_binary():
    with open(f'{ROOT}/disks/SLPS_000.65','rb') as f:
        return f.read()

def disasm_at(blob, addr, size):
    off = (addr - 0x80041000) + 0x800
    if off < 0 or off + size > len(blob): return []
    return list(CS.disasm(blob[off:off+size], addr))

def trace_base(insns, target_base):
    """Return list of (offset, op, width, signed, pc) for accesses to target_base."""
    hits = []
    # Register-state map: reg_name -> int absolute address (only for clean lui+addiu patterns)
    rstate = {}
    for ins in insns:
        op = ins.mnemonic
        ops = ins.op_str
        # `lui REG, IMM`
        if op == 'lui':
            parts = [p.strip() for p in ops.split(',')]
            if len(parts) == 2:
                reg = parts[0]
                try:
                    hi = int(parts[1], 0)
                    rstate[reg] = (hi << 16)
                    continue
                except: pass
        # `addiu DST, SRC, IMM` (folds the lo into a tracked reg)
        if op == 'addiu':
            parts = [p.strip() for p in ops.split(',')]
            if len(parts) == 3 and parts[1] in rstate:
                try:
                    lo = int(parts[2], 0)
                    # sign-extend 16-bit
                    if lo >= 0x8000: lo -= 0x10000
                    rstate[parts[0]] = rstate[parts[1]] + lo
                    continue
                except: pass
        # `addu DST, SRC, IDX` propagates base if SRC is base
        if op in ('addu','add'):
            parts = [p.strip() for p in ops.split(',')]
            if len(parts) == 3 and parts[1] in rstate:
                rstate[parts[0]] = rstate[parts[1]]
                continue
        # `move DST, SRC` (= or DST, SRC, $zero)
        if op in ('move','or') and ',' in ops:
            parts = [p.strip() for p in ops.split(',')]
            if op == 'or' and len(parts) == 3 and parts[2] == '$zero' and parts[1] in rstate:
                rstate[parts[0]] = rstate[parts[1]]; continue
            if op == 'move' and len(parts) == 2 and parts[1] in rstate:
                rstate[parts[0]] = rstate[parts[1]]; continue
        # Memory access? `lw $t0, 16($s0)` — check if base reg has tracked value
        if op in LOADS or op in STORES:
            # parse offset(reg)
            import re
            m = re.match(r'^(\$\w+)\s*,\s*(-?(?:0x)?[0-9a-fA-F]+)\(\s*(\$\w+)\s*\)', ops)
            if m:
                _, off_s, base_reg = m.groups()
                if base_reg in rstate:
                    try:
                        offset = int(off_s, 0)
                        if offset >= 0x8000 and op != 'lui': offset -= 0x10000
                        absolute = rstate[base_reg] + offset
                        # The lui+addiu pattern often loads (base_addr-OFFSET) and uses
                        # +OFFSET in the access; so absolute may equal target_base + small.
                        delta = absolute - target_base
                        if 0 <= delta < 0x10000:
                            hits.append({
                                'offset': delta,
                                'op': op,
                                'width': WIDTH[op],
                                'signed': SIGNED[op],
                                'pc': f'0x{ins.address:08X}',
                            })
                    except: pass
        # Most other ops invalidate dst register's tracked value
        # (conservative: if op writes to a tracked reg, drop it unless we kept it)
        # Keep simple: don't invalidate — over-capture is fine, we can dedupe.
    return hits

def main():
    if len(sys.argv) < 2:
        print(__doc__); sys.exit(1)
    target_base = int(sys.argv[1], 16)

    with open(f'{ROOT}/config/decompiled_functions.json') as f:
        d = json.load(f)['functions']
    blob = load_binary()

    all_hits = []  # (fn_name, hit)
    for fn in d:
        addr = fn.get('address')
        size = fn.get('size')
        if not addr or not size: continue
        if addr < 0x80041000 or addr >= 0x800C0000: continue
        try:
            insns = disasm_at(blob, addr, size)
        except: continue
        hits = trace_base(insns, target_base)
        for h in hits:
            h['fn'] = fn['name']
            all_hits.append(h)

    print(f'Hits found: {len(all_hits)}')

    # Aggregate
    offset_map = defaultdict(lambda: defaultdict(int))  # offset -> width -> count
    for h in all_hits:
        offset_map[h['offset']][h['width']] += 1

    # Emit JSON
    out = {
        'base': f'0x{target_base:08X}',
        'total_hits': len(all_hits),
        'unique_offsets': len(offset_map),
        'offsets': {
            f'0x{off:04X}': dict(widths)
            for off, widths in sorted(offset_map.items())
        },
        'hits': all_hits,
    }
    out_json = f'{ROOT}/config/struct_layout_{target_base:08X}.json'
    open(out_json,'w').write(json.dumps(out, indent=2))
    print(f'Wrote {out_json}')

    # Emit candidate C header
    sorted_offs = sorted(offset_map.items())
    name = f'struct_{target_base:08X}'
    lines = []
    lines.append(f'/* Auto-generated by recover_struct_layout.py from {len(all_hits)} accesses '
                 f'across {len({h["fn"] for h in all_hits})} functions. */')
    lines.append(f'typedef struct {name} {{')
    cursor = 0
    for off, widths in sorted_offs:
        if off > cursor:
            pad = off - cursor
            lines.append(f'    u8 _pad_{cursor:04X}[{pad}];   /* gap */')
            cursor = off
        # Pick dominant width
        w = max(widths, key=widths.get)
        if w == 1: t = 'u8'
        elif w == 2: t = 'u16'
        elif w == 4: t = 'u32'
        else: t = f'u8 /* w={w} */'
        lines.append(f'    {t:8s} f_{off:04X};   /* widths={dict(widths)} */')
        cursor = off + w
    lines.append(f'}} {name};   /* size >= {cursor} bytes */')
    os.makedirs(f'{ROOT}/build/structs', exist_ok=True)
    out_h = f'{ROOT}/build/structs/struct_{target_base:08X}.h'
    open(out_h,'w').write('\n'.join(lines) + '\n')
    print(f'Wrote {out_h}')
    print()
    print(f'Top-10 hottest offsets:')
    cnt = Counter()
    for h in all_hits: cnt[h['offset']] += 1
    for off, c in cnt.most_common(10):
        print(f'  +0x{off:04X}  {c} accesses  widths={dict(offset_map[off])}')

if __name__ == '__main__':
    main()
