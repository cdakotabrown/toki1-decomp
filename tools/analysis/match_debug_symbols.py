#!/usr/bin/env python3
"""Match retail main-binary functions to o.bin debug names,
calibrated by leave-one-out on the landed names.

WHAT THE DATA SAYS (2026-09-26, measured before this was written)
  * o.bin is a different source revision linked in the SAME ORDER: over the
    355 landed anchors, consecutive-anchor distances correlate 0.985 and only
    0.6% of anchor pairs are order-inverted, but the per-function debug gap has
    Spearman 0.00 against retail size.  So: position + order are the signal,
    size is not, and name semantics against callee/caller/neighbour names
    scored 0.50 precision in LOO (chance) -- they are NOT used for tiering.
  * The names landed by the 2026-04 matcher are of unequal quality: the 105
    tracker-LOW names show a median drift discontinuity 4x that of the
    tracker-ANCHOR names, and 22 landed names violate the debug link order
    outright.  Order violators are excluded from the anchor set and reported.

METHOD
  1. anchors = landed names (config/symbol_names.json) present in o.bin;
     trusted = the maximum-weight order-consistent chain (LIS on debug address,
     weighted by tracker class).
  2. every interval between consecutive trusted anchors gives a retail list R
     (non-enclosed unnamed functions) and a debug list D (unnamed externs);
     a monotone DP alignment (Needleman-Wunsch, gaps allowed on both sides)
     pairs them, scored by position error against linear interpolation of the
     two bounding anchors' drifts (+ a small size term).
  3. tiers, from the LOO calibration on 211 game-region trusted anchors
     (`--calibrate` reproduces the table):
       HIGH   = 1:1 interval with strong bounds or error <= 100 B (10/10), or position error <= 64 B with the
                runner-up >= 200 B away and STRONG bounds (13/13), or error <= 48 B
                with the two bounds' drifts within 48 B and strong bounds (15/15)
       MEDIUM = error <= 64 & runner-up >= 200 (weak bounds), or error <= 100 with
                drift-consistent bounds, or pos >= .8 & margin >= .8 (85-91%)
       LOW    = any other aligned pair
     STRONG bound = tracker ANCHOR/HIGH, a project-assigned name, or a name
     landed by this tool at HIGH.  Library band (>= 0x80086000) is capped at
     MEDIUM: CLAUDE.md forbids a Sony/library-band rename from o.bin alone.

OUTPUT
  build/symbol_match/match_debug_symbols.json   every prediction + evidence
  docs/worklists/SYMBOL-NAMES-REVIEW.md         MEDIUM/LOW/blocked worklist
  --write-names adds HIGH/MEDIUM/LOW candidates (landed=false) to
  config/symbol_names.json for apply_debug_symbols.py.
"""
import argparse, bisect, collections, json, math, os, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import symbol_names as SN  # noqa: E402

OUT_JSON = os.path.join(SN.ROOT, 'build/symbol_match/match_debug_symbols.json')
WORKLIST = os.path.join(SN.ROOT, 'docs/worklists/SYMBOL-NAMES-REVIEW.md')
MAX_CELLS = 4000       # |R| x |D| above which an interval is not aligned
STRONG_CLASSES = ('ANCHOR', 'HIGH', 'not-in-tracker', 'project', 'matcher-HIGH', 'audited')


def audit_verdict(entry):
    return (entry.get('audit') or {}).get('verdict')


def tracker_class(entry, anchor_mode='all'):
    if anchor_mode == 'confirmed' and audit_verdict(entry) == 'confirmed':
        return 'audited'
    ev = entry.get('evidence', '')
    if 'tracker:' in ev:
        return ev.split('tracker:')[1].split(';')[0].strip()
    if entry.get('confidence') == 'HIGH' and 'match_debug_symbols' in ev:
        return 'matcher-HIGH'
    return 'project'


