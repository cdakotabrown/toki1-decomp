#!/usr/bin/env python3
"""RAW instruction-count self-audit for an overlay function.

    python3 tools/overlay/audit_raw_count.py DATE func_80134CF4

Strips EVERY `@PostProc` line from the `.c` (keeping `@CompileEnv`,
`@CompilerFlags`, `@Compiler`, `@SymbolStart`, `@SymbolSize`), compiles with
the file's own CompileEnv, and compares the RAW compiled size against the
target window.

WHY THIS EXISTS: the resolver slices the compiled object to `func_size`, so
`len t=X c=X` is satisfied BY CONSTRUCTION and cannot prove the C emitted the
right number of instructions.  A body that is short by N words scores its
prefix and never looks at the tail; a body padded with `pad_trailing_nop`
fakes the count.  Only a raw, PostProc-free count settles it.  A win requires
this audit to PASS.

=============================================================================
FALSE-POSITIVE MODES (both cost real time before being pinned down)
=============================================================================
1. ⭐ SECTION SIZE vs SYMBOL SIZE.  Read the **ELF SYMBOL size** for the
   function, never the `.text` SECTION size.  Section zero-padding /
   alignment slack MASKS a deficit: EN_NICHI/func_801376AC looked
   count-exact by section and was 3 words SHORT by symbol
   (2026-08-09).  This tool reads the symbol size and reports the
   section size separately, for information only.

2. ⭐ ALLOWED COUNT-CHANGING STEPS MUST BE KEPT (2026-08-09).  A
   naive strip-everything read gives FALSE FAILs on files whose chain
   contains a *removal-only* allowed step — the `cse_lui_at` class, which
   deletes a redundant `lui` the compiler legitimately emits twice.  Measured
   case: raw 293 words vs target 291 = "FAIL, +2" when the object is in fact
   correct.  Use `--keep cse_lui_at` (or `--keep-count-changing`) on those
   files and read the delta against the KEPT chain.
   ⚠️ Corollary: `cse_lui_at` offsets are **CompileEnv-coupled**.  Without the
   `@CompileEnv` the step silently no-ops, producing a phantom 2-word
   surplus that looks exactly like a C bug.  This tool always applies the
   file's own `@CompileEnv`; if you reproduce by hand, do the same.
   ⛔ Never `--keep` a paint (`patch_insn_reg_field`), a `pad_trailing_nop`,
   or a `strip_trailing_instructions` — those are precisely the steps that
   fake a count, and keeping them defeats the audit.

3. `@SymbolSize` beats the `.sym` size.  A windowed function (`@SymbolStart`/
   `@SymbolSize`) is audited against its window, not the whole `.sym` entry.
   If the two disagree wildly, suspect a `.sym` blob that needs splitting.

4. ⭐ SAME-TU LAYOUT (2026-10-03).  One window can hold SEVERAL functions
   from the same translation unit (a split two-function `.sym` entry, a
   dispatcher absorbed with its caller): DATE2/func_801330D4's window is
   948 B = func_801330D4 (376 B) + func_8013324C (572 B).  Reading only the
   named symbol reported delta=-143.  The audit now sums every compiled FUNC
   symbol whose start lies inside the window and measures up to the end of
   the last one.  A body that comes out SHORT lets the next function start
   inside the window, which over-counts and still FAILs -- never a false
   PASS.

5. ⭐ TRAILING ALIGNMENT PADDING (2026-10-03).  A `.sym` size often covers
   ZERO words of inter-function padding after the last `jr ra` + delay slot.
   They are not code, so they are reported as padding on BOTH sides and
   excluded from the count.  Only an all-zero tail after the last
   `jr ra`+slot is padding; a zero IN the delay slot is code, and a window
   with any non-zero word after its last `jr ra`+slot has no padding.

6. The compile is resolve_overlay_relocs.compile_object (the resolver's own),
   so `@Compiler: gcc28x` files are compiled with gcc and `@CompileEnv` /
   `@CompileFlags` / `@SymbolSize` are read from DIRECTIVE lines only (a
   prose mention used to re-window or re-env the audit: RPG_BAT/func_8014E5B4
   read a narrated "@SymbolSize: 304" and reported delta=-38).

`--self-test` runs synthetic fixtures for modes 4 and 5.

A PASS here is necessary, not sufficient: it says the body is the right
LENGTH, not that it is correct.  Pair it with the resolver
(`resolve_overlay_relocs.py`), `audit_paint_bans.py` and
`audit_paint_isomorphism.py`.
"""
import argparse
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools" / "overlay"))
sys.path.insert(0, str(ROOT / "tools" / "build"))
from parse_postproc import parse_symbol_start_text, parse_symbol_size_text  # noqa: E402

