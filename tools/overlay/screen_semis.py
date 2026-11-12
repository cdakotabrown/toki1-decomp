#!/usr/bin/env python3
"""screen_semis.py -- lever-screening battery for remaining overlay semis.

Classifies every remaining semi_matched / count_mismatch overlay function
(config/overlay_match_status.json) into targeted worklists so matching passes
get queues instead of exploration. The levers being screened for live in
docs/reference/MATCHING-TRICKS.md, "The 2026-08-08 Autonomous-Campaign
Addendum".

Screens (priority order):

  1. MISNAME   Symbol-name audit. The target's actually-touched global
               addresses are derived from its own window bytes by %hi/%lo
               pair walking (same arithmetic the resolver applies via ld);
               the C's addresses are derived symmetrically from the
               name-derived-linked compiled window (this cancels the
               array-addend problem: both sides carry addr+addend).
               Orphans on both sides paired by small delta = suspected
               misnamed extern. Loaded-pointer bases and one-sided orphans
               are LOW confidence, never certain (OMIMAI case-6 caveat).
  2. UNROLL    For semis whose RAW emission (no PostProc) exceeds the
               window by >8 words, one recompile with -Wo,-loopunroll,0:
               report the ones that snap to within +/-2 of the window.
  3. LDA-FOLD  Target windows containing `lui $at; s{b,h,w}` absolute-store
               pairs whose address lies within +/-32KB of another global the
               same function builds as a base (lui+addiu la form) -- the
               volatile store-site cast lever (OPTION 8013AF30/C018 win).
  4. JOIN-PHI  Resolver-diff runs featuring a surplus `move` (or/addu with
               $zero) adjacent to a compare/branch word -- dead-store
               copy-prop blocker candidates (RPG_BAT 8015528C cure).
  5. DELAYFILL Count-exact residuals of 1-2 words where the diff is
               nop-vs-insn in a delay slot or a branch-adjacent duplicated
               instruction -- delay-fill spelling / Hook 27 class.
  6. DIV-MACRO Target has checked-divide guard chains (div+break) or the
               5-word signed %2 macro -- cheap spelling levers apply
               (volatile staging local / (u32) cast per site / %2 macro).

Usage:
  python3 tools/overlay/screen_semis.py                 # full battery
  python3 tools/overlay/screen_semis.py --overlay TACO  # one overlay
  python3 tools/overlay/screen_semis.py --jobs 8
  python3 tools/overlay/screen_semis.py --no-compile    # static screens only

Outputs build/worklists/semi_screen.md + semi_screen.json. Re-runnable:
compiles are cached (keyed on source+target hash) in --cache-dir (default
$SCREEN_SEMIS_CACHE or <tmp>/toki_screen_semis). c_unmatchable entries are
skipped; functions documented in the MATCHING-TRICKS EOF addendum or
carrying a header wall-note are labeled, not hidden.
"""
import argparse
import hashlib
import json
import os
import re
import struct
import subprocess
import sys
import tempfile
import time
from collections import Counter
from concurrent.futures import ProcessPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
sys.path.insert(0, str(ROOT / "tools" / "build"))
sys.path.insert(0, str(ROOT / "tools" / "overlay"))

from parse_postproc import (parse_compile_flags, parse_compile_env,
                            parse_compiler, parse_symbol_start_text,
                            parse_symbol_size_text)
from resolve_overlay_relocs import (parse_sym_file, name_to_addr,
                                    compile_object, link_resolved,
                                    get_target_bytes, NAME_ADDR_RE,
                                    IDO_CC, IDO_FLAGS, GCCWRAP,
                                    GCCWRAP_FLAGS, link_options_from_source,
                                    StaticAddrError)
from ido_flags import pic_flags

CACHE_VERSION = "6"          # bump to invalidate cached per-function results
STATUS_FILE = ROOT / "config" / "overlay_match_status.json"
UNMATCHABLE = ROOT / "config" / "c_unmatchable.txt"
TRICKS = ROOT / "docs" / "reference" / "MATCHING-TRICKS.md"
ADDENDUM_HEADER = "The 2026-08-08 Autonomous-Campaign Addendum"
SRC_DIR = ROOT / "src" / "overlay"
OUT_MD = ROOT / "build" / "worklists" / "semi_screen.md"
OUT_JSON = ROOT / "build" / "worklists" / "semi_screen.json"
OBJDUMP = "mipsel-linux-gnu-objdump"

RAM_LO, RAM_HI = 0x80000000, 0x801FFFFF

# ---------------------------------------------------------------- MIPS decode

LOAD_OPS = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26}
STORE_OPS = {0x28, 0x29, 0x2A, 0x2B, 0x2E}
ALUI_OPS = {0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E}


