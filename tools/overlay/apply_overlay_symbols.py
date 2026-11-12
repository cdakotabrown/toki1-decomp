#!/usr/bin/env python3
"""Land OVERLAY function names from config/symbol_names.json into the tree.

The overlay sibling of tools/analysis/apply_debug_symbols.py (which cannot: it
always locks config/overlay/, and the overlay resolver used to read a symbol's
address off its `func_<ADDR>` name).  Names are recorded per overlay,

    "RENSYU:0x80132040": {"name": "<ov>_main", "kind": "func", "scope": "RENSYU", ...}

the address being the prologue address
the function's `func_<ADDR>` name carried.  Every overlay loads at 0x80132000, so
the same name or address may exist in several overlays: every rename here is
scoped to ONE overlay and never touches another overlay or the main binary.

What a landing rewrites (whole-word, both hex cases of the address name):

  * src/overlay/<OV>/            every file: definition, externs, calls, comments;
                                 the `<old>.c` file is renamed
  * asm/overlay/<OV>/            the disassembly `<old>.s` (renamed) and its text:
                                 build_clean_exn assembles it when C falls back
  * config/overlay/<OV>.sym      the name field of the entry (order and comments kept)
  * config/*.json|txt            the overlay-scoped records: overlay_match_status,
                                 clean_build_debt, overlay_conversion_status,
                                 overlay_decomp_categories, overlay_baseline_counts,
                                 paint_dataflow, fragment_asm_only, gamewide_blockers,
                                 c_unmatchable.txt, ... (only where the context says
                                 this overlay: a key `<OV>`, an `overlay: <OV>` record,
                                 or an `<OV>/<name>` / `<OV>::<name>` / path token)
  * pc/                          pc/src/overlay_<OV>.c, overlay_inits/overlay_init_<OV>.c,
                                 pc/src/overlay/<OV>/ (the generated and hand-written
                                 modules: `<OV>_func_<ADDR>` becomes `<OV>_<name>`)
  * wiki/notes/<OV>/, wiki/manifest.json
  * config/symbol_names.json     landed = true

and the resolver follows with no further step: resolve_overlay_relocs reads the
overlay-scoped names through tools/overlay/overlay_names.py.

What it REFUSES (each printed with its reason):
  * confidence below --min-tier, or no `reviewed:consistent` verdict (T4 needs
    tier T4, style derived, confidence DERIVED and --min-tier DERIVED)
  * a record whose `func_<ADDR>` is not (or no longer) in <OV>.sym, unless
    the record says `internal_of: <window>`: a function the C defines inside
    that window (merged-symbol window); then the window must contain the
    address and some src/overlay/<OV>/*.c must define `func_<ADDR>`
  * a new name that is not a clean identifier / is address-shaped, that another
    record of the same overlay holds, that is a main name (landed record,
    registry, src/main file) or is already an identifier in the overlay's tree,
    in src/main/ or in include/
  * naming_lint.py ERROR (use --no-lint only for fixtures)
  * any file the rename would touch that is git-modified (someone else is
    editing it) unless --allow-locked PATH; --lock PATH adds locks;
    --no-git-lock locks nothing but --lock paths (multi-batch runs)
  * a target file that already exists

Usage:
    python3 tools/overlay/apply_overlay_symbols.py --dry-run --ov RENSYU
    python3 tools/overlay/apply_overlay_symbols.py --apply --only RENSYU:0x80132040
    python3 tools/overlay/apply_overlay_symbols.py --apply --ov RENSYU --batch 40 --min-tier DERIVED
    python3 tools/overlay/apply_overlay_symbols.py --gates --ov RENSYU          # print the gate commands
    python3 tools/overlay/apply_overlay_symbols.py --run-gates --ov RENSYU     # run them, serially
    python3 tools/overlay/apply_overlay_symbols.py --revert --apply --ov RENSYU --any-verdict
    python3 tools/overlay/apply_overlay_symbols.py --revert --apply --only RENSYU:0x80132040 --any-verdict --forget

--revert puts a landed name back to `func_<ADDR>` (a refuted or corrected name);
the record is kept as `confidence: REVERTED` unless --forget drops it.
Commit with `git commit -F msg -- <paths>`; --apply prints the touched paths.
"""
import argparse
import collections
import glob
import json
import os
import re
import shutil
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, os.path.join(ROOT, 'tools', 'analysis'))
sys.path.insert(0, HERE)
import symbol_names as SN   # noqa: E402
import overlay_names as ON  # noqa: E402

IDENT_RE = re.compile(r'^[A-Za-z_][A-Za-z0-9_]*$')
ADDR_NAME_RE = ON.NAME_ADDR_RE
FUNC_ADDR_RE = ON.FUNC_NAME_RE
OV_PAT = r'[A-Z][A-Z0-9_]*'

# (regex, group index of the overlay) -> a path that belongs to ONE overlay
SCOPED_PATHS = [
    re.compile(r'^src/overlay/(' + OV_PAT + r')/'),
    re.compile(r'^asm/overlay/(' + OV_PAT + r')/'),
    re.compile(r'^wiki/notes/(' + OV_PAT + r')/'),
    re.compile(r'^pc/src/overlay/(' + OV_PAT + r')/'),
    re.compile(r'^pc/src/overlay_(' + OV_PAT + r')\.c$'),
    re.compile(r'^pc/src/overlay_inits/overlay_init_(' + OV_PAT + r')\.c$'),
]
CONFIG_STRUCTURED = re.compile(r'^(config/[^/]+\.(json|txt|csv)|wiki/manifest\.json)$')
NEVER_TOUCH = ('config/symbol_names.json', 'config/main_retail_symbols.json',
               'config/symbol_mapping_tracker.json')
