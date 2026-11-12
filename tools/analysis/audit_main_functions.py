#!/usr/bin/env python3
"""
audit_main_functions.py — address-keyed audit of every known main-binary
function (0x80041000-0x800C0000). Resolves debug-name aliases via
config/symbol_mapping_tracker.json + 'Address: 0x...' headers in source files,
so a debug-named matched file (e.g. system_init.c at 0x80056670) doesn't show
up as 'exit_function missing' in the report.

Outputs:
  config/main_function_audit_by_addr.json — machine-readable, keyed by address

Stubs are flagged 'shadowed_by: pc-port' if any alias has a pc-port override
that wins in the build (so the stub is dead, not blocking).

Re-run after each batch of decompilation:
  python3 tools/analysis/audit_main_functions.py
"""
import os, json, re, glob
from collections import Counter, defaultdict

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import sys as _sys_paths
_sys_paths.path.insert(0, os.path.join(ROOT, 'tools'))
from repo_paths import PC as _PC_ROOT  # noqa: E402
# src/main/pc-port belongs to the PC port: its
# Address headers and overrides are read where that part is, and are simply
# absent (no aliases, no pc_port_only rows) without it.
_PC_BASE = (ROOT if _PC_ROOT and os.path.realpath(str(_PC_ROOT)) == os.path.realpath(ROOT)
            else (str(_PC_ROOT) if _PC_ROOT else None))
PCPORT = os.path.join(_PC_BASE, 'src', 'main', 'pc-port') if _PC_BASE else None
import sys as _sys
_sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
_CODE_END = 0x800B5A60     # see audit_main_coverage.CODE_END

def fn_defs_in_c(path):
    try: txt = open(path).read()
    except: return set()
    txt = re.sub(r'/\*.*?\*/','',txt,flags=re.S)
    txt = re.sub(r'//[^\n]*','',txt)
    out = set()
    # Match function definitions at column 0 (top-level) with explicit return type.
    # This avoids matching `if (...)`, `while (...)`, etc. as function definitions.
    # NB: the return type must accept a typedef'd name (e.g. `TableEntry *`),
    # not just the primitive list -- get_k_work returns a struct pointer via
    # a typedef and was invisible here, so a registered, byte-perfect function
    # audited as 'missing'.  Keywords that can open a `kw (...) {` construct are
    # excluded instead of whitelisting types.
    pat = re.compile(
        r'(?:^|\n)(?:static\s+|inline\s+)?'
        r'(?!(?:if|for|while|switch|do|else|return|sizeof)\b)'
        r'(?:struct\s+\w+|unsigned\s+\w+|signed\s+\w+|\w+)'
        r'\s*\**\s*'
        r'(?!(?:if|for|while|switch|do|else|return|sizeof)\s*\()'
        r'(\w+)\s*\([^;{]*\)'
        # K&R parameter declarations between `)` and `{` (the -cckr
        # rewrites).  Without this every K&R definition audited as
        # 'missing' (22 functions on 2026-09-19).
        r'(?:\s*[A-Za-z_][^;{}()]*;)*\s*\{')
    for m in pat.finditer(txt):
        out.add(m.group(1))
    # K&R implicit-int definitions: no return type, name at column 0
    # (`touzyou_basyo_check(sel, sub)\nu8 *sel; ...\n{`).
    knr = re.compile(
        r'(?:^|\n)(?!(?:if|for|while|switch|do|else|return|sizeof)\b)'
        r'([A-Za-z_]\w*)\s*\([^;{]*\)'
        r'(?:\s*[A-Za-z_][^;{}()]*;)*\s*\{')
    for m in knr.finditer(txt):
        out.add(m.group(1))
    return out

def fn_defs_in_asm(path):
    try: txt = open(path).read()
    except: return set()
    return set(m.group(1) for m in re.finditer(
        r'^\s*(?:glabel|\.globl|\.ent)\s+(\w+)', txt, re.M))

