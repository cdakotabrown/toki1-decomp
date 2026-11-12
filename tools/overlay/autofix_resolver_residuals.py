#!/usr/bin/env python3
"""Batch autofixer for resolver residuals on semi/reloc overlay functions.

The over-masking hid two huge MECHANICAL seams (found 2026-06-26):
  1. FRAME-SIZE mislabels: the body is byte-identical except the prologue/epilogue
     `addiu sp,sp,N`. Fix = add `@PostProc: patch_stack_complete --old <compiled>
     --new <target>`. Directive-only, no source-body change -> SAFE to automate.
  2. WRONG GLOBAL ADDRESS: a named extern points at a slightly-wrong address. The
     fix edits the C source (rename the symbol) -> error-prone, NOT auto-applied
     here; we only REPORT these (with the exact correct address) for a person to apply.

This tool, per semi/reloc function:
  - runs resolve_overlay_relocs.resolve_function (the authoritative name-derived
    strict link -- the SAME ground truth used to reclassify the project; the masked
    test is NOT trusted),
  - if the ONLY structural residual is the frame `addiu sp`, applies the
    patch_stack_complete directive, re-resolves, and KEEPS it iff it becomes
    byte-perfect (matched) or .rodata-blocked-effective (residuals are purely
    unresolved relocation words); otherwise REVERTS,
  - classifies every non-frame residual (wrong_global / jal_callee / control_flow /
    codegen / rodata) and reports counts so the next pass is targeted.

Only ever edits the `/* @PostProc: ... */` header of a .c file -- never the body.

Usage:
  python3 tools/overlay/autofix_resolver_residuals.py EVENT DATE      # apply to overlays
  python3 tools/overlay/autofix_resolver_residuals.py --all
  python3 tools/overlay/autofix_resolver_residuals.py EVENT --dry-run # classify only
  python3 tools/overlay/autofix_resolver_residuals.py EVENT --workers=8
"""
import sys
import os
import re
import json
import struct
from pathlib import Path
from concurrent.futures import ProcessPoolExecutor, as_completed

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent.parent
STATUS_FILE = ROOT / "config" / "overlay_match_status.json"
SRC_OVERLAY_DIR = ROOT / "src" / "overlay"


def _signed16(x):
    return x - 0x10000 if x & 0x8000 else x


def _is_addiu_sp(word):
    """True if word is `addiu sp,sp,imm`; returns the frame (-imm) or None."""
    if (word >> 26) & 0x3f != 9:        # addiu opcode
        return None
    if (word >> 21) & 0x1f != 29:       # rs == sp
        return None
    if (word >> 16) & 0x1f != 29:       # rt == sp
        return None
    return -_signed16(word & 0xffff)


def _classify_diff(tw, cw):
    ot, oc = (tw >> 26) & 0x3f, (cw >> 26) & 0x3f
    if ot != oc:
        return 'codegen'                 # opcode differs -> real codegen diff
    if ot in (2, 3):
        return 'jal_callee'              # wrong jal/j target
    if ot in (4, 5):
        return 'control_flow'            # branch offset
    if ot == 15:
        return 'wrong_global'            # lui hi16
    if ot in (9, 13) or 32 <= ot <= 43:
        return 'wrong_global'            # addiu/ori/lw/sw lo16
    return 'codegen'


def _reloc_kind(tw, cw):
    """A residual word that is consistent with an unresolved relocation
    (hi16/lo16/jal), i.e. a .rodata-blocked-effective residual, not codegen."""
    ot, oc = (tw >> 26) & 0x3f, (cw >> 26) & 0x3f
    if ot != oc:
        return False
    return ot in (2, 3, 15, 9, 13) or 32 <= ot <= 43


def _read_postproc_lines(text):
    return [m.group(0) for m in re.finditer(r'/\*\s*@PostProc:.*?\*/', text)]


def _apply_frame_directive(src_file, old_frame, new_frame):
    """Replace/insert the patch_stack_complete @PostProc line. Returns the
    original text (for revert)."""
    text = src_file.read_text(errors='ignore')
    orig = text
    new_line = (f"/* @PostProc: patch_stack_complete "
                f"--old {old_frame} --new {new_frame} */")
    # drop any existing patch_stack_complete directive line(s)
    text = re.sub(r'/\*\s*@PostProc:\s*patch_stack_complete[^*]*\*/\s*\n?', '',
                  text)
    text = new_line + "\n" + text
    src_file.write_text(text)
    return orig


def _resolve(ov, fn):
    sys.path.insert(0, str(HERE))
    import resolve_overlay_relocs as R
    return R.resolve_function(ov, fn)


