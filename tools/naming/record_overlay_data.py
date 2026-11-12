#!/usr/bin/env python3
"""Record adjudicated OVERLAY data names as reviewed, pending data records.

The OVERLAY-DATA sibling of record_data_decided.py (main data) and
record_decided.py (overlay functions).  Input: one or more decision files,
each a JSON list of rows
    {"id": "OV:0x8015BDA4" (or bare "0xADDR" with --ov OV),
     "decision": "LAND"|"DEFER", "name": "...", "size": N, "type": "u16",
     "tier": "T1"|"T2"|"T3"|"T4", "style": "sony"|"derived"|"konami"|...,
     "confidence": "HIGH"|"MEDIUM"|"DERIVED"|"LOW", "evidence": "...",
     "note": "..." (optional)}

Each LAND row becomes a config/symbol_names.json record
    {"name", "kind": "data", "scope": OV, "size", "type", "tier", "style",
     "confidence", "evidence": "...; reviewed:consistent (DATE, BY)",
     "proposed_by", "date", "landed": false}
ready for a future overlay data lander.  This tool NEVER lands or renames
anything in the tree: `landed` is always false.

Refused (reported as REFUSE lines, exit 1 -- nothing proceeds silently):
  * a name held by ANY other record in the ledger, by a batch row recorded
    first, or used as an identifier in the tree (`git grep -w`, comments and
    strings stripped, sources only: *.c *.h *.s outside pc/3rd/);
  * an overwrite of a record whose `landed` is true (corrections are
    refine_landed.py's job).  A PENDING record at the id may be updated; its
    old content is preserved in `history` with `confidence: REVERTED`;
  * an address-shaped name (D_/func_ forms) or a non-identifier;
  * a Sony-style name (the naming_lint.py R2 shape) or a T1/T2/T3 row without
    `corroborated:` evidence -- a Sony name is never invented;
  * a scope that is not a real directory under src/overlay/, the never-loaded
    EVENT/GYOZI overlays, an address outside the overlay RAM window, or a
    main-scope id (record_data_decided.py records main data).

Skipped (reported): DEFER rows, rows for another overlay (--ov filter), rows
whose record already matches the row exactly (idempotent re-run: no churn).

The ledger writer holds landing.lock at the tree root throughout (the
land_data.sh / land_overlay.sh convention).  --ledger PATH redirects the
ledger; the tree root (src/overlay/ check, git grep, landing.lock) is the
parent of the ledger's config/ directory, so tests run on temp copies and
never touch the real config/symbol_names.json.

usage: record_overlay_data.py DECIDED.json [...] [--ov OV] [--by LABEL]
           [--apply | --dry-run] [--ledger PATH]
"""
import argparse
import fcntl
import json
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, 'tools', 'analysis'))
sys.path.insert(0, os.path.join(ROOT, 'tools', 'overlay'))
import naming_lint as NL  # noqa: E402
import overlay_names as ON  # noqa: E402
import symbol_names as SN  # noqa: E402

IDENT_RE = re.compile(r'^[A-Za-z_][A-Za-z0-9_]*$')
STR_COMMENT = re.compile(r'''"(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*'|/\*[\s\S]*?\*/|//[^\n]*''')
TIERS = ('T1', 'T2', 'T3', 'T4')
CONFIDENCES = ('HIGH', 'MEDIUM', 'DERIVED', 'LOW')
DEAD_OVERLAYS = ('EVENT', 'GYOZI')      # never loaded
SRC_EXTS = ('.c', '.h', '.s')
CORE_FIELDS = ('name', 'kind', 'scope', 'size', 'type', 'tier', 'style', 'confidence')


# ------------------------------------------------------------------ checks
def looks_sony(name):
    """The naming_lint.py R2 shape: Sony prefix + not plain snake_case."""
    return bool(NL.SONY_PREFIX.match(name)) and not NL.SNAKE.match(name)


def needs_corroboration(name, tier, style):
    return style == 'sony' or tier in ('T1', 'T2', 'T3') or looks_sony(name)


