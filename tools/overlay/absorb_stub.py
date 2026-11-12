#!/usr/bin/env python3
"""Absorb a fake cross-prologue-head stub into its host function.

Background
----------
Many overlay `.sym` files register the 2-word cross-prologue head that IDO -O2
hoists above a function's prologue (`lui $rX,hi; l{w,h,bu} $rX,lo($rX)`) as its
own symbol.  Those symbols have no `jr ra` and cannot be a function.  The `.c`
files written for them return the loaded global (4 insns) and then use
`strip_trailing_instructions --count 2` to chop off the `jr ra; nop` that the
target never had -- fabrication that counted as a byte-perfect win.

The honest fix is to absorb the head into the host function that follows it:
the host's C reads the global first, IDO -O2 hoists that load above the
prologue on its own, and the enlarged compare window covers the head.

This tool does the MECHANICAL half (sym rewrite, compare-window annotation,
PostProc offset shift, backup/rollback, verification).  The C-level half --
replacing the host's hidden parameter with a read of the global -- is a
judgement call and is left to the caller.

Usage
-----
    absorb_stub.py OVERLAY STUB_FUNC            # show the plan, change nothing
    absorb_stub.py OVERLAY STUB_FUNC --apply    # rewrite .sym + host window
    absorb_stub.py OVERLAY STUB_FUNC --verify   # resolver check (strict)
    absorb_stub.py OVERLAY STUB_FUNC --revert   # restore from backup

`--apply` refuses to run unless the host currently verifies, so a host that is
already broken is never silently blamed on the absorption.
"""

import argparse
import os
import re
import shutil
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
BACKUP_DIR = os.path.join(ROOT, 'build', 'absorb_backups')

sys.path.insert(0, os.path.join(ROOT, 'tools', 'build'))
from parse_postproc import (parse_symbol_start_text,  # noqa: E402
                            parse_symbol_size_text)

OFFSET_RE = re.compile(r'0x([0-9a-fA-F]+):(rt|rs|rd):')


def sym_path(overlay):
    return os.path.join(ROOT, 'config', 'overlay', '%s.sym' % overlay)


def c_path(overlay, func):
    return os.path.join(ROOT, 'src', 'overlay', overlay, '%s.c' % func)


def read_sym(overlay):
    """Return [(addr, size, name, raw_line_index)] sorted by address."""
    entries = []
    lines = open(sym_path(overlay)).readlines()
    for i, line in enumerate(lines):
        stripped = line.split('#')[0].strip()
        parts = stripped.split(':')
        if len(parts) < 3:
            continue
        try:
            entries.append((int(parts[0], 16), int(parts[1], 0), parts[2], i))
        except ValueError:
            continue
    entries.sort()
    return entries, lines


def find_pair(overlay, stub):
    """Locate the stub entry and the host entry that immediately follows it."""
    entries, lines = read_sym(overlay)
    for idx, (addr, size, name, li) in enumerate(entries):
        if name != stub:
            continue
        if size > 16:
            sys.exit('%s is %d bytes -- not a 2-word head, refusing' % (stub, size))
        if idx + 1 >= len(entries):
            sys.exit('%s has no following symbol to absorb into' % stub)
        haddr, hsize, hname, hli = entries[idx + 1]
        if haddr != addr + size:
            sys.exit('gap between %s (ends 0x%08X) and %s (0x%08X) -- not contiguous'
                     % (stub, addr + size, hname, haddr))
        return (addr, size, name, li), (haddr, hsize, hname, hli), lines
    sys.exit('%s not found in %s.sym' % (stub, overlay))


