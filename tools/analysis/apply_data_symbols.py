#!/usr/bin/env python3
"""Land DATA renames from config/symbol_names.json (the single source of truth)
into the tree: the sibling of apply_debug_symbols.py (which only handles
`kind: func`).  A data record

    "0x800EF123": {"name": "now_example", "kind": "data", "scope": "main",
                   "size": 2, "type": "s16", "tier": "T2", "style": "sony",
                   "confidence": "HIGH", "evidence": "...; corroborated: ...;
                   reviewed:consistent (...)", "landed": false}

is landed by a whole-word, hex-case-insensitive replacement of the address name
`D_800EF123` by `now_example` in every file that carries it.  Only the identifier
token changes: every `extern` keeps its own type, array bound and qualifiers.

WHERE A `D_` NAME IS BOUND TO AN ADDRESS (everything below was found by reading
the build, 2026-10-08; a rename that missed one of them would not link):
  * the name itself.  tools/analysis/audit_main_relocs.py (sym_addr; the main
    build's fixup_relocations and audit_reloc_positions go through it) and
    tools/overlay/resolve_overlay_relocs.py (name_to_addr) read the address out
    of the `D_XXXXXXXX` suffix.  A landed name carries no suffix, so both read
    the landed-name table instead: config/symbol_names.json (landed entries,
    scope main) and the GENERATED config/main_retail_symbols.json.  This tool
    regenerates the latter and asserts every landed name is in the resolver's
    table (load_name_table, which also refuses a name landed at two addresses).
  * `@StaticAddr: D_80150164 0x80150164` (overlay statics) names the object AND
    gives its address, so a rename of the name keeps the address.
  * TU-local `static` definitions of a main object are named like any extern;
    build_level3_c._rodata_extent_ok resolves them through the same table.
  * PostProc scripts that match relocation symbol NAMES: tools/ido/*.py,
    tools/overlay/custom/*.py (renamed with the rest).
  * the PC port: pc/include/game/globals_data.h (`#define D_X (*(s32 *)(blob+K))`),
    pc/src/globals.c (definitions), pc/hal/*, src/main/pc-port/* (renamed).
    pc/tools/generate_overlay_modules.py parses overlay `extern ... D_X`
    declarations and a landed name is invisible to it: the gates list the
    overlays whose C was touched, and its regeneration step.
  * Makefile `--defsym=D_800E6644=0x800E6644` (legacy splat link) -- the name is
    renamed in place, the address stays.
  * NOT bindings (verified): asm/main/matched/*.s and asm/overlay/*.s address
    memory by number, `@RodataBase` / `@SymbolStart` hold addresses not names,
    `Makefile`/ld scripts carry no other D_ symbols.
  Address-identical ALIAS VIEWS `D_800EEE7C_v`, `D_800D07EC_b`, `D_801271B0_r`
  are NOT renamed: the suffix rule is what resolves them, and `foo_v` would not
  resolve.  They stay as they are and are listed.

INTERIOR ADDRESSES.  `D_800EE7A0` may be a field of a bigger object
(`D_800EE760 + 0x40`).  This first version renames the EXACT token of the
record's address only.  Every `D_` token whose address lies inside the object
(record `size`, else the data_access_census object at that base) is LISTED, not
renamed, for a later struct pass.  A record may carry the struct pass's plan:
    "type": "SYS_WORK", "struct": "SYS_WORK", "fields": {"0x1D9": "girl"}
(validated here, printed next to the interior tokens it names, never applied).

What it REFUSES (each printed with its reason):
  * not kind data / scope main, already landed, address outside main RAM
  * confidence below --min-tier (default HIGH); no `reviewed:consistent`
    verdict (symbol_names.py review); a `reviewed:contradicted` one
  * a Sony name (one the Psy-Q archives export, or style sony / tier T1,T2)
    without `corroborated:` evidence and a T1/T2 tier -- o.bin alone never
    names Sony data; an o.bin-sourced (T3) name without `corroborated:`
  * the address is a registered function; the name is address-shaped
  * naming_lint.py ERROR (collisions with main/overlay/landed names, a
    Sony-style name no archive exports, the 1994 style rules)
  * the new name already appears as code anywhere in the program: in a file
    the rename touches (shadowing), at column 0 of any other file (a global,
    macro or definition of that name), or in a header the touched files
    include (a conflicting declaration; e.g. Sony's include/psyq/libgs.h).
    Hits in comments and vendor trees do not count.  `--allow-decl PATH` is the
    reviewed escape (e.g. a struct FIELD of that name)
  * a touched file that is locked: git-modified (someone else is editing it),
    config/overlay/*.sym, --lock paths; `--allow-locked PATH` overrides
  * no reference in the rename scope at all (nothing would verify the name)
    unless --allow-unreferenced

REVERT.  `--revert --apply --only 0xADDR` puts `D_XXXXXXXX` back in exactly the
files the landing recorded (`landed_files` in the record).  Limits, by design:
every whole-word occurrence of the new name in those files is rewritten (a prose
mention that predates the landing too; read the diff), and the hex digits come
back in upper case.  If a later function rename MOVED a recorded file, or the
name has since been used elsewhere, the revert refuses until `--search` is given
(whole-word search over the rename scope).  A record landed by an earlier tool
has no `landed_files` and needs `--search` too.

Usage:
    python3 tools/analysis/apply_data_symbols.py --list                  # pending data records
    python3 tools/analysis/apply_data_symbols.py --dry-run               # what would land
    python3 tools/analysis/apply_data_symbols.py --apply --only 0x800EF123
    python3 tools/analysis/apply_data_symbols.py --apply --batch 10
    python3 tools/analysis/apply_data_symbols.py --gates 0x800EF123      # gate commands
    python3 tools/analysis/apply_data_symbols.py --revert --apply --only 0x800EF123
"""
import argparse
import json
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import symbol_names as SN  # noqa: E402