# Removal-only allowed steps: keeping them is legitimate (see FP mode 2).
COUNT_CHANGING_ALLOWED = ["cse_lui_at"]


def strip_postproc(text, keep=()):
    """Drop every @PostProc line (preserving block-comment structure), except
    directives naming one of `keep`."""
    out = []
    for ln in text.splitlines():
        if "@PostProc" not in ln or any(k in ln for k in keep):
            out.append(ln)
            continue
        op = ln.lstrip().startswith("/*")
        cl = ln.rstrip().endswith("*/")
        if op and cl:
            continue          # self-contained directive comment: drop whole line
        elif op:
            out.append("/*")
        elif cl:
            out.append(" */")
        else:
            out.append(" *")
    return "\n".join(out) + "\n"


def sym_entry(ov, fn):
    """(address, size) of `fn` in config/overlay/<ov>.sym, or (None, None)."""
    for ln in (ROOT / "config/overlay" / f"{ov}.sym").read_text().splitlines():
        p = ln.strip().split(":")
        if len(p) >= 3 and p[2].strip() == fn:
            return int(p[0], 16), int(p[1], 0)
    return None, None


def sym_addr(ov, fn, text):
    """Target load address: @SymbolStart wins, else the .sym entry."""
    return parse_symbol_start_text(text) or sym_entry(ov, fn)[0]


JR_RA = 0x03E00008


def trailing_pad_words(words):
    """Number of trailing ZERO words that are inter-function alignment
    padding: zeros lying strictly after the last `jr ra` AND its delay slot.

    A zero word IN the delay slot is code (the nop the function executes),
    never padding.  With no `jr ra` at all (a fall-through fragment) nothing
    is padding.  If ANY non-zero word follows the last `jr ra`+slot, the
    words there are code (a trailing branch whose zero delay slot must not be
    mistaken for padding), and nothing is padding."""
    last = None
    for i, w in enumerate(words):
        if w == JR_RA:
            last = i
    if last is None:
        return 0
    tail = words[last + 2:]
    return len(tail) if all(w == 0 for w in tail) else 0


def window_span(funcs, base, size):
    """Functions from one TU that the target window covers.

    `funcs` is [(value, size, name)] for every DEFINED FUNC symbol in the
    object.  The audited function sits at `base` and corresponds to the start
    of the target window, so a compiled function whose start lies in
    [base, base+size) is laid out inside the window (same-TU layout, a split
    two-function symbol).  Returns (functions_in_window, span_end) where
    span_end is the end of the last of them: the honest compiled length of
    the window is span_end - base, never one symbol's size."""
    inwin = sorted(f for f in funcs if base <= f[0] < base + size)
    end = max([f[0] + f[1] for f in inwin] + [base])
    return inwin, end


def words_of(data, lo, hi):
    import struct
    hi = min(hi, len(data))
    return [struct.unpack_from("<I", data, a)[0] for a in range(lo, hi - 3, 4)]


def target_words(ov, addr, size):
    exn = ROOT / "assets/overlays" / f"{ov}.EXN"
    if not exn.exists():
        exn = ROOT / "assets/overlays" / f"{ov}.exn"
    if not exn.exists():
        return None
    data = exn.read_bytes()
    off = addr - 0x80132000
    if off < 0 or off + size > len(data):
        return None
    return words_of(data, off, off + size)


