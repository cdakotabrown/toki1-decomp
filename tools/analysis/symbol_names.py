#!/usr/bin/env python3
"""config/symbol_names.json -- the single source of truth for retail symbol names.

    retail address -> {name, kind (func|data), size, confidence, evidence, date, landed}

Nothing renames from o.bin directly: the matcher (match_debug_symbols.py)
writes candidates here, the lander (apply_debug_symbols.py) lands entries
marked HIGH, and config/main_retail_symbols.json (the resolver's named-extern
table read by audit_main_relocs.py) is GENERATED from this file.

Sub-commands:
    inventory   regenerate the o.bin cache and write the function/data split
                with debug gap sizes (build/slps00065/symbols/obin_split.json)
    seed        create config/symbol_names.json from the registry's named
                entries, config/main_retail_symbols.json and header hints
    gen-retail  write config/main_retail_symbols.json from symbol_names.json
                (default: legacy content -- the entries carrying a `from`
                provenance -- which reproduces the pre-existing file byte for
                byte; --full emits every landed function name)
    check       invariants: unique names, unique addresses, registry agreement
    show ADDR|NAME
    propose     record a PENDING overlay candidate ("OV:0xADDR" NAME --evidence ...)
    review      code-reading verdict on a PENDING candidate (consistent|contradicted);
                accepts "OV:0xADDR" keys
    note        free-text note on a LANDED name
    audit       anchor-audit verdict on a LANDED name (confirmed|suspect|refuted),
                stored as a structured `audit` field; `--from-json FILE` imports a
                list of {addr, name, verdict, evidence, proposed_action}.  The
                matcher's `--anchors confirmed` uses only confirmed anchors.

Overlay records (2026-10, tools/overlay/apply_overlay_symbols.py): keyed
"<OV>:0xADDR" with "scope": "<OV>".  The
address is the one the function's `func_<ADDR>` name carried (its prologue).
Every overlay loads at 0x80132000, so the same name or address may exist in
several overlays; names are unique WITHIN an overlay and never equal a landed
MAIN name.  `load_names()` returns the MAIN view only (so the main tools --
matcher, census, lander -- never meet an overlay key) and `save_names()` puts
the overlay records back; pass `all_scopes=True` to see and edit both.

Design notes (measured 2026-09-26):
  * o.bin is a DIFFERENT SOURCE REVISION linked in the same order: consecutive
    anchor distances correlate 0.985 but the per-function debug gap has ZERO
    rank correlation with retail size, so a debug gap is not a size signal.
  * the debug link's text runs to 0x800C1480 (BIOS thunks), not the retail
    0x800BB3C0: LIBGTE sits at +48624 in the debug link.  The split below is
    by debug address against DEBUG_TEXT_END.
"""
import argparse, collections, datetime, json, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
NAMES = os.path.join(ROOT, 'config/symbol_names.json')
RETAIL = os.path.join(ROOT, 'config/main_retail_symbols.json')
REGISTRY = os.path.join(ROOT, 'config/level3_functions.json')
TRACKER = os.path.join(ROOT, 'config/symbol_mapping_tracker.json')
OBIN_CACHE = os.path.join(ROOT, 'build/slps00065/symbols/obin_symbols.json')
OBIN_SPLIT = os.path.join(ROOT, 'build/slps00065/symbols/obin_split.json')
EXTRACT = os.path.join(ROOT, 'tools/analysis/extract_obin_symbols.py')
AUDIT = os.path.join(ROOT, 'config/main_function_audit_by_addr.json')
BOUNDARIES = os.path.join(ROOT, 'config/main_symbol_boundaries.json')
EXE = os.path.join(ROOT, 'disks/SLPS_000.65')
TEXT_START, FILE_BASE = 0x80041000, 0x800
RETAIL_TEXT_END = 0x800BB3C0     # verified table-data range starts here (CLAUDE.md)
DEBUG_TEXT_END = 0x800C1480      # last BIOS thunk extern in the debug link + 16
LIB_START = 0x80086000
ADDR_NAME_RE = re.compile(r'^(?:func|FUN|D|DAT)_([0-9A-Fa-f]{8})$')
TODAY = datetime.date.today().isoformat()