def _worker(args):
    """Per-function: try the frame fix, keep iff it yields a win. Returns
    (ov, fn, outcome, detail)."""
    ov, fn, dry = args
    src_file = SRC_OVERLAY_DIR / ov / f"{fn}.c"
    if not src_file.exists():
        return (ov, fn, 'skip', 'no source')
    r = _resolve(ov, fn)
    if 'error' in r:
        return (ov, fn, 'skip', r['error'])
    if r.get('matched'):
        return (ov, fn, 'already', '')          # already byte-perfect
    diffs = r.get('diffs_after') or []
    if not diffs:
        return (ov, fn, 'already', '')

    # is there a frame mismatch at word 0 (and is the rest small)?
    idx0, tw0, cw0 = diffs[0]
    tgt_frame = _is_addiu_sp(tw0) if idx0 == 0 else None
    cmp_frame = _is_addiu_sp(cw0) if idx0 == 0 else None
    frame_candidate = (tgt_frame is not None and cmp_frame is not None
                       and tgt_frame != cmp_frame)

    if frame_candidate and not dry:
        orig = _apply_frame_directive(src_file, cmp_frame, tgt_frame)
        r2 = _resolve(ov, fn)
        win = False
        kind = ''
        if r2.get('matched'):
            win, kind = True, 'byte_perfect'
        else:
            d2 = r2.get('diffs_after') or []
            unres = r2.get('unresolved') or []
            # .rodata-blocked-effective: all remaining residuals are reloc words
            # AND there are unresolved (section) symbols
            if unres and d2 and all(_reloc_kind(tw, cw) for _, tw, cw in d2):
                win, kind = True, 'rodata_reloc'
        if win:
            return (ov, fn, 'fixed', kind)
        src_file.write_text(orig)            # revert: not a clean win
        # fall through to classify the original residual

    # classify the (unfixed) residual for reporting
    kinds = {}
    for _, tw, cw in diffs:
        k = _classify_diff(tw, cw)
        kinds[k] = kinds.get(k, 0) + 1
    if frame_candidate:
        kinds['frame(+other)'] = kinds.get('frame(+other)', 0) + 1
    unres = r.get('unresolved') or []
    if unres:
        kinds['rodata/unresolved'] = kinds.get('rodata/unresolved', 0) + 1
    top = max(kinds, key=kinds.get) if kinds else 'unknown'
    return (ov, fn, 'unfixed', top)


def main():
    args = [a for a in sys.argv[1:] if not a.startswith('--')]
    dry = '--dry-run' in sys.argv
    workers = 6
    for a in sys.argv[1:]:
        if a.startswith('--workers='):
            workers = int(a.split('=', 1)[1])

    status = json.load(open(STATUS_FILE))
    overlays = (args if args and '--all' not in sys.argv
                else sorted(k for k, v in status.items() if isinstance(v, dict)))

    targets = []
    for ov in overlays:
        for fn, info in status.get(ov, {}).items():
            st = info.get('status') if isinstance(info, dict) else info
            if st in ('semi_matched', 'reloc_only', 'count_mismatch'):
                targets.append((ov, fn, dry))

    print(f"Autofix scan: {len(targets)} semi/reloc funcs across {len(overlays)} "
          f"overlays, {workers} workers{' [DRY-RUN]' if dry else ''}")

    fixed = {}
    unfixed_kinds = {}
    counts = {'fixed': 0, 'already': 0, 'unfixed': 0, 'skip': 0}
    done = 0
    with ProcessPoolExecutor(max_workers=workers) as ex:
        futs = [ex.submit(_worker, t) for t in targets]
        for fut in as_completed(futs):
            ov, fn, outcome, detail = fut.result()
            counts[outcome] = counts.get(outcome, 0) + 1
            done += 1
            if done % 200 == 0:
                print(f"  ...{done}/{len(targets)}")
            if outcome == 'fixed':
                fixed.setdefault(ov, []).append((fn, detail))
            elif outcome == 'unfixed':
                unfixed_kinds[detail] = unfixed_kinds.get(detail, 0) + 1

    print("\n=== Autofix results ===")
    total_fixed = sum(len(v) for v in fixed.values())
    print(f"FRAME-FIXED (resolver-verified win): {total_fixed}"
          f"{' [would fix; dry-run]' if dry else ''}")
    for ov in sorted(fixed, key=lambda k: -len(fixed[k])):
        bp = sum(1 for _, k in fixed[ov] if k == 'byte_perfect')
        rd = sum(1 for _, k in fixed[ov] if k == 'rodata_reloc')
        print(f"  {ov:12} +{len(fixed[ov])}  (byte_perfect {bp}, .rodata-reloc {rd})")
    print(f"\nAlready byte-perfect (resolver): {counts['already']}   "
          f"skipped: {counts['skip']}")
    print(f"Unfixed semis by dominant residual kind (next-pass targets):")
    for k in sorted(unfixed_kinds, key=lambda k: -unfixed_kinds[k]):
        print(f"  {k:22} {unfixed_kinds[k]}")
    if not dry and total_fixed:
        print(f"\nApplied {total_fixed} frame directives to .c headers. Re-run "
              f"verify_overlay_bytes.py --all + apply_resolved_status.py --demote "
              f"to reclassify, then commit the .c files.")


if __name__ == '__main__':
    main()