class Matcher:
    def __init__(self, names_doc, ext, uni, callees, callers, hide=None, anchor_mode='all'):
        self.uni = uni
        self.R = sorted(a for a in uni if a < SN.RETAIL_TEXT_END)
        self.D = [(a, n) for a, n in ext if a < SN.DEBUG_TEXT_END]
        self.dname = {n: a for a, n in self.D}
        self.anchors, self.cls = {}, {}
        for k, e in names_doc['symbols'].items():
            a = int(k, 16)
            if e.get('kind') != 'func' or not e.get('landed') or e['name'] not in self.dname:
                continue
            if hide is not None and a == hide:
                continue
            if a not in uni:
                continue
            if anchor_mode == 'confirmed' and audit_verdict(e) != 'confirmed':
                continue
            self.anchors[a] = e['name']
            self.cls[a] = tracker_class(e, anchor_mode)
        self.trusted, self.order_violators = self._trusted_chain()
        self.callees, self.callers = callees, callers

    def _trusted_chain(self):
        ra = sorted(self.anchors)
        w = {'audited': 3, 'ANCHOR': 3, 'HIGH': 3, 'not-in-tracker': 3, 'project': 3, 'matcher-HIGH': 3, 'MEDIUM': 2, 'LOW': 1}
        da = [self.dname[self.anchors[r]] for r in ra]
        n = len(ra)
        best, prev = [0] * n, [-1] * n
        for i in range(n):
            wi = w.get(self.cls[ra[i]], 1)
            best[i] = wi
            for j in range(i):
                if da[j] < da[i] and best[j] + wi > best[i]:
                    best[i], prev[i] = best[j] + wi, j
        k = max(range(n), key=lambda i: best[i]) if n else -1
        chain = set()
        while k != -1:
            chain.add(ra[k]); k = prev[k]
        return {r: self.anchors[r] for r in ra if r in chain}, {r: self.anchors[r] for r in ra if r not in chain}

    def strong(self, r):
        return self.cls.get(r) in STRONG_CLASSES

    def interval_for(self, f):
        ta = sorted(self.trusted)
        i = bisect.bisect_left(ta, f)
        if i == 0 or i >= len(ta):
            return None
        return self.interval(ta[i - 1], ta[i])

    def interval(self, r1, r2):
        d1, d2 = self.dname[self.trusted[r1]], self.dname[self.trusted[r2]]
        if d2 <= d1:
            return None
        used = set(self.anchors.values())
        R = [a for a in self.R if r1 < a < r2 and a not in self.anchors]
        D = [(a, n) for a, n in self.D if d1 < a < d2 and n not in used]
        return dict(r1=r1, r2=r2, d1=d1, d2=d2, R=R, D=D)

    def predicted(self, f, iv):
        return iv['d1'] + (f - iv['r1']) * (iv['d2'] - iv['d1']) / (iv['r2'] - iv['r1'])

    def score(self, f, dn, iv):
        da, name = dn
        dp = self.predicted(f, iv)
        scale = max(128, 0.25 * (iv['d2'] - iv['d1']))
        pos = math.exp(-abs(da - dp) / scale)
        sz = self.uni[f]['size']
        gap = self.gaps.get(name) or sz
        size = math.exp(-abs(math.log(max(gap, 4) / max(sz, 4))))
        return pos + 0.2 * size, pos

    def align(self, iv, forbid=None, gap=0.45):
        R, D = iv['R'], iv['D']
        n, m = len(R), len(D)
        S = [[0.0] * (m + 1) for _ in range(n + 1)]
        B = [[None] * (m + 1) for _ in range(n + 1)]
        for i in range(1, n + 1):
            S[i][0] = -gap * i
        for j in range(1, m + 1):
            S[0][j] = -gap * j
        for i in range(1, n + 1):
            for j in range(1, m + 1):
                ms, _ = self.score(R[i - 1], D[j - 1], iv)
                if forbid and (R[i - 1], D[j - 1][1]) in forbid:
                    ms = -9
                S[i][j], B[i][j] = max((S[i - 1][j - 1] + ms, 'm'), (S[i - 1][j] - gap, 'u'), (S[i][j - 1] - gap, 'l'))
        pairs, i, j = [], n, m
        while i > 0 and j > 0:
            if B[i][j] == 'm':
                pairs.append((R[i - 1], D[j - 1])); i -= 1; j -= 1
            elif B[i][j] == 'u':
                i -= 1
            else:
                j -= 1
        return S[n][m], list(reversed(pairs))

    def predict_interval(self, iv):
        out = []
        if not iv['R']:
            return out
        if not iv['D']:
            return [dict(f=f, name=None, reason='no unnamed debug extern in the interval') for f in iv['R']]
        if len(iv['R']) * len(iv['D']) > MAX_CELLS:
            # a proximity alignment over hundreds of functions carries no signal, and the
            # per-pair margin re-alignment is O(pairs * n * m) -- refuse instead of guessing
            return [dict(f=f, name=None, reason=f"interval too wide ({len(iv['R'])} retail x {len(iv['D'])} debug "
                                                f"between {self.trusted[iv['r1']]} and {self.trusted[iv['r2']]})")
                    for f in iv['R']]
        s, pairs = self.align(iv)
        got = {}
        for f, dn in pairs:
            s2, _ = self.align(iv, forbid={(f, dn[1])})
            _, pos = self.score(f, dn, iv)
            dp = self.predicted(f, iv)
            err = abs(dn[0] - dp)
            others = sorted(abs(x[0] - dp) for x in iv['D'] if x[1] != dn[1])
            err2 = others[0] if others else 10 ** 6
            got[f] = dict(f=f, name=dn[1], daddr=dn[0], pos=round(pos, 3), margin=round(s - s2, 3),
                          err=int(err), err2=int(min(err2, 10 ** 6)), nR=len(iv['R']), nD=len(iv['D']),
                          ddrift=abs((iv['d1'] - iv['r1']) - (iv['d2'] - iv['r2'])),
                          strong=self.strong(iv['r1']) and self.strong(iv['r2']),
                          bounds=(self.trusted[iv['r1']], self.trusted[iv['r2']]),
                          bound_cls=(self.cls[iv['r1']], self.cls[iv['r2']]))
        for f in iv['R']:
            out.append(got.get(f, dict(f=f, name=None, reason='unaligned (DP gap)', nR=len(iv['R']), nD=len(iv['D']))))
        return out

    def predict_all(self):
        out = []
        ta = sorted(self.trusted)
        for i in range(len(ta) - 1):
            iv = self.interval(ta[i], ta[i + 1])
            if iv:
                out.extend(self.predict_interval(iv))
        return out

    def predict_one(self, f):
        iv = self.interval_for(f)
        if iv is None:
            return dict(f=f, name=None, reason='no bounding trusted anchors')
        for p in self.predict_interval(iv):
            if p['f'] == f:
                return p
        return dict(f=f, name=None, reason='?')


