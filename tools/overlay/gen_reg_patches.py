#!/usr/bin/env python3
"""Generate strict patch_insn_reg_field batches from test_overlay_match -v output.

Runs the single-function overlay test, classifies every diffed word pair, and
appends register-field patches to the function's @PostProc header for pairs
that are SAFE to patch:

  - Same primary opcode.
  - R-type (SPECIAL): funct and shamt must also match; then rs/rt/rd may be
    patched.
  - I-type: imm16 must match exactly, OR the compiled imm16 must be 0 (a
    relocation placeholder).  Pairs whose immediates differ otherwise are
    classified OTHER and never patched.

The imm16 rule is load-bearing.  The masked metric does NOT compare imm16,
so a lax classifier that reg-patches imm-mismatched pairs fabricates words
that look matched but are wrong ("frankenwords"); the old matching playbook's
"Large-function full-rewrite playbook" (kept in the project's private
archive) has the history.

Usage:
    python3 tools/overlay/gen_reg_patches.py OVERLAY func_XXXXXXXX [--dry-run]

Run it repeatedly (2-3 passes) until it reports NREG=0; each pass appends to
the existing @PostProc: patch_insn_reg_field line (created after the
scheduler_normalizer line if missing).  Patches are computed POST-scheduler,
so the C file must already carry "@PostProc: scheduler_normalizer" (or none,
consistently) before the first pass.

=============================================================================
EMIT-TIME REFUSALS  (2026-08-09; there is no --force)
=============================================================================
A generated paint is REFUSED, with a reason and a suggested alternative, when
it falls in a provably-unsound class.  Refused entries are dropped; the rest
are still emitted.  If everything is refused, nothing is written and the tool
exits 1 — that function needs a C fix, not a paint layer.

  1. COMPILED NOP.  A nop IS `sll $zero,$zero,0`; painting a register field
     into it fabricates an instruction and fakes a ✅ (CLAUDE.md ban).
  2. BRANCH CONDITION FIELDS.  rt on REGIMM (op 1) or on blez/bgtz (op 6/7)
     is the branch CONDITION, not a register; and zero<->register on any
     branch word changes the comparison.  ⭐ CORRECTED RULE: beq/bne (op 4/5)
     `rt` IS an ordinary register operand — those paints are still emitted.
  3. $at.  The assembler temp is created by macro expansion, never by this C.
  4. REACHING-DEF BREAK.  The paint would re-point an operand at a different
     producing instruction (`audit_paint_isomorphism.py`, filtered for the
     upstream-repair false positive by `minimize_paint_strip.nflags`).
     Costs 4 extra compiles; on by default when emitting, off in --dry-run
     unless --iso-check.
  5. ⭐ %hi/%lo SYMBOL RE-PAIRING — THE WRONG-GLOBAL CLASS.  A paint can
     re-point a `%lo` consumer at a different `lui %hi` of the same shape, so
     a store lands on the WRONG GLOBAL.  The producer graph is identical and
     the register dataflow is consistent, so NEITHER semcheck NOR the
     isomorphism audit can see it; only the relocation symbol differs.
     Found shipped on DATE/func_80134CF4 (6 wrong-symbol pairs inside a
     28-entry, iso-clean layer).  Every `%lo`'s dominating `%hi` must carry
     the same symbol before and after the paint layer.

Sibling audits for layers that already exist on disk:
  tools/overlay/audit_paint_bans.py         (per-paint ban screen)
  tools/overlay/audit_paint_isomorphism.py  (reaching-def, ADVISORY)
  tools/overlay/minimize_paint_strip.py     (repair-aware minimal strip)
  tools/overlay/audit_raw_count.py          (raw instruction-count self-audit)
"""
import argparse
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

REGS = ['zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3',
        't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
        's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7',
        't8', 't9', 'k0', 'k1', 'gp', 'sp', 'fp', 'ra']

REPO = Path(__file__).resolve().parents[2]


