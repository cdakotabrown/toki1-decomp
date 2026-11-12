#!/usr/bin/env python3
"""screen_main_semis.py -- lever-screening battery for the MAIN binary.

The main-binary counterpart of tools/overlay/screen_semis.py.  The overlays
have had a lever screen (docs/worklists/semi_screen.md) for a while; the main
binary's open functions had nothing sorting or classifying them, so they could
not be prioritised or dispatched.  This builds that queue.

Universe comes from config/main_function_audit_by_addr.json (regenerate with
tools/analysis/audit_main_functions.py first):

  semi_real   -> RESIDUAL queue (M2).  C exists and is a faithful
                 decompilation; only bytes differ.  Compiled through the FULL
                 PostProc pipeline and measured.
  semi_stub   -> CONVERSION queue (M3).  C exists but the body is a stub.
  missing     -> CONVERSION queue (M3).  No C anywhere.

MAIN-BINARY SPECIFICS THAT A NAIVE PORT OF THE OVERLAY SCREEN GETS WRONG
-----------------------------------------------------------------------
1. THE types.h THUNK PREFIX.  Every IDO object built against include/types.h
   carries `jr ra; nop; jr ra; nop` (the two unreferenced `read_*_unaligned`
   statics) BEFORE the real body.  `@PostProc: strip_leading_instructions
   --count 4` removes it.  A faithful semi that lacks the directive reads
   "0% exact" -- every word is shifted by four.  This screen strips leading
   `jr ra; nop` pairs from the compiled bytes ITSELF, records how many it had
   to strip, and reports "missing/insufficient strip_leading" as its own
   mechanical lever (THUNK).  Nothing here depends on the directive being
   present.
2. AUTHORITATIVE VERDICT.  There is no name-derived resolver for the main
   binary; the build "links" by copying the target's immediate into compiled
   words via build_level3_c.fixup_relocations.  That masks far more than
   relocations: at a NON-relocation site it silently corrects frame offsets,
   branch displacements and ALU immediates from the target.  So this screen
   counts a word as matched only if it matches after masking at GENUINE
   relocation sites (build_level3_c.reloc_site_indices) -- every word in the
   fixup's non-reloc audit is counted as a MISMATCH.  That honest number is
   `resolved_exact`; the flattering one is reported separately as
   `masked_exact` and is never used for ranking.
3. LENGTH EQUALITY.  A short compile can print "N/N words exact" while the
   window tail is never examined.  `len_equal` is required for any
   count-exact classification and is printed on every row.
4. TWO COMPILERS.  0x80041000-0x80085FFF is IDO game region; 0x80086000+ is
   mostly IDO but partly Psy-Q GCC.  IDO never emits `j` (opcode 2); a target
   window containing `j` is GCC-compiled.  Files without `@Compiler: gcc*`
   whose target contains `j` are flagged (GCCREGION) -- they are being built
   with the wrong compiler.
5. Directives appear both as `/* @PostProc: ...` and as ` * @PostProc: ...`.
   Both are live.  This tool only ever reads directives through
   tools/build/parse_postproc.py, which handles both forms.

⚠️ BIAS WARNING, stated here and in the generated report: exact-% is a
   CLOSENESS SORT, not a probability of matching.  A 90% function can be a
   hard structural blocker and a 10% one can be a four-word thunk fix.  Rank
   by lever, then by closeness within a lever.

Usage:
  python3 tools/analysis/screen_main_semis.py                 # full battery
  python3 tools/analysis/screen_main_semis.py --jobs 8
  python3 tools/analysis/screen_main_semis.py --no-compile    # static only
  python3 tools/analysis/screen_main_semis.py --kind semi_real
  python3 tools/analysis/screen_main_semis.py --limit 40      # smoke test

Outputs build/worklists/main_screen.md + main_screen.json.  Compiles are cached
(keyed on source + target bytes) in --cache-dir.
"""
import argparse
import hashlib
import json
import os
import re
import struct
import sys
import tempfile
import time
from collections import Counter, defaultdict
from concurrent.futures import ProcessPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
sys.path.insert(0, str(ROOT / "tools" / "build"))
sys.path.insert(0, str(ROOT / "tools" / "overlay"))

from build_level3_c import (compile_c_function, fixup_relocations,
                            reloc_site_indices)
from parse_postproc import (parse_postproc_header, parse_compile_flags,
                            parse_compiler, parse_compile_env,
                            parse_symbol_start_text, parse_symbol_size_text,
                            directive_bodies)

# Static target-window screens shared with the overlay battery.  Importing
# them keeps ONE definition of each lever: if the overlay screen's notion of
# lda_fold/div_macro/join_phi/delay_fill changes, the main screen follows.
from screen_semis import (fields, is_move, has_delay_slot, walk_hilo,
                          screen_lda_fold, screen_div_macro, screen_join_phi,
                          screen_delay_fill, WALL_NOTE_RE)

CACHE_VERSION = "1"
AUDIT_JSON = ROOT / "config" / "main_function_audit_by_addr.json"
LEVEL3 = ROOT / "config" / "level3_functions.json"
UNMATCHABLE = ROOT / "config" / "c_unmatchable.txt"
EXE = ROOT / "disks" / "SLPS_000.65"
OUT_MD = ROOT / "build" / "worklists" / "main_screen.md"
OUT_JSON = ROOT / "build" / "worklists" / "main_screen.json"

TEXT_START = 0x80041000
FILE_BASE = 0x800
GAME_HI = 0x80086000            # game region below, library region at/above
MAIN_LO, MAIN_HI = 0x80041000, 0x800C0000

JR_RA = 0x03E00008
THUNK_PAIR = (JR_RA, 0x00000000)

# Difficulty tiers for the conversion queue (instruction counts).
TIER_BOUNDS = [(8, 'TRIVIAL'), (32, 'SMALL'), (96, 'MEDIUM'),
               (256, 'LARGE'), (10 ** 9, 'HUGE')]


# ------------------------------------------------------------------ helpers

def exe_bytes():
    with open(EXE, 'rb') as f:
        return f.read()