def shift_postproc_offsets(text, delta):
    """Shift every .text-relative PostProc byte offset by delta.

    The body moves `delta` bytes earlier in the compare window, so every
    instruction offset within that window grows by `delta`.

    Two syntaxes carry instruction offsets and BOTH must move:
      * `0xNN:field:old:new`            (patch_insn_reg_field, ...)
      * `--offset/--offset1/--offset2`  (swap_instructions,
                                         swap_spill_with_move, nop_at_offset,
                                         hoist_out_of_delay_slot, ...)

    Deliberately NOT shifted, because they are not .text offsets:
      * `patch_stack_complete --old/--new`  (frame sizes)
      * `remap_stack_slots --from/--to`     (sp-relative slots)
      * `--count`                           (instruction counts)

    `--offsets`, `--exclude-offsets` and `--address` need per-directive unit
    knowledge and are handled by `shift_ambiguous_offsets` (call it after this).
    """
    def bump_colon(m):
        return '0x%x:%s:' % (int(m.group(1), 16) + delta, m.group(2))
    text = OFFSET_RE.sub(bump_colon, text)

    def bump_flag(m):
        value = int(m.group('value'), 0) + delta
        fmt = '0x%x' if m.group('value').lower().startswith('0x') else '%d'
        return '%s%s' % (m.group('flag'), fmt % value)
    text = re.sub(r'(?P<flag>--offset[12]?[= ])(?P<value>0[xX][0-9a-fA-F]+|\d+)',
                  bump_flag, text)

    # patch_insn_imm16 uses `0xNN:field:value` with fields that are not
    # rt/rs/rd, so OFFSET_RE misses it. Shift offsets on those lines only.
    def bump_imm16_line(m):
        # `patch_insn_imm16 OFF:FROM:TO ...` -- ONLY the leading OFF field is a
        # .text offset.  FROM/TO are immediate VALUES and are frequently written
        # in hex (`0x10:0xffe8:0xffd8`), so a blanket `0xNN:` substitution
        # silently corrupts the from-value and the directive then no-ops or
        # mis-fires.  Anchor on a token boundary and rewrite the first field only.
        return re.sub(r'(?<![:\w])0x([0-9a-fA-F]+)(?=:)',
                      lambda o: '0x%x' % (int(o.group(1), 16) + delta), m.group(0))
    return re.sub(r'patch_insn_imm16[^\n]*', bump_imm16_line, text)


# `--offsets` carries WORD indices (shift delta/4) only for these directives;
# every other .text `--offsets` consumer takes BYTE offsets (shift delta).
# Determined by reading each tool: convert_ori_to_addiu keys on `word_idx =
# i // 4`; invert_branch / swap_branch_operands / cse_lui_at / strip_arg_homes
# / strip_instructions / patch_load_signedness / strip_lw_ra_from_branch_delays
# all take 4-byte-aligned `.text` byte offsets.
_WORD_INDEX_OFFSETS_DIRECTIVES = frozenset({'convert_ori_to_addiu'})
_BYTE_OFFSETS_DIRECTIVES = frozenset({
    'invert_branch', 'swap_branch_operands', 'cse_lui_at', 'strip_arg_homes',
    'strip_instructions', 'patch_load_signedness',
    'strip_lw_ra_from_branch_delays',
})

_POSTPROC_LINE_RE = re.compile(r'^\s*(?:(?:/\*+|\*+|//)\s*)*@PostProc:\s*(.*)$')
# A run of hex/decimal numeric tokens, comma- or space-separated. Stops at the
# next `--flag` or any non-numeric token, so trailing flags (`--sp-offset`,
# `--allow-uncond`, ...) are left untouched.
_NUM_LIST = r'(?:0x[0-9a-fA-F]+|\d+)(?:[,\s]+(?:0x[0-9a-fA-F]+|\d+))*'


def _first_token(body):
    m = re.match(r'\s*([A-Za-z_]\w*)', body)
    return m.group(1) if m else ''


def _shift_num_list(list_text, per):
    """Add `per` to every numeric token in `list_text`, preserving hex/dec form
    and the original separators."""
    def one(m):
        raw = m.group(0)
        val = int(raw, 0) + per
        return ('0x%x' % val) if raw.lower().startswith('0x') else str(val)
    return re.sub(r'0x[0-9a-fA-F]+|\d+', one, list_text)