PC_PREFIXED = re.compile(r'^pc/')
TEXT_EXT = {'.c', '.h', '.s', '.md', '.txt', '.json', '.inc', '.cpp', ''}

# file moves for a rename of OLD -> NEW in overlay OV
MOVE_TEMPLATES = (('src/overlay/{ov}/{n}.c'), ('asm/overlay/{ov}/{n}.s'),
                  ('wiki/notes/{ov}/{n}.md'), ('pc/src/overlay/{ov}/{n}.c'))


# ------------------------------------------------------------------ helpers
def git(*args):
    return subprocess.run(['git'] + list(args), capture_output=True, text=True, cwd=ROOT).stdout


def rel(path):
    return os.path.relpath(path, ROOT)


def read(path):
    with open(os.path.join(ROOT, path), encoding='utf-8', newline='') as fh:
        return fh.read()


def write(path, text):
    with open(os.path.join(ROOT, path), 'w', encoding='utf-8', newline='') as fh:
        fh.write(text)


def exists(path):
    return os.path.exists(os.path.join(ROOT, path))


def locked_files(extra, no_git):
    out = set(extra)
    if not no_git:
        for line in git('status', '--porcelain', '--untracked-files=no').splitlines():
            p = line[3:].strip()
            if ' -> ' in p:
                p = p.split(' -> ')[-1]
            out.add(p)
    return out


def is_address_name(n):
    return bool(FUNC_ADDR_RE.match(n)) or bool(ADDR_NAME_RE.match(n))


def evidence_text(e):
    ev = e.get('evidence', '')
    if isinstance(ev, str):
        return ev
    parts = []
    for x in ev:
        parts.append(x if isinstance(x, str) else json.dumps(x, sort_keys=True))
    if e.get('review'):
        parts.append(f"review:{e['review']}")
    return '; '.join(parts)


def grep_files(word, ignore_case):
    """tracked + untracked-unignored text files containing WORD as a whole word."""
    args = ['grep', '--untracked', '-I', '-l', '-w', '-F']
    if ignore_case:
        args.append('-i')
    return [x for x in git(*args, '--', word).splitlines() if x]


def scope_of(path):
    for rx in SCOPED_PATHS:
        m = rx.match(path)
        if m:
            return m.group(1)
    return None


def pair_regex(pairs):
    """one alternation over the OLD names of PAIRS ({old: new}); address names
    match in either hex case, real names exactly."""
    ci = {o.lower(): n for o, n in pairs.items() if is_address_name(o)}
    cs = {o: n for o, n in pairs.items() if not is_address_name(o)}
    return ci, cs


def sub_words(text, pairs, prefix=''):
    """whole-word replacement of every OLD in PAIRS (case-insensitive for address
    names).  `prefix` ('RENSYU_' in the PC modules) is an optional leading part
    that is kept: `RENSYU_func_X` -> `RENSYU_new`.  In the PC modules a static
    function's name also carries its host window's address as a suffix
    (`func_X_801350A8`, generate_overlay_modules.py); that suffix is kept too."""
    ci, cs = pair_regex(pairs)
    n = 0
    lead = r'(?<![A-Za-z0-9_])' + (r'(' + re.escape(prefix) + r')?' if prefix else '()')
    tail = (r'((?:_[0-9A-Fa-f]{8})?)' if prefix else r'()') + r'(?![A-Za-z0-9_])'
    if ci:
        rx = re.compile(lead + '(' + '|'.join(re.escape(o) for o in sorted(ci, key=len, reverse=True)) +
                        r')' + tail, re.I)

        def f(m):
            nonlocal n
            n += 1
            return (m.group(1) or '') + ci[m.group(2).lower()] + m.group(3)
        text = rx.sub(f, text)
    if cs:
        rx = re.compile(lead + '(' + '|'.join(re.escape(o) for o in sorted(cs, key=len, reverse=True)) +
                        r')' + tail)

        def g(m):
            nonlocal n
            n += 1
            return (m.group(1) or '') + cs[m.group(2)] + m.group(3)
        text = rx.sub(g, text)
    return text, n


# ------------------------------------------------------------ JSON context walk
def json_format(text, obj):
    """The (indent, ensure_ascii, sort_keys, trailing newline) that reproduces TEXT."""
    for ind in (2, 1, 4):
        for ea in (True, False):
            for sk in (False, True):
                for nl in ('', '\n'):
                    if json.dumps(obj, indent=ind, ensure_ascii=ea, sort_keys=sk) + nl == text:
                        return ind, ea, sk, nl
    return None