def target_window(exe, addr, size):
    off = (addr - TEXT_START) + FILE_BASE
    if off < 0 or size <= 0 or off + size > len(exe):
        return b''
    return exe[off:off + size]


def to_words(b):
    return [struct.unpack('<I', b[i:i + 4])[0] for i in range(0, len(b) - 3, 4)]


def strip_thunk_prefix(words):
    """Strip leading `jr ra; nop` pairs (the include/types.h statics).

    Returns (words_without_prefix, n_words_stripped).  A real function can
    never START with `jr ra`, so this is unambiguous.
    """
    k = 0
    while k + 1 < len(words) and words[k] == THUNK_PAIR[0] \
            and words[k + 1] == THUNK_PAIR[1]:
        k += 2
    return words[k:], k


def frame_size(words, limit=10):
    """`addiu sp,sp,-N` in the first `limit` words -> N, else None.

    Searched (not just word 0) because IDO -O2 hoists the first global read
    ABOVE the prologue in the main binary too.
    """
    for w in words[:limit]:
        if (w >> 26) == 9 and ((w >> 21) & 31) == 29 and ((w >> 16) & 31) == 29:
            imm = w & 0xFFFF
            if imm & 0x8000:
                return 0x10000 - imm
    return None


def has_j(words):
    """IDO 5.2 never emits `j` (opcode 2).  Psy-Q GCC does."""
    return any((w >> 26) == 2 for w in words)


def count_jal(words):
    return sum(1 for w in words if (w >> 26) == 3)


def has_jr_table(words):
    """`jr $rX` on something other than $ra -> switch dispatch table."""
    for w in words:
        if (w >> 26) == 0 and (w & 0x3F) == 8 and ((w >> 21) & 31) != 31:
            return True
    return False


def has_divmul(words):
    for w in words:
        if (w >> 26) == 0 and (w & 0x3F) in (0x18, 0x19, 0x1A, 0x1B):
            return True
    return False


def tier_for(n_insn):
    for bound, name in TIER_BOUNDS:
        if n_insn <= bound:
            return name
    return 'HUGE'


def load_unmatchable_names():
    """Bare function names listed in config/c_unmatchable.txt (main binary).

    The overlay entries are `OVERLAY/func_x`; main-binary entries are bare
    names.  Only the bare ones are relevant here.
    """
    out = {}
    if not UNMATCHABLE.exists():
        return out
    for line in UNMATCHABLE.read_text(errors='ignore').splitlines():
        s = line.strip()
        if not s or s.startswith('#'):
            continue
        m = re.match(r'([A-Za-z_][A-Za-z0-9_]*)\s*(?:#\s*(.*))?$', s)
        if m and '/' not in s.split('#')[0]:
            out[m.group(1)] = (m.group(2) or '').strip()
    return out


def load_level3_sizes():
    """name -> (addr, size) from config/level3_functions.json."""
    out = {}
    try:
        d = json.load(open(LEVEL3))
    except Exception:
        return out
    for f in d.get('functions', []):
        a = f.get('address')
        if isinstance(a, str):
            a = int(a, 16)
        sz = f.get('size')
        if isinstance(sz, str):
            sz = int(sz, 0)
        if a and sz:
            out[f['name']] = (a, sz)
    return out


HEADER_SIZE_RE = re.compile(r'Size:\s*(\d+)\s*bytes')


def header_size(content):
    m = HEADER_SIZE_RE.search(content[:4000])
    return int(m.group(1)) if m else None


# ------------------------------------------------------------------ universe

def build_universe(kinds, only=None):
    """Rows from the address-keyed audit + derived window sizes."""
    audit = json.load(open(AUDIT_JSON))
    addrs = audit['addresses']
    all_sorted = sorted(int(a, 16) for a in addrs)
    nxt = {}
    for i, a in enumerate(all_sorted):
        nxt[a] = all_sorted[i + 1] if i + 1 < len(all_sorted) else MAIN_HI

    l3 = load_level3_sizes()
    unmatch = load_unmatchable_names()
    rows, skipped = [], Counter()

    # A handful of source files back MORE THAN ONE audited address (dual-entry
    # thunks, absorbed heads). Their .text compares whole-file against one
    # window, so both the name and the count delta need a caveat.
    src_addr_count = Counter(v['source'] for v in addrs.values()
                             if v.get('source'))

    for a_s, v in sorted(addrs.items(), key=lambda kv: int(kv[0], 16)):
        kind = v['kind']
        if kind not in kinds:
            continue
        addr = int(a_s, 16)
        aliases = v.get('aliases') or []
        blocker = None
        for n in aliases:
            if n in unmatch:
                blocker = unmatch[n] or 'listed in c_unmatchable.txt'
                break
        if blocker is not None:
            skipped['c_unmatchable'] += 1
            continue

        src = v.get('source')
        multi = bool(src) and src_addr_count[src] > 1
        # Preferred symbol name: the basename of its source, else func_ADDR.
        # When one file backs several addresses, fall back to the canonical
        # func_ADDR alias so rows stay distinguishable.
        if src and not multi:
            name = os.path.basename(src)[:-2]
        elif src:
            cand = [n for n in aliases
                    if n.lower() == f'func_{addr:08x}']
            name = cand[0] if cand else \
                f'{os.path.basename(src)[:-2]}@{addr:08X}'
        else:
            name = f'func_{addr:08X}'
        # level3 / header / derived size, in that order
        size, size_src = None, None
        for n in [name] + list(aliases):
            if n in l3:
                size, size_src = l3[n][1], 'level3'
                break
        content = ''
        if src and os.path.exists(src):
            try:
                content = open(src, errors='ignore').read()
            except OSError:
                content = ''
        if size is None and content:
            hs = header_size(content)
            if hs:
                size, size_src = hs, 'header'
        if size is None:
            size, size_src = min(nxt[addr] - addr, 0x2000), 'derived'

        # @SymbolStart / @SymbolSize windowing (same rule as
        # test_single_match.py: @SymbolStart may only move the start EARLIER)
        if content:
            ss = parse_symbol_start_text(content)
            sz = parse_symbol_size_text(content)
            if ss is not None and ss < addr:
                size += addr - ss
                addr = ss
            if sz is not None:
                size = sz

        if not (MAIN_LO <= addr < MAIN_HI):
            skipped['out_of_range'] += 1
            continue
        if only and name not in only:
            continue

        rows.append({
            'name': name, 'addr': addr, 'size': size, 'size_source': size_src,
            'kind': kind, 'source': src, 'aliases': aliases,
            'shadowed_by': v.get('shadowed_by'),
            'multi_func_file': multi,
            'region': 'game' if addr < GAME_HI else 'library',
        })
    return rows, skipped, unmatch


