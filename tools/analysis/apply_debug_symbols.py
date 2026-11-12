#!/usr/bin/env python3
"""Land function renames from config/symbol_names.json (the single source of
truth) into the tree, doing all four rename searches (CLAUDE.md directive 7):

  1. direct calls / externs / definitions   -- every tracked text file
  2. type references                         -- include/*.h, pc/ headers
  3. string literals                         -- asm/*.s, config/*.json, docs/, tools/
  4. dynamic references                      -- src/main/pc-port/, pc/Makefile lists

A rename is a whole-word replacement of the old address name (both hex cases:
func_8004ABCD and func_8004abcd) by the new name in every tracked file that
carries it, plus file renames (src/main/matched|semi-matched/OLD.c,
asm/main/matched/OLD.s, src/main/pc-port/OLD.c), the registry entry
(config/level3_functions.json name + source), the symbol_names.json entry
(landed=true) and a regenerated config/main_retail_symbols.json.

What it REFUSES (each printed with its reason):
  * confidence below --min-tier (default HIGH), or not review-approved when
    --require-review is set (the default: `symbol_names.py review` verdicts)
  * library band (>= 0x80086000) unless the entry's evidence records a
    corroboration outside o.bin (CLAUDE.md: never from o.bin alone)
  * a new name already used anywhere in the tree (git grep -w)
  * the new name failing the overlay resolver's name-table check
    (resolve_overlay_relocs.load_name_table: a name landed twice, or an
    address-shaped name disagreeing with its own suffix).  Overlay references
    are NOT refused since 2026-09-27 -- the resolver reads the landed-name table
    -- but every overlay whose C is touched is listed for a serial
    verify_overlay_bytes.py + apply_resolved_status.py in the gates.
  * any touched file that is locked: git-modified/staged files (someone else
    is editing them), config/overlay/*.sym, and --lock paths
  * the retail bytes: run the gates after every batch (see --gates)

Usage:
    python3 tools/analysis/apply_debug_symbols.py --dry-run            # what would land
    python3 tools/analysis/apply_debug_symbols.py --apply --batch 10
    python3 tools/analysis/apply_debug_symbols.py --apply --only 0x8004FC90
    python3 tools/analysis/apply_debug_symbols.py --gates FUNC ...    # print the gate commands
    python3 tools/analysis/apply_debug_symbols.py --revert --dry-run --batch 40   # refuted -> func_XXXXXXXX
    python3 tools/analysis/apply_debug_symbols.py --revert --apply --batch 40
"""
import argparse, json, os, re, shutil, subprocess, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import symbol_names as SN  # noqa: E402

ALWAYS_LOCKED = ('config/overlay/', 'docs/reference/IDO52-VALIDATION.md')
TEXT_EXT = {'.c', '.h', '.s', '.json', '.md', '.txt', '.py', '.sh', '.csv', '.patch', '.cfg', '.ini', '.yml', '.yaml', ''}


def git(*args):
    return subprocess.run(['git'] + list(args), capture_output=True, text=True, cwd=SN.ROOT).stdout


def locked_files(extra):
    out = set()
    for line in git('status', '--porcelain', '--untracked-files=no').splitlines():
        out.add(line[3:].strip())
    for p in extra:
        out.add(p)
    return out


def refs(name):
    """files containing NAME as a whole word, case-insensitive on the hex: tracked
    files plus untracked, non-ignored ones under the source trees (an earlier
    rename's file move leaves the new file untracked until it is committed;
    tracked-only search missed func_800412C8 in the just-moved
    draw_and_disp_env_init.c, batch 1 2026-09-27)."""
    r = git('grep', '--untracked', '-I', '-l', '-i', '-w', '--', name)
    tracked = set(git('ls-files').splitlines())
    return [x for x in r.splitlines() if x and (x in tracked or x.startswith(UNTRACKED_OK))]


def overlay_refs(name):
    return [f for f in refs(name) if f.startswith('src/overlay/')]