# ----------------------------------------------------------------- loaders
def load_obin(regen=False):
    if regen or not os.path.exists(OBIN_CACHE):
        os.makedirs(os.path.dirname(OBIN_CACHE), exist_ok=True)
        subprocess.run([sys.executable, EXTRACT, '--all', '--json', OBIN_CACHE], check=True,
                       stdout=subprocess.DEVNULL)
    syms = json.load(open(OBIN_CACHE))['symbols']
    ext = sorted((int(s['address'], 16), s['name']) for s in syms
                 if s['type'] == 'Global' and s['storage'] == 'Undefined')
    return ext


def obin_split(ext):
    """functions / data by debug address, each with the gap to the next extern."""
    funcs, data = [], []
    for i, (a, n) in enumerate(ext):
        gap = (ext[i + 1][0] - a) if i + 1 < len(ext) else None
        row = {'name': n, 'address': f'0x{a:08X}', 'gap': gap}
        (funcs if a < DEBUG_TEXT_END else data).append(row)
    return funcs, data


def load_registry():
    return json.load(open(REGISTRY))


def split_key(key):
    """'0x8004ABCD' -> ('main', 0x8004ABCD);  'RENSYU:0x80132040' -> ('RENSYU', ...)."""
    if ':' in key:
        scope, _, a = key.partition(':')
        return scope, int(a, 16)
    return 'main', int(key, 16)


def norm_key(arg):
    """Normalise a command-line address argument to a record key."""
    scope, addr = split_key(arg.strip())
    # scope case is not significant: 'MAIN:0x...' must normalise to the bare
    # main key, or record_decided's has-a-record check misses existing main
    # records and a later write can clobber terminal REVERTED history.
    return akey(addr) if scope.lower() == 'main' else f'{scope.upper()}:{akey(addr)}'


def is_overlay_key(key):
    return ':' in key


def _sort_key(kv):
    scope, addr = split_key(kv[0])
    return (scope != 'main', scope if scope != 'main' else '', addr)


def load_names(all_scopes=False):
    """The names document.  By default `symbols` holds the MAIN records only and
    the overlay records ("<OV>:0xADDR") ride along under `_overlay_symbols`
    untouched, so every main-binary consumer keeps `int(key, 16)` semantics;
    save_names() merges them back.  all_scopes=True returns one flat dict."""
    if not os.path.exists(NAMES):
        return {'version': 1, 'generated_by': 'tools/analysis/symbol_names.py',
                'description': 'retail address -> symbol name; single source of truth for renames',
                'symbols': {}, 'hints': {}}
    doc = json.load(open(NAMES))
    if not all_scopes:
        syms = doc.get('symbols', {})
        doc['_overlay_symbols'] = {k: v for k, v in syms.items() if is_overlay_key(k)}
        doc['symbols'] = {k: v for k, v in syms.items() if not is_overlay_key(k)}
    return doc


def save_names(doc):
    out = dict(doc)
    syms = dict(doc['symbols'])
    syms.update(out.pop('_overlay_symbols', {}))
    out['symbols'] = dict(sorted(syms.items(), key=_sort_key))
    out['hints'] = dict(sorted(doc.get('hints', {}).items(), key=lambda kv: int(kv[0], 16)))
    with open(NAMES, 'w') as fh:
        json.dump(out, fh, indent=1)
        fh.write('\n')


def is_address_name(name):
    return bool(ADDR_NAME_RE.match(name))


def akey(addr):
    return f'0x{addr:08X}'


# ------------------------------------------------------------ retail image
_EXE = None


def word(addr):
    global _EXE
    if _EXE is None:
        _EXE = open(EXE, 'rb').read()
    off = addr - TEXT_START + FILE_BASE
    return int.from_bytes(_EXE[off:off + 4], 'little')