def reloc_offsets(keep_dir, ov, fn):
    """Text-relocation offsets of the kept object (pre-postproc)."""
    obj = Path(keep_dir) / ov / fn / (fn + '.o')
    if not obj.exists():
        return set()
    out = subprocess.run(['mips-linux-gnu-objdump', '-r', str(obj)],
                         capture_output=True, text=True).stdout
    offs = set()
    in_text = False
    for line in out.splitlines():
        if line.startswith('RELOCATION RECORDS FOR [.text]'):
            in_text = True
            continue
        if line.startswith('RELOCATION RECORDS FOR ['):
            in_text = False
        m = re.match(r'([0-9a-f]+)\s+R_MIPS_', line) if in_text else None
        if m:
            offs.add(int(m.group(1), 16))
    return offs


def func_text_offset(keep_dir, ov, fn):
    """Real function offset inside .text (leading thunks). A file that does
    not #include types.h compiles with NO 4-word thunk preamble; assuming
    0x10 unconditionally made every generated offset 0x10 too high
    (D2, 2026-07-22: one function went 271/351 -> 351/351 once corrected)."""
    obj = Path(keep_dir) / ov / fn / (fn + '.o')
    if obj.exists():
        try:
            sys.path.insert(0, str(REPO / 'tools' / 'overlay'))
            from overlay_postproc import get_func_text_offset
            fto = get_func_text_offset(obj)
            if fto is not None:
                return fto
        except Exception:
            pass
    return 0x10


def classify(out, relocs=frozenset(), fto=0x10, words=None):
    """Classify the -v diff listing into paintable patches and OTHER.

    `words` (optional dict) is filled with {object_offset: compiled_word} for
    every diffed pair, which the emit-time refusal screens then use without
    having to re-read the object.
    """
    patches = []
    other = []
    for mt in re.finditer(r'\[\s*(\d+)\] target=([0-9a-f]{8}) compiled=([0-9a-f]{8})', out):
        idx = int(mt.group(1))
        tw = int(mt.group(2), 16)
        cw = int(mt.group(3), 16)
        off = fto + idx * 4           # object offset: real thunk-preamble size
        if words is not None:
            words[off] = cw
        op = tw >> 26
        bad = op != (cw >> 26)
        if not bad and op == 0 and (tw & 0x7FF) != (cw & 0x7FF):
            bad = True                 # funct/shamt mismatch
        # op 4/5 = beq/bne: the imm16 is the PC-relative branch offset, which
        # is reloc-masked (test_overlay_match masks a branch when its upper 16
        # bits — opcode+rs+rt — match). Treat like j/jal: skip the imm check so
        # the rs/rt register diff gets painted; the offset is don't-care.
        if not bad and op not in (0, 2, 3, 4, 5):
            timm, cimm = tw & 0xFFFF, cw & 0xFFFF
            # imm16 must match unless this offset is a relocation site
            # (placeholder may be 0 OR a non-zero addend) — pilot
            # feedback 2026-06-12, BUNKAKEN/func_8014B800
            if timm != cimm and cimm != 0 and off not in relocs:
                bad = True
        if bad:
            other.append((idx, mt.group(2), mt.group(3)))
            continue
        trs, crs = (tw >> 21) & 31, (cw >> 21) & 31
        trt, crt = (tw >> 16) & 31, (cw >> 16) & 31
        if op == 0:
            trd, crd = (tw >> 11) & 31, (cw >> 11) & 31
            if trd != crd:
                patches.append('0x%x:rd:%s:%s' % (off, REGS[crd], REGS[trd]))
        if trs != crs:
            patches.append('0x%x:rs:%s:%s' % (off, REGS[crs], REGS[trs]))
        if trt != crt:
            patches.append('0x%x:rt:%s:%s' % (off, REGS[crt], REGS[trt]))
    return patches, other


# =====================================================================
# EMIT-TIME REFUSALS
# =====================================================================
# Three screens run before anything is written.  Each prints the offending
# entry, the reason, and what to do instead.  There is deliberately NO
# --force: a refused paint is a defect in the C (or upstream in the PostProc
# chain), and painting over it produces an object that scores well and
# computes the wrong thing.
# ---------------------------------------------------------------------