def is_excluded(path, args):
    """--exclude PREFIX (skipped, not refused: e.g. pc/ and src/main/pc-port/ while the
    PC port is out of scope) and --skip-prose (vendor trees, docs/, *.md: the revert
    mode's REVERT_EXCLUDE set)."""
    ex = tuple(getattr(args, 'exclude', None) or ())
    if ex and path.startswith(ex):
        return True
    return bool(getattr(args, 'skip_prose', False)) and revert_excluded(path)


def is_locked(path, locked):
    return path in locked or any(path.startswith(p) or path == p for p in ALWAYS_LOCKED)


def rename_in_file(path, old, new):
    p = os.path.join(SN.ROOT, path)
    try:
        s = open(p, encoding='utf-8').read()
    except UnicodeDecodeError:
        return False
    pat = re.compile(r'\b' + re.escape(old) + r'\b', re.IGNORECASE)
    s2 = pat.sub(new, s)
    if s2 != s:
        open(p, 'w', encoding='utf-8').write(s2)
        return True
    return False


def address_form_alt(name):
    """The other FUN_/func_ address-name spelling, if `name` is one.

    The tree carries BOTH forms after the old FUN_-import (the registry says
    FUN_800790A0 while the code says func_800790A0).  Renaming only the
    registered form leaves the lowercase twin behind, which broke the PC link
    on 2026-10-17 (undefined reference from overlay_load_main.c).
    """
    m = re.fullmatch(r'(FUN|func)_([0-9A-Fa-f]{8})', name)
    if not m:
        return None
    return ('func' if m.group(1) == 'FUN' else 'FUN') + '_' + m.group(2)


def file_moves(old, new):
    out = []
    for d, ext in (('src/main/matched', '.c'), ('src/main/semi-matched', '.c'),
                   ('asm/main/matched', '.s'), ('src/main/pc-port', '.c')):
        src = os.path.join(d, old + ext)
        if os.path.exists(os.path.join(SN.ROOT, src)):
            out.append((src, os.path.join(d, new + ext)))
    # demoted archive attempts follow the <name>_painted.c convention; the
    # revert path already knows these pairs (revert_moves)
    src = os.path.join('src/main/archive', old + '_painted.c')
    if os.path.exists(os.path.join(SN.ROOT, src)):
        out.append((src, os.path.join('src/main/archive', new + '_painted.c')))
    alt = address_form_alt(old)
    if alt and alt != old:
        out.extend(file_moves(alt, new))
    return out


def plan(entry, key, reg_names, locked, args):
    """Return (ok, reasons, moves, files) for one candidate."""
    new = entry['name']
    addr = int(key, 16)
    uni_name = None
    for f in reg_names['by_addr'].get(addr, []):
        uni_name = f['name']
    old = uni_name or f'func_{addr:08X}'
    reasons = []
    ev = entry.get('evidence', '')
    if entry.get('landed'):
        reasons.append('already landed')
    confidence = entry.get('confidence')
    if confidence == 'DERIVED':
        if 'DERIVED' not in args.tiers:
            reasons.append('confidence DERIVED below --min-tier')
        if entry.get('tier') != 'T4' or entry.get('style') != 'derived':
            reasons.append('DERIVED confidence requires a T4 derived-style candidate')
    elif confidence not in args.tiers:
        reasons.append(f"confidence {confidence} below --min-tier")
    if args.require_review and 'reviewed:consistent' not in ev:
        reasons.append('no `reviewed:consistent` verdict (symbol_names.py review)')
    if 'reviewed:contradicted' in ev:
        reasons.append('review CONTRADICTED the name')
    if addr >= SN.LIB_START and 'corroborated:' not in ev:
        reasons.append('library band: o.bin alone cannot name it (needs corroborated: evidence)')
    if not SN.is_address_name(old):
        reasons.append(f'retail function already named {old}')
    if new in reg_names['names']:
        reasons.append(f'new name {new} already registered at {SN.akey(reg_names["names"][new])}')
    hits = [h for h in refs(new) if (os.path.splitext(h)[1] in ('.c', '.h', '.s') or h == 'config/level3_functions.json')
            and h not in args.allow_decl and not is_excluded(h, args)]
    if hits:
        reasons.append(f'new name already used as an identifier in {len(hits)} source files (e.g. {hits[0]})')
    alt = address_form_alt(old)
    ref_files = set(refs(old))
    if alt and alt != old:
        ref_files |= set(refs(alt))
    files = [f for f in sorted(ref_files) if not is_excluded(f, args)]
    lock_hits = [f for f in files if is_locked(f, locked) and f not in args.allow_locked]
    if lock_hits:
        reasons.append(f'touches locked files: {lock_hits[:3]}{"..." if len(lock_hits) > 3 else ""}')
    moves = [(s, d) for s, d in file_moves(old, new) if not is_excluded(s, args)]
    for s, d in moves:
        if is_locked(s, locked) and s not in args.allow_locked:
            reasons.append(f'source file {s} is locked')
        if os.path.exists(os.path.join(SN.ROOT, d)):
            reasons.append(f'target file {d} exists')
    return (not reasons), reasons, old, moves, files