TEXT_EXT = {'.c', '.h', '.s', '.S', '.inc', '.cpp', '.json', '.txt', '.csv', '.py', '.sh',
            '.cfg', '.ini', '.yml', '.yaml', '.mk', '.ld', '.sym', ''}
CODE_EXT = {'.c', '.h', '.s', '.S', '.inc', '.cpp'}
MAIN_LO, MAIN_HI = 0x80010000, 0x80132000     # main image data; >= 0x80132000 is overlay-local
ALWAYS_LOCKED = ('config/overlay/', 'docs/reference/IDO52-VALIDATION.md')
HEX_D = r'D_([0-9A-Fa-f]{8})'

# Where the rename is performed.  Everything else that carries the token is
# LISTED ("left"), never silently skipped, except vendored trees and prose.
RENAME_ROOTS = ('src/', 'include/', 'asm/', 'pc/', 'config/', 'Makefile',
                'tools/ido/', 'tools/overlay/custom/')
# never renamed: vendor / third-party trees, Sony API headers, dated prose, the
# naming ledgers themselves (the record is updated by this tool, the retail
# table regenerated), per-overlay symbol maps (function names, tracked source)
VENDOR = ('pc/3rd/', 'pc/tools/.venv',
          'build/decomp-permuter/', 'tools/ido-static-recomp/', 'build/', 'disks/',
          'assets/', '.claude/', '.venv', '.git/')
PROSE_DIRS = ('docs/', 'wiki/', 'translation/')
PROSE = PROSE_DIRS + ('pc/docs/', 'CLAUDE.md', 'README.md', 'AGENTS.md', 'CONTRIBUTING.md')
LEDGERS = ('config/symbol_names.json', 'config/main_retail_symbols.json',
           'config/symbol_mapping_tracker.json', 'config/overlay/')
SONY_API_HEADER = re.compile(r'^(include/psyq/|pc/include/(lib[a-z0-9_]*|mcgui|psyz)\.h$)')
# the codebase's compile roots, for the collision scan (vendor already excluded)
CODE_ROOTS = ('src/', 'include/', 'pc/', 'asm/')
INCLUDE_DIRS = ('include', 'include/psyq', 'pc/include', 'pc/hal', 'pc/src')


def root():
    return SN.ROOT


# ------------------------------------------------------------------ git helpers
def git(*args):
    return subprocess.run(['git'] + list(args), capture_output=True, text=True, cwd=root()).stdout


def grep_files(name, ignore_case=True):
    """files carrying NAME as a whole word (tracked + untracked-not-ignored), minus
    vendor / prose trees.  -w treats `_` as a word character, so D_800EEE7C_v
    (an alias view) is a different word and is not matched."""
    args = ['grep', '--untracked', '-I', '-l', '-F', '-w']
    if ignore_case:
        args.append('-i')
    args += ['--', name]
    # filtered here, not with ':!prefix' pathspecs: one naming a path that does
    # not exist in this checkout (pc/tools/.venv-pillow) silently emptied the
    # whole result
    return sorted(x for x in git(*args).splitlines()
                  if x and not x.startswith(VENDOR + PROSE_DIRS))