def tier(p):
    if p.get('name') is None:
        return 'NONE'
    lib = p['f'] >= SN.LIB_START
    one = p['nR'] == 1 and p['nD'] == 1 and (p['strong'] or p['err'] <= 100)
    t = 'LOW'
    if one or (p['err'] <= 64 and p['err2'] >= 200 and p['strong']) or \
            (p['err'] <= 48 and p['ddrift'] <= 48 and p['strong']):
        t = 'HIGH'
    elif (p['err'] <= 64 and p['err2'] >= 200) or (p['err'] <= 100 and p['ddrift'] <= 32) or \
            (p['pos'] >= 0.8 and p['margin'] >= 0.8) or (p['nR'] == p['nD'] and p['nR'] <= 3):
        t = 'MEDIUM'
    if lib and t == 'HIGH':
        t = 'MEDIUM'
    return t


def evidence(p, t):
    if p.get('name') is None:
        return p.get('reason', '')
    lib = ' [library band: capped at MEDIUM, o.bin alone cannot name it]' if p['f'] >= SN.LIB_START else ''
    return (f"interval {p['bounds'][0]}..{p['bounds'][1]} (bounds {p['bound_cls'][0]}/{p['bound_cls'][1]}"
            f"{', strong' if p['strong'] else ''}): retail {p['nR']} unnamed vs debug {p['nD']} unnamed; "
            f"debug {SN.akey(p['daddr'])} is {p['err']} B from the drift-interpolated position, runner-up "
            f"{'none' if p['err2'] >= 10 ** 6 else str(p['err2']) + ' B'}; bound drift delta {p['ddrift']} B; "
            f"DP margin {p['margin']}{lib}")


