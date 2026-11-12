#!/usr/bin/env python3
"""Regenerate level3_functions.json with per-function byte-match VERIFICATION.

For every src/main/matched/*.c: verify it builds byte-perfect (collision-free
per-function mini-config build). Register source=.c only if it matches; drop
otherwise (build falls back to correct original ROM bytes). All asm/main/matched
*.s entries are kept as asm. This yields a config where every C entry is
guaranteed byte-perfect.
"""
import json, subprocess, re, os, glob, tempfile
from concurrent.futures import ProcessPoolExecutor, as_completed

ROOT = '.'
os.chdir(ROOT)
OLD = json.load(open('config/level3_functions.json'))
META = {f['name']: f for f in OLD['functions']}


def addr_of(name, src):
    if name in META:
        return int(META[name]['address'], 16)
    m = re.search(r'(?:func|FUN)_([0-9A-Fa-f]{8})', name)
    if m:
        return int(m.group(1), 16)
    h = open(src).read(2000)
    m = re.search(r'Address:\s*(0x[0-9A-Fa-f]{8})', h)
    return int(m.group(1), 16) if m else None


def size_of(name, src):
    if name in META and META[name].get('size'):
        return META[name]['size']
    h = open(src).read(2000)
    for pat in [r'Address:\s*0x[0-9A-Fa-f]+\s*-\s*0x[0-9A-Fa-f]+\s*\((\d+)\s*bytes',
                r'Size:\s*(\d+)\s*bytes', r'\((\d+)\s*bytes', r'(\d+)\s*instructions?\)']:
        m = re.search(pat, h)
        if m:
            n = int(m.group(1))
            return n * 4 if 'instr' in pat else n
    return None


def region_of(addr):
    return "GCC" if 0x80041000 <= addr <= 0x80085FFF else "IDO"


def verify(name, addr, size):
    cfgpath = os.path.join(tempfile.gettempdir(), f'vr_{name}_{os.getpid()}.json')
    mini = {"functions": [{"name": name, "address": f"0x{addr:08X}", "size": size,
            "source": f"src/main/matched/{name}.c", "region": region_of(addr),
            "match_status": "v"}]}
    json.dump(mini, open(cfgpath, 'w'))
    try:
        r = subprocess.run(['python3', 'tools/build/build_level3_c.py', '--config', cfgpath,
                            '-j', '1', '--no-cache'], capture_output=True, text=True, timeout=150)
        ok = ('MATCH!' in r.stdout) and ('MISMATCH' not in r.stdout) and \
             ('COMPILE ERROR' not in r.stdout)
    except subprocess.TimeoutExpired:
        ok = None
    finally:
        try: os.remove(cfgpath)
        except OSError: pass
    return ok


def work(src):
    name = os.path.splitext(os.path.basename(src))[0]
    addr = addr_of(name, src)
    size = size_of(name, src)
    if addr is None or size is None:
        return (name, src, addr, size, 'no_meta')
    ok = verify(name, addr, size)
    return (name, src, addr, size, 'match' if ok else ('timeout' if ok is None else 'mismatch'))


def main():
    cfiles = sorted(glob.glob('src/main/matched/*.c'))
    print(f'verifying {len(cfiles)} matched C files...')
    results = []
    done = 0
    with ProcessPoolExecutor(max_workers=12) as ex:
        futs = {ex.submit(work, c): c for c in cfiles}
        for fut in as_completed(futs):
            results.append(fut.result())
            done += 1
            if done % 100 == 0:
                m = sum(1 for r in results if r[4] == 'match')
                print(f'[{done}/{len(cfiles)}] match={m}', flush=True)
    funcs = []
    seen = set()
    matched = [r for r in results if r[4] == 'match']
    for name, src, addr, size, _ in matched:
        funcs.append({"name": name, "address": f"0x{addr:08X}", "size": size,
                      "source": src, "region": region_of(addr), "match_status": "verified"})
        seen.add(addr)
    # keep asm/main/matched entries
    for s in sorted(glob.glob('asm/main/matched/*.s')):
        name = os.path.splitext(os.path.basename(s))[0]
        m = re.search(r'(?:func|FUN)_([0-9A-Fa-f]{8})', name)
        if not m: continue
        addr = int(m.group(1), 16)
        if addr in seen: continue
        funcs.append({"name": name, "address": f"0x{addr:08X}", "size": META.get(name, {}).get('size', 0),
                      "source": s, "region": region_of(addr), "match_status": "verified"})
        seen.add(addr)
    funcs.sort(key=lambda f: int(f['address'], 16))
    nc = sum(1 for f in funcs if f['source'].endswith('.c'))
    out = {"version": "3.0", "description": "Level 3 verified matched functions",
           "generated_by": "tools/build/verified_regen.py", "total_functions": len(funcs),
           "c_functions": nc, "asm_functions": len(funcs) - nc, "functions": funcs}
    json.dump(out, open('config/level3_functions.json', 'w'), indent=2)
    from collections import Counter
    st = Counter(r[4] for r in results)
    print('\n=== VERIFIED REGEN DONE ===')
    print('result stats:', dict(st))
    print(f'config: {len(funcs)} entries, {nc} C, {len(funcs)-nc} asm')
    json.dump([r[0] for r in results if r[4] != 'match'],
              open('/tmp/regen_nonmatch.json', 'w'))
    print('non-matching C (dropped from C, fall back to ROM) -> /tmp/regen_nonmatch.json')


if __name__ == '__main__':
    main()