class Walker:
    """Rename in a parsed JSON tree, only where the context says the record
    belongs to the overlay: a dict key equal to the overlay name (the status /
    debt files), an `overlay: <OV>` field (per-function records), or an
    unambiguous token (`<OV>/<name>`, `<OV>:<name>`, `<OV>::<name>`, `src/overlay/<OV>/<name>.c`
    ...).  PAIRS is {ov: {old: new}}."""

    def __init__(self, pairs_by_ov):
        self.pairs = pairs_by_ov
        self.count = 0
        self.tok = {}
        for ov, pairs in pairs_by_ov.items():
            ci, cs = pair_regex(pairs)
            alts = [(o, n, True) for o, n in ci.items()] + [(o, n, False) for o, n in cs.items()]
            self.tok[ov] = (ci, cs)

    def _tok_sub(self, s, ov):
        if ov not in s:
            return s
        ci, cs = self.tok[ov]
        o = re.escape(ov)
        pre = (r'(?<![A-Za-z0-9_])(?:' + o + r'(?:/|::|:)|(?:src/overlay|asm/overlay|wiki/notes|pc/src/overlay)/' +
               o + r'/)')
        for table, flags in ((ci, re.I), (cs, 0)):
            if not table:
                continue
            rx = re.compile('(' + pre + ')(' + '|'.join(re.escape(x) for x in sorted(table, key=len, reverse=True)) +
                            r')(?![A-Za-z0-9_])', flags)

            def f(m, table=table, flags=flags):
                self.count += 1
                k = m.group(2).lower() if flags else m.group(2)
                return m.group(1) + table[k]
            s = rx.sub(f, s)
        return s

    def rs(self, s, ctx):
        for ov in self.pairs:
            s = self._tok_sub(s, ov)
        if ctx in self.pairs:
            ci, cs = self.tok[ctx]
            k = s.lower()
            if k in ci and is_address_name(s):
                self.count += 1
                return ci[k]
            if s in cs:
                self.count += 1
                return cs[s]
        return s

    def _legacy_pc_key(self, key, record):
        """Suffix-qualified PC keys need the record's source as scope proof."""
        if not isinstance(record, dict):
            return key
        for ov, pairs in self.pairs.items():
            for old, new in pairs.items():
                if (key == old + '_' + ov and
                        record.get('ps1_source') in
                        (f'src/overlay/{ov}/{old}.c', f'src/overlay/{ov}/{new}.c')):
                    self.count += 1
                    return new + '_' + ov
        return key

    def walk(self, node, ctx):
        if isinstance(node, dict):
            local = ctx
            f = node.get('overlay')
            if isinstance(f, str):
                local = f
            out = {}
            for k, v in node.items():
                kctx = k if k in self.pairs else local
                out[self.rs(self._legacy_pc_key(k, v), local)] = self.walk(v, kctx)
            return out
        if isinstance(node, list):
            return [self.walk(x, ctx) for x in node]
        if isinstance(node, str):
            return self.rs(node, ctx)
        return node


def dup_pairs_hook(pairs):
    seen = set()
    for k, _ in pairs:
        if k in seen:
            raise ValueError('duplicate key ' + k)
        seen.add(k)
    return dict(pairs)


def rewrite_config(path, pairs_by_ov):
    """New text of a config file after the renames, or None when nothing in it
    belongs to those overlays.  JSON is re-dumped in the exact format it was
    read in (refused if that cannot be reproduced); text files get the token
    rule only."""
    text = read(path)
    if path.endswith('.json'):
        try:
            obj = json.loads(text, object_pairs_hook=dup_pairs_hook)
        except ValueError:
            return special_json(path, text, pairs_by_ov)
        fmt = json_format(text, obj)
        if fmt is None:
            raise RuntimeError(f'{path}: cannot reproduce its JSON formatting; refusing to reformat it')
        w = Walker(pairs_by_ov)
        new = w.walk(obj, None)
        if not w.count:
            return None
        ind, ea, sk, nl = fmt
        return json.dumps(new, indent=ind, ensure_ascii=ea, sort_keys=sk) + nl
    w = Walker(pairs_by_ov)
    lines = text.split('\n')
    changed = False
    for i, ln in enumerate(lines):
        s = ln
        for ov in pairs_by_ov:
            s = w._tok_sub(s, ov)
        if s != ln:
            lines[i] = s
            changed = True
    return '\n'.join(lines) if changed else None


def special_json(path, text, pairs_by_ov):
    """Rewrite PC status without discarding duplicate legacy keys.

    Both bare legacy names and overlay-qualified keys occur in this file;
    the record's own `ps1_source` remains the required scope proof.
    """
    if path != 'config/pc_port_function_status.json':
        raise RuntimeError(f'{path}: JSON with duplicate keys / not parseable; no handler')
    n = 0
    for ov, pairs in pairs_by_ov.items():
        for old, new in pairs.items():
            names = '(?:' + re.escape(old) + '|' + re.escape(new) + ')'
            rx = re.compile(r'"(?P<scope>' + re.escape(ov) + r'(?:::|:|/))?'
                            + names + r'(?P<legacy>_' + re.escape(ov) + r')?'
                            + r'": \{(?P<body>\s+"category": "[A-Z]",'
                            + r'\s+"ps1_source": "src/overlay/' + re.escape(ov) + r'/)'
                            + names + r'(?P<suffix>\.c")'
                            + r'(?P<tail>[^{}]*)\}', re.I)

            def f(m):
                nonlocal n
                tail = re.sub(r'("func_name":\s*")' + re.escape(old) + r'"',
                              lambda field: field.group(1) + new + '"',
                              m.group('tail'), flags=re.I)
                result = ('"' + (m.group('scope') or '') + new
                          + (m.group('legacy') or '') + '": {'
                          + m.group('body') + new + m.group('suffix') + tail + '}')
                if result != m.group(0):
                    n += 1
                return result
            text = rx.sub(f, text)
    return text if n else None


# ----------------------------------------------------------------- the planner
class Plan(object):
    def __init__(self, key, entry, ov, addr, old, new):
        self.key, self.entry, self.ov, self.addr = key, entry, ov, addr
        self.old, self.new = old, new
        self.reasons = []
        self.moves = []          # (src, dst)
        self.scoped = []         # files in the overlay's own trees containing old
        self.config = []         # candidate config files
        self.pc = []             # other pc/ files with <OV>_<old>
        self.left = []           # informational: outside the rewrite set

    @property
    def ok(self):
        return not self.reasons


def main_names():
    """every MAIN function name an overlay name could be confused with."""
    names = set()
    doc = SN.load_names()
    for e in doc['symbols'].values():
        names.add(e['name'])
    try:
        for f in SN.load_registry()['functions']:
            names.add(f['name'])
    except OSError:
        pass
    for d in ('src/main/matched', 'src/main/semi-matched', 'src/main/pc-port'):
        full = os.path.join(ROOT, d)
        if os.path.isdir(full):
            names.update(os.path.splitext(f)[0] for f in os.listdir(full))
    return names