def load_all():
    doc = SN.load_names()
    ext = SN.load_obin()
    uni = SN.retail_universe()
    callees, callers = SN.callgraph(uni)
    return doc, ext, uni, callees, callers


def calibrate(doc, ext, uni, callees, callers, anchor_mode='all'):
    """Leave-one-out over the trusted anchors: hide one landed name, predict it."""
    base = Matcher(doc, ext, uni, callees, callers, anchor_mode=anchor_mode)
    base.gaps = dict((n, g) for n, g in gaps_of(ext).items())
    res = collections.Counter()
    wrong_high = []
    total = 0
    for x in sorted(base.trusted):
        m = Matcher(doc, ext, uni, callees, callers, hide=x, anchor_mode=anchor_mode)
        m.gaps = base.gaps
        p = m.predict_one(x)
        t = tier(p)
        ok = p.get('name') == base.anchors[x]
        region = 'game' if x < SN.LIB_START else 'lib'
        res[(region, t, 'ok' if ok else 'wrong')] += 1
        total += 1
        if t == 'HIGH' and not ok:
            wrong_high.append((SN.akey(x), base.anchors[x], p.get('name'), evidence(p, t)))
    print(f'LOO over {total} trusted anchors, anchor set = {anchor_mode} '
          f'({len(base.order_violators)} order violators excluded):')
    for region in ('game', 'lib'):
        print(f'  {region}:')
        for t in ('HIGH', 'MEDIUM', 'LOW', 'NONE'):
            ok, wr = res[(region, t, 'ok')], res[(region, t, 'wrong')]
            n = ok + wr
            hidden = sum(v for k, v in res.items() if k[0] == region)
            print(f'    {t:6s} n={n:3d} correct={ok:3d} precision={ok / n if n else 0:.2f} recall={ok / hidden if hidden else 0:.2f}')
    if wrong_high:
        print('  HIGH but wrong:')
        for w in wrong_high:
            print('   ', w)
    return res


def gaps_of(ext):
    out = {}
    for i, (a, n) in enumerate(ext):
        out[n] = (ext[i + 1][0] - a) if i + 1 < len(ext) else None
    return out