def sx16(x):
    return x - 0x10000 if x & 0x8000 else x


def fields(w):
    return (w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31, w & 0x3F)


def writes_reg(w):
    """Destination GPR of instruction word w, or None."""
    op, rs, rt, rd, funct = fields(w)
    if op == 0:
        if funct == 8:                       # jr
            return None
        if funct in (0x18, 0x19, 0x1A, 0x1B, 0x11, 0x13):  # mult/div/mthi/mtlo
            return None
        return rd if rd != 0 else None       # incl. jalr rd
    if op == 3:
        return 31                            # jal
    if op in (1, 2, 4, 5, 6, 7):             # branches / j
        return None
    if op in LOAD_OPS or op in ALUI_OPS or op == 0x0F:
        return rt if rt != 0 else None
    return None


def has_delay_slot(w):
    op, rs, rt, rd, funct = fields(w)
    if op in (1, 2, 3, 4, 5, 6, 7):
        return True
    return op == 0 and funct in (8, 9)       # jr / jalr


def is_move(w):
    """or/addu rd, rs, $zero (or rd, $zero, rt) -- register copy."""
    op, rs, rt, rd, funct = fields(w)
    return (op == 0 and funct in (0x21, 0x25) and rd != 0
            and (rt == 0 or rs == 0) and w != 0)


def is_cmp_or_branch(w):
    op, rs, rt, rd, funct = fields(w)
    if op in (1, 4, 5, 6, 7):                # bltz/bgez, beq, bne, blez, bgtz
        return True
    if op in (0x0A, 0x0B):                   # slti / sltiu
        return True
    return op == 0 and funct in (0x2A, 0x2B)  # slt / sltu


def walk_hilo(words):
    """Linear %hi/%lo pair walk over instruction words.

    Returns list of dicts: {i, addr, kind, base} where kind is
      'la'    lui rX,hi ; addiu rY,rX,lo   (address materialised as a base)
      'mem'   lui rX,hi ; l*/s* rt,lo(rX)  (absolute memory access)
      'jal'   jal target
      'ori'   lui rX,hi ; ori rY,rX,imm    (raw-cast constant form)
    Only RAW lui values are eligible for lo pairing (an addiu-completed base
    is a full address; later `off(base)` accesses are NOT relocs -- this is
    what keeps loaded-pointer/based accesses out of the address set). Any
    write to a tracked register kills its lui value.
    """
    # Branch-target labels: linear pairing across a join point produces
    # PHANTOM addresses (arm 2's %lo paired with arm 1's stale $at hi) --
    # kill all lui tracking at any in-window branch destination. Real
    # cross-BB pairs (as1 XBB delay-slot lui) are lost symmetrically on
    # both windows; under-collection is safe, phantoms are not.
    labels = set()
    for i, w in enumerate(words):
        op = w >> 26
        if op in (1, 4, 5, 6, 7):
            t = i + 1 + sx16(w & 0xFFFF)
            if 0 <= t < len(words):
                labels.add(t)
    lui = {}
    out = []
    for i, w in enumerate(words):
        if i in labels:
            lui.clear()
        op, rs, rt, rd, funct = fields(w)
        if op == 0x0F:                       # lui
            lui[rt] = (w & 0xFFFF) << 16
            continue
        if op in (2, 3):                     # j / jal
            a = 0x80000000 | ((w & 0x3FFFFFF) << 2)
            if op == 3 and RAM_LO <= a <= RAM_HI:
                out.append({'i': i, 'addr': a, 'kind': 'jal', 'base': None})
            lui.pop(31, None)
            continue
        if op == 0x09 and rs in lui:         # addiu rt, rs, %lo
            a = (lui[rs] + sx16(w & 0xFFFF)) & 0xFFFFFFFF
            if RAM_LO <= a <= RAM_HI:
                out.append({'i': i, 'addr': a, 'kind': 'la', 'base': rt})
        elif op == 0x0D and rs in lui:       # ori (lui;ori constant form)
            a = lui[rs] | (w & 0xFFFF)
            if RAM_LO <= a <= RAM_HI:
                out.append({'i': i, 'addr': a, 'kind': 'ori', 'base': rt})
        elif (op in LOAD_OPS or op in STORE_OPS) and rs in lui:
            a = (lui[rs] + sx16(w & 0xFFFF)) & 0xFFFFFFFF
            if RAM_LO <= a <= RAM_HI:
                out.append({'i': i, 'addr': a, 'kind': 'mem', 'base': rs,
                            'op': op})
        wr = writes_reg(w)
        if wr is not None:
            lui.pop(wr, None)
    return out


# ------------------------------------------------------------------ universe