def is_stub_body(path):
    try: txt = open(path).read()
    except: return False
    txt = re.sub(r'/\*.*?\*/','',txt,flags=re.S)
    txt = re.sub(r'//[^\n]*','',txt)
    m = re.search(r'\b(?:void|int|s32|u32|s8|u8|s16|u16|float|double|char)\s*\*?\s*\w+\s*\([^)]*\)(?:\s*[A-Za-z_][^;{}()]*;)*\s*\{', txt)
    if not m: return False
    body = txt[m.end():]; depth=1; end=0
    for i,c in enumerate(body):
        if c=='{': depth+=1
        elif c=='}': depth-=1
        if depth==0: end=i; break
    body = body[:end]
    stmts = [s.strip() for s in body.split(';') if s.strip()]
    real = [s for s in stmts if re.search(r'[\(\=\+\-\*\/\&\|\<\>\!]', s) or s.startswith('return')]
    if len(real) > 3: return False
    # A tiny body that calls another function or writes a named global is a
    # real (small) function, not a placeholder (2026-09-02).
    for st in real:
        if re.search(r'\b(?:func_[0-9A-Fa-f]{8}|[A-Za-z_]\w*)\s*\(', st) and not st.startswith('(void)'):
            if not re.match(r'(?:return\s+)?\(?\s*(?:void|s32|u32|s16|u16|u8|s8)\s*\)', st):
                return False
        if re.search(r'\b(?:D_[0-9A-Fa-f]{8}|g_\w+)\b\s*(?:\[[^\]]*\])?\s*=[^=]', st):
            return False
    return True