def mdebug_procs(raw):
    """{address: name} of every procedure (stProc=6 / stStaticProc=14 in
    scText=1) recorded in an IDO object's .mdebug.  IDO drops file-private
    `static` functions from the ELF symtab and records them only here, so a
    window holding `void f(void){} static void g(void){}` (NAME_ENT/
    func_8013BCA0) is invisible to an ELF-only read."""
    import struct
    from elftools.elf.elffile import ELFFile
    import io
    out = {}
    elf = ELFFile(io.BytesIO(raw))
    md = elf.get_section_by_name(".mdebug")
    if md is None:
        return out
    base = md["sh_offset"]
    end = "<" if struct.unpack_from("<H", raw, base)[0] == 0x7009 else ">"
    if struct.unpack_from(end + "H", raw, base)[0] != 0x7009:
        return out
    hdr = struct.unpack_from(end + "HH23I", raw, base)
    isym_max, cb_sym, cb_ss, ifd_max, cb_fd = (hdr[9], hdr[10], hdr[16],
                                                hdr[19], hdr[20])
    for f in range(ifd_max):
        _a, _r, iss_base, _c, isym_base, csym = struct.unpack_from(
            end + "6I", raw, cb_fd + 72 * f)
        for i in range(isym_base, min(isym_base + csym, isym_max)):
            iss, value, bits = struct.unpack_from(end + "3I", raw,
                                                  cb_sym + 12 * i)
            if end == ">":
                st, sc = bits >> 26, (bits >> 21) & 0x1F
            else:
                st, sc = bits & 0x3F, (bits >> 6) & 0x1F
            if st in (6, 14) and sc == 1:
                so = cb_ss + iss_base + iss
                out.setdefault(value, raw[so:raw.index(b"\0", so)].decode(
                    "ascii", "replace"))
    return out


def read_elf(obj):
    """([(value, size, name)] every defined function, .text bytes, .text size).

    Sizes come from the ELF symtab; a function only .mdebug knows (an IDO
    `static`) runs to the next function's start (or the section end), minus
    trailing alignment padding."""
    import io
    from elftools.elf.elffile import ELFFile
    raw = Path(obj).read_bytes()
    elf = ELFFile(io.BytesIO(raw))
    text = elf.get_section_by_name(".text")
    data = text.data()
    tidx = elf.get_section_index(".text")
    sized = {}
    for st in elf.get_section_by_name(".symtab").iter_symbols():
        if (st["st_info"]["type"] == "STT_FUNC" and st["st_shndx"] == tidx
                and st["st_size"] > 0):
            sized[st["st_value"]] = (st["st_size"], st.name)
    starts = dict(mdebug_procs(raw))
    starts.update({v: n for v, (_z, n) in sized.items()})
    order = sorted(starts)
    funcs = []
    for k, v in enumerate(order):
        if v in sized:
            funcs.append((v, sized[v][0], sized[v][1]))
            continue
        nxt = order[k + 1] if k + 1 < len(order) else len(data)
        w = words_of(data, v, nxt)
        funcs.append((v, 4 * (len(w) - trailing_pad_words(w)), starts[v]))
    return funcs, data, len(data)