def tier_reasons(entry, args):
    out = []
    ev = evidence_text(entry)
    confidence = entry.get('confidence')
    if confidence == 'DERIVED':
        if 'DERIVED' not in args.tiers:
            out.append('confidence DERIVED below --min-tier')
        if entry.get('tier') != 'T4' or entry.get('style') != 'derived':
            out.append('DERIVED confidence requires a T4 derived-style candidate')
    elif confidence not in args.tiers:
        out.append(f'confidence {confidence} below --min-tier')
    if args.require_review and 'reviewed:consistent' not in ev:
        out.append('no `reviewed:consistent` verdict (symbol_names.py review)')
    if 'reviewed:contradicted' in ev:
        out.append('review CONTRADICTED the name')
    return out


_LINT = {}


def lint_reasons(name, ov, addr):
    """naming_lint ERROR findings for NAME at OV:ADDR.  The lint index (o.bin, Psy-Q
    archives, every name in the tree) is built once per run, not per name."""
    import naming_lint as NL
    if 'idx' not in _LINT:
        _LINT['idx'] = NL.Index()
    res = NL.lint(name, addr, 'func', ov, _LINT['idx'], {name})
    return [f'naming_lint ERROR: [{rule}] {msg}' for lv, rule, msg in res if lv == 'ERROR']


def collect_files(plan, pairs_hint=None):
    """Fill plan.scoped / config / pc / left for renaming plan.old -> plan.new."""
    ov, old = plan.ov, plan.old
    seen = set()
    for f in grep_files(old, is_address_name(old)):
        if f in seen:
            continue
        seen.add(f)
        sc = scope_of(f)
        if sc == ov:
            plan.scoped.append(f)
        elif sc is not None:
            continue                      # another overlay's own copy of the name
        elif f in NEVER_TOUCH or f.startswith('config/overlay/'):
            continue
        elif CONFIG_STRUCTURED.match(f):
            plan.config.append(f)
        elif f.startswith(('docs/', 'tools/', 'translation/')) or f.endswith('.md'):
            plan.left.append(f)
        else:
            plan.left.append(f)
    # <OV>_<old> in the PC port's hand-written files
    for f in grep_files(f'{ov}_{old}', is_address_name(old)):
        if scope_of(f) == ov:
            plan.scoped.append(f)            # the overlay's own PC module / init / override
        elif f.startswith('pc/') and f not in plan.pc:
            plan.pc.append(f)
    plan.scoped = sorted(set(plan.scoped))


def move_pairs(ov, old, new):
    out = []
    for t in MOVE_TEMPLATES:
        s = t.format(ov=ov, n=old)
        if exists(s):
            out.append((s, t.format(ov=ov, n=new)))
        else:
            # address names: the file may use the other hex case
            d, base = os.path.split(s)
            full = os.path.join(ROOT, d)
            if os.path.isdir(full):
                for fn in os.listdir(full):
                    if fn.lower() == base.lower() and is_address_name(old):
                        out.append((os.path.join(d, fn), t.format(ov=ov, n=new)))
    return out


def sym_entry_names(ov):
    return [n for _, _, n in ON.parse_sym(ov)]


def code_mentions(path, name):
    """Collision checks concern code identifiers, not prose or source comments.

    Preserve quoted literals while stripping comments so a literal containing
    comment delimiters cannot hide the following declaration. Strings remain
    conservative hits (they may contain inline assembly).
    """
    if os.path.splitext(path)[1].lower() not in ('.c', '.h', '.s'):
        return False
    pattern = r'''"(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*'|/\*[\s\S]*?\*/|//[^\n]*'''
    # a collision scan only: an untracked non-UTF-8 file (e.g. Shift-JIS) must not abort it
    with open(os.path.join(ROOT, path), encoding='utf-8', errors='replace', newline='') as fh:
        raw = fh.read()
    text = re.sub(pattern, lambda m: ' ' if m[0].startswith(('/*', '//')) else m[0], raw)
    if path.lower().endswith('.s'):
        text = re.sub(r'(?m)^\s*#(?!\s*(?:include|define|if|else|endif)\b)[^\n]*', '', text)
    return bool(re.search(r'(?<![A-Za-z0-9_])' + re.escape(name) + r'(?![A-Za-z0-9_])', text))