def classify(path):
    """'rename' | 'left' | 'skip' for a file that carries the token."""
    if path.startswith(VENDOR) or path.endswith('.md') or path.startswith(PROSE):
        return 'skip'
    if path.startswith(LEDGERS) or SONY_API_HEADER.match(path):
        return 'skip'
    if path.startswith(RENAME_ROOTS) and os.path.splitext(path)[1] in TEXT_EXT:
        return 'rename'
    return 'left'


def locked_files(extra):
    out = set()
    for line in git('status', '--porcelain', '--untracked-files=no').splitlines():
        out.add(line[3:].strip())
    out.update(extra)
    return out


def is_locked(path, locked):
    return path in locked or any(path.startswith(p) or path == p for p in ALWAYS_LOCKED)


# ------------------------------------------------------------------ text helpers
def word_re(name, ignore_case=False):
    return re.compile(r'(?<![A-Za-z0-9_])' + re.escape(name) + r'(?![A-Za-z0-9_])',
                      re.IGNORECASE if ignore_case else 0)


def strip_comments(text, path=''):
    text = re.sub(r'/\*.*?\*/', ' ', text, flags=re.S)
    text = re.sub(r'//[^\n]*', ' ', text)
    if path.endswith(('.s', '.S')):
        text = re.sub(r'(?m)^[ \t]*#[^\n]*', ' ', text)
    return text


def read(path):
    try:
        with open(os.path.join(root(), path), encoding='utf-8') as fh:
            return fh.read()
    except (UnicodeDecodeError, FileNotFoundError):
        return None


def rename_in_file(path, old, new, ignore_case=True):
    s = read(path)
    if s is None:
        return 0
    s2, n = word_re(old, ignore_case).subn(new, s)
    if n:
        with open(os.path.join(root(), path), 'w', encoding='utf-8') as fh:
            fh.write(s2)
    return n


def code_hits(path, name):
    """(any code occurrence, column-0 code occurrence) of NAME in PATH, comments stripped."""
    s = read(path)
    if s is None:
        return False, False
    code = strip_comments(s, path)
    rx = word_re(name)
    anywhere = bool(rx.search(code))
    col0 = any(rx.search(line) for line in code.splitlines() if line and not line[0].isspace())
    return anywhere, col0


INCLUDE_RE = re.compile(r'^\s*#\s*include\s*[<"]([^>"]+)[>"]', re.M)


def include_closure(paths):
    """headers (repo-relative) reachable from PATHS through #include, using the
    build's search order (the includer's directory, include/, include/psyq/,
    pc/include/ ...)."""
    seen, todo = set(), list(paths)
    while todo:
        p = todo.pop()
        s = read(p)
        if s is None:
            continue
        for inc in INCLUDE_RE.findall(s):
            for d in (os.path.dirname(p),) + INCLUDE_DIRS:
                cand = os.path.normpath(os.path.join(d, inc))
                if os.path.exists(os.path.join(root(), cand)):
                    if cand not in seen:
                        seen.add(cand)
                        todo.append(cand)
                    break
    return seen


# --------------------------------------------------------------- census / tokens
_TOKENS = None


def d_token_index():
    """{address: reference count} of every D_XXXXXXXX token in the C/headers the
    builds read (main, overlays, headers, PC port): the universe interior
    addresses are drawn from."""
    global _TOKENS
    if _TOKENS is None:
        out = git('grep', '--untracked', '-I', '-h', '-o', '-i', '-E', '-w', HEX_D, '--',
                  'src', 'include', 'pc/src', 'pc/hal', 'pc/include')
        cnt = {}
        for t in out.split():
            a = int(t[2:], 16)
            cnt[a] = cnt.get(a, 0) + 1
        _TOKENS = cnt
    return _TOKENS


_CENSUS = None


def census():
    global _CENSUS
    if _CENSUS is None:
        _CENSUS = {'objects': {}, 'addresses': {}}
        p = os.path.join(root(), 'build/analysis/data_access_census.json')
        try:
            with open(p) as fh:
                d = json.load(fh)
            _CENSUS['objects'] = {int(o['base'], 16): o for o in d['objects']}
            _CENSUS['addresses'] = d['addresses']
        except (OSError, ValueError, KeyError):
            pass
    return _CENSUS