# ------------------------------------------------------- per-function worker

_EXE_CACHE = {}


def _exe():
    if 'b' not in _EXE_CACHE:
        _EXE_CACHE['b'] = exe_bytes()
    return _EXE_CACHE['b']


def static_screens(twords):
    """Screens that need only the TARGET window."""
    taddrs = walk_hilo(twords)
    checked, mod2 = screen_div_macro(twords)
    return {
        'lda_fold': screen_lda_fold(twords, taddrs),
        'checked_div': checked,
        'mod2_macro': mod2,
        'target_frame': frame_size(twords),
        'target_has_j': has_j(twords),
        'n_jal': count_jal(twords),
        'jr_table': has_jr_table(twords),
        'divmul': has_divmul(twords),
        'cross_prologue': (frame_size(twords, limit=1) is None
                           and frame_size(twords) is not None),
    }


def analyze(job):
    os.chdir(ROOT)
    name, addr, size = job['name'], job['addr'], job['size']
    res = {k: job[k] for k in ('name', 'addr', 'size', 'size_source', 'kind',
                               'source', 'region', 'shadowed_by',
                               'multi_func_file')}
    res['labels'] = []
    res['screens'] = []

    target = target_window(_exe(), addr, size)
    if len(target) < 4:
        res['error'] = 'no target window'
        return res
    twords = to_words(target)
    res['n_target'] = len(twords)
    res.update(static_screens(twords))
    res['tier'] = tier_for(len(twords))

    src = job['source']
    if not src or not os.path.exists(src) or not job['do_compile']:
        return res

    content = open(src, errors='ignore').read()
    if WALL_NOTE_RE.search(content[:4000]):
        res['labels'].append('wall-note')
    if re.search(r'UNMATCHABLE|ASM-ONLY|ASM_ONLY', content[:4000]):
        res['labels'].append('header-blocked')

    cache_dir = Path(job['cache_dir'])
    key = hashlib.sha1(
        (CACHE_VERSION + name + str(addr) + str(size)).encode()
        + content.encode('utf-8', 'ignore') + target).hexdigest()
    cache_file = cache_dir / f"{name}-{key}.json"
    if cache_file.exists():
        try:
            cached = json.load(open(cache_file))
            cached['labels'] = res['labels']
            return cached
        except Exception:
            pass
    cache_dir.mkdir(parents=True, exist_ok=True)

    # parse_postproc_header returns None when the header carries no directive
    steps = parse_postproc_header(src) or []
    res['postproc'] = sorted({s['type'] for s in steps})
    res['compiler'] = parse_compiler(src) or 'ido52'
    flags = parse_compile_flags(src)
    env = parse_compile_env(src)
    strip_step = next((s for s in steps
                       if s['type'] == 'strip_leading_instructions'), None)
    res['strip_leading'] = strip_step['count'] if strip_step else 0
    frame_step = next((s for s in steps if s['type'] == 'patch_frame'), None)
    if frame_step:
        res['frame_directive'] = [frame_step['old_size'],
                                  frame_step['new_size']]

    # A custom_script that paints hardcoded instruction words makes
    # build_level3_c call sys.exit(1) mid-pipeline, which in a pool worker
    # kills the process and breaks the whole run. Detect it up front, DROP
    # the step and measure the honest baseline without it — the directive has
    # to go anyway (CLAUDE.md PostProc policy), so the number that matters is
    # the one the C produces unaided.
    has_custom = any(s['type'] == 'custom_script' for s in steps)
    banned = False
    for s in steps:
        if s['type'] == 'custom_script':
            try:
                from banned_postproc import is_painting_script
                if is_painting_script(ROOT / 'tools' / 'ido' / s['script']):
                    banned = True
            except Exception:
                pass
    steps_eff = [s for s in steps if s['type'] != 'custom_script'] \
        if banned else steps
    if banned:
        res['banned_custom_script'] = True
        res['labels'].append('banned-custom-script')

    def build(extra=None, use=None):
        try:
            # BaseException: the pipeline calls sys.exit() on policy refusals,
            # and SystemExit in a pool worker is fatal to the whole run.
            return compile_c_function(
                src, name, addr,
                postproc_steps=(steps_eff if use is None else use),
                extra_flags=list(flags) + list(extra or []),
                compiler=res['compiler'], compile_env=env)
        except BaseException:
            return None

    b = build()
    if b is None and has_custom and not banned:
        # Any other custom_script failure (missing intercept source, stale
        # size assertion): measure without it rather than dropping the row.
        steps_eff = [s for s in steps if s['type'] != 'custom_script']
        res['labels'].append('custom_script-dropped')
        b = build()
    if b is None:
        res['compile_error'] = True
        json.dump(res, open(cache_file, 'w'))
        return res

    cwords = to_words(b)
    cwords, thunk = strip_thunk_prefix(cwords)
    res['thunk_autostripped'] = thunk
    # trailing IDO 16-byte .text zero padding is benign
    while cwords and cwords[-1] == 0 and len(cwords) > len(twords):
        cwords = cwords[:-1]
    res['n_compiled'] = len(cwords)
    res['len_equal'] = (len(cwords) == len(twords))
    res['count_delta'] = len(cwords) - len(twords)

    # --- honest vs masked comparison -------------------------------------
    n = min(len(twords), len(cwords))
    raw_exact = sum(1 for i in range(n) if twords[i] == cwords[i])
    res['raw_exact'] = raw_exact

    cb = struct.pack(f'<{len(cwords)}I', *cwords)[:size]
    audit = []
    fixed = fixup_relocations(cb, addr, _exe(),
                              (addr - TEXT_START) + FILE_BASE,
                              func_name=name, audit=audit)
    fwords = to_words(fixed)
    nf = min(len(twords), len(fwords))
    masked_exact = sum(1 for i in range(nf) if twords[i] == fwords[i])
    res['masked_exact'] = masked_exact
    res['fixup_nonreloc'] = len(audit)
    # HONEST: every word the fixup corrected at a NON-relocation site is a
    # genuine codegen diff, not a relocation.  Count it as a mismatch.
    res['resolved_exact'] = masked_exact - len(audit)
    res['resolved_n'] = nf
    # THE TRUNCATED-COMPARE TRAP: the comparison only runs to the SHORTER of
    # the two buffers, so a 2-word window against a 213-word compile would
    # score "2/2 = 100%". Divide by the LONGER side so a length mismatch can
    # never inflate the ranking.
    res['resolved_pct'] = round(
        100.0 * res['resolved_exact'] / max(len(twords), len(cwords), 1), 1)
    res['resolved_pct_over_prefix'] = round(
        100.0 * res['resolved_exact'] / max(nf, 1), 1)
    res['reloc_sites'] = len(reloc_site_indices(cwords))

    nonreloc_idx = {a['index'] for a in audit}
    diffs = [(i, twords[i], fwords[i]) for i in range(nf)
             if twords[i] != fwords[i] or i in nonreloc_idx]
    res['n_diffs'] = len(diffs)
    res['first_diffs'] = [{'i': i, 't': f'{t:08x}', 'c': f'{c:08x}'}
                          for i, t, c in diffs[:8]]
    res['compiled_frame'] = frame_size(cwords)

    # --- dynamic screens --------------------------------------------------
    res['join_phi'] = screen_join_phi(diffs, twords, cwords)
    res['delay_fill'] = screen_delay_fill(diffs, twords, cwords,
                                          res['len_equal'])

    # raw emission (no PostProc at all) for the unroll screen
    try:
        # postproc_steps=[] still applies the region default (patch_frame
        # 24->40 / convert_or_to_addu); both preserve instruction count, so
        # this is the raw EMISSION SIZE the unroll screen needs.
        raw = compile_c_function(src, name, addr, postproc_steps=[],
                                 extra_flags=list(flags),
                                 compiler=res['compiler'], compile_env=env)
    except BaseException:
        raw = None
    if raw is not None:
        rw, _ = strip_thunk_prefix(to_words(raw))
        while rw and rw[-1] == 0:
            rw = rw[:-1]
        res['raw_words'] = len(rw)
        if (len(rw) - len(twords) > 8 and 'loopunroll' not in ' '.join(flags)
                and not res['compiler'].startswith('gcc')):
            u = build(extra=['-Wo,-loopunroll,0'])
            if u is not None:
                uw, _ = strip_thunk_prefix(to_words(u))
                while uw and uw[-1] == 0 and len(uw) > len(twords):
                    uw = uw[:-1]
                res['unroll_words'] = len(uw)
                if abs(len(uw) - len(twords)) <= 2:
                    res['unroll_snap'] = True

    # annotate static sites with diff proximity (a lever site that already
    # matches is not actionable)
    didx = {i for i, _t, _c in diffs}

    def near(i, span=4):
        return any(j in didx for j in range(i - span, i + span + 1))

    for h in res['lda_fold']:
        h['near_diff'] = near(h['store_i'])
    for h in res['checked_div']:
        h['near_diff'] = near(h['div_i'], span=8)
    for h in res['mod2_macro']:
        h['near_diff'] = near(h['i'], span=5)

    json.dump(res, open(cache_file, 'w'))
    return res