def plan_forward(key, entry, args, ctx):
    scope, addr = SN.split_key(key)
    old = f'func_{addr:08X}'
    p = Plan(key, entry, scope, addr, old, entry['name'])
    new = p.new
    if SN.is_overlay_key(key) is False:
        p.reasons.append('not an overlay record (key has no OV: prefix)')
        return p
    if entry.get('landed'):
        p.reasons.append('already landed')
    if entry.get('kind') != 'func':
        p.reasons.append(f'kind {entry.get("kind")} is not supported by this lander (functions only)')
    p.reasons += tier_reasons(entry, args)
    if scope not in ctx['overlays']:
        p.reasons.append(f'unknown overlay {scope}')
        return p
    syms = [n for n in sym_entry_names(scope) if n.lower() == old.lower()]
    if not syms and entry.get('internal_of'):
        # an INTERNAL function: a complete C function (own prologue and
        # jr ra) that the overlay's C defines inside another function's
        # .sym window (a merged-symbol window).  It has no .sym entry; the
        # resolver reads its address from the landed record.  The window
        # must still contain it and the C must define it.
        host = [(st, sz, n) for st, sz, n in ON.parse_sym(scope)
                if n == entry['internal_of'] and st < addr < st + sz]
        if not host:
            p.reasons.append(f'internal_of {entry["internal_of"]}: no {scope}.sym window of that name '
                             f'contains 0x{addr:08X}')
        defn = re.compile(r'^\w[\w\s\*]*\b%s\s*\([^;]*$' % re.escape(old), re.M)
        if not any(defn.search(read(f)) for f in glob.glob(os.path.join(ROOT, 'src/overlay', scope, '*.c'))):
            p.reasons.append(f'internal function {old}: no definition in src/overlay/{scope}')
    elif not syms:
        p.reasons.append(f'{scope}.sym has no {old} entry (already renamed, or the address is not a function '
                         'prologue name)')
    else:
        p.old = syms[0] if is_address_name(syms[0]) else old
        old = p.old
    if not IDENT_RE.match(new):
        p.reasons.append(f'`{new}` is not a C identifier')
    elif is_address_name(new):
        p.reasons.append(f'`{new}` is address-shaped; a landed name must not carry an address')
    if p.reasons and not IDENT_RE.match(new):
        return p
    # collisions
    for k2, e2 in ctx['all_overlay_records'].items():
        if k2 != key and SN.split_key(k2)[0] == scope and e2.get('name') == new:
            p.reasons.append(f'{new} is already held by {k2} ({"landed" if e2.get("landed") else "pending"})')
    if new in ctx['main_names'] and not entry.get('shadows_main'):
        p.reasons.append(f'{new} is a MAIN name: a reference to it from {scope} would be ambiguous '
                         '(a reviewed exception carries `shadows_main: <reason>`)')
    if new in sym_entry_names(scope):
        p.reasons.append(f'{new} is already a {scope}.sym symbol')
    hits = []
    for f in grep_files(new, False):
        if not code_mentions(f, new):
            continue
        sc = scope_of(f)
        if sc == scope:
            hits.append(f)
        elif sc is None and (f.startswith(('src/main/', 'include/', 'src/overlay/')) and
                             os.path.splitext(f)[1] in ('.c', '.h', '.s')):
            if not f.startswith('src/overlay/') and not entry.get('shadows_main'):
                hits.append(f)
    hits = [h for h in hits if h not in args.allow_decl]
    if hits:
        p.reasons.append(f'{new} is already used as an identifier in {len(hits)} file(s) (e.g. {hits[0]})')
    if args.lint and IDENT_RE.match(new):
        p.reasons += lint_reasons(new, scope, addr)
    collect_files(p)
    p.moves = [m for m in move_pairs(scope, old, new)]
    return p


def plan_revert(key, entry, args, ctx):
    scope, addr = SN.split_key(key)
    new = f'func_{addr:08X}'
    old = entry['name']
    p = Plan(key, entry, scope, addr, old, new)
    if not entry.get('landed'):
        p.reasons.append('not landed')
    au = entry.get('audit') or {}
    if au.get('verdict') != 'refuted' and not args.any_verdict:
        p.reasons.append('audit verdict is not `refuted` (use --any-verdict to revert anyway)')
    if old not in sym_entry_names(scope):
        p.reasons.append(f'{scope}.sym has no {old} entry')
    if new.lower() in [n.lower() for n in sym_entry_names(scope)]:
        p.reasons.append(f'{scope}.sym already has {new}')
    for f in grep_files(new, True):
        if scope_of(f) == scope:
            p.reasons.append(f'{new} already appears in {f}')
            break
    collect_files(p)
    p.moves = move_pairs(scope, old, new)
    return p


def plan_all(args):
    doc = SN.load_names(all_scopes=True)
    syms = doc['symbols']
    overlays = sorted(d for d in os.listdir(os.path.join(ROOT, 'src/overlay'))
                      if os.path.isdir(os.path.join(ROOT, 'src/overlay', d)))
    ctx = {'overlays': overlays,
           'all_overlay_records': {k: v for k, v in syms.items() if SN.is_overlay_key(k)},
           'main_names': main_names()}
    want = {SN.norm_key(o) for o in args.only}
    todo = []
    for key, e in ctx['all_overlay_records'].items():
        scope = SN.split_key(key)[0]
        if args.only and key not in want:
            continue
        if not args.only and args.ov and scope not in [o.upper() for o in args.ov]:
            continue
        if not args.only and not args.ov:
            continue
        if args.revert:
            if not e.get('landed'):
                continue
            au = e.get('audit') or {}
            if au.get('verdict') != 'refuted' and not args.any_verdict:
                continue
            todo.append(plan_revert(key, e, args, ctx))
        else:
            if e.get('landed') or e.get('kind') != 'func':
                continue
            todo.append(plan_forward(key, e, args, ctx))
    missing = want - set(ctx['all_overlay_records'])
    for m in sorted(missing):
        print(f'NO RECORD {m}')
    return doc, todo


def check_locks(plans, args):
    locked = locked_files(args.lock, args.no_git_lock)
    allow = set(args.allow_locked)
    for p in plans:
        if not p.ok:
            continue
        touched = set(p.scoped) | set(p.pc) | {s for s, _ in p.moves} | {f'config/overlay/{p.ov}.sym'}
        for f in p.config:
            # a candidate only counts when the overlay's name appears in it
            if p.ov in read(f):
                touched.add(f)
        bad = sorted(f for f in touched if f in locked and f not in allow)
        if bad:
            p.reasons.append(f'touches locked file(s): {bad[:4]}{"..." if len(bad) > 4 else ""}')
        for s, d in p.moves:
            if exists(d):
                p.reasons.append(f'target file {d} exists')


# -------------------------------------------------------------------- the apply
def pairs_of(plans):
    pairs_by_ov = collections.defaultdict(dict)
    for p in plans:
        pairs_by_ov[p.ov][p.old] = p.new
    return dict(pairs_by_ov)