def object_extent(addr, e):
    """(extent in bytes, where it came from).  Record size wins; else the census
    object that BEGINS at addr; else unknown (0)."""
    if (e.get('size') or 0) > 0:
        return int(e['size']), 'record size'
    o = census()['objects'].get(addr)
    if o and o.get('size'):
        return int(o['size']), 'census object'
    return 0, 'unknown'


def parse_fields(e, extent):
    """validate the optional struct-pass plan; returns ({offset: name}, problems)."""
    probs, out = [], {}
    f = e.get('fields')
    if f is None:
        return out, probs
    if not isinstance(f, dict):
        return out, ['`fields` must be a map {"0xOFF": "name"}']
    for k, v in f.items():
        try:
            off = int(k, 16) if isinstance(k, str) else int(k)
        except ValueError:
            probs.append(f'field offset {k!r} is not hex')
            continue
        if not isinstance(v, str) or not re.match(r'^[A-Za-z_][A-Za-z0-9_]*$', v):
            probs.append(f'field {k}: {v!r} is not an identifier')
            continue
        if extent and off >= extent:
            probs.append(f'field {k} {v}: offset beyond the object extent 0x{extent:X}')
            continue
        out[off] = v
    return out, probs


# ------------------------------------------------------------------ the plan
_LINT = None


def lint_index():
    global _LINT
    if _LINT is None:
        import naming_lint as NL
        _LINT = (NL, NL.Index())
    return _LINT


def lint_findings(name, addr):
    NL, idx = lint_index()
    return NL.lint(name, addr, 'data', None, idx), (name in idx.sony)


def sony_tier_ok(e, ev):
    return e.get('tier') in ('T1', 'T2') and 'corroborated:' in ev