def self_test():
    """Synthetic fixtures for the two false-positive shapes and their
    negative controls.  Exit 0 = all pass."""
    N, J = 0, JR_RA
    X = 0x27BDFFE8  # addiu sp,sp,-24 (any non-zero word)
    cases = [
        # (name, words, expected pad)
        ("pad after jr ra+slot", [X, J, X, N, N], 2),
        ("nop delay slot is code", [X, J, N], 0),
        ("nop slot + 1 pad", [X, J, N, N], 1),
        ("no jr ra = fragment, no pad", [X, X, N, N], 0),
        ("code after last jr ra: no pad", [X, J, N, X, N], 0),
        ("two functions, pad after 2nd", [X, J, N, X, J, X, N, N, N], 3),
        ("zeros before last jr ra are code", [X, N, N, J, N], 0),
    ]
    bad = 0
    for name, w, exp in cases:
        got = trailing_pad_words(w)
        ok = got == exp
        bad += not ok
        print(f"  {'ok ' if ok else 'BAD'} pad: {name}: got {got} want {exp}")
    # window_span fixtures: (funcs, base, size, expected names, expected end)
    spans = [
        ("single function", [(0, 40, "a")], 0, 40, ["a"], 40),
        ("two-function split symbol", [(0, 376, "a"), (376, 572, "b")],
         0, 948, ["a", "b"], 948),
        ("preceding same-TU function excluded",
         [(0, 64, "prev"), (64, 40, "a")], 64, 40, ["a"], 104),
        ("next function past window excluded",
         [(0, 40, "a"), (40, 100, "b")], 0, 40, ["a"], 40),
        ("short body lets next fn start inside window (counted -> FAIL)",
         [(0, 32, "a"), (32, 100, "b")], 0, 40, ["a", "b"], 132),
    ]
    for name, funcs, base, size, names, end in spans:
        inwin, got_end = window_span(funcs, base, size)
        ok = [f[2] for f in inwin] == names and got_end == end
        bad += not ok
        print(f"  {'ok ' if ok else 'BAD'} span: {name}: "
              f"{[f[2] for f in inwin]} end={got_end}")
    # compiled fixture: an IDO `static` function is absent from the ELF
    # symtab and must still be found (via .mdebug) and laid out
    cc = ROOT / "tools/ido-static-recomp/build/5.2/out/cc"
    if cc.exists():
        import subprocess
        with tempfile.TemporaryDirectory() as td:
            c = Path(td) / "fx.c"
            c.write_text("void fx_a(void) {}\nstatic void fx_b(void) {}\n"
                         "void fx_c(int *p) { *p = 1; }\n")
            o = Path(td) / "fx.o"
            env = dict(os.environ, TMPDIR=td)
            r = subprocess.run([str(cc), "-O2", "-G0", "-mips1", "-EL",
                                "-non_shared", "-c", str(c), "-o", str(o)],
                               capture_output=True, env=env, cwd=td)
            if r.returncode == 0:
                funcs, _d, _n = read_elf(o)
                got = [(f[0], f[1], f[2]) for f in funcs]
                want = [(0, 8, "fx_a"), (8, 8, "fx_b"), (16, 12, "fx_c")]
                ok = got == want
                bad += not ok
                print(f"  {'ok ' if ok else 'BAD'} compiled: static function "
                      f"found via .mdebug: {got}")
            else:
                bad += 1
                print("  BAD compiled fixture did not compile")
    print("SELF-TEST", "PASS" if not bad else f"FAIL ({bad})")
    return 1 if bad else 0