def gates(names, overlays=()):
    print('Batch gates (run all, in the venv):')
    print('  python3 tools/build/build_level3_c.py                 # retail SHA1 2d8d4748..., 0 mismatched')
    print('  python3 tools/analysis/audit_main_relocs.py           # 0 MISMATCH')
    for n in names:
        print(f'  python3 tools/analysis/audit_reloc_positions.py {n}')
    print('  python3 tools/analysis/audit_registry_overlaps.py      # clean')
    print('  python3 tools/overlay/resolve_overlay_relocs.py --check-name-table')
    for ov in sorted(overlays):
        print(f'  python3 tools/overlay/verify_overlay_bytes.py {ov} && '
              f'python3 tools/overlay/apply_resolved_status.py {ov} --demote   # 100%, no status change')
    print('  make -f pc/Makefile                                    # the port still builds')
    for n in names:
        pass
    print('  git grep -n -i -w <old name>                            # zero survivors per landed rename')


# ------------------------------------------------------------------ revert mode
# Undo a LANDED name whose anchor audit REFUTED it: the retail function goes back
# to its address name (func_XXXXXXXX), in every place the four rename searches
# reach.  What is deliberately NOT touched (a refuted o.bin name is often a real
# name of something else, or the name o.bin itself carries):
#   * third-party / vendor trees (pc/3rd, the Psy-Q SDK, qemu,
#     decomp-permuter, rabbitizer, a vendored pip venv) and the Sony API headers
#     (include/psyq/, pc/include/lib*.h): there `_spu_close` IS Sony's _spu_close
#   * o.bin-name records: config/symbol_mapping_tracker.json (status set to
#     `reverted`, obin_name kept), the o.bin matcher tools that pattern-match
#     o.bin names, and the Phase 1b worklist generator's prose
#   * docs/, CLAUDE.md, translation/ -- dated prose; the worklist carries the map
#   * locked files (git-modified by someone else, config/overlay/*.sym)
REVERT_EXCLUDE = (
    'pc/3rd/', 'pc/tools/.venv-pillow/',
    'build/decomp-permuter/', 'tools/ido-static-recomp/tools/',
    'include/psyq/', 'docs/', 'translation/', '.claude/', 'disks/', 'pc/docs/',
    'CLAUDE.md', 'README.md', 'pc/README.md', 'run_game.sh',
    'config/overlay/', 'config/symbol_names.json', 'config/main_retail_symbols.json',
    'config/symbol_mapping_tracker.json',
    'tools/symbols/map_obin_symbols.py', 'tools/analysis/symbol_match_advanced.py',
    'tools/analysis/match_debug_symbols.py',
    # the naming pipeline's o.bin-name tables (band markers, role words, alignment
    # candidates) are o.bin NAMES, not tree symbols
    'tools/analysis/naming_census.py', 'tools/analysis/align_candidates.py',
    'tools/analysis/naming_lint.py', 'tools/analysis/data_access_census.py',
)
SONY_API_HEADER = re.compile(r'^pc/include/(lib[a-z0-9_]*|mcgui|psyz)\.h$')
# Common English words that are also refuted names: whole-word replacement would
# hit prose, JSON keys and unrelated locals, so only these files are rewritten.
REVERT_RESTRICT = {
    'title': ('config/level3_functions.json', 'config/main_function_audit_by_addr.json',
              'config/pc_port_function_status.json', 'include/functions.h',
              'src/main/matched/title.c'),
}
MOVE_DIRS = ('src/main/matched', 'src/main/semi-matched', 'src/main/pc-port', 'src/main/archive',
             'asm/main/matched', 'tools/ido')