def plan(key, e, funcs_by_addr, func_names, locked, args):
    """returns dict(ok, reasons, warnings, rename, left, interior, aliases, ...)."""
    new = e['name']
    addr = int(key, 16)
    old = f'D_{addr:08X}'
    reasons, warns = [], []
    ev = e.get('evidence', '') if isinstance(e.get('evidence', ''), str) else json.dumps(e.get('evidence'))
    if e.get('landed'):
        reasons.append('already landed')
    if e.get('scope', 'main') != 'main':
        reasons.append(f'scope {e.get("scope")!r}: overlay-local data is not landed by this tool')
    if not (MAIN_LO <= addr < MAIN_HI):
        reasons.append(f'address outside main data RAM 0x{MAIN_LO:08X}-0x{MAIN_HI:08X}')
    conf = e.get('confidence')
    if conf == 'DERIVED':
        if 'DERIVED' not in args.tiers:
            reasons.append('confidence DERIVED below --min-tier')
        if e.get('tier') != 'T4' or e.get('style') != 'derived':
            reasons.append('DERIVED confidence requires a T4 derived-style candidate')
    elif conf not in args.tiers:
        reasons.append(f'confidence {conf} below --min-tier')
    if args.require_review and 'reviewed:consistent' not in ev:
        reasons.append('no `reviewed:consistent` verdict (symbol_names.py review)')
    if 'reviewed:contradicted' in ev:
        reasons.append('review CONTRADICTED the name')
    if SN.is_address_name(new) or re.search(r'_[0-9A-Fa-f]{8}(_[a-z]{1,3})?$', new):
        reasons.append('the new name is address-shaped: the resolvers would read an address out of it')
    if new in func_names:
        reasons.append(f'new name {new} is a registered function at {SN.akey(func_names[new])}')
    if addr in funcs_by_addr:
        reasons.append(f'0x{addr:08X} is the registered function {funcs_by_addr[addr]}, not data')
    # lint (collisions with names, the 1994 style, Sony-name existence)
    findings, is_sony = lint_findings(new, addr)
    for lv, rule, msg in findings:
        if lv == 'ERROR':
            reasons.append(f'naming_lint ERROR [{rule}]: {msg}')
        elif lv == 'WARN':
            warns.append(f'naming_lint WARN [{rule}]: {msg}')
    # evidence tiers
    if is_sony or e.get('style') == 'sony' or e.get('tier') in ('T1', 'T2'):
        if not sony_tier_ok(e, ev):
            reasons.append('Sony data name needs tier T1/T2 and `corroborated:` evidence '
                           '(o.bin alone never names Sony data)')
        if not is_sony:
            reasons.append('T1/T2/sony record whose name no Psy-Q archive on disk exports')
    elif e.get('tier') == 'T3' or e.get('style') == 'obin':
        if 'corroborated:' not in ev:
            reasons.append('o.bin-sourced (T3) name needs `corroborated:` evidence '
                           '(structural + semantic), never o.bin position alone')
    # struct-pass plan
    ext, ext_src = object_extent(addr, e)
    fields, fprobs = parse_fields(e, ext)
    reasons += [f'fields: {p}' for p in fprobs]
    # references
    carriers = grep_files(old)
    rename = [f for f in carriers if classify(f) == 'rename']
    left = [f for f in carriers if classify(f) == 'left']
    if not rename and not args.allow_unreferenced:
        reasons.append(f'no reference to {old} in the rename scope: nothing would verify the name '
                       '(--allow-unreferenced to land the record anyway)')
    # collisions as CODE
    touched = set(rename)
    hits = []
    for f in grep_files(new, ignore_case=False):
        if f in args.allow_decl or classify(f) == 'skip' and not SONY_API_HEADER.match(f):
            continue
        if not f.startswith(CODE_ROOTS) or os.path.splitext(f)[1] not in CODE_EXT:
            continue
        anywhere, col0 = code_hits(f, new)
        if f in touched:
            if anywhere:
                hits.append((f, 'in a file this rename touches (shadowing / double definition)'))
        elif f.endswith('.h') or f.endswith('.inc'):
            hits.append((f, 'header'))
        elif col0:
            hits.append((f, 'file-scope declaration/definition/macro'))
    hdrs = [h for h, why in hits if why == 'header']
    if hdrs:
        closure = set()
        for f in rename:
            if os.path.splitext(f)[1] in CODE_EXT:
                closure |= include_closure([f])
        for h in hdrs:
            if h in closure or h in touched:
                reasons.append(f'header {h} declares {new} and is included by a touched file')
            else:
                warns.append(f'header {h} also mentions {new} (not included by any touched file)')
    for f, why in hits:
        if why != 'header':
            reasons.append(f'{new} already used as code in {f}: {why}')
    lock_hits = [f for f in rename if is_locked(f, locked) and f not in args.allow_locked]
    if lock_hits:
        reasons.append(f'touches locked files: {lock_hits[:3]}{"..." if len(lock_hits) > 3 else ""}')
    # interior tokens / alias views (listed, never renamed)
    interior = []
    if ext > 1:
        for a, n in sorted(d_token_index().items()):
            if addr < a < addr + ext:
                interior.append((a, n, fields.get(a - addr)))
    aliases = sorted(set(git('grep', '--untracked', '-I', '-h', '-o', '-i', '-E', '-w',
                             old + '_[A-Za-z0-9_]+', '--', 'src', 'include', 'pc/src',
                             'pc/hal', 'pc/include').split()))
    o = census()['addresses'].get(old)
    if o and o.get('object') and int(o['object'], 16) != addr:
        warns.append(f'census places {old} inside the object at {o["object"]} (+0x{o.get("offset", 0):X}): '
                     'an interior field; the exact token is still renamed')
    return {'ok': not reasons, 'reasons': reasons, 'warnings': warns, 'old': old, 'rename': rename,
            'left': left, 'interior': interior, 'aliases': aliases, 'extent': ext, 'extent_src': ext_src,
            'fields': fields}


# ------------------------------------------------------------------ gates
def gate_targets(files):
    """(main function names, overlay 'OV/func' ids, overlays, pc-port files) among FILES."""
    reg = {f['source']: f['name'] for f in SN.load_registry()['functions']}
    mains, ovs, ovl, pcp = [], [], set(), []
    for f in sorted(files):
        m = re.match(r'^src/overlay/([^/]+)/([^/]+)\.c$', f)
        if m:
            ovs.append(f'{m.group(1)}/{m.group(2)}')
            ovl.add(m.group(1))
        elif f in reg:
            mains.append(reg[f])
        elif f.startswith(('src/main/pc-port/', 'pc/')):
            pcp.append(f)
    return mains, ovs, sorted(ovl), pcp