def preflight_config(plans):
    """Compute every config/wiki rewrite in memory BEFORE anything is touched:
    {path: new text}.  A file whose JSON formatting cannot be reproduced (or that
    has no handler) raises here, so a batch never lands half-way."""
    pairs_by_ov = pairs_of(plans)
    out = collections.OrderedDict()
    for p in plans:
        for f in p.config:
            if f in out:
                continue
            new = rewrite_config(f, pairs_by_ov)
            if new is not None:
                out[f] = new
    return out


def append_evidence(e, text):
    """Record text on a record without changing the shape of its evidence (the v2
    schema allows a list of {type, detail} entries)."""
    ev = e.get('evidence', '')
    if isinstance(ev, list):
        ev.append({'type': 'lander', 'detail': text})
    else:
        e['evidence'] = ev + '; ' + text


def apply_plans(doc, plans, args, cfg_new):
    revert = args.revert
    pairs_by_ov = pairs_of(plans)
    touched = set()
    counts = collections.Counter()

    # 1. file moves
    moved = {}
    for p in plans:
        for s, d in p.moves:
            os.rename(os.path.join(ROOT, s), os.path.join(ROOT, d))
            moved[s] = d
            touched.update((s, d))
    # 2. every file of the overlay's own trees (and the PC modules): whole-word rename
    for p in plans:
        for f in p.scoped:
            f2 = moved.get(f, f)
            if not exists(f2):
                continue
            pairs = pairs_by_ov[p.ov]
            text = read(f2)
            prefix = p.ov + '_' if f2.startswith('pc/') else ''
            new, n = sub_words(text, pairs, prefix)
            if n and new != text:
                write(f2, new)
                touched.add(f2)
                counts[p.key] += 1
    done = set()
    for p in plans:
        for f in p.pc:
            if (f, p.ov) in done:
                continue
            done.add((f, p.ov))
            text = read(f)
            # only the OV-prefixed spelling: a bare name in an unscoped pc file is not ours
            new = prefixed_only(text, pairs_by_ov[p.ov], p.ov + '_')
            if new != text:
                write(f, new)
                touched.add(f)
                counts[p.key] += 1
    # 3. the .sym
    for ov, pairs in pairs_by_ov.items():
        sp = f'config/overlay/{ov}.sym'
        text = read(sp)
        new, n = sub_words(text, pairs)
        if n:
            write(sp, new)
            touched.add(sp)
    # 4. config / wiki registries (one parse per file, all overlays and names of the batch)
    for f, new in cfg_new.items():
        write(f, new)
        touched.add(f)
    # 5. the records
    for p in plans:
        e = p.entry
        if revert:
            e['reverted'] = {'date': SN.TODAY, 'from': p.old, 'by': 'apply_overlay_symbols.py --revert',
                             'reason': 'audit ' + (e.get('audit') or {}).get('verdict', 'any-verdict')}
            if args.forget:
                doc['symbols'].pop(p.key, None)
            else:
                e['landed'] = False
                e['confidence'] = 'REVERTED'
                append_evidence(e, f'reverted {SN.TODAY} to {p.new}')
        else:
            e['landed'] = True
            e['date'] = SN.TODAY
            append_evidence(e, f'landed {SN.TODAY} by apply_overlay_symbols.py ({counts[p.key]} files)')
    # an internal function names its host window (`internal_of`): follow the
    # host's rename (2026-11-10: refining TT tt_main1 stranded three records)
    renamed = {(p.ov, p.old): p.new for p in plans}
    for key, e in doc['symbols'].items():
        host = e.get('internal_of')
        scope = key.partition(':')[0]
        if host and (scope, host) in renamed:
            e['internal_of'] = renamed[(scope, host)]
    SN.save_names(doc)
    ON.load_tables(force=True)
    touched.add('config/symbol_names.json')
    return sorted(touched), counts


def prefixed_only(text, pairs, prefix):
    """rename only the `PREFIX<old>` spelling (the PC port's per-overlay symbol prefix)."""
    n = 0
    for old, new in pairs.items():
        flags = re.I if is_address_name(old) else 0
        rx = re.compile(r'(?<![A-Za-z0-9_])' + re.escape(prefix) + re.escape(old) + r'(?![A-Za-z0-9_])', flags)
        text = rx.sub(prefix + new, text)
    return text


# ------------------------------------------------------------------- survivors
def survivors(ov, names):
    """whole-word survivors of NAMES inside the overlay's own trees and .sym."""
    out = []
    for n in names:
        ci = is_address_name(n)
        for f in grep_files(n, ci):
            if scope_of(f) == ov or f == f'config/overlay/{ov}.sym':
                out.append((n, f))
        for t in MOVE_TEMPLATES:
            if exists(t.format(ov=ov, n=n)):
                out.append((n, t.format(ov=ov, n=n)))
    return out


# ---------------------------------------------------------------------- gates
def gate_commands(ovs):
    py = 'python3'
    cmds = [('symbol_names.py check', f'{py} tools/analysis/symbol_names.py check'),
            ('name table', f'{py} tools/overlay/resolve_overlay_relocs.py --check-name-table')]
    for ov in sorted(ovs):
        cmds += [(f'{ov}: verify (serial)', f'{py} tools/overlay/verify_overlay_bytes.py {ov} -j 1'),
                 (f'{ov}: reconcile', f'{py} tools/overlay/apply_resolved_status.py {ov} --demote'),
                 (f'{ov}: plain-vs-shipped', f'{py} tools/analysis/audit_plain_vs_shipped.py --overlay {ov}'),
                 (f'{ov}: proof', f'{py} tools/analysis/prove_equivalence.py --overlay {ov}'),
                 (f'{ov}: clean EXN', f'{py} tools/overlay/build_clean_exn.py {ov} --verify'),
                 (f'{ov}: coverage', f'{py} tools/overlay/audit_overlay_coverage.py --overlay {ov}')]
    cmds += [('universe', f'{py} tools/overlay/audit_overlay_universe.py')]
    return cmds