SUGGEST = {
    'nop': ("the TARGET has a real instruction where the C emitted a nop: "
            "that is a MISSING instruction (usually a hidden parameter, a "
            "missing narrow-type cast on a callee extern, or wrong control "
            "flow), not a paint target. A nop IS sll $zero,$zero,0 — painting "
            "it FABRICATES an instruction (CLAUDE.md ban)."),
    'regimm': ("the branch CONDITION differs, not a register: fix the C's "
               "guard, or use @PostProc: invert_branch / swap_branch_operands."),
    'brzero': ("the compare OPERAND differs (reg vs $zero): fix the C's "
               "comparison; a paint here changes branch semantics."),
    'at': ("$at is the assembler temp — it is created by macro expansion, "
           "never by this C, so a read re-pointed at $at reads garbage. "
           "Change the C's addressing form (named extern vs raw-address cast) "
           "or use cse_lui_at; do not paint $at."),
    'iso': ("this paint re-points an operand at a DIFFERENT producing "
            "instruction, so the object computes a different value while the "
            "masked/resolved compare still scores it. That is a C bug (the C "
            "is not reproducing the target's dataflow) or upstream "
            "scheduler_normalizer damage — not a paint target."),
    'reloc': ("this paint re-pairs a %lo with a DIFFERENT %hi, so the "
              "load/store would hit the WRONG GLOBAL. Invisible to semcheck "
              "and to the producer-graph audit. Never paint an object whose "
              "block alignment has drifted; fix the C's symbol usage or "
              "replace the whole-function scheduler_normalizer with explicit "
              "swap_instructions first."),
}


def screen_provable(patches, words):
    """Refuse the provable-unsound classes (free — no rebuild needed).

    Uses the CORRECTED branch rule from audit_paint_bans.screen_paint: the
    rt-field ban covers REGIMM (op 1) and blez/bgtz (op 6/7) only; beq/bne rt
    is an ordinary register operand and painting it is legal.
    """
    sys.path.insert(0, str(REPO / 'tools' / 'overlay'))
    from audit_paint_bans import screen_paint
    keep, refused = [], []
    for p in patches:
        off_s, fld, old, new = p.split(':')
        off = int(off_s, 16)
        w = words.get(off)
        if w is None:
            keep.append(p)
            continue
        if 'at' in (old, new):
            refused.append((p, 'at', "paint involves $at"))
            continue
        verdicts = screen_paint(w, fld, old, new)
        ban = next((r for v, r in verdicts if v == 'BANNED'), None)
        if ban is None:
            keep.append(p)
            continue
        if 'NOP' in ban:
            refused.append((p, 'nop', ban))
        elif 'zero<->reg' in ban:
            refused.append((p, 'brzero', ban))
        else:
            refused.append((p, 'regimm', ban))
    return keep, refused