def retail_universe():
    """Non-enclosed retail function starts (audit matched_c/asm_only + registry)
    -> {addr: {size, name, kind}}.  Entries lying inside an earlier entry's
    window (registered cross-prologue heads, duplicate windows) are dropped."""
    reg = load_registry()['functions']
    rega = {int(f['address'], 16): f for f in reg}
    au = json.load(open(AUDIT))['addresses'] if os.path.exists(AUDIT) else {}
    starts = set(int(a, 16) for a, v in au.items() if v['kind'] in ('matched_c', 'asm_only')) | set(rega)
    starts = sorted(starts)
    uni = {}
    for i, a in enumerate(starts):
        nxt = starts[i + 1] if i + 1 < len(starts) else None
        sz = rega[a]['size'] if a in rega and rega[a].get('size') else (nxt - a if nxt else 0)
        uni[a] = {'size': sz, 'name': rega[a]['name'] if a in rega else None,
                  'kind': au.get(akey(a), {}).get('kind', 'registry'), 'registered': a in rega}
    enclosed = set()
    for i, a in enumerate(starts):
        end = a + uni[a]['size']
        for b in starts[i + 1:]:
            if b < end:
                enclosed.add(b)
            else:
                break
    return {a: v for a, v in uni.items() if a not in enclosed}


def callgraph(uni):
    """jal edges inside each function window (exact: read from the retail bytes)."""
    callees, callers = {}, collections.defaultdict(set)
    for a, v in uni.items():
        cs = set()
        for x in range(a, a + v['size'], 4):
            w = word(x)
            if (w >> 26) == 3:
                t = ((w & 0x3ffffff) << 2) | 0x80000000
                if t in uni and t != a:
                    cs.add(t)
        callees[a] = cs
        for c in cs:
            callers[c].add(a)
    return callees, callers


# ------------------------------------------------------------- commands
def cmd_inventory(args):
    ext = load_obin(regen=True)
    funcs, data = obin_split(ext)
    out = {'generated_by': 'tools/analysis/symbol_names.py inventory', 'date': TODAY,
           'debug_text_end': akey(DEBUG_TEXT_END), 'retail_text_end': akey(RETAIL_TEXT_END),
           'note': 'gap = distance to the next debug extern; NOT a retail size predictor '
                   '(rank correlation 0.00 on 222 game-region anchors, 2026-09-26)',
           'functions': funcs, 'data': data}
    json.dump(out, open(OBIN_SPLIT, 'w'), indent=1)
    print(f'o.bin externs: {len(ext)} = {len(funcs)} functions (< {akey(DEBUG_TEXT_END)}) '
          f'+ {len(data)} data  -> {os.path.relpath(OBIN_SPLIT, ROOT)}')
    below = sum(1 for a, n in ext if a < RETAIL_TEXT_END)
    print(f'  (a split at the RETAIL text end {akey(RETAIL_TEXT_END)} would give {below} functions: '
          f'{len(funcs) - below} LIBGTE/BIOS names sit above it in the debug link)')


def header_hints():
    """`o.bin name: X` notes recorded by hand in main headers, keyed by file address."""
    hints = {}
    pat = re.compile(r'o\.bin name[: ]+[`"\']?([A-Za-z_][A-Za-z0-9_]{3,})')
    for d in ('src/main/matched', 'src/main/semi-matched', 'asm/main/matched'):
        dd = os.path.join(ROOT, d)
        if not os.path.isdir(dd):
            continue
        for fn in os.listdir(dd):
            m = ADDR_NAME_RE.match(os.path.splitext(fn)[0])
            if not m:
                continue
            head = open(os.path.join(dd, fn), errors='replace').read(4000)
            for h in pat.findall(head):
                if h in ('is', 'on', 'of', 'that', 'the', 'for'):
                    continue
                hints.setdefault(akey(int(m.group(1), 16)), {'name': h, 'evidence': f'header note in {d}/{fn}'})
    return hints