# ------------------------------------------------------------ lever assignment

LEVERS = {
    'banned_script': ('BANNED custom_script PRESENT',
                      'The header still points at a painting `custom_script` '
                      '(hardcoded instruction words). CLAUDE.md bans it and '
                      'the build refuses to run it. Scores on these rows were '
                      'measured with the script DROPPED — that is the honest '
                      'baseline. Delete the directive and the script, then '
                      'fix the C or file a blocker.'),
    'thunk': ('THUNK PREFIX',
              'Compiled output still carries the include/types.h `jr ra; nop` '
              'prefix: `@PostProc: strip_leading_instructions --count N` is '
              'missing or short. Mechanical.'),
    'gccregion': ('WRONG COMPILER (`j` in target)',
                  'Target emits `j` (opcode 2). IDO 5.2 never does — this is '
                  'a Psy-Q GCC function built as IDO. Add `@Compiler: '
                  'gcc260`/`gcc281`.'),
    'frame': ('FRAME SIZE',
              'Compiled prologue frame != target frame. Fix with '
              '`patch_stack_complete --old <ACTUAL compiled frame> --new '
              '<target>` — a wrong `--old` silently no-ops.'),
    'frame_stale': ('STALE FRAME DIRECTIVE',
                    '`patch_frame --old X` where X is NOT the frame the '
                    'compiler actually emits: the directive is a silent '
                    'no-op masquerading as a deep residual.'),
    'oraddu': ('or -> addu',
               'Library-region IDO function whose diffs are `or rd,rs,$zero` '
               'vs `addu rd,rs,$zero`. Add `convert_or_to_addu`.'),
    'unroll': ('UNROLL SNAP',
               'Raw emission snaps to the window +/-2 with '
               '`-Wo,-loopunroll,0`.'),
    'delay_fill': ('DELAY-FILL',
                   '1-2 word count-exact residual in a branch delay slot — '
                   'spelling / Hook 27 class.'),
    'join_phi': ('JOIN-PHI',
                 'TARGET-side surplus `move` with a guard compare within +/-3 '
                 'words — dead-store copy-prop cure candidate.'),
    'lda_fold': ('LDA-FOLD',
                 '`lui $at` absolute store within +/-32KB of an la-built base '
                 '— volatile store-site cast lever.'),
    'div_macro': ('CHECKED-DIVIDE / %2 MACRO',
                  'div+break guard chain or the 5-word signed %2 macro near a '
                  'residual — spelling levers.'),
    'fixup_mask': ('FIXUP-MASKED WORDS',
                   'The build\'s fixup_relocations corrected words at '
                   'NON-relocation sites (frame offsets, branch '
                   'displacements). Those are real codegen diffs the masked '
                   'view hides.'),
}