def load_unmatchable():
    """Set of (OVERLAY, func) pairs from config/c_unmatchable.txt."""
    out = set()
    if not UNMATCHABLE.exists():
        return out
    for line in UNMATCHABLE.read_text().splitlines():
        line = line.strip()
        if not line or line.startswith('#'):
            continue
        m = re.match(r'([A-Z0-9_]+)/(\S+)', line)
        if m:
            out.add((m.group(1), m.group(2)))
    return out


def load_addendum_funcs():
    """func_ names mentioned in the MATCHING-TRICKS EOF addendum."""
    try:
        text = TRICKS.read_text(errors='ignore')
    except OSError:
        return set()
    idx = text.find(ADDENDUM_HEADER)
    if idx < 0:
        return set()
    return set(re.findall(r'func_[0-9A-Fa-f]{8}', text[idx:]))


WALL_NOTE_RE = re.compile(
    r'Blocker class:|REFUTATION|refut|genuine wall|BLOCKER|Hook \d+\b',
    re.IGNORECASE)


def collect_universe(only_overlay=None):
    status = json.load(open(STATUS_FILE))
    unmatchable = load_unmatchable()
    universe, skipped = [], Counter()
    for ov in sorted(status):
        if only_overlay and ov != only_overlay:
            continue
        for fn, info in sorted(status[ov].items()):
            st = info.get('status')
            if st not in ('semi_matched', 'count_mismatch'):
                continue
            if (ov, fn) in unmatchable:
                skipped['c_unmatchable'] += 1
                continue
            src = SRC_DIR / ov / f"{fn}.c"
            if not src.exists():
                skipped['no_source'] += 1
                continue
            head = src.read_text(errors='ignore')
            if "Status: ASM-ONLY" in head or "@AsmOnly" in head:
                skipped['asm_only_marker'] += 1
                continue
            universe.append({'ov': ov, 'fn': fn, 'status': st,
                             'exact_pct': info.get('exact_pct', 0.0)})
    return universe, skipped


# ------------------------------------------------------- per-function worker

def parse_extern_decls(content):
    """{name: is_pointer} for address-named externs declared in the C."""
    body = re.sub(r'/\*.*?\*/', ' ', content, flags=re.DOTALL)
    out = {}
    for m in re.finditer(r'\bextern\b([^;]*);', body):
        decl = m.group(1)
        for im in re.finditer(r'[A-Za-z_][A-Za-z0-9_]*', decl):
            name = im.group(0)
            if name_to_addr(name) is None:
                continue
            before = decl[:im.start()]
            is_ptr = bool(re.search(r'\*\s*$', before))
            # func externs with (...) after are calls, keep them too
            out[name] = out.get(name, False) or is_ptr
    return out


def symbol_size_words(obj, func_name):
    """Raw emission size (words) of func_name in obj via its ELF symbol."""
    try:
        r = subprocess.run([OBJDUMP, "-t", str(obj)],
                           capture_output=True, text=True, timeout=30)
    except Exception:
        return None
    best = None
    for line in r.stdout.splitlines():
        if ".text" not in line:
            continue
        parts = line.split()
        if not parts or parts[-1] != func_name:
            continue
        for p in parts:
            if re.fullmatch(r'[0-9a-f]{8}', p) and p != parts[0]:
                best = int(p, 16) // 4
        break
    return best


def compile_raw(ov, fn, src, tmpdir, extra=None):
    """Compile WITHOUT PostProc (raw emission), return obj path or None."""
    extra_flags = list(parse_compile_flags(src)) + (extra or [])
    compile_env = parse_compile_env(src)
    compiler = parse_compiler(src)
    obj = tmpdir / f"{fn}.raw.o"
    inc = ROOT / "include"
    if compiler.startswith("gcc"):
        cmd = [sys.executable, str(GCCWRAP), str(src), "-o", str(obj),
               "--gcc-version", compiler[3:],
               "-I", str(inc), "-I", str(inc / "psyq")]
        cc1_extra = [f for f in extra_flags if f not in GCCWRAP_FLAGS]
        cmd += [f for f in extra_flags if f in GCCWRAP_FLAGS]
        if cc1_extra:
            cmd += ["--extra-flags", " ".join(cc1_extra)]
    else:
        cmd = [str(IDO_CC)] + IDO_FLAGS \
              + pic_flags(src, compile_flags=extra_flags) + extra_flags + \
              ["-c", str(src), "-o", str(obj)]
    env = os.environ.copy()
    env["TMPDIR"] = str(tmpdir)
    if compile_env:
        env.update(compile_env)
    try:
        r = subprocess.run(cmd, capture_output=True, text=True, timeout=90,
                           env=env, cwd=str(tmpdir))
    except Exception:
        return None
    return obj if (r.returncode == 0 and obj.exists()) else None