def corroborated(evidence):
    """`corroborated:` with a non-empty source clause after it."""
    if 'corroborated:' not in evidence:
        return False
    clause = evidence.split('corroborated:', 1)[1].split(';')[0].strip()
    return bool(clause)


def code_mentions(path, name):
    """Is NAME used outside comments/strings in this source file?"""
    with open(path, encoding='utf-8', errors='replace', newline='') as fh:
        raw = fh.read()
    text = STR_COMMENT.sub(lambda m: ' ' if m[0].startswith(('/*', '//')) else m[0], raw)
    if path.lower().endswith('.s'):
        text = re.sub(r'(?m)^\s*#(?!include|define|if|else|endif)\b[^\n]*', '', text)
    return re.search(r'(?<![A-Za-z0-9_])' + re.escape(name) + r'(?![A-Za-z0-9_])', text)


def identifier_hits(name):
    """Files where NAME is a whole-word identifier use (git grep -w), or an
    error string when git grep itself fails (never silently proceed)."""
    out = subprocess.run(['git', 'grep', '--untracked', '-I', '-l', '-w', '-F', '--', name],
                         capture_output=True, text=True)
    if out.returncode not in (0, 1):
        return None, f'git grep failed: {out.stderr.strip()}'
    hits = []
    for f in out.stdout.splitlines():
        if not f or f.startswith('pc/3rd/'):
            continue
        if os.path.splitext(f)[1].lower() not in SRC_EXTS:
            continue
        if code_mentions(f, name):
            hits.append(f)
    return hits, None


# ------------------------------------------------------------------ records
def build_evidence(r, by):
    ev = r.get('evidence', '') or ''
    if r.get('note'):
        ev += f"; note: {r['note']}"
    if 'reviewed:' not in ev:
        ev += f'; reviewed:consistent ({SN.TODAY}, {by})'
    return ev


def build_record(r, scope, by):
    rec = {'name': r['name'], 'kind': 'data', 'scope': scope, 'size': r.get('size')}
    if r.get('type'):
        rec['type'] = r['type']
    rec.update({'tier': r.get('tier', 'T4'), 'style': r.get('style', 'derived'),
                'confidence': r.get('confidence', 'DERIVED'),
                'evidence': build_evidence(r, by), 'proposed_by': by,
                'date': SN.TODAY, 'landed': False})
    return rec


def already_recorded(rec, new, row_ev):
    """Same substantive content and the row's evidence is present (idempotent)."""
    return (all(rec.get(f) == new.get(f) for f in CORE_FIELDS)
            and (not row_ev or row_ev in rec.get('evidence', '')))


def supersede(cur, new):
    """A pending-record update: the old content goes to `history` verbatim,
    with the REVERTED confidence marker (the corrections convention)."""
    old = {k: v for k, v in cur.items() if k != 'history'}
    old['confidence'] = 'REVERTED'
    new['history'] = cur.get('history', []) + [old]
    return new