def cmd_seed(args):
    if os.path.exists(NAMES) and not args.force:
        sys.exit(f'{NAMES} exists; --force to rebuild')
    ext = load_obin()
    dname = {n: a for a, n in ext}
    reg = load_registry()['functions']
    tracker = {}
    if os.path.exists(TRACKER):
        for e in json.load(open(TRACKER))['entries']:
            if e.get('status') == 'applied':
                tracker[e['obin_name']] = e['confidence']
    doc = load_names()
    syms = doc['symbols']
    for f in reg:
        if is_address_name(f['name']):
            continue
        a = int(f['address'], 16)
        ev = [f"registry:{f['source']}"]
        if f['name'] in dname:
            ev.append(f"o.bin:{akey(dname[f['name']])} drift{dname[f['name']] - a:+d}")
            ev.append(f"tracker:{tracker.get(f['name'], 'not-in-tracker')}")
        else:
            ev.append('not an o.bin name (project-assigned)')
        syms[akey(a)] = {'name': f['name'], 'kind': 'func', 'size': f.get('size'),
                         'confidence': 'LANDED', 'evidence': '; '.join(ev), 'date': TODAY, 'landed': True}
    if os.path.exists(RETAIL):
        for name, v in json.load(open(RETAIL)).items():
            a = int(v['address'], 16)
            k = akey(a)
            if k in syms and syms[k]['name'] != name:
                print(f'  WARN retail table {name}@{k} disagrees with registry {syms[k]["name"]}')
                continue
            e = syms.setdefault(k, {'name': name, 'kind': 'func', 'size': None, 'confidence': 'LANDED',
                                    'evidence': f'main_retail_symbols.json (inferred from {v.get("from")})',
                                    'date': TODAY, 'landed': True})
            e['from'] = v.get('from')
    doc['hints'] = header_hints()
    save_names(doc)
    print(f'seeded {len(syms)} symbols, {len(doc["hints"])} header hints -> {os.path.relpath(NAMES, ROOT)}')


def gen_retail(doc, full=False):
    out = {}
    for k, e in doc['symbols'].items():
        # landed DATA names carry a `from` too (batch 1, 2026-09-27: Sprites), so
        # audit_main_relocs resolves them; data is never in the function registry
        if e.get('kind') not in ('func', 'data') or not e.get('landed'):
            continue
        if 'from' in e:
            out[e['name']] = {'address': f'0x{int(k, 16):08x}', 'from': e['from']}
        elif full:
            out[e['name']] = {'address': f'0x{int(k, 16):08x}', 'from': 'symbol_names.json'}
    return dict(sorted(out.items()))


def cmd_gen_retail(args):
    doc = load_names()
    out = gen_retail(doc, full=args.full)
    text = json.dumps(out, indent=1)
    if args.check:
        cur = open(RETAIL).read() if os.path.exists(RETAIL) else ''
        if cur == text:
            print(f'{os.path.relpath(RETAIL, ROOT)} is byte-identical to the generated content ({len(out)} entries)')
            return 0
        print('DIFFERS from the generated content:')
        curd = json.loads(cur) if cur else {}
        for k in sorted(set(curd) | set(out)):
            if curd.get(k) != out.get(k):
                print(f'  {k}: file={curd.get(k)} generated={out.get(k)}')
        return 1
    open(RETAIL, 'w').write(text)
    print(f'wrote {len(out)} entries -> {os.path.relpath(RETAIL, ROOT)}')
    return 0