def screen_lda_fold(twords, taddrs):
    """LUI $at absolute stores within +/-32KB of a la-built base."""
    bases = [a for a in taddrs if a['kind'] == 'la' and a['base'] != 1]
    hits = []
    for a in taddrs:
        if a['kind'] != 'mem' or a['base'] != 1:
            continue
        if a.get('op') not in (0x28, 0x29, 0x2B):    # sb/sh/sw
            continue
        for b in bases:
            delta = a['addr'] - b['addr']
            if -32768 <= delta <= 32767 and a['addr'] != b['addr']:
                hits.append({'store_i': a['i'], 'store_addr': a['addr'],
                             'store_op': {0x28: 'sb', 0x29: 'sh',
                                          0x2B: 'sw'}[a['op']],
                             'base_addr': b['addr'], 'delta': delta})
                break
    return hits


def screen_div_macro(twords):
    """Checked-divide guard chains and the 5-word signed %2 macro."""
    n = len(twords)
    divs = [i for i, w in enumerate(twords)
            if fields(w)[0] == 0 and fields(w)[4] in (0x1A, 0x1B)]
    breaks = [i for i, w in enumerate(twords)
              if fields(w)[0] == 0 and fields(w)[4] == 0x0D]
    checked = []
    for d in divs:
        near = [b for b in breaks if 0 < b - d <= 8]
        if near:
            checked.append({'div_i': d, 'break_i': near[0],
                            'kind': 'divu' if fields(twords[d])[4] == 0x1B
                            else 'div'})
    mod2 = []
    for i in range(n - 4):
        w0, w1, w2, w3, w4 = twords[i:i + 5]
        op0, rs0, rt0, _, _ = fields(w0)
        if not (op0 == 1 and rt0 == 1):                      # bgez
            continue
        op1, rs1, rt1, _, _ = fields(w1)
        if not (op1 == 0x0C and (w1 & 0xFFFF) == 1 and rs1 == rs0):  # andi x,1
            continue
        op2, rs2, rt2, _, _ = fields(w2)
        if not (op2 in (4, 5) and rs2 == rt1 and rt2 == 0):  # beq/bne t,$zero
            continue
        if w3 != 0:                                          # template nop
            continue
        op4 = fields(w4)[0]
        f4 = fields(w4)[4]
        if (op4 == 0x09 and (w4 & 0xFFFF) == 0xFFFE) or \
           (op4 == 0 and f4 in (0x22, 0x23)):                # addiu -2 / subu
            mod2.append({'i': i, 'reg': rs0})
    return checked, mod2


def screen_misname_dynamic(taddrs, caddrs, externs):
    """Compare target vs compiled (name-linked) address multisets.

    A compiled-side orphan address is only a misname CANDIDATE if it is
    ANCHORED to a declared extern (exact decl address, or within a 0x1000
    array window above one) -- everything else is treated as a hi/lo-walk
    artifact and dropped. Loaded-pointer bases are capped at LOW confidence
    (OMIMAI case-6 caveat: confirm against the .s before any rename).
    """
    tset = Counter(a['addr'] for a in taddrs if a['kind'] != 'ori')
    cset = Counter(a['addr'] for a in caddrs if a['kind'] != 'ori')
    t_orphan = sorted((tset - cset).elements())
    c_orphan = sorted((cset - tset).elements())
    decls = sorted((name_to_addr(n), n, isp) for n, isp in externs.items()
                   if name_to_addr(n) is not None)

    def anchor(ca):
        for da, name, isp in decls:
            if ca == da:
                return name, isp, 'exact'
            if 0 <= ca - da < 0x1000:
                return name, isp, 'window'
        return None

    # Target orphans that themselves anchor to a DECLARED extern are count/
    # schedule imbalances of two legitimate globals (SHOUGATU 80137364 base-
    # lui hoist, TACO 8013C570 loop-end sentinel &A[16]), NOT misnames:
    # exact-anchored -> excluded entirely; window-anchored -> pair capped LOW.
    pairs, used_t, dropped = [], set(), 0
    eligible_t = []
    t_cap_low = set()
    for ta in sorted(set(t_orphan)):
        t_anc = anchor(ta)
        if t_anc is not None and t_anc[2] == 'exact':
            continue
        if t_anc is not None:
            t_cap_low.add(ta)
        eligible_t.append(ta)
    for ca in sorted(set(c_orphan)):
        anc = anchor(ca)
        if anc is None:
            dropped += 1
            continue
        name, is_ptr, how = anc
        best = None
        for ta in eligible_t:
            if ta in used_t:
                continue
            d = abs(ca - ta)
            if d <= 0x4000 and (best is None or d < best[1]):
                best = (ta, d)
        if best is None:
            continue
        used_t.add(best[0])
        if is_ptr or best[0] in t_cap_low:
            conf = 'LOW'      # loaded-pointer base / count-imbalance caveat
        elif how == 'exact':
            conf = 'HIGH'
        else:
            conf = 'MED'      # addend inside a declared array window
        pairs.append({'compiled_addr': ca, 'target_addr': best[0],
                      'delta': best[0] - ca, 'confidence': conf,
                      'extern': name, 'anchor': how})
    lone_t = sorted(set(t_orphan) - used_t)
    lone_c = sorted(set(c_orphan) - {p['compiled_addr'] for p in pairs})
    return pairs, lone_t, lone_c


