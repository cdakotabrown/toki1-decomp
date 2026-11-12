#!/usr/bin/env python3
"""Check every recorded main-binary function size against its real end.

Three recorded sizes were found wrong on 2026-08-29, in BOTH directions, and
each one changed what the function looked like:

  cal_sprite_init       824 recorded, 828 real   -- the missing word was the
                      store in the `jr ra` delay slot, so a perfectly aligned
                      compile read one word short.
  geko_judge 448 recorded, 444 real   -- the extra word pulled the
                      NEXT function's prologue into the compare window.
  _mk_xpndsp       756 recorded, 708 real   -- 48 bytes long.  Measured
                      against 756 it read "189 target vs 178 compiled" and
                      looked 11 words short; against 708 it is COUNT-EXACT.

So "my compile is N words short at the tail" is a window bug until proven
otherwise, and this sweep is how you prove it.

METHOD, and its limits.  For each address-keyed symbol we scan its recorded
window (plus a lookahead) for return sites and take the LAST one that starts
at or before the next recorded symbol.  A return is `jr ra` + its delay slot.
Everything after that, up to the recorded end, is classified:

  PAD      all-zero words  -- benign, IDO pads .text to 16 bytes and a recorded
           size routinely covers that.  Not reported.
  CODE     non-zero words  -- the recorded size runs into something else.

We deliberately do NOT try to decide code-versus-data (CLAUDE.md refutes three
plausible tests for that).  We only measure the distance to a return we can
see, and we report rather than rewrite.  Functions with NO return in range are
reported separately as NO-RETURN: switch-case bodies that `b` to a shared
epilogue, tail-call fragments, and the BIOS thunks that return via `jr t2` all
land there legitimately, so that bucket is a worklist, not a defect list.
"""
import json, os, struct, sys, argparse

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
TEXT_START = 0x80041000
FILE_BASE = 0x800
JR_RA = 0x03E00008


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--lookahead', type=int, default=8,
                    help='bytes past the recorded end to search for a return. '
                         'Keep this TINY. It exists only to catch a recorded '
                         'size that clipped a return\'s delay slot. Widening '
                         'it walks into the NEXT function and invents "too '
                         'short" findings -- at 256 it claimed RotAverageNclip4 '
                         'was 404 bytes when its recorded 176 is correct.')
    ap.add_argument('--only', nargs='*')
    ap.add_argument('--json')
    args = ap.parse_args()

    img = open(os.path.join(ROOT, 'disks/SLPS_000.65'), 'rb').read()

    def word(a):
        o = (a - TEXT_START) + FILE_BASE
        if o < 0 or o + 4 > len(img):
            return None
        return struct.unpack('<I', img[o:o + 4])[0]

    aud = json.load(open(os.path.join(ROOT,
                    'config/main_function_audit_by_addr.json')))['addresses']
    entries = sorted((int(k, 16), k, v) for k, v in aud.items())
    starts = [a for a, _, _ in entries]

    short, long_code, noret = [], [], []
    for i, (a, k, v) in enumerate(entries):
        if args.only and not (set(v.get('aliases', [])) & set(args.only)):
            continue
        rec = v.get('size') or 0
        if not rec:
            continue
        inside = None      # last return ending at or before the recorded end
        just_past = None    # a return ending just past it = clipped delay slot
        p = a
        while p < a + rec + args.lookahead:
            w = word(p)
            if w is None:
                break
            if w == JR_RA and word(p + 4) is not None:
                end = p + 8 - a
                if end <= rec:
                    inside = end
                elif just_past is None and end <= rec + args.lookahead:
                    just_past = end
            p += 4
        if inside == rec:
            continue
        if just_past is not None and inside is None or (
                just_past is not None and inside is not None and inside < rec):
            # a return straddles the recorded end: the size clipped its delay slot
            short.append((k, rec, just_past, v))
            continue
        if inside is None:
            noret.append((k, rec, v))
            continue
        tail = [word(x) for x in range(a + inside, a + rec, 4)]
        if any(tail):
            long_code.append((k, rec, inside, v))

    print(f"RECORDED TOO SHORT ({len(short)}) -- a return lies past the recorded end")
    for k, rec, true, v in short:
        print(f"  {k}  recorded={rec:<6} real={true:<6} (+{true-rec})  {v.get('kind'):<12} {(v.get('aliases') or [''])[0]}")
    print(f"\nRECORDED TOO LONG, tail is CODE ({len(long_code)}) -- the window covers something else")
    for k, rec, true, v in long_code:
        print(f"  {k}  recorded={rec:<6} real={true:<6} ({true-rec})  {v.get('kind'):<12} {(v.get('aliases') or [''])[0]}")
    print(f"\nNO RETURN IN RANGE ({len(noret)}) -- worklist, not a defect list")
    for k, rec, v in noret[:40]:
        print(f"  {k}  recorded={rec:<6} {v.get('kind'):<12} {(v.get('aliases') or [''])[0]}")
    if len(noret) > 40:
        print(f"  ... and {len(noret)-40} more")

    if args.json:
        json.dump({'short': [(k, rec, true) for k, rec, true, _ in short],
                   'long_code': [(k, rec, true) for k, rec, true, _ in long_code],
                   'no_return': [k for k, _, _ in noret]},
                  open(args.json, 'w'), indent=2)
    return 0


if __name__ == '__main__':
    sys.exit(main())