def revert_excluded(path):
    # *.md anywhere: dated prose and stale o.bin-guess tables (asm/, src/main/matched/
    # README "likely symbol" columns record o.bin NAMES, not tree symbols)
    return (path.startswith(REVERT_EXCLUDE) or bool(SONY_API_HEADER.match(path))
            or path.endswith('.md'))


UNTRACKED_OK = ('src/', 'include/', 'asm/', 'pc/', 'tools/', 'config/')


def refs_cs(name):
    """text files containing NAME as a whole word (case-sensitive): tracked files
    plus untracked, non-ignored ones under the source trees -- an earlier batch's
    file moves leave the renamed file untracked, and it must still be searched."""
    r = git('grep', '--untracked', '-I', '-l', '-w', '--', name)
    tracked = set(git('ls-files').splitlines())
    return [x for x in r.splitlines() if x and (x in tracked or x.startswith(UNTRACKED_OK))]


def rename_tokens(path, pairs):
    """whole-word, case-sensitive replacement of each (old, new); returns True if changed."""
    p = os.path.join(SN.ROOT, path)
    try:
        s = open(p, encoding='utf-8').read()
    except (UnicodeDecodeError, FileNotFoundError):
        return False
    s2 = s
    for old, new in pairs:
        s2 = re.sub(r'(?<![A-Za-z0-9_])' + re.escape(old) + r'(?![A-Za-z0-9_])', new, s2)
    if s2 != s:
        open(p, 'w', encoding='utf-8').write(s2)
        return True
    return False


SELF_TRAMPOLINE = re.compile(r'^.*\bvoid\s+(\w+)\s*\(void\)\s*\{\s*__asm__\("jmp\s+(\w+)"\);\s*\}\s*\n', re.M)
SELF_ALIAS = re.compile(r'^[^\n;]*\b(\w+)\s*\([^;{]*\)\s*__attribute__\(\(alias\("(\w+)"\)\)\);[^\n]*\n', re.M)


def drop_self_aliases(path):
    """after a revert, `func_X: jmp func_X` trampolines and `func_X alias("func_X")`
    declarations (which bridged the address name to the refuted name) are cycles."""
    p = os.path.join(SN.ROOT, path)
    s = open(p, encoding='utf-8').read()
    dropped = []

    def keep(m):
        if m.group(1) == m.group(2):
            dropped.append(m.group(0).strip())
            return ''
        return m.group(0)
    s2 = SELF_ALIAS.sub(keep, SELF_TRAMPOLINE.sub(keep, s))
    if s2 != s:
        open(p, 'w', encoding='utf-8').write(s2)
    return dropped


def json_dup_keys(path):
    dups = []

    def hook(pairs):
        seen = set()
        for k, _ in pairs:
            if k in seen:
                dups.append(k)
            seen.add(k)
        return dict(pairs)
    try:
        json.loads(open(os.path.join(SN.ROOT, path)).read(), object_pairs_hook=hook)
    except ValueError as exc:
        return [f'UNPARSEABLE: {exc}']
    return dups