def check_overlay_records(ov_syms, main_syms, reg_names):
    """Invariants of the overlay-scoped records ("<OV>:0xADDR"):
      * key scope == the record's `scope`; kind func|data; address in the
        overlay RAM window;
      * names unique WITHIN an overlay (landed and pending alike); the same name
        may exist in several overlays;
      * an overlay name never equals a MAIN name (landed main record or
        registered main function): every overlay reference to it would be
        ambiguous between the overlay's function and the main one -- unless
        the record carries a reviewed `shadows_main: <reason>` (an overlay
        copy of a main function that never calls the main one);
      * a landed function has a `.sym` entry of that name whose window
        contains the record address and no `func_<ADDR>` entry for the
        address is left behind; a pending one still has its `func_<ADDR>`."""
    sys.path.insert(0, os.path.join(ROOT, 'tools', 'overlay'))
    import overlay_names as ON
    bad = 0
    per = collections.defaultdict(collections.Counter)
    main_names = {e['name'] for e in main_syms.values()} | set(reg_names)
    syms_cache = {}
    for k, e in ov_syms.items():
        try:
            scope, addr = split_key(k)
        except ValueError:
            print(f'BAD overlay key {k}'); bad += 1; continue
        if e.get('scope') != scope:
            print(f'OVERLAY RECORD {k}: scope {e.get("scope")!r} does not match its key'); bad += 1
        if e.get('kind') not in ('func', 'data'):
            print(f'OVERLAY RECORD {k}: kind {e.get("kind")!r}'); bad += 1
        if not (ON.OVERLAY_BASE <= addr <= ON.RAM_HI):
            print(f'OVERLAY RECORD {k}: address outside the overlay window'); bad += 1
        name = e.get('name')
        per[scope][name] += 1
        if name in main_names and not e.get('shadows_main'):
            print(f'OVERLAY NAME {scope}:{name} ({k}) is also a MAIN name: ambiguous '
                  '(a reviewed exception carries `shadows_main: <reason>`)'); bad += 1
        if e.get('kind') != 'func':
            continue
        if scope not in syms_cache:
            syms_cache[scope] = ON.parse_sym(scope)
        entries = syms_cache[scope]
        mine = [(a, n, nm) for a, n, nm in entries if nm == name]
        old = [(a, n, nm) for a, n, nm in entries if nm == f'func_{addr:08X}']
        if e.get('landed') and not mine and e.get('internal_of'):
            # an internal function (defined in C inside another window): the
            # named window must contain it
            if not any(nm == e['internal_of'] and a < addr < a + n for a, n, nm in entries):
                print(f'LANDED internal function {scope}:{name} ({k}): no {scope}.sym window '
                      f'{e["internal_of"]} contains it'); bad += 1
        elif e.get('landed'):
            if not mine:
                print(f'LANDED overlay name {scope}:{name} ({k}) has no {scope}.sym entry'); bad += 1
            elif not any(a <= addr < a + n for a, n, _ in mine):
                print(f'LANDED overlay name {scope}:{name}: no .sym window contains {k}'); bad += 1
            if old:
                print(f'LANDED overlay name {scope}:{name} ({k}): {scope}.sym still holds func_{addr:08X}'); bad += 1
        else:
            if not old and not mine:
                print(f'PENDING overlay record {k} {name}: {scope}.sym has no func_{addr:08X}'); bad += 1
    for scope, c in per.items():
        for n, cnt in c.items():
            if cnt > 1:
                print(f'DUP overlay name {scope}:{n} x{cnt}'); bad += 1
    return bad


def cmd_check(args):
    doc = load_names(all_scopes=True)
    all_syms = doc['symbols']
    syms = {k: v for k, v in all_syms.items() if not is_overlay_key(k)}
    ov_syms = {k: v for k, v in all_syms.items() if is_overlay_key(k)}
    names = collections.Counter(e['name'] for e in syms.values())
    bad = 0
    for n, c in names.items():
        if c > 1:
            print(f'DUP name {n} x{c}'); bad += 1
    reg_fns = load_registry()['functions']
    reg = {f['name']: int(f['address'], 16) for f in reg_fns}
    with open(BOUNDARIES) as fh:
        boundaries = json.load(fh)['boundaries']

    def registry_addresses(k):
        """An absorbed IDO head may precede the named prologue in the registry."""
        addresses = {int(k, 16)}
        boundary = boundaries.get(k, {})
        if boundary.get('category') == 'head':
            addresses.add(int(boundary['true_start'], 16))
        return addresses

    for k, e in syms.items():
        if e['kind'] == 'func' and e['name'] in reg and reg[e['name']] not in registry_addresses(k):
            print(f'REGISTRY DISAGREES {e["name"]}: names {k} registry {akey(reg[e["name"]])}'); bad += 1
        if e.get('landed') and e['kind'] == 'func' and e['name'] not in reg and 'from' not in e:
            print(f'LANDED but not registered: {e["name"]} {k}'); bad += 1
    # window-key guard (2026-10-17): an address inside another registered
    # function's verified window is not a separate entry.  Same-name window
    # records are the absorbed-head alias (registry_addresses above) and are
    # fine; a DIFFERENT name on a window key is the week_day_init2 / 0x80083D34
    # shape and must retire to func_<ADDR> + SUPERSEDED.
    windows = [(int(f['address'], 16), f.get('size') or 0, f['name']) for f in reg_fns]
    for k, e in syms.items():
        if e['kind'] != 'func' or is_address_name(e['name']) or e['name'] in reg:
            continue
        ai = int(k, 16)
        for start, size, n in windows:
            if size and start < ai < start + size - 1:
                if e.get('internal_of') == n:
                    break       # a function the window's C defines after its own (2026-11-10)
                print(f'WINDOW KEY {e["name"]} {k}: inside registered {n} {akey(start)} [{size}]')
                bad += 1
                break
    for n, a in reg.items():
        if not is_address_name(n) and not any(
                e['name'] == n and e['kind'] == 'func' and a in registry_addresses(k)
                for k, e in syms.items()):
            print(f'registry name missing from symbol_names: {n} {akey(a)}'); bad += 1
    bad += check_overlay_records(ov_syms, syms, reg)
    tail = ''
    if ov_syms:
        tail = (f' + {len(ov_syms)} overlay ({sum(1 for e in ov_syms.values() if e.get("landed"))} landed, '
                f'{len({e.get("scope") for e in ov_syms.values()})} overlays)')
    print(f'{len(syms)} symbols, {sum(1 for e in syms.values() if e.get("landed"))} landed, '
          f'{sum(1 for e in syms.values() if not e.get("landed"))} pending{tail}; {bad} problems')
    return 1 if bad else 0