# Which levers can be handed over directly (no expert judgement needed).
AGENT_SAFE = {'thunk', 'gccregion', 'frame', 'frame_stale', 'oraddu',
              'unroll', 'delay_fill'}


def actionable(sites):
    return [h for h in sites if h.get('near_diff') is not False]


def assign_levers(r):
    hits = []
    if r.get('error') or r.get('compile_error'):
        return ['banned_script'] if r.get('banned_custom_script') else hits
    if r.get('banned_custom_script'):
        hits.append('banned_script')
    if r.get('thunk_autostripped'):
        hits.append('thunk')
    if r.get('target_has_j') and not str(r.get('compiler', '')).startswith('gcc'):
        hits.append('gccregion')
    tf, cf = r.get('target_frame'), r.get('compiled_frame')
    fd = r.get('frame_directive')
    if tf is not None and cf is not None and tf != cf:
        hits.append('frame')
    if fd and cf is not None and fd[0] != cf and cf != fd[1]:
        hits.append('frame_stale')
    if r.get('unroll_snap'):
        hits.append('unroll')
    if r.get('delay_fill'):
        hits.append('delay_fill')
    if r.get('join_phi'):
        hits.append('join_phi')
    if actionable(r.get('lda_fold') or []):
        hits.append('lda_fold')
    if actionable(r.get('checked_div') or []) or \
            actionable(r.get('mod2_macro') or []):
        hits.append('div_macro')
    if r.get('fixup_nonreloc'):
        hits.append('fixup_mask')
    if r['region'] == 'library' and not str(
            r.get('compiler', '')).startswith('gcc'):
        if 'convert_or_to_addu' not in (r.get('postproc') or []):
            for d in r.get('first_diffs') or []:
                t, c = int(d['t'], 16), int(d['c'], 16)
                if (t >> 26) == 0 and (c >> 26) == 0 and \
                        (t & 0x3F) == 0x21 and (c & 0x3F) == 0x25 and \
                        (t & 0xFFFFF800) == (c & 0xFFFFF800):
                    hits.append('oraddu')
                    break
    return hits


# ------------------------------------------------------------------- reports

def band(pct):
    if pct >= 90:
        return '90-100%'
    if pct >= 75:
        return '75-90%'
    if pct >= 50:
        return '50-75%'
    if pct >= 25:
        return '25-50%'
    return '<25%'


BAND_ORDER = ['90-100%', '75-90%', '50-75%', '25-50%', '<25%']


def md_residual(rows, lines):
    by_lever = defaultdict(list)
    for r in rows:
        for k in r['levers']:
            by_lever[k].append(r)
    for k in by_lever:
        by_lever[k].sort(key=lambda r: -(r.get('resolved_pct') or 0))

    lines += ["## Queue M2 — residual chase (`semi_real`)", "",
              f"{len(rows)} functions. The C is a faithful decompilation; "
              "only the bytes differ.", ""]

    n_lever = sum(1 for r in rows if r['levers'])
    n_safe = sum(1 for r in rows
                 if any(k in AGENT_SAFE for k in r['levers']))
    lines += [f"**{n_lever}** carry at least one named lever; **{n_safe}** "
              f"carry at least one **SAFE** lever.", "",
              "| Lever | Hits | Safe | What it means |", "|---|---:|---|---|"]
    for k, (title, note) in LEVERS.items():
        lines.append(f"| {title} | {len(by_lever.get(k, []))} | "
                     f"{'✅' if k in AGENT_SAFE else '⚠️'} | {note} |")
    lines.append("")

    # closeness + count-delta distributions
    b = Counter(band(r.get('resolved_pct') or 0.0) for r in rows
                if 'resolved_pct' in r)
    lines += ["**Closeness distribution** (honest `resolved_pct` — reloc-site "
              "masking only). ⚠️ *closeness sort, not a match prediction*:",
              "", "| Band | Count |", "|---|---:|"]
    for k in BAND_ORDER:
        lines.append(f"| {k} | {b.get(k, 0)} |")
    lines.append("")

    cd = Counter()
    for r in rows:
        d = r.get('count_delta')
        if d is None:
            cd['no compile'] += 1
        elif d == 0:
            cd['count-exact (delta 0)'] += 1
        elif abs(d) <= 2:
            cd['near (+/-1..2)'] += 1
        elif d > 0:
            cd['compiled LONGER (>2)'] += 1
        else:
            cd['compiled SHORTER (>2)'] += 1
    lines += ["**Instruction-count distribution** — a count-exact function is "
              "a register/scheduling problem; a count mismatch is a C "
              "problem:", "", "| Count | Functions |", "|---|---:|"]
    for k in ('count-exact (delta 0)', 'near (+/-1..2)',
              'compiled LONGER (>2)', 'compiled SHORTER (>2)', 'no compile'):
        if cd.get(k):
            lines.append(f"| {k} | {cd[k]} |")
    lines.append("")

    # region split
    rc = Counter((r['region'], bool(r['levers'])) for r in rows)
    lines += ["**By region:**", "",
              "| Region | With lever | No lever |", "|---|---:|---:|"]
    span = {'game': f"0x{TEXT_START:08X}–0x{GAME_HI - 1:08X}, IDO 5.2",
            'library': f"0x{GAME_HI:08X}+, IDO 5.2 + some Psy-Q GCC"}
    for reg in ('game', 'library'):
        lines.append(f"| {reg} ({span[reg]}) | "
                     f"{rc.get((reg, True), 0)} | {rc.get((reg, False), 0)} |")
    lines.append("")

    # --- M2a: ready / near-ready -----------------------------------------
    ready = [r for r in rows if r.get('len_equal')
             and (r.get('n_diffs') if r.get('n_diffs') is not None else 999)
             <= 3]
    ready.sort(key=lambda r: (r.get('n_diffs', 999), -(r.get('resolved_pct')
                                                       or 0)))
    lines += [f"### M2a — ready / near-ready · {len(ready)} · **START HERE**",
              "",
              "Count-exact (`len_equal`) with at most three honest residual "
              "words. These are the only rows where a single edit plausibly "
              "finishes the function. `n_diffs = 0` with `fixup 0` means the "
              "screen believes it already matches and only a directive or "
              "registration is missing — confirm with `test_single_match.py` "
              "before recording anything.", "",
              "| Function | Addr | Reg | exact% | words | diffs | fixup-masked "
              "| Levers |", "|---|---|---|---:|---:|---:|---:|---|"]
    for r in ready:
        lines.append(
            f"| `{r['name']}` | 0x{r['addr']:08X} | {r['region'][:3]} | "
            f"{r.get('resolved_pct', 0):.1f} | {r.get('n_target')} | "
            f"{r.get('n_diffs')} | {r.get('fixup_nonreloc')} | "
            f"{', '.join(LEVERS[k][0] for k in r['levers']) or '—'} |")
    lines.append("")

    # top targets
    tagged = [r for r in rows if r['levers']]
    tagged.sort(key=lambda r: (0 if any(k in AGENT_SAFE for k in r['levers'])
                               else 1, -(r.get('resolved_pct') or 0)))
    lines += ["### Top 25 (safe lever first, then closeness)", "",
              "| Function | Addr | Reg | exact% | t/c words | Levers |",
              "|---|---|---|---:|---|---|"]
    for r in tagged[:25]:
        lines.append(row_md(r))
    lines.append("")

    for k, (title, note) in LEVERS.items():
        rs = by_lever.get(k, [])
        lines += [f"### {title} · {len(rs)} · "
                  f"{'SAFE' if k in AGENT_SAFE else 'NEEDS-JUDGEMENT'}",
                  "", note, ""]
        if not rs:
            lines += ["No hits.", ""]
            continue
        lines += ["| Function | Addr | Reg | exact% | t/c words | Evidence |",
                  "|---|---|---|---:|---|---|"]
        for r in rs[:120]:
            lines.append(row_md(r, evidence=evidence_for(k, r)))
        if len(rs) > 120:
            lines.append(f"| … +{len(rs) - 120} more (see main_screen.json) "
                         f"| | | | | |")
        lines.append("")

    nolever = [r for r in rows if not r['levers']]
    nolever.sort(key=lambda r: -(r.get('resolved_pct') or 0))
    lines += [f"### No lever · {len(nolever)} · NEEDS-JUDGEMENT", "",
              "No known recipe screens positive. Re-derive semantics from the "
              "target `.s` before editing — collapsed dispatch arms, missing "
              "hidden parameters and wrong control flow are the usual causes.",
              "", "| Function | Addr | Reg | exact% | t/c words | Notes |",
              "|---|---|---|---:|---|---|"]
    for r in nolever[:120]:
        lines.append(row_md(r))
    if len(nolever) > 120:
        lines.append(f"| … +{len(nolever) - 120} more | | | | | |")
    lines.append("")