def screen_join_phi(diffs, twords, cwords):
    """Dead-store copy-prop blocker signature (MATCHING-TRICKS addendum:
    RPG_BAT func_8015528C / NAME_ENT func_8013909C): the TARGET carries a
    `move` (or/addu with $zero) the compiled output lacks, with a
    bgez/bltz-on-temp or slt-family compare within +/-3 words in the
    target. Compiled-side surplus moves are NOT this class (those are
    ordinary register-pressure diffs) and are excluded to keep the queue
    tight."""
    hits = []
    for i, tw, cw in diffs:
        if not (is_move(tw) and not is_move(cw)):
            continue
        lo, hi = max(0, i - 3), min(len(twords), i + 4)
        near = [j for j in range(lo, hi) if j != i]
        has_guard = any(
            (twords[j] >> 26) == 1 or                       # bgez/bltz
            ((twords[j] >> 26) == 0 and
             (twords[j] & 0x3F) in (0x2A, 0x2B)) or         # slt/sltu
            (twords[j] >> 26) in (0x0A, 0x0B)               # slti/sltiu
            for j in near)
        if has_guard:
            hits.append({'i': i, 'target': tw, 'compiled': cw,
                         'surplus_side': 'target'})
    return hits


def screen_delay_fill(diffs, twords, cwords, len_equal):
    """1-2 word count-exact residuals: delay-slot nop-vs-insn or dup fill."""
    if not len_equal or not (1 <= len(diffs) <= 2):
        return []
    hits = []
    for i, tw, cw in diffs:
        in_delay = i > 0 and (has_delay_slot(twords[i - 1]) or
                              has_delay_slot(cwords[i - 1]))
        nop_vs_insn = (tw == 0) != (cw == 0)
        dup = False
        if in_delay and tw != 0:
            dup = twords.count(tw) >= 2      # duplicated fill word
        if in_delay and (nop_vs_insn or dup):
            hits.append({'i': i, 'target': tw, 'compiled': cw,
                         'nop_vs_insn': nop_vs_insn, 'duplicated': dup})
    return hits if len(hits) == len(diffs) else []