def shift_ambiguous_offsets(text, delta):
    """Shift `--offsets`, `--exclude-offsets` and `--address` on @PostProc lines.

    The absorbed head widens the compare window by `delta` bytes at the FRONT,
    so:
      * `--offsets`         byte offsets grow by `delta`; word indices
                            (convert_ori_to_addiu) grow by `delta // 4`.
      * `--exclude-offsets` byte `.text` offsets (swap_registers /
                            convert_or_to_addu / rename_register_chain) grow by
                            `delta`.
      * `--address`         the function/window START the tool reads its
                            reference schedule from; the symbol now starts
                            `delta` bytes EARLIER, so it shrinks by `delta`.
                            (`--base-address` is the overlay base and is left
                            alone.)

    Stack/frame flags (`--old`, `--new`, `--count`, `--from`, `--to`,
    `--sp-offset`, `--range`) are never touched. Returns (new_text, notes).
    """
    notes = []
    out = []
    for line in text.splitlines(keepends=True):
        m = _POSTPROC_LINE_RE.match(line)
        if not m:
            out.append(line)
            continue
        directive = _first_token(m.group(1))
        new_line = line

        if '--offsets' in new_line:
            if directive in _WORD_INDEX_OFFSETS_DIRECTIVES:
                per, unit = delta // 4, 'word idx +%d' % (delta // 4)
            else:
                per, unit = delta, 'byte +%d' % delta
                if directive not in _BYTE_OFFSETS_DIRECTIVES:
                    notes.append('WARNING: `--offsets` on unrecognised directive '
                                 '`%s` -- ASSUMED byte offsets (+%d); verify the '
                                 'tool unit.' % (directive, delta))

            def _off(mo, _per=per):
                return mo.group(1) + _shift_num_list(mo.group(2), _per)
            new_line, n = re.subn(r'(--offsets[=\s]+)(' + _NUM_LIST + ')',
                                  _off, new_line)
            if n:
                notes.append('%s --offsets shifted (%s)' % (directive, unit))

        if '--exclude-offsets' in new_line:
            def _excl(mo):
                return mo.group(1) + _shift_num_list(mo.group(2), delta)
            new_line, n = re.subn(r'(--exclude-offsets[=\s]+)(' + _NUM_LIST + ')',
                                  _excl, new_line)
            if n:
                notes.append('%s --exclude-offsets shifted (byte +%d)'
                             % (directive, delta))

        if re.search(r'--address\b', new_line):
            def _addr(mo):
                raw = mo.group(2)
                val = int(raw, 0) - delta
                fmt = '0x%x' if raw.lower().startswith('0x') else '%d'
                return mo.group(1) + (fmt % val)
            new_line, n = re.subn(r'(--address[=\s]+)(0x[0-9a-fA-F]+|\d+)',
                                  _addr, new_line)
            if n:
                notes.append('%s --address shifted (-%d, window starts earlier)'
                             % (directive, delta))

        out.append(new_line)
    return ''.join(out), notes


def set_annotation(text, key, value):
    """Insert or replace a `@Key: value` header annotation.

    An existing annotation is rewritten IN PLACE, preserving whatever comment
    prefix it already had.  Substituting a standalone `/* ... */` line for a
    ` * @Key:` line inside a block comment would terminate that block early and
    turn the remaining prose into code -- C block comments do not nest.
    """
    pattern = re.compile(r'^(?P<prefix>\s*(?:/\*+|\*)?\s*)@%s:[^\n*]*(?P<suffix>\*/)?\s*$'
                         % key, re.MULTILINE)
    match = pattern.search(text)
    if match:
        prefix = match.group('prefix')
        suffix = ' */' if match.group('suffix') else ''
        return text[:match.start()] + '%s@%s: %s%s' % (prefix, key, value, suffix) + \
            text[match.end():]
    return '/* @%s: %s */\n' % (key, value) + text


def backup(overlay, host):
    """Snapshot the host .c only.

    The `.sym` is deliberately NOT snapshotted wholesale: absorptions are run
    in batches over one overlay, and restoring a whole-file snapshot would
    silently undo every other absorption applied since it was taken.  The sym
    is reverted surgically instead (see `revert`), and it is git-tracked, so
    `git diff config/overlay/<OV>.sym` is always available as a backstop.
    """
    os.makedirs(BACKUP_DIR, exist_ok=True)
    src = c_path(overlay, host)
    dst = os.path.join(BACKUP_DIR, '%s.%s.c' % (overlay, host))
    if os.path.exists(src) and not os.path.exists(dst):
        shutil.copy2(src, dst)