def evidence_for(k, r):
    if k == 'banned_script':
        return ("measured with the painting custom_script dropped"
                if r.get('n_compiled') is not None
                else "does not compile even with the script dropped")
    if k == 'thunk':
        return (f"{r['thunk_autostripped']} thunk word(s) auto-stripped; "
                f"directive has --count {r.get('strip_leading', 0)}")
    if k == 'gccregion':
        return "target contains `j`; built as " + str(r.get('compiler'))
    if k == 'frame':
        return (f"target frame {r.get('target_frame')} vs compiled "
                f"{r.get('compiled_frame')}"
                + (f"; directive --old {r['frame_directive'][0]} --new "
                   f"{r['frame_directive'][1]}" if r.get('frame_directive')
                   else "; no frame directive"))
    if k == 'frame_stale':
        return (f"directive --old {r['frame_directive'][0]} but compiler "
                f"emits {r.get('compiled_frame')} (silent no-op)")
    if k == 'oraddu':
        return "library-region or/addu diff, no convert_or_to_addu directive"
    if k == 'unroll':
        return (f"raw {r.get('raw_words')}w -> {r.get('unroll_words')}w vs "
                f"window {r.get('n_target')}w")
    if k == 'delay_fill':
        h = r['delay_fill'][0]
        return (f"{r.get('n_diffs')} diff(s), word[{h['i']}] "
                f"{'nop-vs-insn' if h['nop_vs_insn'] else 'dup-fill'}")
    if k == 'join_phi':
        h = r['join_phi'][0]
        return (f"word[{h['i']}] target-side move t={h['target']:08x} "
                f"c={h['compiled']:08x}" +
                (f" +{len(r['join_phi']) - 1} more"
                 if len(r['join_phi']) > 1 else ""))
    if k == 'lda_fold':
        s = actionable(r['lda_fold'])
        h = s[0]
        return (f"{h['store_op']} @{h['store_addr']:08X} vs base "
                f"{h['base_addr']:08X} (d={h['delta']:+#x})"
                + (f" +{len(s) - 1} more" if len(s) > 1 else ""))
    if k == 'div_macro':
        cd = actionable(r.get('checked_div') or [])
        m2 = actionable(r.get('mod2_macro') or [])
        parts = []
        if cd:
            parts.append(f"{len(cd)} checked-div site(s)")
        if m2:
            parts.append(f"{len(m2)} %2-macro site(s)")
        return ", ".join(parts)
    if k == 'fixup_mask':
        return (f"{r['fixup_nonreloc']} word(s) taken from the TARGET at "
                f"non-relocation sites")
    return ""