def gates(entries, overlays=None):
    """entries: [(addr_key, new_name, [files])]"""
    files = sorted({f for _, _, fs in entries for f in fs})
    mains, ovs, ovl, pcp = gate_targets(files)
    ovl = sorted(set(ovl) | set(overlays or ()))
    print('Batch gates (run all, in the venv; serial overlay verifies):')
    print('  python3 tools/analysis/symbol_names.py check')
    print('  python3 tools/analysis/symbol_names.py gen-retail --check')
    print('  python3 tools/build/build_level3_c.py                  # retail SHA1 2d8d4748..., 0 mismatched')
    print('  python3 tools/analysis/audit_main_relocs.py            # 0 MISMATCH, 0 UNRESOLVED')
    if mains:
        print(f'  python3 tools/analysis/audit_reloc_positions.py {" ".join(mains)}')
    print('  python3 tools/analysis/audit_registry_overlaps.py      # clean')
    for fn in mains:
        print(f'  python3 tools/analysis/audit_plain_vs_shipped.py --only {fn}   # SAME')
    for fn in mains:
        print(f'  python3 tools/analysis/prove_equivalence.py --only {fn}        # PROVEN')
    for o in ovs:
        print(f'  python3 tools/analysis/audit_plain_vs_shipped.py --only {o}   # SAME')
        print(f'  python3 tools/analysis/prove_equivalence.py --only {o}        # PROVEN')
    print('  python3 tools/overlay/resolve_overlay_relocs.py --check-name-table')
    for ov in ovl:
        print(f'  python3 tools/overlay/verify_overlay_bytes.py {ov} && '
              f'python3 tools/overlay/apply_resolved_status.py {ov} --demote   # 100%, no status change')
    if ovl:
        print('  python3 pc/tools/generate_overlay_modules.py             # PC overlay modules parse D_ externs')
    print('  make check-raw')
    print('  make check-clean-main                                    # if feasible')
    print('  make -f pc/Makefile                                      # PC unit tests')
    for key, name, _ in entries:
        print(f'  git grep -n -i -w D_{int(key, 16):08X}                         # zero survivors ({name})')
    if pcp:
        print(f'  (PC-port / pc files renamed too: {len(pcp)})')


# ------------------------------------------------------------------ apply
def survivors(old):
    return [f for f in grep_files(old) if classify(f) == 'rename']


def print_plan(key, e, p, apply_mode, verbose=True):
    head = f'{"LAND  " if apply_mode else "WOULD "} {key} {p["old"]} -> {e["name"]} [{e.get("confidence")}, ' \
           f'{e.get("tier", "-")}]: {len(p["rename"])} files'
    print(head)
    if verbose and not apply_mode:
        for f in p['rename']:
            print(f'          {f}')
    for w in p['warnings']:
        print(f'          WARN {w}')
    if p['left']:
        print(f'          left (outside the rename scope, {len(p["left"])}): {p["left"][:6]}'
              f'{"..." if len(p["left"]) > 6 else ""}')
    if p['aliases']:
        print(f'          alias views NOT renamed (suffix rule resolves them): {p["aliases"]}')
    if p['interior']:
        print(f'          INTERIOR D_ tokens inside 0x{int(key, 16):08X}+0x{p["extent"]:X} '
              f'({p["extent_src"]}) -- listed for the struct pass, NOT renamed:')
        for a, n, fld in p['interior'][:12]:
            print(f'              D_{a:08X}  +0x{a - int(key, 16):X}  {n} refs'
                  f'{"  -> field " + fld if fld else ""}')
        if len(p['interior']) > 12:
            print(f'              ... {len(p["interior"]) - 12} more')
    elif p['extent_src'] != 'unknown':
        print(f'          no interior tokens in the {p["extent"]}-byte extent ({p["extent_src"]})')
    else:
        print('          extent unknown (no record size, no census object): interior scan skipped; '
              f'`data_access_census.py --object {key}`')


def finish(doc, landed_entries, args):
    """write ledgers: symbol_names.json, regenerated main_retail_symbols.json; verify the resolver table."""
    SN.save_names(doc)
    with open(SN.RETAIL, 'w') as fh:
        fh.write(json.dumps(SN.gen_retail(doc), indent=1))
    if getattr(args, 'no_resolver_check', False):
        return
    sys.path.insert(0, os.path.join(root(), 'tools', 'overlay'))
    import resolve_overlay_relocs as R  # noqa: E402
    tab = R.load_name_table(force=True)       # raises NameTableError on any inconsistency
    for key, name in landed_entries:
        if name in tab:
            assert tab[name] == int(key, 16), f'{name} resolves to 0x{tab[name]:08X}, not {key}'
    print(f'resolver name table OK ({len(tab)} names)')


