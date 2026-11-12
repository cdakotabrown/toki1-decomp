#!/usr/bin/env python3
"""Stamp the `Address:` / `Size:` header lines test_single_match.py parses into
main-binary semi files that lack them (2026-09-06).

The address comes from @SymbolStart/@SymbolSize when the file carries them,
else from config/main_function_audit_by_addr.json (the entry whose source is
this file).  Only files missing a parseable `Address: 0x...` get touched.

    python3 tools/analysis/stamp_main_headers.py [--apply] [FILE ...]
"""
import os, re, sys, json, glob
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
ADDR = re.compile(r'Address:\s*(0x[0-9a-fA-F]+)')
SIZE = re.compile(r'Size:\s*(\d+)\s*bytes')

def main():
    apply = '--apply' in sys.argv
    files = [a for a in sys.argv[1:] if not a.startswith('--')] or glob.glob(f'{ROOT}/src/main/semi-matched/*.c')
    aud = json.load(open(f'{ROOT}/config/main_function_audit_by_addr.json'))['addresses']
    bysrc = {os.path.abspath(v['source']): (int(a, 16), v) for a, v in aud.items() if v.get('source')}
    n = 0
    for p in files:
        p = os.path.abspath(p); s = open(p).read()
        head_end = s.find('*/')
        head = s[:head_end] if head_end > 0 else ''
        if ADDR.search(head) and SIZE.search(head):
            continue
        ss = re.search(r'@SymbolStart:\s*(0x[0-9a-fA-F]+)', head); sz = re.search(r'@SymbolSize:\s*(\d+)', head)
        if ss and sz:
            addr, size, why = int(ss.group(1), 16), int(sz.group(1)), '@SymbolStart/@SymbolSize'
        elif p in bysrc:
            addr, v = bysrc[p]; size = v.get('size') or 0; why = 'audit registry'
        else:
            print(f'SKIP {os.path.relpath(p, ROOT)}: no address source'); continue
        lines = []
        if not ADDR.search(head): lines.append(f' * Address: 0x{addr:08X}')
        if not SIZE.search(head): lines.append(f' * Size: {size} bytes ({size // 4} instructions)')
        ins = '\n'.join(lines) + f'\n *   (stamped 2026-09-06 from the {why}; the file had no parseable header line)\n'
        if not s.startswith('/*'):
            new = '/*\n' + ins + ' */\n' + s
        else:
            nl = s.find('\n'); new = s[:nl + 1] + ins + s[nl + 1:]
        print(f'{"STAMP" if apply else "would stamp"} {os.path.relpath(p, ROOT)}: {lines}')
        if apply: open(p, 'w').write(new)
        n += 1
    print(f'{n} file(s)')

if __name__ == '__main__':
    main()