def revert_moves(old, new):
    out = []
    for d in MOVE_DIRS:
        dd = os.path.join(SN.ROOT, d)
        if not os.path.isdir(dd):
            continue
        for fn in os.listdir(dd):
            stem, ext = os.path.splitext(fn)
            for so, sn in ((old, new), (old + '_painted', new + '_painted')):
                if stem == so:
                    out.append((os.path.join(d, fn), os.path.join(d, sn + ext)))
    return out


CODE_CALL = r'(?<![A-Za-z0-9_]){}\s*\('


def strip_comments(text):
    return re.sub(r'/\*.*?\*/', ' ', text, flags=re.S)


def revert_target(key, old):
    """The address name a refuted landed name goes back to.  Normally the entry
    address.  But when the entry address is an absorbed cross-prologue HEAD that
    the PC port still defines as its own shim (src/main/pc-port/func_<key>.c
    calling the host), the host needs its OWN address name or the shim and the
    host collide (same file name shadows the host; same symbol recurses):
      * the host C already defines an address-named symbol -> that symbol;
      * the shim calls the refuted name -> func_<prologue>, the first
        `addiu sp,sp,-N` at/after the head (the host's pre-landing name).
    Returns (name, why)."""
    addr = int(key, 16)
    kn = f'func_{addr:08X}'
    shim = os.path.join(SN.ROOT, 'src/main/pc-port', kn + '.c')
    if not os.path.exists(shim):
        return kn, 'entry address'
    code = strip_comments(open(shim, errors='replace').read())
    host = None
    for d in ('src/main/matched', 'src/main/semi-matched'):
        hp = os.path.join(SN.ROOT, d, old + '.c')
        if os.path.exists(hp):
            host = strip_comments(open(hp, errors='replace').read())
            break
    if host:
        for sym in re.findall(r'^[A-Za-z_][\w \*]*?\b(func_[0-9A-Fa-f]{8})\s*\([^;]*\)\s*(?:\{|$)', host, re.M):
            if sym.upper() != kn.upper() and re.search(CODE_CALL.format(sym), code):
                return 'func_' + sym[5:].upper(), f'host C symbol {sym} called by head shim {os.path.basename(shim)}'
    if re.search(CODE_CALL.format(re.escape(old)), code):
        for a in range(addr, addr + 0x60, 4):
            w = SN.word(a)
            if (w >> 16) == 0x27BD and (w & 0x8000):
                if a != addr:
                    return f'func_{a:08X}', f'host prologue behind head shim {os.path.basename(shim)}'
                break
    return kn, 'entry address'