MAIN_GATES = [('main build', 'python3 tools/build/build_level3_c.py'),
              ('main relocs', 'python3 tools/analysis/audit_main_relocs.py')]


def print_gates(ovs, names, with_main=True):
    print('Overlay landing gates (run all, in the venv; verify SERIALLY):')
    for label, c in gate_commands(ovs):
        print(f'  {c:<86} # {label}')
    if with_main:
        for label, c in MAIN_GATES:
            print(f'  {c:<86} # {label}')
    for ov in sorted(ovs):
        for n in names.get(ov, []):
            print(f"  git grep -n -i -w {n}  -- src/overlay/{ov} asm/overlay/{ov} config/overlay/{ov}.sym"
                  "   # zero survivors")
    print('  python3 pc/tools/generate_overlay_modules.py                        # the PC modules follow the names')


def status_by_name(text, ov):
    try:
        return {k: (v.get('status') if isinstance(v, dict) else v)
                for k, v in json.loads(text).get(ov, {}).items()}
    except ValueError:
        return {}


def run_gates(ovs, renames, args):
    """Run the gates serially; returns the number of failures.  `renames` is
    {ov: {landed name: address name}} so statuses can be compared by the
    function's identity across the rename."""
    fails = 0

    def run(label, cmd, ok=lambda r: r.returncode == 0, show=6, retries=0):
        nonlocal fails
        # the gates need the venv's pyelftools: run them with THIS interpreter
        for attempt in range(retries + 1):
            r = subprocess.run(cmd.replace('python3 ', sys.executable + ' ', 1), shell=True,
                               capture_output=True, text=True, cwd=ROOT)
            good = ok(r)
            if good:
                break
            if attempt < retries:
                print(f'  [retry] {label}: first attempt failed (verify is flaky under load); running it once more')
        out = (r.stdout + r.stderr).strip().splitlines()
        print(f'  [{"PASS" if good else "FAIL"}] {label}')
        for ln in (out if not good else out[-show:])[-40:]:
            print(f'          {ln}')
        if not good:
            print(f'          (exit status {r.returncode})')
        if not good:
            fails += 1
        return r

    head_status = git('show', 'HEAD:config/overlay_match_status.json')
    for label, cmd in gate_commands(ovs):
        if 'verify_overlay_bytes' in cmd:
            ov = cmd.split()[-1]
            run(label, cmd, ok=lambda r: r.returncode == 0 and 'Count mismatch: 0' in r.stdout
                and 'Errors:         0' in r.stdout, show=4, retries=1)
        elif 'apply_resolved_status' in cmd:
            ov = cmd.split()[-2]

            def ok(r, ov=ov):
                if r.returncode != 0 or 'Pruned' in r.stdout or 'Demoted codegen-residual reloc_only -> semi_matched: 0' \
                        not in r.stdout:
                    return False
                now = status_by_name(read('config/overlay_match_status.json'), ov)
                before = status_by_name(head_status, ov)
                inv = renames.get(ov, {})
                # compare by function identity: a landed name stands for its address name
                # on both sides (HEAD may or may not already carry the landing)
                return ({inv.get(k, k): v for k, v in now.items()} ==
                        {inv.get(k, k): v for k, v in before.items()})
            run(label + ' (no status change vs HEAD, Pruned 0)', cmd, ok=ok, show=4)
        elif 'audit_plain_vs_shipped' in cmd:
            run(label, cmd, ok=lambda r: r.returncode == 0, show=4)
        elif 'prove_equivalence' in cmd:
            run(label, cmd, ok=lambda r: r.returncode == 0, show=4)
        elif 'build_clean_exn' in cmd:
            run(label, cmd, ok=lambda r: r.returncode == 0 and 'BYTE-PERFECT' in r.stdout, show=3)
        elif 'audit_overlay_coverage' in cmd:
            run(label, cmd, ok=lambda r: r.returncode == 0, show=4)
        else:
            run(label, cmd)
    # zero survivors of the old address names in each overlay's own trees and .sym
    for ov in sorted(ovs):
        olds = list(renames.get(ov, {}).values())
        left = survivors(ov, olds)
        good = not left
        print(f'  [{"PASS" if good else "FAIL"}] {ov}: zero survivors of {len(olds)} replaced address name(s) '
              f'in src/overlay, asm/overlay, {ov}.sym')
        for n, f in left[:6]:
            print(f'          {n}: {f}')
        fails += 0 if good else 1
    if args.with_main:
        for label, cmd in MAIN_GATES:
            if 'build_level3_c' in cmd:
                run(label, cmd, ok=lambda r: r.returncode == 0 and
                    '2d8d474816e4bf937b09dc3b017f550f7ce4e14b' in (r.stdout + r.stderr), show=3)
            else:
                run(label, cmd, ok=lambda r: r.returncode == 0 and 'MISMATCH' not in (r.stdout + r.stderr), show=3)
    return fails