def overlay_referenced_names():
    """main func_ names referenced from overlay C.  Since 2026-09-27 the overlay
    resolver reads the landed-name table (resolve_overlay_relocs.load_name_table),
    so these CAN be renamed; a rename that touches overlay C needs a serial
    verify_overlay_bytes.py of every touched overlay (informational flag only)."""
    import subprocess
    r = subprocess.run(['git', 'grep', '-h', '-o', '-E', r'\bfunc_800[4-9AB][0-9A-Fa-f]{4}\b', '--', 'src/overlay'],
                       capture_output=True, text=True, cwd=SN.ROOT)
    return set(x.strip().upper() for x in r.stdout.split())


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--calibrate', action='store_true', help='leave-one-out over the trusted anchors')
    ap.add_argument('--write-names', action='store_true', help='add candidates to config/symbol_names.json')
    ap.add_argument('--no-worklist', action='store_true')
    ap.add_argument('--anchors', choices=['all', 'confirmed'], default='all',
                    help="anchor set: every landed o.bin name (default), or only the names the "
                         "anchor audit marked confirmed (symbol_names.py audit)")
    args = ap.parse_args()
    doc, ext, uni, callees, callers = load_all()
    if args.calibrate:
        calibrate(doc, ext, uni, callees, callers, anchor_mode=args.anchors)
        return 0
    m = Matcher(doc, ext, uni, callees, callers, anchor_mode=args.anchors)
    m.gaps = gaps_of(ext)
    preds = m.predict_all()
    ovl = overlay_referenced_names()
    used_names = {e['name'] for e in doc['symbols'].values() if e.get('landed')}
    rows = []
    for p in preds:
        t = tier(p)
        cur = uni[p['f']]['name'] or f'func_{p["f"]:08X}'
        blocked = []
        if p.get('name') is not None and p.get('name') == cur:
            blocked.append('re-predicts its own landed name (corroborates a non-anchor landed name; nothing to land)')
        elif p.get('name') in used_names:
            blocked.append('name already assigned elsewhere')
        elif not SN.is_address_name(cur):
            blocked.append(f'function already carries the landed name {cur} (disagrees)')
        rows.append(dict(address=SN.akey(p['f']), current=cur, name=p.get('name'), tier=t,
                         overlay_ref=cur.upper() in ovl,
                         size=uni[p['f']]['size'], kind=uni[p['f']]['kind'], registered=uni[p['f']]['registered'],
                         evidence=evidence(p, t), blocked=blocked,
                         hint=doc.get('hints', {}).get(SN.akey(p['f']), {}).get('name')))
    counts = collections.Counter((r['tier'], 'blocked' if r['blocked'] else 'free') for r in rows)
    print(f'anchor set = {args.anchors}: {len(m.anchors)} anchors, {len(m.trusted)} trusted')
    print(f'{len(rows)} unnamed retail functions between trusted anchors; {len(m.order_violators)} order-violating '
          f'landed names excluded from the anchor set: {sorted(m.order_violators.values())}')
    for k in sorted(counts):
        print(f'  {k[0]:6s} {k[1]:8s} {counts[k]}')
    os.makedirs(os.path.dirname(OUT_JSON), exist_ok=True)
    json.dump({'date': SN.TODAY, 'anchor_mode': args.anchors, 'trusted_anchors': len(m.trusted),
               'order_violators': {SN.akey(a): n for a, n in m.order_violators.items()},
               'predictions': rows}, open(OUT_JSON, 'w'), indent=1)
    print(f'-> {os.path.relpath(OUT_JSON, SN.ROOT)}')
    if args.write_names:
        n = 0
        for r in rows:
            if r['name'] is None or r['tier'] == 'NONE':
                continue
            if 'name already assigned elsewhere' in r['blocked'] or \
                    any(e2['name'] == r['name'] and k2 != r['address'] for k2, e2 in doc['symbols'].items()):
                continue        # the name is held by another entry (landed or pending); worklist still lists it
            k = r['address']
            e = doc['symbols'].get(k)
            if e and e.get('landed'):
                continue
            keep = ''
            conf = r['tier']
            if e and e['name'] == r['name']:
                import re as _re
                mm = _re.search(r'; reviewed:\w+.*$', e.get('evidence', ''))
                keep = mm.group(0) if mm else ''
                if 'reviewed:contradicted' in keep and conf == 'HIGH':
                    conf = 'MEDIUM'
            doc['symbols'][k] = {'name': r['name'], 'kind': 'func', 'size': r['size'], 'confidence': conf,
                                 'evidence': 'match_debug_symbols: ' + r['evidence'] +
                                             ('; BLOCKED: ' + '; '.join(r['blocked']) if r['blocked'] else '') + keep,
                                 'date': SN.TODAY, 'landed': False}
            n += 1
        SN.save_names(doc)
        print(f'wrote {n} candidates (landed=false) -> config/symbol_names.json')
    if not args.no_worklist:
        write_worklist(rows, m, args.anchors)
    return 0