def analyze_function(job):
    """Full per-function analysis. Runs in a worker process; cached."""
    ov, fn = job['ov'], job['fn']
    import overlay_names as _ON
    _ON.set_ambient(ov)             # this worker's landed overlay names resolve first
    cache_dir = Path(job['cache_dir']) / ov / fn
    src = SRC_DIR / ov / f"{fn}.c"
    do_compile = job['do_compile']
    os.chdir(ROOT)

    funcs = parse_sym_file(ov)
    if fn not in funcs:
        return {'ov': ov, 'fn': fn, 'error': 'not in .sym'}
    func_addr, func_size = funcs[fn]
    content = src.read_text(errors='ignore')
    _sz = parse_symbol_size_text(content)
    if _sz is not None:
        func_size = _sz
    _st = parse_symbol_start_text(content)
    if _st is not None:
        func_addr = _st

    target = get_target_bytes(ov, func_addr, func_size)
    key = hashlib.sha1(
        (CACHE_VERSION + str(do_compile)).encode() +
        content.encode('utf-8', 'ignore') + target).hexdigest()
    cache_file = cache_dir / f"result-{key}.json"
    if cache_file.exists():
        try:
            return json.load(open(cache_file))
        except Exception:
            pass

    twords = [struct.unpack('<I', target[i:i + 4])[0]
              for i in range(0, len(target) - 3, 4)]
    taddrs = walk_hilo(twords)
    externs = parse_extern_decls(content)

    res = {'ov': ov, 'fn': fn, 'addr': func_addr,
           'window_words': len(twords), 'status': job['status'],
           'exact_pct': job['exact_pct'], 'labels': []}
    m = re.match(r'(/\*.*?\*/)', content, flags=re.DOTALL)
    header = m.group(1) if m else content[:2000]
    if WALL_NOTE_RE.search(header):
        res['labels'].append('wall-note')

    # static screens
    res['lda_fold'] = screen_lda_fold(twords, taddrs)
    checked, mod2 = screen_div_macro(twords)
    res['checked_div'] = checked
    res['mod2_macro'] = mod2

    if not do_compile:
        cache_dir.mkdir(parents=True, exist_ok=True)
        json.dump(res, open(cache_file, 'w'))
        return res

    cache_dir.mkdir(parents=True, exist_ok=True)

    # raw emission size (no PostProc)
    raw_obj = compile_raw(ov, fn, src, cache_dir)
    raw_words = symbol_size_words(raw_obj, fn) if raw_obj else None
    res['raw_words'] = raw_words

    # screen 2: unroll snap
    flags = " ".join(parse_compile_flags(src))
    compiler = parse_compiler(src)
    if (raw_words is not None and raw_words - len(twords) > 8
            and 'loopunroll' not in flags
            and not compiler.startswith('gcc')):
        u_obj = compile_raw(ov, fn, src, cache_dir,
                            extra=['-Wo,-loopunroll,0'])
        u_words = symbol_size_words(u_obj, fn) if u_obj else None
        res['unroll_words'] = u_words
        if u_words is not None and abs(u_words - len(twords)) <= 2:
            res['unroll_snap'] = True

    # compile + PostProc + name-derived link (resolver path)
    obj = compile_object(ov, fn, src, func_addr, cache_dir)
    if obj is None:
        res['compile_error'] = True
        json.dump(res, open(cache_file, 'w'))
        return res
    # @RodataBase / @StaticAddr exactly as the resolver honours them.
    try:
        linked = link_resolved(obj, func_addr, func_size, cache_dir,
                               func_name=fn, overlay_name=ov,
                               **link_options_from_source(content))
    except StaticAddrError:
        linked = None
    if linked is None:
        res['link_error'] = True
        json.dump(res, open(cache_file, 'w'))
        return res
    compiled, _resolved, unresolved = linked
    cwords = [struct.unpack('<I', compiled[i:i + 4])[0]
              for i in range(0, len(compiled) - 3, 4)]
    n = min(len(twords), len(cwords))
    diffs = [(i, twords[i], cwords[i]) for i in range(n)
             if twords[i] != cwords[i]]
    res['resolved_exact'] = n - len(diffs)
    res['resolved_n'] = n
    res['len_equal'] = (len(twords) == len(cwords))
    res['n_diffs'] = len(diffs)
    if n:
        res['resolved_pct'] = round(100.0 * (n - len(diffs)) / n, 1)

    caddrs = walk_hilo(cwords)
    pairs, lone_t, lone_c = screen_misname_dynamic(taddrs, caddrs, externs)
    res['misname'] = {'pairs': pairs, 'lone_target': lone_t[:12],
                      'lone_compiled': lone_c[:12]}
    res['join_phi'] = screen_join_phi(diffs, twords, cwords)
    res['delay_fill'] = screen_delay_fill(diffs, twords, cwords,
                                          res['len_equal'])

    # A static lda_fold / div-macro site is only actionable if the words
    # AROUND it still mismatch -- annotate sites with diff proximity so the
    # report can drop already-matching sites (the lever isn't needed there).
    diff_idx = set(i for i, _t, _c in diffs)

    def near_diff(i, span=4):
        return any(j in diff_idx for j in range(i - span, i + span + 1))

    for h in res['lda_fold']:
        h['near_diff'] = near_diff(h['store_i'])
    for h in res['checked_div']:
        h['near_diff'] = near_diff(h['div_i'], span=8)
    for h in res['mod2_macro']:
        h['near_diff'] = near_diff(h['i'], span=5)
    json.dump(res, open(cache_file, 'w'))
    return res


# ------------------------------------------------------------------- reports

def fmt_row(r, evidence):
    pct = r.get('resolved_pct', r.get('exact_pct', 0.0)) or 0.0
    return (f"| {r['ov']}/{r['fn']} | 0x{r['addr']:08X} | {pct:.1f} | "
            f"{evidence} |")


def sort_key(r):
    return -(r.get('resolved_pct', r.get('exact_pct', 0.0)) or 0.0)