def cmd_review(args):
    """Record a code-reading verdict on a pending candidate (the reviewer step the
    plan requires before a name lands): reviewed:consistent | reviewed:contradicted."""
    doc = load_names(all_scopes=True)
    k = norm_key(args.addr)
    e = doc['symbols'].get(k)
    if e is None:
        sys.exit(f'{k} has no entry')
    if e.get('landed'):
        sys.exit(f'{k} {e["name"]} is already landed')
    e['evidence'] = re.sub(r'; reviewed:\w+[^;]*', '', e.get('evidence', ''))
    e['evidence'] += f'; reviewed:{args.verdict} ({TODAY}, {args.by}): {args.note}'
    if args.verdict == 'contradicted' and e.get('confidence') == 'HIGH':
        e['confidence'] = 'MEDIUM'      # never auto-land a contradicted name
    save_names(doc)
    print(f'{k} {e["name"]}: reviewed:{args.verdict}')


def cmd_note(args):
    """Attach a note to a LANDED name (e.g. 'suspect: ...') without changing it;
    the matcher's worklist prints these under 'landed names under suspicion'."""
    doc = load_names(all_scopes=True)
    k = norm_key(args.addr)
    e = doc['symbols'].get(k)
    if e is None:
        sys.exit(f'{k} has no entry')
    e.setdefault('notes', []).append(f'{TODAY} ({args.by}): {args.note}')
    save_names(doc)
    print(f'{k} {e["name"]}: note added')


AUDIT_VERDICTS = ('confirmed', 'suspect', 'refuted')


def set_audit(e, verdict, note, by, action=None):
    e['audit'] = {'verdict': verdict, 'note': note, 'by': by, 'date': TODAY}
    if action and verdict == 'refuted':
        e['audit']['proposed_action'] = action


def cmd_audit(args):
    """Record an anchor-audit verdict on a LANDED name.  The name is never
    changed here: a refuted name stays in the tree until a reviewer re-derives
    it (the worklist lists the proposed action)."""
    doc = load_names()
    syms = doc['symbols']
    if args.from_json:
        items = json.load(open(args.from_json))
    else:
        if not (args.addr and args.verdict and args.note):
            sys.exit('audit needs ADDR VERDICT --note, or --from-json FILE')
        items = [{'addr': args.addr, 'verdict': args.verdict, 'evidence': args.note,
                  'proposed_action': args.action}]
    n, bad = collections.Counter(), 0
    for it in items:
        k = akey(int(it['addr'], 16))
        e = syms.get(k)
        v = it.get('verdict')
        if e is None or not e.get('landed'):
            print(f'  SKIP {k}: no landed entry'); bad += 1; continue
        if it.get('name') and it['name'] != e['name']:
            print(f'  SKIP {k}: verdict names {it["name"]}, entry is {e["name"]}'); bad += 1; continue
        if v not in AUDIT_VERDICTS:
            print(f'  SKIP {k}: bad verdict {v!r}'); bad += 1; continue
        set_audit(e, v, it.get('evidence', ''), args.by, it.get('proposed_action'))
        n[v] += 1
    save_names(doc)
    print(f'audit recorded: {dict(n)}; {bad} skipped')
    return 1 if bad else 0