def write_worklist(rows, m, anchor_mode='all'):
    doc = SN.load_names()
    def review_of(addr):
        e = doc['symbols'].get(addr, {})
        ev = e.get('evidence', '')
        i = ev.find('; reviewed:')
        return ev[i + 2:] if i >= 0 else ''
    L = ['# Symbol-name review worklist (o.bin -> retail, Phase 1)', '',
         f'Generated {SN.TODAY} by `tools/analysis/match_debug_symbols.py`. HIGH rows land via '
         '`apply_debug_symbols.py` only after a `symbol_names.py review ... consistent` verdict; MEDIUM lands only '
         'after a reviewer reads the retail code; LOW never lands. '
         'Method, calibration and the evidence format: the tool docstring.', '',
         '## What the measurements changed (Phase 0/1 findings, 2026-09-26)', '',
         '* o.bin is a DIFFERENT SOURCE REVISION linked in the same order: consecutive-anchor distances correlate '
         '0.985, only 0.6% of anchor pairs are order-inverted, but the debug gap has Spearman 0.00 against retail size '
         '-- the plan\'s "size within +-32 B" hypothesis is refuted; gap is not a size signal.',
         '* The debug link\'s text ends at 0x800C1480 (BIOS thunks); LIBGTE sits at +48624, above the retail data '
         'boundary 0x800BB3C0. `symbol_names.py inventory` splits by the debug boundary (1,559 functions / 573 data).',
         '* The 355 landed o.bin names are of unequal quality: 163 tracker-ANCHOR, 105 LOW, 38 MEDIUM, 17 HIGH, 32 '
         'project-assigned. LOW names show a median drift discontinuity 4x the ANCHOR names; 23 violate the link '
         'order outright (table below); code reading refuted the tpage cluster and the file_select window (notes below).',
         '* Leave-one-out over the 335 order-consistent anchors: HIGH 29/29 (recall 14%), MEDIUM 24/34, LOW 56/115 in '
         'the game region -- but that precision is measured AGAINST proximity guesses, and code reading contradicted '
         '6 of the 9 real HIGH rows, so no name lands on the matcher tier alone.',
         '* RESOLVED 2026-09-27: the overlay resolver (`resolve_overlay_relocs.py`) now consults the landed-name '
         'table from `config/symbol_names.json` before the `_XXXXXXXX` suffix rule (hard error on a name landed twice '
         'or disagreeing with its own suffix; `--check-name-table`). Overlay references no longer block a rename; '
         'they add a serial `verify_overlay_bytes.py` of every touched overlay to the gates.',
         '* ANCHOR AUDIT 2026-09-27 (code reading of all 358 landed o.bin names): 27 confirmed (20 LIBGTE '
         'whole-member byte identities + InvSquareRoot; only 6 game-region names), 100 suspect, 231 refuted. The '
         '2026-04 proximity names are mostly WRONG, in structured ways: the retail library band holds '
         'libpress/libcd/libsnd/libspu/libgs/libgpu where the debug link has holiday_*/SD_*/bust_up*/Ss*/Snd_*/Cont* '
         'names (retail SetProgramChange is libgpu DrawSyncCallback, _spu_r is set_alarm, SpuQuit is rsin); retail '
         'card_check/cardfile_init/card_w_clear are o.bin initView/initLight/initCoordinate. The "linked in the same '
         'order" correlation (0.985) was measured on proximity anchors and is circular; it does not hold region-wide.',
         '* With confirmed anchors only (`--anchors confirmed`), leave-one-out recovers 0 of 6 game-region anchors and '
         '14/14 LIBGTE (trivial: verbatim, constant drift); the old HIGH 29/29 was proximity names predicting '
         'proximity names. Intervals wider than MAX_CELLS are no longer aligned. Two names landed after code review: '
         '`menu_check_2` (0x8005BEC0, the one HIGH that survived the audit) and then `menu_check_1` (0x8005BE0C, HIGH '
         'once menu_check_2 was an anchor). LOO then reads 3/3 game HIGH -- three adjacent menu-window anchors '
         'predicting each other, not a region-wide calibration.', '']
    aud = collections.Counter(audit_verdict(e) for e in doc['symbols'].values() if e.get('landed'))
    L += [f'## Anchor audit of the landed names ({SN.TODAY})', '',
          f'Verdicts recorded with `symbol_names.py audit` (structured `audit` field): '
          f'{aud.get("confirmed", 0)} confirmed, {aud.get("suspect", 0)} suspect, {aud.get("refuted", 0)} refuted, '
          f'{aud.get(None, 0)} not audited (project-assigned). Matcher run with `--anchors {anchor_mode}`. '
          'Refuted names are NOT renamed back automatically; each row carries the proposed action for a reviewer.', '',
          '### Refuted', '', '| retail | landed name | evidence | proposed action |', '|---|---|---|---|']
    for k, e in doc['symbols'].items():
        au = e.get('audit') or {}
        if au.get('verdict') == 'refuted':
            L.append(f"| {k} | `{e['name']}` | {au.get('note', '')} | {au.get('proposed_action', '')} |")
    L += ['', '### Suspect', '', '| retail | landed name | evidence |', '|---|---|---|']
    for k, e in doc['symbols'].items():
        au = e.get('audit') or {}
        if au.get('verdict') == 'suspect':
            L.append(f"| {k} | `{e['name']}` | {au.get('note', '')} |")
    L.append('')
    L += ['## Landed names under suspicion (code reading / prior analyses)', '',
          '| retail | landed name | note |', '|---|---|---|']
    for k, e in doc['symbols'].items():
        for n in e.get('notes', []):
            L.append(f'| {k} | `{e["name"]}` | {n} |')
    L.append('')
    L += ['## Landed names the debug link order REFUTES', '',
          'These names are already in the tree but sit out of order against the o.bin link order '
          '(the o.bin name belongs to a neighbour, or the 2026-04 proximity match was wrong). '
          'Excluded from the anchor set; each needs a reviewer to re-derive the name from the code.', '',
          '| retail | landed name | debug addr | tracker class |', '|---|---|---|---|']
    for a, n in sorted(m.order_violators.items()):
        L.append(f'| {SN.akey(a)} | `{n}` | {SN.akey(m.dname[n])} | {m.cls.get(a)} |')
    for t, title in (('HIGH', 'HIGH tier (lands only after review:consistent)'), ('MEDIUM', 'MEDIUM tier (reviewer lands)'),
                     ('LOW', 'LOW tier (never lands; reference only)')):
        sel = [r for r in rows if r['tier'] == t]
        L += ['', f'## {title} -- {len(sel)} rows', '', '| retail | current | candidate | size | hint | evidence |', '|---|---|---|---|---|---|']
        for r in sel:
            ev = r['evidence'] + (' **BLOCKED: ' + '; '.join(r['blocked']) + '**' if r['blocked'] else '') + \
                (' [overlay-referenced]' if r.get('overlay_ref') else '')
            rv = review_of(r['address'])
            if rv:
                ev += ' **' + rv + '**'
            L.append(f"| {r['address']} | `{r['current']}` | `{r['name']}` | {r['size']} | {r['hint'] or ''} | {ev} |")
    none = [r for r in rows if r['tier'] == 'NONE']
    L += ['', f'## No candidate -- {len(none)} retail functions',
          '', 'Unnamed retail functions in intervals with no unnamed debug extern (retail-only functions: '
          'statics, or functions the debug revision did not have) or left unaligned by the DP.', '',
          '| retail | current | size | reason |', '|---|---|---|---|']
    for r in none:
        L.append(f"| {r['address']} | `{r['current']}` | {r['size']} | {r['evidence']} |")
    os.makedirs(os.path.dirname(WORKLIST), exist_ok=True)
    open(WORKLIST, 'w').write('\n'.join(L) + '\n')
    print(f'-> {os.path.relpath(WORKLIST, SN.ROOT)}')


if __name__ == '__main__':
    sys.exit(main())