def revert_main(args):
    doc = SN.load_names()
    reg = SN.load_registry()
    # --no-git-lock: a multi-batch revert run leaves its own earlier batches
    # git-modified; lock only the explicit --lock paths (e.g. a file someone else is editing)
    locked = set(args.lock) if args.no_git_lock else locked_files(args.lock)
    todo = []
    for key, e in doc['symbols'].items():
        au = e.get('audit') or {}
        if e.get('kind') != 'func' or not e.get('landed'):
            continue
        if au.get('verdict') != 'refuted' and not (args.any_verdict and args.only):
            continue
        if args.only and key.upper() not in [o.upper() for o in args.only]:
            continue
        todo.append((key, e))
    todo = todo[:args.batch]
    if args.to:
        if len(todo) != 1 or not SN.is_address_name(args.to):
            sys.exit('--to needs exactly one --only entry and an address-shaped name (e.g. the pre-landing '
                     'prologue name of an absorbed-head host)')
    tracker = json.load(open(SN.TRACKER)) if os.path.exists(SN.TRACKER) else None
    report, overlays, problems = [], set(), []
    for key, e in todo:
        old = e['name']
        new, why = (args.to, '--to') if args.to else revert_target(key, old)
        files = [f for f in refs_cs(old) + refs_cs(old + '_painted')
                 if not revert_excluded(f) and not is_excluded(f, args)]
        files = sorted(set(files))
        if old in REVERT_RESTRICT:
            files = [f for f in files if f in REVERT_RESTRICT[old]]
        moves = [(s, d) for s, d in revert_moves(old, new) if not is_excluded(s, args)]
        reasons = []
        for f in files:
            if is_locked(f, locked) and f not in args.allow_locked:
                reasons.append(f'touches locked file {f}')
        for s, d in moves:
            if os.path.exists(os.path.join(SN.ROOT, d)):
                reasons.append(f'target file {d} exists (merge by hand)')
        if reasons and not args.force_partial:
            print(f'REFUSE {key} {old} -> {new}:')
            for r in reasons:
                print(f'        - {r}')
            continue
        print(f'{"REVERT" if args.apply else "WOULD "} {key} {old} -> {new}: {len(files)} files, '
              f'{len(moves)} file moves  [{why}]')
        if not args.apply:
            for f in files:
                print(f'          {f}')
            for s, d in moves:
                print(f'          mv {s} -> {d}')
            continue
        for s, d in moves:
            if not os.path.exists(os.path.join(SN.ROOT, d)):
                shutil.move(os.path.join(SN.ROOT, s), os.path.join(SN.ROOT, d))
        mv = dict(moves)
        touched = 0
        kn = f'func_{int(key, 16):08X}'
        shim = f'src/main/pc-port/{kn}.c'
        for f in files:
            f2 = mv.get(f, f)
            # split target (host behind a head shim): the host's own files, the shim
            # and the records (configs) take the host name; every other CODE
            # reference is a call, and retail calls the entry address -> kn
            to = new
            if new != kn and f not in mv and f != shim and f.endswith(('.c', '.h', '.s', '.cpp', '.inc')):
                to = kn
                # ...except a DEFINITION of the host under the refuted name in some
                # other file (a pc-port translation such as day_plus.c): host name
                fp = os.path.join(SN.ROOT, f2)
                t = open(fp, encoding='utf-8').read()
                t2 = re.sub(r'^([A-Za-z_][\w \*]*?\b)' + re.escape(old) + r'(\s*\([^;{]*\)\s*\{)',
                            lambda m: m.group(1) + new + m.group(2), t, flags=re.M)
                if t2 != t:
                    open(fp, 'w', encoding='utf-8').write(t2)
                    print(f'          host definition in {f2} -> {new}')
            _pairs = [(old + '_painted', to + '_painted'), (old, to)]
            _alt = address_form_alt(old)
            if _alt and _alt != old:
                _pairs.append((_alt + '_painted', to + '_painted'))
                _pairs.append((_alt, to))
            if rename_tokens(f2, _pairs):
                touched += 1
            if f2.endswith(('.c', '.h')):
                for d in drop_self_aliases(f2):
                    print(f'          dropped self-alias in {f2}: {d[:100]}')
            if f2.endswith('.json'):
                dk = json_dup_keys(f2)
                if dk:
                    problems.append(f'{f2}: duplicate JSON keys after rename {dk[:5]}')
            if f2.startswith('src/overlay/') and f2.count('/') >= 3:
                overlays.add(f2.split('/')[2])
        for f in reg['functions']:
            if f['name'] == old:
                f['name'] = new
                f['source'] = mv.get(f['source'], f['source'])
        if tracker:
            for t in tracker.get('entries', []):
                if t.get('obin_name') == old and t.get('retail_address', '').upper() == key.upper():
                    t['status'] = 'reverted'
                    t['reverted'] = SN.TODAY
        e['reverted'] = {'date': SN.TODAY, 'from': old, 'by': 'apply_debug_symbols.py --revert',
                         'reason': 'anchor audit ' + (e.get('audit') or {}).get('verdict', '?') + ' (audit field)',
                         'target': why}
        e['name'] = new
        e['landed'] = False
        e['confidence'] = 'REVERTED'
        e['evidence'] = e.get('evidence', '') + f'; reverted {SN.TODAY} to {new} ({touched} files, {len(moves)} moves)'
        report.append((key, old, new))
    if args.apply and report:
        with open(SN.REGISTRY, 'w') as fh:
            json.dump(reg, fh, indent=2)
        SN.save_names(doc)
        open(SN.RETAIL, 'w').write(json.dumps(SN.gen_retail(doc), indent=1))
        if tracker:
            with open(SN.TRACKER, 'w') as fh:
                json.dump(tracker, fh, indent=2)
                fh.write('\n')
        sys.path.insert(0, os.path.join(SN.ROOT, 'tools', 'overlay'))
        import resolve_overlay_relocs as R  # noqa: E402
        R.load_name_table(force=True)
        print(f'reverted {len(report)}; registry, symbol_names.json, main_retail_symbols.json, tracker updated')
        print('touched overlays: ' + (' '.join(sorted(overlays)) or '(none)'))
        for p in problems:
            print('PROBLEM ' + p)
        for key, old, new in report:
            left = [f for f in refs_cs(old) if not revert_excluded(f) and not is_excluded(f, args)
                    and (old not in REVERT_RESTRICT or f in REVERT_RESTRICT[old])]
            if left:
                print(f'SURVIVOR {old}: {left}')
    return 0


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument('--dry-run', action='store_true')
    g.add_argument('--apply', action='store_true')
    g.add_argument('--gates', nargs='*', metavar='FUNC', help='print the gate commands for landed names')
    ap.add_argument('--revert', action='store_true',
                    help='revert REFUTED landed names to their address names (with --dry-run/--apply)')
    ap.add_argument('--to', metavar='func_XXXXXXXX',
                    help='--revert with one --only: the address name to restore when it is not the entry key '
                         '(an absorbed-head host whose pre-landing name was its prologue address)')
    ap.add_argument('--any-verdict', action='store_true',
                    help='--revert with --only: also revert a landed name whose audit is not `refuted` '
                         '(e.g. demoted to suspect by later evidence)')
    ap.add_argument('--no-git-lock', action='store_true',
                    help='--revert: do not treat git-modified files as locked (use --lock for others\' files)')
    ap.add_argument('--force-partial', action='store_true',
                    help='--revert: apply even when a refusal reason is printed (review by hand)')
    ap.add_argument('--min-tier', default='HIGH', choices=['HIGH', 'MEDIUM', 'DERIVED'])
    ap.add_argument('--no-require-review', action='store_true', help='land on the matcher tier alone (not recommended)')
    ap.add_argument('--only', action='append', default=[], help='address key 0x8004ABCD (repeatable)')
    ap.add_argument('--batch', type=int, default=10)
    ap.add_argument('--lock', action='append', default=[], help='extra path prefixes to treat as locked')
    ap.add_argument('--exclude', action='append', default=[], metavar='PREFIX',
                    help='path prefix the rename SKIPS (not refuses), e.g. pc/ and src/main/pc-port/ while '
                         'the PC port is out of scope; skipped references are listed')
    ap.add_argument('--skip-prose', action='store_true',
                    help='forward mode: also skip the revert mode\'s exclusions (vendor trees, Sony API '
                         'headers, docs/, *.md dated prose)')
    ap.add_argument('--allow-locked', action='append', default=[], metavar='PATH',
                    help='exact path of a git-modified file the rename may still edit (reviewed by hand: '
                         'a whole-word rename in it must be what regenerating it would produce)')
    ap.add_argument('--allow-decl', action='append', default=[], metavar='PATH',
                    help='exact path whose existing mention of the NEW name is a stale prototype/comment of this '
                         'same function (reviewed by hand; fix the prototype to the definition\'s signature)')
    args = ap.parse_args()
    args.tiers = {'HIGH'}
    if args.min_tier in ('MEDIUM', 'DERIVED'):
        args.tiers.add('MEDIUM')
    if args.min_tier == 'DERIVED':
        args.tiers.add('DERIVED')
    args.require_review = not args.no_require_review
    if args.gates is not None:
        gates(args.gates)
        return 0
    if args.revert:
        return revert_main(args)
    doc = SN.load_names()
    reg = SN.load_registry()
    reg_names = {'names': {f['name']: int(f['address'], 16) for f in reg['functions']}, 'by_addr': {}}
    for f in reg['functions']:
        reg_names['by_addr'].setdefault(int(f['address'], 16), []).append(f)
    # --no-git-lock (2026-10-17, same fix the revert path and
    # land_overlay.sh's forward pass needed): a multi-batch run leaves its own
    # earlier batches git-modified; lock only the explicit --lock paths.
    locked = set(args.lock) if args.no_git_lock else locked_files(args.lock)
    todo, refused = [], []
    for key, e in doc['symbols'].items():
        if e.get('kind') != 'func' or e.get('landed'):
            continue
        if args.only and key.upper() not in [o.upper() for o in args.only]:
            continue
        if e.get('confidence') not in args.tiers and not args.only:
            continue
        ok, reasons, old, moves, files = plan(e, key, reg_names, locked, args)
        (todo if ok else refused).append((key, e, old, moves, files, reasons))
    for key, e, old, moves, files, reasons in refused:
        print(f'REFUSE {key} {old} -> {e["name"]} [{e.get("confidence")}]')
        for r in reasons:
            print(f'        - {r}')
    todo = todo[:args.batch]
    landed, overlays = [], set()
    moved = {}          # cumulative: a file moved by an EARLIER rename in this batch
    for key, e, old, moves, files, _ in todo:
        overlays |= {f.split('/')[2] for f in files if f.startswith('src/overlay/') and f.count('/') >= 3}
        print(f'{"LAND  " if args.apply else "WOULD "} {key} {old} -> {e["name"]} [{e.get("confidence")}]: '
              f'{len(files)} files, {len(moves)} file moves')
        if not args.apply:
            for f in files:
                print(f'          {f}')
            continue
        for s, d in moves:
            shutil.move(os.path.join(SN.ROOT, moved.get(s, s)), os.path.join(SN.ROOT, d))
            moved[s] = d
        touched = 0
        for f in files:
            # files were planned before any move of this batch (a caller's file can be
            # renamed by an earlier entry): follow the cumulative move map
            f2 = moved.get(f, f)
            if rename_in_file(f2, old, e['name']):
                touched += 1
        for f in reg['functions']:
            if f['name'].upper() == old.upper():
                f['name'] = e['name']
                for s, d in moves:
                    if f['source'] == s:
                        f['source'] = d
        skipped = [f for f in refs(old) if is_excluded(f, args) and not revert_excluded(f)]
        if skipped:
            print(f'          left by --exclude ({len(skipped)}): {skipped[:6]}{"..." if len(skipped) > 6 else ""}')
        e['landed'] = True
        e['date'] = SN.TODAY
        e['evidence'] = e.get('evidence', '') + f'; landed {SN.TODAY} by apply_debug_symbols.py ({touched} files)'
        landed.append((old, e['name']))
    if args.apply and landed:
        with open(SN.REGISTRY, 'w') as fh:
            json.dump(reg, fh, indent=2)      # no trailing newline: the registry's own format
        SN.save_names(doc)
        open(SN.RETAIL, 'w').write(json.dumps(SN.gen_retail(doc), indent=1))
        sys.path.insert(0, os.path.join(SN.ROOT, 'tools', 'overlay'))
        import resolve_overlay_relocs as R  # noqa: E402
        tab = R.load_name_table(force=True)    # raises NameTableError on any inconsistency
        for _, n in landed:
            assert n in tab, f'{n} missing from the resolver name table'
        print(f'resolver name table OK ({len(tab)} names)')
        print(f'landed {len(landed)}; registry, symbol_names.json and main_retail_symbols.json updated')
        gates([n for _, n in landed], overlays)
    elif not todo:
        print('nothing landable' + (' (dry run)' if args.dry_run else ''))
    return 0


if __name__ == '__main__':
    sys.exit(main())