def main():
    # alias map: address -> {names...}
    addr_aliases = defaultdict(set)
    with open(f'{ROOT}/config/symbol_mapping_tracker.json') as f: d = json.load(f)
    for ent in d['entries']:
        a = int(ent['retail_address'], 16)
        addr_aliases[a].add(ent['retail_name'])
        if ent.get('obin_name'): addr_aliases[a].add(ent['obin_name'])
    # Byte-verified Sony Psy-Q symbols (config/psyq_lib_symbols.json).  This is
    # hand-authored tracked source, not an o.bin guess: Konami linked LIBGTE
    # verbatim, so these addresses, sizes and names come from the shipped Sony
    # archive members that the retail bytes reproduce exactly.  Seeding the
    # alias map from it individuates the members' functions that no repo source
    # mentions -- they were invisible, sitting inside merged blobs.
    _psyq = f'{ROOT}/config/psyq_lib_symbols.json'
    psyq_sizes = {}
    if os.path.exists(_psyq):
        with open(_psyq) as f: d = json.load(f)
        for ent in d['symbols']:
            a = int(ent['address'], 16)
            addr_aliases[a].add(ent['name'])
            psyq_sizes[a] = ent['size']

    # Address: 0x... headers
    for path in (glob.glob(f'{ROOT}/src/main/matched/*.c') +
                 glob.glob(f'{ROOT}/src/main/semi-matched/*.c') +
                 (glob.glob(f'{PCPORT}/*.c') if PCPORT else []) +
                 glob.glob(f'{ROOT}/asm/main/matched/*.s')):
        try: head = open(path).read(2000)
        except: continue
        for m in re.finditer(r'Address:\s*0x([0-9a-fA-F]+)', head):
            a = int(m.group(1),16)
            try: body = open(path).read()
            except: continue
            if path.endswith('.c'):
                # Only the function the header is ABOUT is an alias of `a`.
                # Taking every definition in the file made helpers aliases
                # too: `if`/`for`/`while` (keywords), file-static helpers
                # (`emit_field` in a pc-port override at 0x800B4870, the BIOS
                # rand thunk), and the sibling functions of a multi-function
                # semi -- schedule_mark_on.c listed get_calx_pos, whose
                # REGISTERED C then made the unmatched 2,348-byte
                # schedule_mark_on audit as matched_c (2026-10-03).
                defs = fn_defs_in_c(path)
                stat = set(re.findall(r'(?m)^static\s+[^;{(=]*?\b(\w+)\s*\(', body))
                cand = [d for d in defs if d not in stat]
                stem = os.path.splitext(os.path.basename(path))[0]
                own = f'func_{a:08X}'
                if stem in defs:
                    pick = [stem]
                elif own in defs or own.lower() in defs:
                    pick = [own if own in defs else own.lower()]
                else:
                    pick = cand[:1] if len(cand) == 1 else []
                for n in pick:
                    addr_aliases[a].add(n)
                continue
            for fm in re.finditer(r'\b(\w+)\s*\([^)]*\)(?:\s*[A-Za-z_][^;{}()]*;)*\s*\{', body):
                addr_aliases[a].add(fm.group(1))
            # NB: `\b` before the alternation can NEVER match ahead of a
            # leading '.', so `.globl`/`.ent` were invisible here and only
            # `glabel` ever seeded the alias map.  Anchor per line instead.
            for gm in re.finditer(r'(?m)^\s*(?:glabel|\.globl|\.ent)\s+(\w+)', body):
                addr_aliases[a].add(gm.group(1))

    # name -> (path, kind)
    loc = {}
    def add(n,p,k):
        if n in loc and loc[n][1] in ('matched_c','semi_real'): return
        loc[n] = (p,k)
    for p in glob.glob(f'{ROOT}/src/main/matched/*.c'):
        for n in fn_defs_in_c(p): add(n,p,'matched_c')
    for p in glob.glob(f'{ROOT}/src/main/semi-matched/*.c'):
        k = 'semi_stub' if is_stub_body(p) else 'semi_real'
        for n in fn_defs_in_c(p): add(n,p,k)
    for p in (glob.glob(f'{PCPORT}/*.c') if PCPORT else []):
        for n in fn_defs_in_c(p):
            if n not in loc: add(n,p,'pc_port_only')
    for p in glob.glob(f'{ROOT}/src/main/extras/*.c'):
        for n in fn_defs_in_c(p):
            if n not in loc: add(n,p,'extras')
    for p in glob.glob(f'{ROOT}/asm/main/matched/*.s'):
        for n in fn_defs_in_asm(p):
            # A byte-verified .s is the terminal decomp state; a PC-port
            # override or extras copy does not outrank it (2026-09-02).
            if n not in loc or loc[n][1] in ('pc_port_only', 'extras'):
                loc[n] = (p, 'asm_only')

    # Universe of addresses in main range
    universe = set(addr_aliases.keys())
    for cfg in ['config/level3_functions.json','config/decompiled_functions.json']:
        with open(f'{ROOT}/{cfg}') as f: d = json.load(f)
        for fn in d.get('functions', []):
            # `address` is a hex STRING ("0x800B4020") in level3_functions.json,
            # and `address_hex` does not exist there at all -- so before this fix
            # the level3 registry contributed ZERO addresses to the universe.
            a = fn.get('address_hex', fn.get('address'))
            if isinstance(a, str):
                try: universe.add(int(a, 16))
                except ValueError: pass
            elif isinstance(a, int):
                universe.add(a)
    with open(f'{ROOT}/config/asm_conversion_status.json') as f: d = json.load(f)
    for n in d.get('functions', {}):
        m = re.match(r'func_([0-9a-fA-F]+)', n)
        if m:
            try: universe.add(int(m.group(1),16))
            except: pass
    main_addrs = {a for a in universe if 0x80041000 <= a < 0x800C0000}

    order = {'matched_c':0,'semi_real':1,'asm_only':2,'pc_port_only':3,'semi_stub':4,'extras':5}
    def kind_for(a):
        aliases = addr_aliases.get(a, set()) | {f'func_{a:08X}', f'func_{a:08x}'}
        best = None; bestpath = None
        # sorted(): ties between same-kind aliases must not depend on the
        # interpreter's string-hash seed (the source path used to change
        # from run to run).
        for n in sorted(aliases):
            if n in loc:
                k = loc[n][1]
                if best is None or order.get(k,9) < order.get(best,9):
                    best = k; bestpath = loc[n][0]
        return (best or 'missing', bestpath, aliases)

    # --- window coverage -------------------------------------------------
    # An address that falls INSIDE another registered function's [addr, addr+size)
    # is not open work: the enclosing function's .c already produces those bytes.
    # 163 of the 942 "open" entries were this, an artefact of the 48 overlapping
    # registry entries (see tools/analysis/audit_registry_overlaps.py).  They are
    # reported as `covered` and excluded from the open tally.
    with open(f'{ROOT}/config/level3_functions.json') as f:
        _reg = json.load(f)['functions']
    _iv = sorted((int(e['address'], 16), int(e['size'])) for e in _reg)

    def enclosing(x):
        for a0, sz in _iv:
            if a0 > x:
                break
            # a0 == x counts too: a registered entry that STARTS here under
            # another name (a head-widened window, e.g. cal_font_load at
            # 0x800689FC) produces these bytes.  Only non-matched kinds are
            # ever tested against this, so a function's own entry is safe.
            if a0 <= x < a0 + sz:
                return a0
        return None

    # A <= 8-byte entry with no source is a cross-prologue head fragment -- the
    # `lui`/load pair IDO -O2 hoists above the NEXT function's prologue.  It has
    # no `jr ra` and is not a function; the overlay side of the project absorbs
    # these into their host rather than decompiling them.
    # --- boundary corrections -------------------------------------------
    # config/main_symbol_boundaries.json is produced by
    # tools/analysis/audit_phantom_symbols.py, which walks back from each
    # symbol to the previous `jr ra` + delay slot (skipping alignment padding)
    # and lets the gap decide what the symbol really is.  Three corrections
    # matter here:
    #   phantom    - mid-basic-block, nothing enters it.  Not open work, and
    #                it TRUNCATES the size of the function enclosing it, so it
    #                must also be skipped when computing the next address.
    #   head       - the symbol was recorded AFTER the `lui`/load pair IDO -O2
    #                hoists above the prologue.  The function really starts
    #                `true_start` bytes earlier; any separate symbol sitting at
    #                true_start is a head fragment to be absorbed, not decompiled.
    #   delay-slot - the symbol sits in the previous function's branch-delay
    #                slot; the real entry is one word later.
    _bounds = {}
    _bpath = f'{ROOT}/config/main_symbol_boundaries.json'
    if os.path.exists(_bpath):
        with open(_bpath) as f:
            _bounds = {int(k, 16): v for k, v in json.load(f)['boundaries'].items()}
    _phantoms = {a for a, v in _bounds.items() if v['category'] == 'phantom'}
    # Symbols inside the verified table-data range are not functions at all.
    _data = {a for a, v in _bounds.items() if v['category'] == 'data'}
    _phantoms |= _data      # they must not act as size boundaries either
    # A `head` verdict means the symbol at `a` sits at the PROLOGUE, with the
    # hoisted `lui`/load pair at `true_start` just above it.  When BOTH
    # addresses carry a symbol, exactly one of them is the artefact -- and
    # which one cannot be decided from position alone.  Decide it by SOURCE:
    # whichever address actually has decompiled C is the function, and the
    # other is the fragment.  The usual arrangement is a tiny head stub above a
    # real matched body (fragment = true_start); the inverse happens when a
    # function was correctly recorded from its hoisted head and the
    # disassembler additionally split it at the prologue (fragment = a).
    _REAL = ('matched_c', 'semi_real', 'asm_only')
    _absorbed = set()      # redundant prologue split; the head address is real
    _headfrag = set()      # classic 8-byte head stub above a real body
    _late_start = {}       # no symbol above: widen this function's window back
    for a, v in _bounds.items():
        if v['category'] != 'head' or 'true_start' not in v:
            continue
        ts = int(v['true_start'], 16)
        if ts == a:
            continue
        if ts not in main_addrs:
            _late_start[a] = ts
            continue
        k_a = kind_for(a)[0]
        k_ts = kind_for(ts)[0]
        if k_ts in _REAL and k_a not in _REAL:
            _absorbed.add(a)
        elif k_a in _REAL and k_ts not in _REAL:
            _headfrag.add(ts)
        elif k_a in _REAL and k_ts in _REAL:
            # Both decompiled: leave both alone and let the overlap audit
            # (tools/analysis/audit_registry_overlaps.py) adjudicate.
            pass
        else:
            _headfrag.add(ts)

    _sorted_addrs = sorted(main_addrs)
    # A phantom does not end the function it sits inside, so it must not act as
    # a size boundary -- otherwise every host is silently truncated.
    _real_addrs = [a for a in _sorted_addrs if a not in _phantoms]
    _next_addr = {}
    for i, a in enumerate(_sorted_addrs):
        nxt = None
        for b in _real_addrs:
            if b > a:
                nxt = b
                break
        # The last symbol runs to the end of the code section (StopPAD at
        # 0x800B5A50 is a 16-byte BIOS thunk, not 4 bytes).
        _next_addr[a] = nxt if nxt is not None else max(a + 4, _CODE_END)

    addresses = {}
    for a in main_addrs:
        k, src, aliases = kind_for(a)
        enc = enclosing(a)
        was = None
        if enc is not None and k in ('missing', 'semi_real', 'semi_stub', 'pc_port_only'):
            # The enclosing registered function's .c already produces these
            # bytes, so the address is not open work whatever source happens to
            # sit next to it.  Keep what it WAS: a semi_real inside another
            # window usually means someone decompiled a fragment as if it were
            # its own function, which is worth knowing when the overlap is
            # eventually resolved.
            was, k, src = k, 'covered', None
        elif a in _data:
            was, k, src = k, 'data', None
        elif a in _phantoms:
            was, k, src = k, 'phantom', None
        elif a in _absorbed:
            was, k, src = k, 'covered', None
        elif a in _headfrag:
            was, k, src = k, 'head_fragment', None
        elif k == 'missing' and _next_addr[a] - a <= 8 and a not in psyq_sizes:
            # <= 8 bytes with no source is normally the cross-prologue head
            # IDO -O2 hoists above the NEXT function.  A byte-verified Sony
            # symbol is exempt: LIBGTE really does export 8-byte entry stubs
            # (the RCpoly* pairs), and they are functions, not fragments.
            k, src = 'head_fragment', None
        # shadow check for stubs
        shadowed = None
        if k == 'semi_stub':
            for n in aliases:
                for sub, d in (('matched', f'{ROOT}/src/main/matched'), ('pc-port', PCPORT)):
                    if d and os.path.exists(f'{d}/{n}.c'):
                        shadowed = sub; break
                if shadowed: break
        _ts = _late_start.get(a)
        addresses[f'0x{a:08X}'] = {
            'kind': k, 'source': os.path.relpath(src, ROOT) if src else src,
            'aliases': sorted(aliases),
            **({'shadowed_by': shadowed} if k=='semi_stub' else {}),
            **({'enclosed_by': f'0x{enc:08X}'} if enc is not None else {}),
            **({'was': was} if was else {}),
            # For a cross-prologue head the recorded address is the PROLOGUE;
            # the function really begins at the hoisted global reads above it.
            # Surface the corrected window so whoever writes the C starts in
            # the right place -- C written from the prologue is missing the
            # opening statements and cannot match, however good it is.
            **({'true_start': _bounds[a]['true_start'],
                'true_size': (_next_addr[a] - int(_bounds[a]['true_start'], 16))}
               if a in _bounds and _bounds[a].get('category') == 'head'
               and 'true_start' in _bounds[a] else {}),
            # Sony's own symbol-table size beats the next-address estimate.
            **({'size': psyq_sizes.get(a, _next_addr[a] - a)}
               if k != 'matched_c' else {}),
            'region': 'library' if a >= 0x80086000 else 'game',
        }
    # --- byte-level coverage (2026-10-03) ---------------------------------
    # Everything above is SYMBOL-driven: an address no symbol names cannot be
    # counted, open or done.  tools/analysis/audit_main_coverage.py walks the
    # retail BYTES and reports every non-zero range no registered C window and
    # no asm_only listing covers.  Each such range is open work whatever the
    # symbol tables say: a new `uncovered` entry, or -- when a symbol there was
    # wrongly counted as done -- the existing entry re-kinded.
    _uncovered = []
    try:
        import audit_main_coverage as _cov
        _words = _cov.load_words()
        _probs, _notes = [], []
        _wins = (_cov.registry_windows()
                 + _cov.asm_only_windows(_words, _probs, _notes)
                 + _cov.load_allowlist(_cov.ALLOWLIST, _probs))
        _uncovered, _ = _cov.compute_gaps(_words, _cov.TEXT_BASE, _wins)
    except Exception as _e:   # never let the gate silently vanish
        print(f'  WARNING: coverage pass failed ({_e}); uncovered code NOT counted')
    _DONE = ('matched_c', 'asm_only', 'covered', 'head_fragment', 'phantom', 'data')
    for _a, _b in _uncovered:
        _k = f'0x{_a:08X}'
        _e = addresses.get(_k)
        if _e is None:
            _kk, _src, _al = kind_for(_a)
            addresses[_k] = {'kind': 'uncovered',
                             # an existing semi / PC-port stub is worth knowing
                             'source': os.path.relpath(_src, ROOT)
                                      if _src and _kk in ('semi_real', 'semi_stub',
                                                          'pc_port_only') else None,
                             **({'existing': _kk} if _kk in ('semi_real', 'semi_stub',
                                                            'pc_port_only') else {}),
                             'aliases': sorted(_al),
                             'size': _b - _a,
                             'region': 'library' if _a >= 0x80086000 else 'game'}
        else:
            if _e['kind'] in _DONE:
                _e['was'] = _e['kind']
                _e['kind'] = 'uncovered'
            _e['uncovered_bytes'] = _b - _a
    counts = Counter(v['kind'] for v in addresses.values())

    out = {
        'summary': dict(counts),
        'total_addresses': len(addresses),
        'active_semi_stubs': sorted(
            [a for a,v in addresses.items() if v['kind']=='semi_stub' and not v.get('shadowed_by')],
            key=lambda x: int(x,16)),
        'shadowed_stubs': sorted(
            [a for a,v in addresses.items() if v['kind']=='semi_stub' and v.get('shadowed_by')],
            key=lambda x: int(x,16)),
        'addresses': addresses,
    }
    with open(f'{ROOT}/config/main_function_audit_by_addr.json','w') as f:
        json.dump(out, f, indent=2)
    for k in ['matched_c','semi_real','semi_stub','asm_only','extras','missing']:
        print(f'  {k:18s}: {counts.get(k,0)}')
    print(f'  active stubs:        {len(out["active_semi_stubs"])}')
    print(f'  shadowed stubs:      {len(out["shadowed_stubs"])}')
    print()
    print('  --- NOT open decompilation work ---')
    _was = Counter(v['was'] for v in addresses.values() if v.get('was'))
    print(f'  {"covered":18s}: {counts.get("covered",0)}   '
          f'(inside another registered function\'s window; was {dict(_was)})')
    print(f'  {"head_fragment":18s}: {counts.get("head_fragment",0)}   '
          f'(<=8B cross-prologue hoist, not a function)')
    print(f'  {"pc_port_only":18s}: {counts.get("pc_port_only",0)}   '
          f'(a PC-port OVERRIDE exists, no decompilation -- see below)')
    print()
    # pc_port_only is not a decompilation state.  The file in src/main/pc-port/
    # is a hand-written gameplay override, not an attempt at the retail bytes,
    # so counting it as progress OR as a partially-done decomp is wrong both
    # ways.  It is reported separately and folded into the open tally as
    # UNSTARTED, because that is what it is.
    print(f'  {"uncovered":18s}: {counts.get("uncovered",0)}   '
          f'(retail code no registered window / asm_only listing covers -- '
          f'audit_main_coverage.py; OPEN)')
    openwork = sum(counts.get(k, 0) for k in
                   ('semi_real', 'semi_stub', 'missing', 'pc_port_only',
                    'uncovered'))
    game = sum(1 for v in addresses.values()
               if v['region'] == 'game' and v['kind'] in
               ('semi_real', 'semi_stub', 'missing', 'pc_port_only',
                'uncovered'))
    print(f'  OPEN DECOMP WORK   : {openwork}   ({game} game-region, '
          f'{openwork - game} library-region)')
    print(f'  DONE (matched_c)   : {counts.get("matched_c",0)}')
    print(f'\nWrote: config/main_function_audit_by_addr.json')

if __name__ == '__main__':
    main()