def row_md(r, evidence=None):
    pct = r.get('resolved_pct')
    pcts = f"{pct:.1f}" if pct is not None else "—"
    nt, nc = r.get('n_target'), r.get('n_compiled')
    lens = (f"{nt}/{nc}" + ("" if r.get('len_equal') else " ⚠️")) \
        if nc is not None else f"{nt}/—"
    last = evidence if evidence is not None else \
        (", ".join(LEVERS[k][0] for k in r['levers']) or "—")
    if r['labels']:
        last += " [" + ",".join(r['labels']) + "]"
    if r.get('compile_error'):
        last = "COMPILE ERROR"
    if r.get('shadowed_by'):
        last += f" [shadowed by {r['shadowed_by']}]"
    if r.get('multi_func_file'):
        last += " [multi-func file: count delta unreliable]"
    return (f"| `{r['name']}` | 0x{r['addr']:08X} | {r['region'][:3]} | "
            f"{pcts} | {lens} | {last} |")


def md_conversion(rows, lines):
    lines += ["## Queue M3 — conversion (`semi_stub` + `missing`)", "",
              f"{len(rows)} functions with no usable C body. These need "
              "decompilation, not residual chasing — a different skill and a "
              "different queue. Recipe: "
              "`docs/reference/CONVERSION-REFERENCE.md`.", ""]

    tc = Counter((r['kind'], r['tier']) for r in rows)
    lines += ["| Tier (target instructions) | `semi_stub` | `missing` |",
              "|---|---:|---:|"]
    bounds = {'TRIVIAL': '<=8', 'SMALL': '9-32', 'MEDIUM': '33-96',
              'LARGE': '97-256', 'HUGE': '>256'}
    for t in ('TRIVIAL', 'SMALL', 'MEDIUM', 'LARGE', 'HUGE'):
        lines.append(f"| {t} ({bounds[t]}) | {tc.get(('semi_stub', t), 0)} | "
                     f"{tc.get(('missing', t), 0)} |")
    lines.append("")

    flags = Counter()
    for r in rows:
        if r.get('jr_table'):
            flags['jr-table dispatch'] += 1
        if r.get('target_has_j'):
            flags['GCC (`j` in target)'] += 1
        if r.get('cross_prologue'):
            flags['cross-prologue head'] += 1
        if r.get('divmul'):
            flags['div/mult present'] += 1
        if r.get('n_jal') == 0:
            flags['leaf (no jal)'] += 1
        if r.get('shadowed_by'):
            flags['stub shadowed by pc-port (dead)'] += 1
        if r.get('size_source') == 'derived':
            flags['⚠️ window size DERIVED from next symbol'] += 1
    lines += ["**Structural flags across the queue:**", "",
              "| Flag | Count |", "|---|---:|"]
    for k, v in flags.most_common():
        lines.append(f"| {k} | {v} |")
    lines.append("")

    easy = [r for r in rows
            if r['tier'] in ('TRIVIAL', 'SMALL') and not r.get('jr_table')
            and not r.get('cross_prologue') and not r.get('shadowed_by')]
    easy.sort(key=lambda r: (r['n_target'], r['addr']))
    lines += [f"### M3a — starter set · {len(easy)} · SAFE", "",
              "<=32 instructions, no jr-table, no cross-prologue head, not "
              "shadowed by a pc-port override. Small enough to derive from "
              "the target `.s` directly.", "",
              "| Function | Addr | Reg | insns | Kind | Flags |",
              "|---|---|---|---:|---|---|"]
    for r in easy[:150]:
        lines.append(conv_row(r))
    if len(easy) > 150:
        lines.append(f"| … +{len(easy) - 150} more | | | | | |")
    lines.append("")

    easy_ids = {id(r) for r in easy}
    rest = [r for r in rows if id(r) not in easy_ids]
    rest.sort(key=lambda r: (r['n_target'], r['addr']))
    lines += [f"### M3b — the rest · {len(rest)} · NEEDS-JUDGEMENT", "",
              "| Function | Addr | Reg | insns | Kind | Flags |",
              "|---|---|---|---:|---|---|"]
    for r in rest[:150]:
        lines.append(conv_row(r))
    if len(rest) > 150:
        lines.append(f"| … +{len(rest) - 150} more (see main_screen.json) | "
                     f"| | | | |")
    lines.append("")


def conv_row(r):
    f = []
    if r.get('jr_table'):
        f.append('jr-table')
    if r.get('target_has_j'):
        f.append('GCC')
    if r.get('cross_prologue'):
        f.append('cross-prologue')
    if r.get('divmul'):
        f.append('div/mult')
    if r.get('n_jal') == 0:
        f.append('leaf')
    if r.get('shadowed_by'):
        f.append(f"shadowed:{r['shadowed_by']}")
    if r.get('size_source') == 'derived':
        f.append('size-derived')
    return (f"| `{r['name']}` | 0x{r['addr']:08X} | {r['region'][:3]} | "
            f"{r.get('n_target', 0)} | {r['kind']} | {', '.join(f) or '—'} |")