# ------------------------------------------------------------------------ main
def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument('--dry-run', action='store_true')
    g.add_argument('--apply', action='store_true')
    g.add_argument('--gates', action='store_true', help='print the gate commands for --ov/--only overlays')
    g.add_argument('--run-gates', action='store_true', help='run the overlay gates serially (slow)')
    ap.add_argument('--with-main', action='store_true',
                    help='--run-gates: also run the main-binary gates (build_level3_c retail SHA1, audit_main_relocs)')
    ap.add_argument('--revert', action='store_true', help='landed overlay names back to func_<ADDR>')
    ap.add_argument('--forget', action='store_true', help='--revert: also drop the record from symbol_names.json')
    ap.add_argument('--any-verdict', action='store_true',
                    help='--revert: also revert a landed name whose audit is not `refuted`')
    ap.add_argument('--only', action='append', default=[], metavar='OV:0xADDR')
    ap.add_argument('--ov', action='append', default=[], metavar='OV', help='every record of this overlay')
    ap.add_argument('--min-tier', default='HIGH', choices=['HIGH', 'MEDIUM', 'DERIVED'])
    ap.add_argument('--no-require-review', action='store_true')
    ap.add_argument('--no-lint', action='store_true', help='skip naming_lint (fixtures only)')
    ap.add_argument('--batch', type=int, default=10)
    ap.add_argument('--lock', action='append', default=[], help='extra path to treat as locked')
    ap.add_argument('--no-git-lock', action='store_true',
                    help='do not treat git-modified files as locked (multi-batch runs); --lock still applies')
    ap.add_argument('--allow-locked', action='append', default=[], metavar='PATH')
    ap.add_argument('--allow-decl', action='append', default=[], metavar='PATH',
                    help='exact path whose existing mention of the NEW name is a stale prototype/comment of '
                         'this same function (reviewed by hand)')
    args = ap.parse_args()
    args.tiers = {'HIGH'}
    if args.min_tier in ('MEDIUM', 'DERIVED'):
        args.tiers.add('MEDIUM')
    if args.min_tier == 'DERIVED':
        args.tiers.add('DERIVED')
    args.require_review = not args.no_require_review
    args.lint = not args.no_lint

    if args.gates or args.run_gates:
        ovs = [o.upper() for o in args.ov] + [SN.split_key(SN.norm_key(o))[0] for o in args.only]
        if not ovs:
            sys.exit('--gates needs --ov OV (or --only OV:0xADDR)')
        doc = SN.load_names(all_scopes=True)
        names = collections.defaultdict(list)
        renames = collections.defaultdict(dict)
        for k, e in doc['symbols'].items():
            if SN.is_overlay_key(k) and e.get('landed'):
                sc, a = SN.split_key(k)
                addr_name = f'func_{a:08X}'
                names[sc].append(addr_name)
                renames[sc][e['name']] = addr_name
                # A refined record's superseded name is the same identity: in the
                # correction flow (refine_pre.sh, the 0x80084ED0 pattern) HEAD still
                # carries the old landed name and the reconcile gate must map it too.
                for h in e.get('history', []):
                    hn = h.get('name')
                    if hn and renames[sc].get(hn, addr_name) == addr_name:
                        renames[sc][hn] = addr_name
        if args.gates:
            print_gates(set(ovs), names)
            return 0
        fails = run_gates(set(ovs), renames, args)
        print(f'{fails} gate(s) FAILED' if fails else 'all overlay gates passed')
        return 1 if fails else 0

    doc, plans = plan_all(args)
    check_locks(plans, args)
    refused = [p for p in plans if not p.ok]
    ok = [p for p in plans if p.ok][:args.batch]
    verb = 'REVERT' if args.revert else 'LAND  '
    for p in refused:
        print(f'REFUSE {p.key} {p.old} -> {p.new}')
        for r in p.reasons:
            print(f'        - {r}')
    if not ok:
        print('nothing ' + ('revertible' if args.revert else 'landable') + (' (dry run)' if args.dry_run else ''))
        return 1 if refused else 0
    for p in ok:
        nfiles = len(p.scoped) + len(p.pc)
        print(f'{verb if args.apply else "WOULD "} {p.key} {p.old} -> {p.new}: {nfiles} files, '
              f'{len(p.moves)} file moves, {len(p.config)} config candidates')
        if not args.apply:
            for f in p.scoped + p.pc:
                print(f'          {f}')
            for s, d in p.moves:
                print(f'          mv {s} -> {d}')
            for f in p.config:
                print(f'          (config) {f}')
            if p.left:
                code = [f for f in p.left if f.startswith(('src/', 'include/', 'pc/'))]
                print(f'          left alone ({len(p.left)} files outside the overlay: docs/tools/prose'
                      + (f'; CODE: {code}' if code else '') + ')')
    try:
        cfg_new = preflight_config(ok)
    except RuntimeError as exc:
        print(f'REFUSE batch: {exc}')
        return 2
    if args.dry_run:
        print(f'config/wiki registries that would change: {", ".join(cfg_new) or "(none)"}')
        return 0
    touched, counts = apply_plans(doc, ok, args, cfg_new)
    ovs = sorted({p.ov for p in ok})
    # resolver check: every landed name resolves to its address in its overlay
    for p in ok:
        if not args.revert:
            a = ON.name_to_addr(p.new, p.ov)
            # an absorbed head: the name resolves to its window start, up to
            # 184 B before the prologue-keyed record (overlay_names `entry`)
            head = (a is not None and 0 < p.addr - a <= 184
                    and ON.load_tables().entry.get(p.ov.upper(), {}).get(p.new) == a)
            assert a == p.addr or head, f'{p.ov}:{p.new} resolves to {a}, expected 0x{p.addr:08X}'
    bad = []
    for ov in ovs:
        bad += survivors(ov, [p.old for p in ok if p.ov == ov])
    print(f'{"reverted" if args.revert else "landed"} {len(ok)}; resolver name table reloaded')
    if bad:
        for n, f in bad:
            print(f'SURVIVOR {n}: {f}')
    print('touched paths (commit with `git commit -F msg -- <these>`):')
    for t in touched:
        print(f'  {t}')
    names = collections.defaultdict(list)
    for p in ok:
        names[p.ov].append(p.old)
    print_gates(set(ovs), names)
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