# ---------------------------------------------------------------------
# B2: %hi/%lo RELOCATION-SYMBOL PAIRING
# ---------------------------------------------------------------------
def _text_words(obj):
    with tempfile.TemporaryDirectory() as td:
        bin_ = Path(td) / 'text.bin'
        subprocess.run(['mipsel-linux-gnu-objcopy', '-O', 'binary',
                        '--only-section=.text', str(obj), str(bin_)],
                       capture_output=True)
        if not bin_.exists():
            return []
        b = bin_.read_bytes()
    return list(struct.unpack('<%dI' % (len(b) // 4), b[:len(b) // 4 * 4]))


def _text_relocs(obj):
    """{offset: (type, symbol)} for .text relocations."""
    out = subprocess.run(['mipsel-linux-gnu-objdump', '-r', str(obj)],
                         capture_output=True, text=True).stdout
    rel, in_text = {}, False
    for line in out.splitlines():
        if line.startswith('RELOCATION RECORDS FOR [.text]'):
            in_text = True
            continue
        if line.startswith('RELOCATION RECORDS FOR ['):
            in_text = False
            continue
        if not in_text:
            continue
        m = re.match(r'([0-9a-f]+)\s+(R_MIPS_\S+)\s+(\S+)', line)
        if m:
            rel[int(m.group(1), 16)] = (m.group(2), m.group(3).split('+')[0])
    return rel


def _defines(word):
    """Destination register of `word`, or None."""
    op, f = word >> 26, word & 0x3F
    if op == 0:
        if f in (0x08,):                      # jr
            return None
        if f in (0x18, 0x19, 0x1a, 0x1b):     # mult/div
            return None
        return (word >> 11) & 31
    if op in (2, 3):                          # j / jal
        return 31 if op == 3 else None
    if op in (1, 4, 5, 6, 7):                 # branches
        return None
    if op in (0x28, 0x29, 0x2b, 0x2a, 0x2e):  # stores
        return None
    return (word >> 16) & 31                  # I-type rt


def pair_symbols(words, rel):
    """{lo_offset: (hi_offset|None, hi_symbol|None, lo_symbol)}.

    For each R_MIPS_LO16 site, walk backwards for the nearest `lui rt`
    carrying an R_MIPS_HI16 whose rt equals the LO16 instruction's base
    register, stopping if that register is redefined in between.  That is the
    %hi the assembler/linker pairing effectively uses at run time.
    """
    pairs = {}
    for off, (rtype, sym) in sorted(rel.items()):
        if rtype != 'R_MIPS_LO16':
            continue
        i = off // 4
        if i >= len(words):
            continue
        base = (words[i] >> 21) & 31
        hi_off = hi_sym = None
        for j in range(i - 1, -1, -1):
            w = words[j]
            if (w >> 26) == 0x0f and ((w >> 16) & 31) == base:   # lui base
                r = rel.get(j * 4)
                hi_off, hi_sym = j * 4, (r[1] if r and r[0] == 'R_MIPS_HI16'
                                         else None)
                break
            if _defines(w) == base:
                break                     # base redefined by something else
        pairs[off] = (hi_off, hi_sym, sym)
    return pairs


def apply_patches_inmem(words, patches):
    """Apply `off:field:old:new` entries to a copy of the word list."""
    shift = {'rs': 21, 'rt': 16, 'rd': 11}
    w = list(words)
    for p in patches:
        off_s, fld, old, new = p.split(':')
        i = int(off_s, 16) // 4
        if i >= len(w) or fld not in shift:
            continue
        s = shift[fld]
        w[i] = (w[i] & ~(0x1F << s)) | (REGS.index(new) << s)
    return w


def screen_reloc_pairing(obj, patches):
    """Refuse any paint that changes which %hi a %lo pairs with.

    THE WRONG-GLOBAL CLASS.  A paint can re-point a %lo consumer at a
    different `lui %hi` of the same shape: the dataflow SHAPE is identical
    (so audit_paint_isomorphism sees nothing) and the register dataflow is
    consistent (so semcheck sees nothing) — only the relocation SYMBOL the
    address is built from changes, and the store lands on the wrong global.
    Found on DATE/func_80134CF4 (6 wrong-symbol pairs inside a 28-entry
    iso-clean layer).
    """
    if not obj or not Path(obj).exists():
        return patches, [], "object not available — pairing NOT checked"
    words = _text_words(obj)
    rel = _text_relocs(obj)
    if not words or not rel:
        return patches, [], None
    before = pair_symbols(words, rel)
    pre_crossed = [lo for lo, (hi, hisym, losym) in before.items()
                   if hisym is not None and hisym != losym]

    def broken(sel):
        """%lo sites whose dominating %hi SYMBOL changes.

        A different `lui` carrying the SAME symbol is fine (that is ordinary
        register reallocation); losing the dominating %hi entirely, or
        acquiring a different symbol's, is not.
        """
        after = pair_symbols(apply_patches_inmem(words, sel), rel)
        out = []
        for lo, (hi, hisym, losym) in after.items():
            b_hi, b_hisym, _ = before.get(lo, (None, None, None))
            if hisym != b_hisym:
                out.append((lo, b_hi, b_hisym, hi, hisym, losym))
        return out

    warn = None
    if pre_crossed:
        warn = ("%d %%lo site(s) ALREADY pair with a different symbol's %%hi "
                "before this batch (0x%s) — pre-existing wrong-global damage, "
                "not attributed to these paints"
                % (len(pre_crossed), ", 0x".join("%x" % o for o in pre_crossed)))
    bad = broken(patches)
    if not bad:
        return patches, [], warn
    # attribute: drop the smallest set of entries that removes the breakage
    implicated = set()
    for lo, b_hi, b_hisym, a_hi, a_hisym, losym in bad:
        offs = {lo}
        if b_hi is not None:
            offs.add(b_hi)
        if a_hi is not None:
            offs.add(a_hi)
        for p in patches:
            if int(p.split(':')[0], 16) in offs:
                implicated.add(p)
    keep = [p for p in patches if p not in implicated]
    if broken(keep):
        # cannot attribute cleanly -> refuse the whole batch
        implicated = set(patches)
        keep = []
    refused = []
    detail = {}
    for lo, b_hi, b_hisym, a_hi, a_hisym, losym in bad:
        detail.setdefault(lo, (b_hisym, a_hisym, losym))
    for p in sorted(implicated, key=lambda x: int(x.split(':')[0], 16)):
        refused.append((p, 'reloc',
                        "breaks a %%hi/%%lo pairing (%s)"
                        % "; ".join("0x%x: %s -> %s" % (lo, b or '?', a or '?')
                                    for lo, (b, a, _) in sorted(detail.items()))))
    return keep, refused, warn


# ---------------------------------------------------------------------
# B1 (dataflow half): reaching-definition isomorphism
# ---------------------------------------------------------------------
def screen_isomorphism(ov, fn, cur_text, patches, insert):
    """Refuse paints that change an operand's producing instruction.

    Runs the promoted audit (`audit_paint_isomorphism`) through the
    repair-aware filter (`minimize_paint_strip.nflags`) on the CURRENT text
    and on the candidate text, and refuses the entries implicated in any
    NEW flag.  Repair-direction flags (the paint FIXING upstream
    scheduler_normalizer damage) are filtered out, so this does not refuse
    the legitimate repair class.
    """
    sys.path.insert(0, str(REPO / 'tools' / 'overlay'))
    import minimize_paint_strip as mps
    base, err = mps.nflags(ov, fn, cur_text)
    if err:
        return patches, [], "iso screen SKIPPED (baseline build failed: %s)" % err
    sel = list(patches)
    for _ in range(4):
        cand, err = mps.nflags(ov, fn, insert(cur_text, sel))
        if err:
            return patches, [], "iso screen SKIPPED (candidate build failed: %s)" % err
        base_idx = {f[0] for f in base}
        new = [f for f in cand if f[0] not in base_idx]
        if not new:
            break
        offs = set()
        for i, ua, ub, sa, sb in new:
            offs.add(i)
            for s in list(sa) + list(sb):
                for p in s:
                    if isinstance(p, int):
                        offs.add(p)
                    elif isinstance(p, tuple):
                        offs.add(p[1])
        drop = [p for p in sel if int(p.split(':')[0], 16) // 4 in offs]
        if not drop:
            break
        sel = [p for p in sel if p not in drop]
    refused = [(p, 'iso', "introduces a reaching-definition producer change")
               for p in patches if p not in sel]
    return sel, refused, None


def report_refusals(refused):
    if not refused:
        return
    print("\n⛔ REFUSED %d paint(s) — provably unsound classes:" % len(refused))
    seen = set()
    for p, kind, reason in refused:
        print("   %-28s %s" % (p, reason))
        if kind not in seen:
            seen.add(kind)
    for kind in seen:
        print("   → %s" % SUGGEST[kind])


def main():
    ap = argparse.ArgumentParser(
        description="Generate strict patch_insn_reg_field batches. Refuses to "
                    "emit provably-unsound paints (compiled-nop, REGIMM/"
                    "blez/bgtz rt, zero<->reg on a branch, $at, "
                    "reaching-def break, %hi/%lo symbol re-pairing).")
    ap.add_argument('overlay')
    ap.add_argument('func')
    ap.add_argument('--dry-run', action='store_true')
    ap.add_argument('--iso-check', dest='iso', action='store_true',
                    default=None,
                    help='force the reaching-def screen in --dry-run mode '
                         '(costs 4 extra compiles)')
    ap.add_argument('--no-iso-check', dest='iso', action='store_false',
                    help='TRIAGE ONLY: skip the reaching-def screen. The '
                         'free provable screens (nop / branch-field / $at / '
                         'reloc pairing) always run.')
    args = ap.parse_args()
    if args.iso is None:
        args.iso = not args.dry_run

    keep = tempfile.mkdtemp(prefix='genreg_')
    out = subprocess.run(
        ['python3', str(REPO / 'tools/overlay/test_overlay_match.py'),
         args.overlay, args.func, '-v', '--keep', keep],
        capture_output=True, text=True, cwd=REPO).stdout

    m = re.search(r'Masked:\s+\d+/\d+ \([\d.]+%\)', out)
    fto = func_text_offset(keep, args.overlay, args.func)
    words = {}
    patches, other = classify(out, reloc_offsets(keep, args.overlay, args.func),
                              fto=fto, words=words)
    print('%s NREG=%d NOTHER=%d' % (m.group(0) if m else '?', len(patches), len(other)))
    for o in other[:30]:
        print('  OTHER [%4d] target=%s compiled=%s' % o)

    if 'POSTPROC CHAIN FAILED' in out:
        print('ERROR: the PostProc chain is broken (see test output) — patches '
              'computed against a partial object would be garbage. Fix the '
              'chain first.', file=sys.stderr)
        sys.exit(1)

    obj = Path(keep) / args.overlay / args.func / (args.func + '.o')

    if args.dry_run:
        if patches:
            patches, ref1 = screen_provable(patches, words)
            patches, ref2, note = screen_reloc_pairing(obj, patches)
            if note:
                print('  note: ' + note)
            report_refusals(ref1 + ref2)
            if ref1 or ref2:
                print('NREG_EMITTABLE=%d (of %d)'
                      % (len(patches), len(patches) + len(ref1) + len(ref2)))
        return
    if not patches:
        return

    path = REPO / 'src' / 'overlay' / args.overlay / (args.func + '.c')
    src = path.read_text()

    # Dedupe (D2): running 3x used to append duplicate triples, and the
    # second application then failed ("field rs is X, expected Y"), which by
    # D1 aborts the whole chain. Never append a triple already present in an
    # existing paint directive.
    existing = set()
    for pl in re.finditer(r'@PostProc:\s*patch_insn_reg_field((?:\s+0x[0-9a-f]+:r[sdt]:\w+:\w+)+)', src):
        existing.update(pl.group(1).split())
    patches = [p for p in patches if p not in existing]
    if not patches:
        print('all %d patches already present — nothing to append' % len(existing))
        return

    # ---------------- EMIT-TIME REFUSALS -------------------------------
    refused = []
    patches, ref = screen_provable(patches, words)
    refused += ref
    patches, ref, note = screen_reloc_pairing(obj, patches)
    refused += ref
    if note:
        print('  note: ' + note)
    if patches and args.iso:
        patches, ref, note = screen_isomorphism(
            args.overlay, args.func, src, patches,
            lambda text, sel: insert_patches(text, sel)[0] or text)
        refused += ref
        if note:
            print('  note: ' + note)
    report_refusals(refused)
    if not patches:
        print('nothing emitted — every generated paint was refused. '
              'This function needs a C fix, not a paint layer.')
        return 1
    if refused:
        print('emitting %d of %d generated paints' % (len(patches),
                                                      len(patches) + len(refused)))

    src2, msg = insert_patches(src, patches)
    if src2 is None:
        print(msg, file=sys.stderr)
        sys.exit(1)
    path.write_text(src2)
    print(msg % (len(patches), path))
    return 0


def insert_patches(src, patches):
    """Return (new_src, message_format) or (None, error_message).

    Splitting this out lets the isomorphism screen build the exact candidate
    source text the emit path would write.
    """
    joined = ' '.join(patches)

    # Safety: patches are computed against the FULL pipeline output, so the
    # new patch line must come AFTER every existing @PostProc step. If any
    # non-patch step follows the scheduler line (multi-step chains:
    # swap_registers, pad_trailing_nop after scheduler, ...), append after
    # the LAST @PostProc line instead of after the scheduler
    # (2026-06-12 sweep regression: wrong insertion order broke 106 files).
    # Directive-line check only (D2): the old `'patch_insn_reg_field' not in
    # src` bailed when the string appeared anywhere, including prose comments.
    has_paint_line = bool(re.search(
        r'(?m)^\s*(?:\*|/\*)\s*@PostProc:\s*patch_insn_reg_field', src))
    all_pp = list(re.finditer(r'(?m)^(?: \* @PostProc: [^\n]+|/\* @PostProc: [^\n]*\*/)$', src))
    if all_pp and not has_paint_line:
        last = all_pp[-1].group(0)
        if 'scheduler_normalizer' not in last:
            style_block = last.startswith(' *')
            new_line = (' * @PostProc: patch_insn_reg_field ' + joined
                        if style_block else
                        '/* @PostProc: patch_insn_reg_field ' + joined + ' */')
            src = src.replace(last, last + '\n' + new_line, 1)
            return src, 'appended %d patches (after last PostProc step) to %s'
    m2 = re.search(r'(?m)^( \* @PostProc: patch_insn_reg_field [^\n]+)$', src)
    m3 = re.search(r'(?m)^(/\* @PostProc: patch_insn_reg_field .+?) \*/$', src)
    if m2:
        src = src.replace(m2.group(1), m2.group(1) + ' ' + joined)
    elif m3:
        src = src.replace(m3.group(0), m3.group(1) + ' ' + joined + ' */')
    elif re.search(r'(?m)^( \* @PostProc: scheduler_normalizer[^\n]*)$', src):
        m4 = re.search(r'(?m)^( \* @PostProc: scheduler_normalizer[^\n]*)$', src)
        src = src.replace(
            m4.group(1) + '\n',
            m4.group(1) + '\n'
            ' * @PostProc: patch_insn_reg_field ' + joined + '\n')
    elif re.search(r'(?m)^(/\* @PostProc: scheduler_normalizer[^\n]*\*/)$', src):
        m4 = re.search(r'(?m)^(/\* @PostProc: scheduler_normalizer[^\n]*\*/)$', src)
        src = src.replace(
            m4.group(1) + '\n',
            m4.group(1) + '\n'
            '/* @PostProc: patch_insn_reg_field ' + joined + ' */\n')
    elif not all_pp:
        # No @PostProc line at all.  This used to be a hard ERROR telling the
        # caller to "add @PostProc lines to the header first", which locked out
        # exactly the STRICT-CLEAN files -- the ones carrying no directives are
        # the ones most worth painting, and there was no directive to anchor to.
        # Create the line after the last header directive, in that line's own
        # comment style (2026-09-20).
        anchor = None
        for m in re.finditer(
                r'(?m)^(?: \* @(?:Compile\w+|Symbol\w+|Compiler|RodataBase|StaticAddr)[^\n]*'
                r'|/\* @(?:Compile\w+|Symbol\w+|Compiler|RodataBase|StaticAddr)[^\n]*\*/)$', src):
            anchor = m
        if anchor is None:
            return None, ('ERROR: no @PostProc and no header directive to anchor '
                          'a new patch line to — add one to the header first')
        line = anchor.group(0)
        new_line = (' * @PostProc: patch_insn_reg_field ' + joined
                    if line.startswith(' *') else
                    '/* @PostProc: patch_insn_reg_field ' + joined + ' */')
        src = src.replace(line + '\n', line + '\n' + new_line + '\n', 1)
        return src, 'created a patch line with %d patches (after the last header directive) in %s'
    else:
        return None, ('ERROR: no scheduler_normalizer line and no existing '
                      'patch line — add @PostProc lines to the header first')
    return src, 'appended %d patches to %s'


if __name__ == '__main__':
    sys.exit(main() or 0)