def revert(overlay, stub):
    """Undo one absorption: restore the host .c and re-split the sym entry.

    Works even though the stub's sym entry is already gone -- the stub address
    and size are recovered from the widened host entry and the stub's `.s`.
    """
    asm = os.path.join(ROOT, 'asm', 'overlay', overlay, '%s.s' % stub)
    saddr = int(stub.replace('func_', ''), 16)
    entries, lines = read_sym(overlay)
    host = None
    for addr, size, name, li in entries:
        if addr == saddr and name != stub:
            host = (addr, size, name, li)
            break
    if host is None:
        sys.exit('no widened entry at 0x%08X -- nothing to revert' % saddr)
    haddr, hsize, hname, hli = host

    ssize = 8
    if os.path.exists(asm):
        insns = [l for l in open(asm) if re.match(r'^\s+[a-z]', l)]
        ssize = 4 * len(insns) or 8

    out = []
    for i, line in enumerate(lines):
        if i == hli:
            out.append('0x%08X:%d:%s\n' % (saddr, ssize, stub))
            out.append('0x%08X:%d:%s\n' % (saddr + ssize, hsize - ssize, hname))
            continue
        out.append(line)
    open(sym_path(overlay), 'w').writelines(out)

    src = os.path.join(BACKUP_DIR, '%s.%s.c' % (overlay, hname))
    if os.path.exists(src):
        shutil.copy2(src, c_path(overlay, hname))
        print('restored %s' % c_path(overlay, hname))
    print('re-split sym: 0x%08X:%d:%s + 0x%08X:%d:%s'
          % (saddr, ssize, stub, saddr + ssize, hsize - ssize, hname))