# --------------------------------------------------------------------- main
def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('decided', nargs='+')
    ap.add_argument('--ov', help='overlay scope for bare 0xADDR ids')
    ap.add_argument('--by', default='overlay-data blind-review',
                    help='provenance label written into the record')
    ap.add_argument('--apply', action='store_true', help='write the ledger (default: dry run)')
    ap.add_argument('--dry-run', action='store_true',
                    help='show the plan and write nothing (the default behaviour)')
    ap.add_argument('--ledger', default=os.path.join(ROOT, 'config', 'symbol_names.json'),
                    help='ledger to update (default config/symbol_names.json)')
    args = ap.parse_args()
    ledger = os.path.abspath(args.ledger)
    tree = os.path.dirname(os.path.dirname(ledger))
    os.chdir(tree)
    SN.NAMES = ledger
    write = args.apply and not args.dry_run

    lock = None
    # the landing.lock convention: hold it while writing the tree -- unless the
    # caller already holds it (land_overlay_data.sh sets TOKI_LOCK_HELD=1;
    # a second flock on a new open file would wait for itself forever)
    if write and os.environ.get('TOKI_LOCK_HELD') != '1':
        lock = open(os.path.join(tree, 'landing.lock'), 'a')
        fcntl.flock(lock, fcntl.LOCK_EX)
    doc = SN.load_names(all_scopes=True)
    syms = doc['symbols']
    want_ov = args.ov.upper() if args.ov else None

    added, updated = 0, 0
    skipped, refused, seen = [], [], {}
    for path in args.decided:
        for r in json.load(open(path)):
            rid = r.get('id', '?')
            if r.get('decision') != 'LAND':
                skipped.append((rid, f"deferred (decision {r.get('decision')})"))
                continue
            try:
                key = SN.norm_key(rid)
                scope, addr = SN.split_key(key)
            except (ValueError, AttributeError):
                refused.append((rid, 'bad id (want OV:0xADDR, or bare 0xADDR with --ov)'))
                continue
            if scope == 'main':
                if ':' in rid or not want_ov:
                    refused.append((rid, 'main-scope row: record_data_decided.py records main data'))
                    continue
                scope, key = want_ov, f'{want_ov}:{SN.akey(addr)}'
            elif want_ov and scope != want_ov:
                skipped.append((key, f'row for another overlay {scope}'))
                continue
            name = r.get('name')
            if not name:
                refused.append((key, 'row has no name'))
                continue
            if scope in DEAD_OVERLAYS:
                refused.append((key, f'{scope} is never loaded: do not name anything in it'))
                continue
            if not os.path.isdir(os.path.join('src', 'overlay', scope)):
                refused.append((key, f'no src/overlay/{scope}/ directory'))
                continue
            if not (ON.OVERLAY_BASE <= addr <= ON.RAM_HI):
                refused.append((key, 'address outside the overlay RAM window'))
                continue
            if not IDENT_RE.match(name):
                refused.append((key, f'`{name}` is not a C identifier'))
                continue
            if SN.is_address_name(name):
                refused.append((key, f'`{name}` is address-shaped; a record must not carry an address'))
                continue
            tier = r.get('tier', 'T4')
            style = r.get('style', 'derived')
            conf = r.get('confidence', 'DERIVED')
            if tier not in TIERS or conf not in CONFIDENCES or not style:
                refused.append((key, f'tier/style/confidence out of range: {tier}/{style}/{conf}'))
                continue
            if needs_corroboration(name, tier, style) and not corroborated(r.get('evidence', '') or ''):
                refused.append((key, f'Sony-style/T1-T3 name without `corroborated:` evidence '
                                     '(a Sony name is never invented)'))
                continue
            if name in seen:
                refused.append((key, f'duplicate name in the batch (also {seen[name]})'))
                continue
            held = [k2 for k2, e2 in syms.items() if k2 != key and e2.get('name') == name]
            if held:
                refused.append((key, f'{name} is already held by {held[0]} '
                                     f'({"landed" if syms[held[0]].get("landed") else "pending"})'))
                continue
            hits, scan_err = identifier_hits(name)
            if scan_err:
                refused.append((key, scan_err))
                continue
            if hits:
                refused.append((key, f'{name} is already used as an identifier in {hits[0]}'))
                continue
            new = build_record(r, scope, args.by)
            cur = syms.get(key)
            if cur is not None:
                if already_recorded(cur, new, r.get('evidence', '') or ''):
                    skipped.append((key, f'already recorded: {cur["name"]}'))
                    continue
                if cur.get('landed'):
                    refused.append((key, f'landed record {cur["name"]}: this tool never overwrites one '
                                         '(corrections are refine_landed.py\'s job)'))
                    continue
                syms[key] = supersede(cur, new)
                updated += 1
            else:
                syms[key] = new
                added += 1
            seen[name] = key
    print(f'{added} records added, {updated} updated, {len(skipped)} skipped, {len(refused)} refused')
    for k, why in skipped:
        print('  SKIP', k, why)
    for k, why in refused:
        print('  REFUSE', k, why)
    if write:
        if added or updated:
            SN.save_names(doc)
        else:
            print('ledger unchanged')
    else:
        print('dry run: ledger not written (use --apply)')
    return 1 if refused else 0


if __name__ == '__main__':
    sys.exit(main())