def build_reports(results, skipped, elapsed, addendum_funcs):
    screens = {k: [] for k in ('misname', 'unroll', 'lda_fold', 'join_phi',
                               'delay_fill', 'div_macro')}
    errors = []
    for r in results:
        if 'error' in r:
            errors.append(r)
            continue
        if r['fn'] in addendum_funcs and 'addendum' not in r['labels']:
            r['labels'].append('addendum')
        def actionable(sites):
            # keep sites near a residual diff; near_diff missing (no
            # compile) counts as unknown -> keep, the row says so
            return [h for h in sites if h.get('near_diff') is not False]

        mis = r.get('misname') or {}
        if mis.get('pairs'):
            screens['misname'].append(r)
        if r.get('unroll_snap'):
            screens['unroll'].append(r)
        if actionable(r.get('lda_fold') or []):
            screens['lda_fold'].append(r)
        if r.get('join_phi'):
            screens['join_phi'].append(r)
        if r.get('delay_fill'):
            screens['delay_fill'].append(r)
        if (actionable(r.get('checked_div') or []) or
                actionable(r.get('mod2_macro') or [])):
            screens['div_macro'].append(r)
    for k in screens:
        screens[k].sort(key=sort_key)

    lines = ["# Semi-matched lever screen (`screen_semis.py`)", ""]
    lines.append(f"Generated {time.strftime('%Y-%m-%d %H:%M')} over "
                 f"{len(results)} semi/count_mismatch functions "
                 f"({elapsed:.0f}s). Levers: MATCHING-TRICKS.md "
                 f"2026-08-08 addendum. Rows sorted by resolved exact% "
                 f"descending (closest-to-done first).")
    lines.append("")
    lines.append("Labels: `wall-note` = header documents a probed wall "
                 "(re-probe with newer hooks before trusting); `addendum` = "
                 "function named in the EOF addendum.")
    lines.append("")
    lines.append("## Summary")
    lines.append("")
    lines.append("| Screen | Hits | Confidence notes |")
    lines.append("|---|---|---|")
    notes = {
        'misname': ("Paired orphan addresses on both sides. LOW = "
                    "loaded-pointer base (never rename without reading the "
                    ".s -- OMIMAI caveat)"),
        'unroll': "Raw emission snaps to window +/-2 with -Wo,-loopunroll,0",
        'lda_fold': ("lui $at absolute store within +/-32KB of a la-built "
                     "base -- volatile store-site cast lever"),
        'join_phi': ("TARGET-side surplus move with a bgez/slt-family guard "
                     "within +/-3 words -- dead-store copy-prop cure "
                     "candidate (compiled-side moves excluded)"),
        'delay_fill': ("1-2 word count-exact delay-slot residuals -- "
                       "spelling / Hook 27 class"),
        'div_macro': ("Target has div+break guard chains or signed %2 "
                      "macro -- spelling levers"),
    }
    titles = {'misname': 'MISNAME SCAN', 'unroll': 'UNROLL SNAP',
              'lda_fold': 'LDA-FOLD CANDIDATES',
              'join_phi': 'JOIN-PHI SIGNATURE',
              'delay_fill': 'DELAY-FILL CLASS',
              'div_macro': 'CHECKED-DIVIDE / %2-MACRO'}
    for k in ('misname', 'unroll', 'lda_fold', 'join_phi', 'delay_fill',
              'div_macro'):
        lines.append(f"| {titles[k]} | {len(screens[k])} | {notes[k]} |")
    lines.append("")
    lines.append(f"Skipped: {dict(skipped)}; analysis errors: "
                 f"{len(errors)}.")
    lines.append("")

    # Cross-screen top targets: closest-to-done functions with >=1 hit.
    tagged = {}
    for k, v in screens.items():
        for r in v:
            key = (r['ov'], r['fn'])
            tagged.setdefault(key, (r, []))[1].append(titles[k])
    top = sorted(tagged.values(), key=lambda t: sort_key(t[0]))[:15]
    lines.append("## Top targets (all screens, closest-to-done first)")
    lines.append("")
    lines.append("| Function | exact% | Screens hit |")
    lines.append("|---|---|---|")
    for r, tags in top:
        pct = r.get('resolved_pct', r.get('exact_pct', 0.0)) or 0.0
        lines.append(f"| {r['ov']}/{r['fn']} | {pct:.1f} | "
                     f"{'; '.join(tags)} |")
    lines.append("")

    def emit(k, evfn):
        lines.append(f"## {titles[k]}")
        lines.append("")
        if not screens[k]:
            lines.append("No hits.")
            lines.append("")
            return
        lines.append("| Function | Addr | exact% | Evidence |")
        lines.append("|---|---|---|---|")
        for r in screens[k]:
            ev = evfn(r)
            if r['labels']:
                ev += " [" + ",".join(r['labels']) + "]"
            lines.append(fmt_row(r, ev))
        lines.append("")

    def ev_misname(r):
        ps = r['misname']['pairs']
        parts = [f"{p['extern']}(+{p['compiled_addr'] - name_to_addr(p['extern']):#x})"
                 f"->target {p['target_addr']:08X} (d={p['delta']:+#x}) "
                 f"{p['confidence']}" for p in ps[:3]]
        if len(ps) > 3:
            parts.append(f"+{len(ps) - 3} more")
        return "; ".join(parts)

    def ev_unroll(r):
        return (f"raw {r['raw_words']}w -> {r['unroll_words']}w vs window "
                f"{r['window_words']}w")

    def ev_lda(r):
        sites = [h for h in r['lda_fold'] if h.get('near_diff') is not False]
        h = sites[0]
        s = (f"{h['store_op']} @{h['store_addr']:08X} vs base "
             f"{h['base_addr']:08X} (d={h['delta']:+#x})")
        if len(sites) > 1:
            s += f" +{len(sites) - 1} more"
        if h.get('near_diff') is None:
            s += " (no-compile: unverified vs diffs)"
        return s

    def ev_joinphi(r):
        h = r['join_phi'][0]
        s = (f"word[{h['i']}] {h['surplus_side']} move "
             f"t={h['target']:08x} c={h['compiled']:08x}")
        if len(r['join_phi']) > 1:
            s += f" +{len(r['join_phi']) - 1} more"
        return s

    def ev_delay(r):
        h = r['delay_fill'][0]
        kind = 'nop-vs-insn' if h['nop_vs_insn'] else 'dup-fill'
        return (f"{r['n_diffs']} diff(s), word[{h['i']}] {kind} "
                f"t={h['target']:08x} c={h['compiled']:08x}")

    def ev_div(r):
        parts = []
        cd = [h for h in r.get('checked_div') or []
              if h.get('near_diff') is not False]
        m2 = [h for h in r.get('mod2_macro') or []
              if h.get('near_diff') is not False]
        if cd:
            parts.append(f"{len(cd)} checked-div site(s) near diffs")
        if m2:
            parts.append(f"{len(m2)} %2-macro site(s) near diffs")
        return ", ".join(parts)

    emit('misname', ev_misname)
    emit('unroll', ev_unroll)
    emit('lda_fold', ev_lda)
    emit('join_phi', ev_joinphi)
    emit('delay_fill', ev_delay)
    emit('div_macro', ev_div)

    md = "\n".join(lines) + "\n"
    js = {
        'generated': time.strftime('%Y-%m-%dT%H:%M:%S'),
        'universe': len(results),
        'skipped': dict(skipped),
        'errors': [{'ov': e['ov'], 'fn': e['fn'], 'error': e['error']}
                   for e in errors],
        'screens': {k: [{kk: vv for kk, vv in r.items()
                         if kk not in ('labels',)} | {'labels': r['labels']}
                        for r in v]
                    for k, v in screens.items()},
    }
    return md, js, screens