def build_report(residual, conversion, skipped, unmatch, elapsed, args):
    lines = ["# Main-binary screen (`screen_main_semis.py`)", "",
             f"Generated {time.strftime('%Y-%m-%d %H:%M')} over "
             f"{len(residual) + len(conversion)} open main-binary functions "
             f"({elapsed:.0f}s). Universe: "
             f"`config/main_function_audit_by_addr.json`. Regenerate with:",
             "", "```bash",
             "python3 tools/analysis/audit_main_functions.py",
             "python3 tools/analysis/screen_main_semis.py --jobs 6",
             "```", "",
             "## How to read this (four traps, all of which have bitten)", "",
             "1. **exact-% is a closeness sort, NOT a match prediction.** "
             "Ranking by it has misled repeatedly. Pick a lever first, then "
             "sort by closeness *within* the lever.",
             "2. **The percentage here is the HONEST one.** The build's "
             "`fixup_relocations` copies the target's immediate into any "
             "compiled I-type word with a matching opcode + top-16 bits — at "
             "a genuine relocation site that is linking, everywhere else it "
             "is masking. Words masked at non-relocation sites are counted "
             "as **mismatches** here and surfaced as the FIXUP-MASKED lever. "
             "The flattering number is in the JSON as `masked_exact`; do not "
             "quote it.",
             "3. **`t/c words` must be equal.** A short compile can score "
             "\"N/N exact\" while the window tail is never examined. A ⚠️ in "
             "that column means the lengths differ and the score is over a "
             "prefix only.",
             "4. **The types.h thunk prefix is handled here, not by the "
             "directive.** Every IDO object carries `jr ra; nop` ×2 from the "
             "unreferenced `read_*_unaligned` statics in `include/types.h`. "
             "This screen strips them from the compiled bytes itself, so a "
             "faithful semi missing `@PostProc: strip_leading_instructions "
             "--count 4` is scored correctly instead of reading 0%. The "
             "missing directive is reported as the THUNK lever.", "",
             "**Definition of done for any function here:** "
             "`python3 tools/analysis/test_single_match.py <FUNC> -v` prints "
             "`*** BYTE-PERFECT MATCH! ***` with `Fixup: 0` non-relocation "
             "words and equal target/compiled counts. `ido_diff.py` is NOT "
             "authoritative for the main binary (it skips `custom_script`). "
             "A new `src/main/matched/*.c` does not enter the build until it "
             "is registered in `config/level3_functions.json`.", ""]

    kinds = Counter(r['kind'] for r in residual + conversion)
    regs = Counter((r['kind'], r['region']) for r in residual + conversion)
    lines += ["## Universe", "",
              "| Kind | Game (0x80041000–0x80085FFF) | Library (0x80086000+) "
              "| Total | Queue |", "|---|---:|---:|---:|---|"]
    for k, q in (('semi_real', 'M2'), ('semi_stub', 'M3'), ('missing', 'M3')):
        lines.append(f"| `{k}` | {regs.get((k, 'game'), 0)} | "
                     f"{regs.get((k, 'library'), 0)} | {kinds.get(k, 0)} | "
                     f"{q} |")
    lines.append(f"| **total** | | | **{len(residual) + len(conversion)}** | "
                 f"|")
    lines.append("")
    lines.append(f"Skipped: {dict(skipped)} "
                 f"(`c_unmatchable.txt` carries {len(unmatch)} bare "
                 f"main-binary names).")
    lines.append("")

    md_residual(residual, lines)
    md_conversion(conversion, lines)

    lines += ["## Caveats a human should resolve", "",
              "- Window sizes marked `size-derived` come from the next known "
              "symbol address, not from a symbol table. For `missing` "
              "functions there is no authoritative size map for the main "
              "binary; treat those instruction counts as upper bounds.",
              "- `semi_stub` entries flagged `shadowed:pc-port` are dead in "
              "the build (a pc-port override wins). Matching them is still "
              "required for a byte-perfect PS1 build but they do not block "
              "the port.",
              "- Files that define more than one audited function compare the "
              "whole `.text` against one function's window; those rows "
              "over-report `count_delta`. There are ~11 of them.", ""]
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------- main

def main():
    ap = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--jobs', type=int, default=6)
    ap.add_argument('--no-compile', action='store_true',
                    help='static target-window screens only (fast)')
    ap.add_argument('--kind', action='append',
                    choices=['semi_real', 'semi_stub', 'missing'],
                    help='restrict to one or more audit kinds')
    ap.add_argument('--func', action='append', help='screen named functions')
    ap.add_argument('--limit', type=int, help='cap the universe (smoke test)')
    ap.add_argument('--cache-dir', default=os.environ.get(
        'SCREEN_MAIN_CACHE',
        str(Path(tempfile.gettempdir()) / 'toki_screen_main')))
    ap.add_argument('--out-md', default=str(OUT_MD))
    ap.add_argument('--out-json', default=str(OUT_JSON))
    args = ap.parse_args()

    kinds = set(args.kind or ['semi_real', 'semi_stub', 'missing'])
    t0 = time.time()
    rows, skipped, unmatch = build_universe(kinds,
                                            only=set(args.func or []) or None)
    if args.limit:
        rows = rows[:args.limit]
    print(f"screening {len(rows)} main-binary functions "
          f"(skipped {dict(skipped)})...", file=sys.stderr)

    jobs = [dict(r, cache_dir=args.cache_dir,
                 do_compile=(not args.no_compile) and r['kind'] != 'missing')
            for r in rows]
    results = []
    if args.jobs > 1 and len(jobs) > 1:
        with ProcessPoolExecutor(max_workers=args.jobs) as ex:
            for i, r in enumerate(ex.map(analyze, jobs, chunksize=1)):
                results.append(r)
                if (i + 1) % 25 == 0:
                    print(f"  {i + 1}/{len(jobs)}", file=sys.stderr)
    else:
        for j in jobs:
            results.append(analyze(j))

    for r in results:
        r['levers'] = assign_levers(r)

    residual = [r for r in results if r['kind'] == 'semi_real']
    conversion = [r for r in results if r['kind'] in ('semi_stub', 'missing')]
    residual.sort(key=lambda r: -(r.get('resolved_pct') or 0))

    elapsed = time.time() - t0
    md = build_report(residual, conversion, skipped, unmatch, elapsed, args)
    Path(args.out_md).parent.mkdir(parents=True, exist_ok=True)
    Path(args.out_md).write_text(md)
    Path(args.out_json).write_text(json.dumps({
        'generated': time.strftime('%Y-%m-%dT%H:%M:%S'),
        'elapsed_s': round(elapsed, 1),
        'universe': {'residual': len(residual), 'conversion': len(conversion)},
        'skipped': dict(skipped),
        'levers': {k: v[0] for k, v in LEVERS.items()},
        'agent_safe_levers': sorted(AGENT_SAFE),
        'residual': residual,
        'conversion': conversion,
    }, indent=1))

    lc = Counter()
    for r in residual:
        for k in r['levers']:
            lc[k] += 1
    print(f"\n{'lever':<14} hits", file=sys.stderr)
    for k, _n in LEVERS.items():
        print(f"{k:<14} {lc.get(k, 0)}", file=sys.stderr)
    print(f"with >=1 lever: {sum(1 for r in residual if r['levers'])}"
          f"/{len(residual)}", file=sys.stderr)
    print(f"wrote {args.out_md} + {args.out_json} ({elapsed:.0f}s)",
          file=sys.stderr)


if __name__ == '__main__':
    main()