def landed_main(args):
    doc = SN.load_names()
    reg = SN.load_registry()
    func_names = {f['name']: int(f['address'], 16) for f in reg['functions']}
    funcs_by_addr = {}
    for f in reg['functions']:
        funcs_by_addr[int(f['address'], 16)] = f['name']
    locked = locked_files(args.lock)
    todo, refused = [], []
    for key, e in doc['symbols'].items():
        if e.get('kind') != 'data' or e.get('landed'):
            continue
        if args.only and key.upper() not in [o.upper() for o in args.only]:
            continue
        if not args.only and e.get('confidence') not in args.tiers:
            continue
        p = plan(key, e, funcs_by_addr, func_names, locked, args)
        (todo if p['ok'] else refused).append((key, e, p))
    for key, e, p in refused:
        print(f'REFUSE {key} {p["old"]} -> {e["name"]} [{e.get("confidence")}]')
        for r in p['reasons']:
            print(f'        - {r}')
        for w in p['warnings']:
            print(f'        . {w}')
    todo = todo[:args.batch]
    landed, ovs, report = [], set(), []
    for key, e, p in todo:
        ovs |= {f.split('/')[2] for f in p['rename'] if f.startswith('src/overlay/') and f.count('/') >= 3}
        print_plan(key, e, p, args.apply)
        if not args.apply:
            report.append((key, e['name'], p['rename']))
            continue
        touched, nrep = [], 0
        for f in p['rename']:
            n = rename_in_file(f, p['old'], e['name'])
            if n:
                touched.append(f)
                nrep += n
        left = survivors(p['old'])
        if left:
            print(f'          SURVIVOR {p["old"]}: {left}')
        e['landed'] = True
        e['date'] = SN.TODAY
        e.setdefault('from', 'symbol_names.json (apply_data_symbols.py)')
        e['landed_files'] = sorted(touched)
        e['evidence'] = e.get('evidence', '') + \
            f'; landed {SN.TODAY} by apply_data_symbols.py ({len(touched)} files, {nrep} references)'
        landed.append((key, e['name']))
        report.append((key, e['name'], touched))
    if args.apply and landed:
        finish(doc, landed, args)
        print(f'landed {len(landed)}; symbol_names.json and main_retail_symbols.json updated')
        gates(report, ovs)
    elif not todo:
        print('nothing landable' + (' (dry run)' if args.dry_run else ''))
    elif args.dry_run:
        gates(report, ovs)
    return 0


# ------------------------------------------------------------------ revert
def revert_main(args):
    """Undo a LANDED data name: NEW -> D_ADDR in exactly the files the landing
    touched (record.landed_files); a record landed by an earlier tool has none
    and needs --search (whole-word, case-sensitive, in the rename scope)."""
    if not args.only:
        sys.exit('--revert needs --only 0xADDR (explicit: a data revert is never automatic)')
    doc = SN.load_names()
    locked = set(args.lock) if args.no_git_lock else locked_files(args.lock)
    done, report = [], []
    for key, e in doc['symbols'].items():
        if e.get('kind') != 'data' or not e.get('landed'):
            continue
        if key.upper() not in [o.upper() for o in args.only]:
            continue
        new, old = e['name'], f'D_{int(key, 16):08X}'
        files = e.get('landed_files')
        if not files:
            if not args.search:
                print(f'REFUSE {key} {new}: no landed_files recorded (landed by an earlier tool); '
                      '--search to rediscover the files by whole-word search')
                continue
            files = [f for f in grep_files(new, ignore_case=False) if classify(f) == 'rename']
        else:
            # a later function rename can MOVE a landed file (src/main/matched/OLD.c ->
            # NEW.c): then the recorded path is gone and the name lives elsewhere.
            missing = [f for f in files if not os.path.exists(os.path.join(root(), f))]
            extra = [f for f in grep_files(new, ignore_case=False)
                     if classify(f) == 'rename' and f not in files]
            if missing or extra:
                print(f'NOTE {key} {new}: {len(missing)} recorded file(s) missing {missing[:4]}, '
                      f'{len(extra)} other file(s) carry the name {extra[:4]}')
                if not args.search:
                    print('REFUSE: re-run with --search to revert the files found by whole-word search '
                          '(review the list: the name may also appear in prose)')
                    continue
                files = sorted((set(files) - set(missing)) | set(extra))
        bad = [f for f in files if is_locked(f, locked) and f not in args.allow_locked]
        if bad:
            print(f'REFUSE {key} {new}: touches locked files {bad[:3]}')
            continue
        print(f'{"REVERT" if args.apply else "WOULD "} {key} {new} -> {old}: {len(files)} files')
        if not args.apply:
            for f in files:
                print(f'          {f}')
            continue
        nrep = sum(rename_in_file(f, new, old, ignore_case=False) for f in files)
        e['landed'] = False
        e.pop('landed_files', None)
        if str(e.get('from', '')).startswith('symbol_names.json (apply_data_symbols.py'):
            e.pop('from')
        if args.refuted:
            e['confidence'] = 'REVERTED'
        e['reverted'] = {'date': SN.TODAY, 'by': 'apply_data_symbols.py --revert', 'from': new,
                         'files': len(files), 'references': nrep}
        e['evidence'] = e.get('evidence', '') + f'; reverted {SN.TODAY} to {old} ({len(files)} files)'
        left = [f for f in grep_files(new, ignore_case=False) if classify(f) == 'rename']
        if left:
            print(f'          SURVIVOR {new}: {left}')
        done.append((key, new))
        report.append((key, new, files))
    if args.apply and done:
        finish(doc, [], args)
        print(f'reverted {len(done)}; symbol_names.json and main_retail_symbols.json updated')
        gates([(k, f'D_{int(k, 16):08X}', fs) for k, _, fs in report])
    return 0