def main():
    ap = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--overlay', help='screen one overlay only')
    ap.add_argument('--jobs', type=int, default=6)
    ap.add_argument('--no-compile', action='store_true',
                    help='static screens only (1-fallback, 3, 6)')
    ap.add_argument('--cache-dir',
                    default=os.environ.get(
                        'SCREEN_SEMIS_CACHE',
                        str(Path(tempfile.gettempdir()) /
                            'toki_screen_semis')))
    ap.add_argument('--out-md', default=str(OUT_MD))
    ap.add_argument('--out-json', default=str(OUT_JSON))
    args = ap.parse_args()

    t0 = time.time()
    universe, skipped = collect_universe(args.overlay)
    print(f"screening {len(universe)} functions "
          f"(skipped {dict(skipped)})...", file=sys.stderr)
    jobs = [dict(u, cache_dir=args.cache_dir,
                 do_compile=not args.no_compile) for u in universe]
    results = []
    if args.jobs > 1:
        with ProcessPoolExecutor(max_workers=args.jobs) as ex:
            for i, r in enumerate(ex.map(analyze_function, jobs)):
                results.append(r)
                if (i + 1) % 25 == 0:
                    print(f"  {i + 1}/{len(jobs)}", file=sys.stderr)
    else:
        for i, j in enumerate(jobs):
            results.append(analyze_function(j))

    elapsed = time.time() - t0
    md, js, screens = build_reports(results, skipped, elapsed,
                                    load_addendum_funcs())
    Path(args.out_md).parent.mkdir(parents=True, exist_ok=True)
    Path(args.out_md).write_text(md)
    Path(args.out_json).write_text(json.dumps(js, indent=1))
    print(f"\n{'screen':<12} hits", file=sys.stderr)
    for k, v in screens.items():
        print(f"{k:<12} {len(v)}", file=sys.stderr)
    print(f"wrote {args.out_md} + {args.out_json} ({elapsed:.0f}s)",
          file=sys.stderr)


if __name__ == '__main__':
    main()