def verify(overlay, func, strict=True):
    """Run the name-derived resolver. Returns (ok, summary_line).

    A match is only accepted when the compiled length equals the target
    length -- the resolver compares up to the shorter of the two, so a short
    compile otherwise reports "N/N words exact" while leaving the tail of the
    window silently unchecked.
    """
    proc = subprocess.run(
        [sys.executable, os.path.join(ROOT, 'tools', 'overlay', 'resolve_overlay_relocs.py'),
         overlay, func, '-v'],
        capture_output=True, text=True, cwd=ROOT)
    out = proc.stdout + proc.stderr
    perfect = 'BYTE-PERFECT' in out
    lens = re.search(r'len t=(\d+) c=(\d+)', out)
    full = bool(lens) and lens.group(1) == lens.group(2)
    summary = ''
    for line in out.splitlines():
        if 'words exact' in line or 'BYTE-PERFECT' in line:
            summary = line.strip()
    if lens and not full:
        summary += '  [TRUNCATED COMPARE: target %s B vs compiled %s B]' % lens.groups()
    return (perfect and (full or not strict)), summary


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('overlay')
    ap.add_argument('stub')
    ap.add_argument('--apply', action='store_true', help='rewrite .sym and host window')
    ap.add_argument('--verify', action='store_true', help='strict resolver check of the host')
    ap.add_argument('--revert', action='store_true', help='restore host .c and .sym from backup')
    args = ap.parse_args()

    # --revert and --verify must run BEFORE find_pair: once an absorption is
    # applied the stub's sym entry is gone, which is exactly when they are
    # wanted.  Both resolve the host from the widened entry instead.
    if args.revert:
        revert(args.overlay, args.stub)
        return

    if args.verify:
        saddr = int(args.stub.replace('func_', ''), 16)
        entries, _ = read_sym(args.overlay)
        host = next((n for a, s_, n, _li in entries
                     if a == saddr and n != args.stub), None)
        if host is None:
            host = next((n for a, s_, n, _li in entries
                         if a > saddr and n != args.stub), None)
        if host is None:
            sys.exit('cannot resolve a host for %s' % args.stub)
        ok, summary = verify(args.overlay, host)
        print('%s %s  (%s)' % ('OK  ' if ok else 'FAIL', summary, host))
        sys.exit(0 if ok else 1)

    (saddr, ssize, sname, sli), (haddr, hsize, hname, hli), lines = find_pair(
        args.overlay, args.stub)

    # Some host windows ALREADY over-count into the following head (the split
    # that created them moved the start but not the size).  Widening such a
    # host by the stub size would double-count those bytes and overlap the next
    # symbol, so the correct move there is start -= ssize with size unchanged.
    entries, _ = read_sym(args.overlay)
    following = [a for a, s, n, li in entries if a > haddr]
    overlap = 0
    if following:
        overlap = max(0, (haddr + hsize) - min(following))
    new_size = hsize + ssize - min(overlap, ssize)

    print('stub  %s  0x%08X  %d bytes' % (sname, saddr, ssize))
    print('host  %s  0x%08X  %d bytes  ->  0x%08X  %d bytes'
          % (hname, haddr, hsize, saddr, new_size))
    if overlap:
        print('note: host window already over-counted %d bytes into the next '
              'symbol; size kept at %d rather than %d'
              % (overlap, new_size, hsize + ssize))

    if not args.apply:
        print('\n(dry run -- pass --apply to write)')
        return

    host_c = c_path(args.overlay, hname)
    if not os.path.exists(host_c):
        sys.exit('host %s has no .c (asm-only) -- absorb by hand or use the '
                 '@AsmOnly fallback on the stub' % hname)

    ok_before, before = verify(args.overlay, hname)
    print('\nhost before: %s' % before)

    backup(args.overlay, hname)

    # 1. sym: drop the stub entry, move the host entry back over the head.
    out = []
    for i, line in enumerate(lines):
        if i == sli:
            continue
        if i == hli:
            out.append('0x%08X:%d:%s\n' % (saddr, new_size, hname))
            continue
        out.append(line)
    open(sym_path(args.overlay), 'w').writelines(out)

    # 2. host .c: widen the compare window and shift PostProc offsets.
    text = open(host_c).read()

    # A host may ALREADY have absorbed its head in an earlier session, with the
    # stub left registered alongside it as a duplicate.  Its offsets are then
    # already correct; re-shifting them corrupts a working file.  There the sym
    # edit above is the entire fix.
    already = parse_symbol_start_text(text)
    if already is not None and already == saddr:
        print('host already declares @SymbolStart 0x%08X -- sym-only fix, .c '
              'left untouched' % saddr)
        ok_after, after = verify(args.overlay, hname)
        print('host after:  %s' % after)
        if ok_after:
            print('\nDelete the duplicate stub: rm %s' % c_path(args.overlay, sname))
        return

    text = shift_postproc_offsets(text, ssize)
    text, amb_notes = shift_ambiguous_offsets(text, ssize)
    for note in amb_notes:
        print(note)

    # The .c's @SymbolSize -- not the .sym entry -- drives the compare window,
    # and many hosts deliberately declare a SMALLER window than their sym entry
    # (trimming the following function's head back out).  Widen whatever the .c
    # already declared; falling back to the sym-derived size would silently
    # over-run those windows.
    declared = parse_symbol_size_text(text)
    window = (declared + ssize) if declared is not None else new_size
    if declared is not None and window != new_size:
        print('note: .c declared @SymbolSize %d (not the sym size %d); window '
              '-> %d' % (declared, hsize, window))
    text = set_annotation(text, 'SymbolSize', str(window))
    text = set_annotation(text, 'SymbolStart', '0x%08X' % saddr)
    open(host_c, 'w').write(text)

    print('sym rewritten; window now %d bytes; PostProc offsets +%d'
          % (new_size, ssize))

    ok_after, after = verify(args.overlay, hname)
    print('host after:  %s' % after)

    if ok_after:
        print('\nBYTE-PERFECT over the widened window.')
        print('Delete the stub: rm %s' % c_path(args.overlay, sname))
    else:
        print('\nNot byte-perfect yet -- the C still needs the head.')
        print('Read %s for the global it loads, then make the host read that'
              % os.path.join('asm', 'overlay', args.overlay, '%s.s' % sname))
        print('global FIRST (drop the hidden param) so IDO -O2 hoists the load')
        print('above the prologue. Trailing zero words in the window are real')
        print('nops -- pad_trailing_nop is legitimate ONLY once the body matches.')
        print('To undo: %s %s %s --revert' % (sys.argv[0], args.overlay, args.stub))


if __name__ == '__main__':
    main()