def cmd_propose(args):
    """Record a PENDING overlay-scoped candidate (`OV:0xADDR NAME`): the T4
    behaviour-derived record apply_overlay_symbols.py lands once a reviewer has
    recorded `review ... consistent`.  Main candidates come from the matcher
    (match_debug_symbols.py); this exists because the overlays have no o.bin."""
    doc = load_names(all_scopes=True)
    k = norm_key(args.addr)
    if not is_overlay_key(k):
        sys.exit('propose is for overlay records ("OV:0xADDR"); main candidates come from match_debug_symbols.py')
    scope, addr = split_key(k)
    if k in doc['symbols'] and not args.force:
        sys.exit(f'{k} already has a record ({doc["symbols"][k]["name"]}); --force to replace')
    ev = args.evidence or ['behaviour: (none given)']
    doc['symbols'][k] = {
        'name': args.name, 'kind': args.kind, 'scope': scope, 'size': args.size,
        'tier': args.tier, 'style': 'derived' if args.tier == 'T4' else 'obin',
        'confidence': 'DERIVED' if args.tier == 'T4' else 'MEDIUM',
        'evidence': '; '.join(ev), 'proposed_by': args.by, 'date': TODAY, 'landed': False}
    save_names(doc)
    print(f'{k} {args.name}: proposed ({args.tier})')


def cmd_show(args):
    doc = load_names(all_scopes=True)
    for k, e in doc['symbols'].items():
        if args.what.lower() in (k.lower(), e['name'].lower()):
            print(k, json.dumps(e, indent=1))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest='cmd', required=True)
    sub.add_parser('inventory')
    s = sub.add_parser('seed'); s.add_argument('--force', action='store_true')
    g = sub.add_parser('gen-retail'); g.add_argument('--full', action='store_true')
    g.add_argument('--check', action='store_true', help='compare against the current file instead of writing')
    sub.add_parser('check')
    s = sub.add_parser('show'); s.add_argument('what')
    r = sub.add_parser('review'); r.add_argument('addr'); r.add_argument('verdict', choices=['consistent', 'contradicted'])
    r.add_argument('--note', required=True); r.add_argument('--by', default='reviewer')
    pr = sub.add_parser('propose'); pr.add_argument('addr'); pr.add_argument('name')
    pr.add_argument('--kind', default='func', choices=['func', 'data']); pr.add_argument('--size', type=int)
    pr.add_argument('--tier', default='T4', choices=['T3', 'T4']); pr.add_argument('--by', default='proposer')
    pr.add_argument('--evidence', action='append'); pr.add_argument('--force', action='store_true')
    n = sub.add_parser('note'); n.add_argument('addr'); n.add_argument('--note', required=True); n.add_argument('--by', default='reviewer')
    a = sub.add_parser('audit'); a.add_argument('addr', nargs='?'); a.add_argument('verdict', nargs='?', choices=AUDIT_VERDICTS)
    a.add_argument('--note'); a.add_argument('--action'); a.add_argument('--by', default='reviewer')
    a.add_argument('--from-json', metavar='FILE')
    args = ap.parse_args()
    return {'inventory': cmd_inventory, 'seed': cmd_seed, 'gen-retail': cmd_gen_retail,
            'check': cmd_check, 'show': cmd_show, 'propose': cmd_propose, 'review': cmd_review, 'note': cmd_note,
            'audit': cmd_audit}[args.cmd](args) or 0


if __name__ == '__main__':
    sys.exit(main())