def main():
    ap = argparse.ArgumentParser(
        description="Raw (PostProc-free) instruction-count self-audit. Reads "
                    "the ELF SYMBOL size, never the section size.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="exit 0 = count-exact, 1 = mismatch, 2 = build error")
    ap.add_argument("overlay", nargs="?")
    ap.add_argument("func", nargs="?")
    ap.add_argument("--self-test", action="store_true",
                    help="run the synthetic pad/span fixtures and exit")
    ap.add_argument("--keep", action="append", default=[], metavar="STEP",
                    help="keep this @PostProc step (repeatable). Only ever "
                         "removal-only allowed steps, e.g. cse_lui_at.")
    ap.add_argument("--keep-count-changing", action="store_true",
                    help="shorthand for --keep " +
                         " --keep ".join(COUNT_CHANGING_ALLOWED))
    args = ap.parse_args()
    if args.self_test:
        return self_test()
    if not (args.overlay and args.func):
        ap.error("overlay and func are required")
    ov, fn = args.overlay, args.func

    keep = list(args.keep)
    if args.keep_count_changing:
        keep += COUNT_CHANGING_ALLOWED
    banned_keep = [k for k in keep
                   if k in ("patch_insn_reg_field", "pad_trailing_nop",
                            "strip_trailing_instructions",
                            "strip_leading_instructions",
                            "insert_instruction_and_shift", "custom_script")]
    if banned_keep:
        print("REFUSED: --keep %s would defeat the audit (these are the steps "
              "that fake a count)" % ", ".join(banned_keep), file=sys.stderr)
        return 2

    src = ROOT / "src/overlay" / ov / f"{fn}.c"
    text = src.read_text()

    # --- compile through the REAL pipeline ---------------------------------
    # resolve_overlay_relocs.compile_object is the compile the resolver and
    # audit_plain_vs_shipped use: directive-anchored @CompileEnv /
    # @CompileFlags / @Compiler (a gcc file is compiled with gcc -- the old
    # hand-rolled IDO line compiled TAIIKU/func_80148AA0, a gcc281 file, with
    # IDO and reported +3), and it applies whatever @PostProc steps remain in
    # the stripped source -- i.e. exactly the --keep'd removal-only steps.
    import ido_flags
    import resolve_overlay_relocs as R
    from parse_postproc import parse_compiler
    # Directive-anchored: a header that NARRATES "@SymbolSize: N" must not
    # re-window the audit (same rule as the resolver).
    symsize = parse_symbol_size_text(text)

    raw = strip_postproc(text, keep)
    if not keep:
        assert "@PostProc" not in raw, "PostProc leaked into raw source"

    with tempfile.TemporaryDirectory() as td:
        td = Path(td)
        rawc = td / f"{fn}.c"
        rawc.write_text(raw)
        extra = []
        if not parse_compiler(src).startswith("gcc"):
            # keep the shipped PIC mode (stripping patch_stack would flip the
            # derived mode); frame size only, never the instruction count
            # (was `pic_flags(...) or ["-Wc,-pic0"]`: a file stamped
            # `-Wc,-pic1` got pic0 appended last and compiled pic0)
            extra = ido_flags.stripped_pic_extra(text, R.parse_compile_flags(src))
        obj = R.compile_object(ov, fn, rawc, sym_addr(ov, fn, text), td,
                               extra_cflags=extra)
        if obj is None:
            print(f"{ov}/{fn}")
            print("COMPILE FAILED (or kept-step PostProc failed)")
            return 2
        if keep and not any(k in raw for k in keep):
            print(f"  (note: no {'/'.join(keep)} step in this file's "
                  f"chain; --keep had no effect)")

        # --- ELF symbols + .text bytes -------------------------------------
        funcs, text_bytes, sec_bytes = read_elf(obj)

    sym = [f for f in funcs if f[2] == fn]
    sym_val = sym[0][0] if sym else None
    sym_bytes = sym[0][1] if sym else None

    # --- target window -------------------------------------------------------
    tgt_addr, tgt_size = sym_entry(ov, fn)
    win_addr = parse_symbol_start_text(text) or tgt_addr
    eff = symsize if symsize else tgt_size
    tgt_words = target_words(ov, win_addr, eff) if (win_addr and eff) else None

    print(f"{ov}/{fn}")
    if keep:
        print(f"  kept PostProc steps: {', '.join(keep)}")
    if sym_val is None or eff is None or tgt_words is None:
        print(f"  RAW SELF-AUDIT: FAIL / MISMATCH   delta=None words "
              f"(symbol {fn} {'missing from object' if sym_val is None else 'ok'}"
              f", window {'unknown' if eff is None else eff})")
        return 1

    tpad = trailing_pad_words(tgt_words)
    tcode = len(tgt_words) - tpad
    in_win, span_end = window_span(funcs, sym_val, eff)
    span_words = words_of(text_bytes, sym_val, span_end)
    cpad = trailing_pad_words(span_words)
    ccode = len(span_words) - cpad

    print(f"  target: .sym size={tgt_size}  @SymbolSize={symsize}  "
          f"window={eff} bytes = {eff // 4} words = {tcode} code + "
          f"{tpad} trailing alignment pad (zero words after the last "
          f"jr ra + delay slot)")
    print(f"  RAW compiled: ELF SYMBOL {fn} size={sym_bytes} bytes = "
          f"{sym_bytes // 4} words  (section .text={sec_bytes} bytes)")
    if len(in_win) > 1:
        print(f"  same-TU functions laid out inside the window "
              f"({len(in_win)}): " + ", ".join(
                  f"{n}@+0x{v - sym_val:x}:{z // 4}w" for v, z, n in in_win))
    print(f"  compiled span inside window: {len(span_words)} words = {ccode} "
          f"code + {cpad} trailing pad")
    delta = ccode - tcode
    ok = delta == 0
    print(f"  RAW SELF-AUDIT: "
          f"{'PASS (count-exact, no PostProc)' if ok else 'FAIL / MISMATCH'}"
          f"   delta={delta} words")
    if not ok and delta > 0 and not keep:
        print("  hint: if this file's chain contains cse_lui_at (a removal-only "
              "allowed step), re-run with --keep-count-changing — a naive "
              "all-strip read is a known FALSE FAIL.")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