def list_main(args):
    doc = SN.load_names()
    rows = [(k, e) for k, e in doc['symbols'].items() if e.get('kind') == 'data']
    for k, e in rows:
        ev = e.get('evidence', '')
        print(f'{k}  {"LANDED" if e.get("landed") else "pending"}  {e.get("confidence"):8} {e.get("tier", "-"):3} '
              f'{e["name"]:24} review={"consistent" if "reviewed:consistent" in ev else "-"}')
    print(f'{len(rows)} data records, {sum(1 for _, e in rows if e.get("landed"))} landed')
    return 0


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument('--dry-run', action='store_true')
    g.add_argument('--apply', action='store_true')
    g.add_argument('--list', action='store_true', help='list the data records and their state')
    g.add_argument('--gates', nargs='*', metavar='0xADDR',
                   help='print the gate commands for landed data records (all landed-by-this-tool ones if none given)')
    ap.add_argument('--revert', action='store_true', help='with --dry-run/--apply and --only: undo a landed data name')
    ap.add_argument('--refuted', action='store_true', help='--revert: mark the record REVERTED (name refuted)')
    ap.add_argument('--search', action='store_true',
                    help='--revert of a record that has no landed_files, or whose files moved: rediscover the '
                         'files by whole-word search')
    ap.add_argument('--no-git-lock', action='store_true',
                    help='--revert: do not treat git-modified files as locked (a landing not yet committed '
                         'leaves its own files modified); --lock still applies')
    ap.add_argument('--min-tier', default='HIGH', choices=['HIGH', 'MEDIUM', 'DERIVED'])
    ap.add_argument('--no-require-review', action='store_true', help='land on the matcher tier alone (not recommended)')
    ap.add_argument('--only', action='append', default=[], help='address key 0x800EF6BF (repeatable)')
    ap.add_argument('--batch', type=int, default=10)
    ap.add_argument('--lock', action='append', default=[], help='extra path prefixes to treat as locked')
    ap.add_argument('--allow-locked', action='append', default=[], metavar='PATH',
                    help='exact path of a git-modified file the rename may still edit (reviewed by hand)')
    ap.add_argument('--allow-decl', action='append', default=[], metavar='PATH',
                    help='exact path whose existing mention of the NEW name is reviewed as harmless '
                         '(a struct field of that name, a stale prototype of this same object)')
    ap.add_argument('--allow-unreferenced', action='store_true',
                    help='land a record with no D_ reference in the rename scope')
    ap.add_argument('--no-resolver-check', action='store_true', help=argparse.SUPPRESS)
    args = ap.parse_args()
    args.tiers = {'HIGH'}
    if args.min_tier in ('MEDIUM', 'DERIVED'):
        args.tiers.add('MEDIUM')
    if args.min_tier == 'DERIVED':
        args.tiers.add('DERIVED')
    args.require_review = not args.no_require_review
    if args.list:
        return list_main(args)
    if args.gates is not None:
        doc = SN.load_names()
        sel = [k for k, e in doc['symbols'].items() if e.get('kind') == 'data' and e.get('landed_files')
               and (not args.gates or k.upper() in [a.upper() for a in args.gates])]
        gates([(k, doc['symbols'][k]['name'], doc['symbols'][k]['landed_files']) for k in sel])
        return 0
    if args.revert:
        return revert_main(args)
    return landed_main(args)


if __name__ == '__main__':
    sys.exit(main())
