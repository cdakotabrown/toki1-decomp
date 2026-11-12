#!/usr/bin/env python3
"""variant_grid.py -- a directed, enumerative permuter for bulk C-variant testing.

Feed it MANY C variants of one function (or a base with named choice-points) and
it compiles + resolves ALL of them IN PARALLEL through the SAME authoritative
pipeline the real match gate uses, then prints a ranked table plus a per-choice
ablation.  Because every variant travels the exact compile -> PostProc -> fixup
-> resolve path that certifies a real match, a row reported WIN here is a real
win: there is no separate target.o and no fabrication surface.

==============================  HONESTY GUARDRAILS  ==========================
This tool RANKS and SHOWS.  It NEVER edits src/, never auto-applies a variant,
never writes a target byte anywhere.  It is a measurement instrument, not a
patcher.  Applying a winning variant, refreshing trackers and committing are
DELIBERATE, SEPARATE manual steps done only AFTER independent verification
(test_*_match.py + audit_raw_count + audit_paint_bans + audit_paint_isomorphism).

Every row reports BOTH raw_delta (post-pipeline body length vs the target
window) AND nonreloc_fix (words masked/over-claimed at NON-relocation sites) so
a body padded/shortened to a fake count, or a "byte-perfect" propped up by
masking a genuine codegen diff, CANNOT masquerade as clean.  A row is
WIN=True ONLY when it is genuinely clean:

    win  ==  exact == total  AND  raw_delta == 0  AND  nonreloc_fix == 0

For overlays the authoritative resolver (resolve_overlay_relocs.resolve_function)
is the win oracle: it links by name-derived symbol addresses and strict-compares,
so a masked reloc_only that does not truly resolve is caught as nonreloc_fix > 0
and is NOT a win.

--------------------------------  INPUT MODES  ------------------------------
1. FULL-VARIANT   --variants-dir DIR
      A directory of complete `variant_*.c` files.  Every file is tested as-is.

2. MATRIX         BASE.c  --choices choices.json
      BASE.c carries placeholders  /*@VG:name@*/  in the body.  choices.json is
      { "name": ["code option A", "code option B", ...] }.  The tool expands the
      CARTESIAN PRODUCT of all choice-points into concrete sources (identical
      expansions deduped), capped at --max (default 400 -- narrow your choices
      if you exceed it).  The placeholder+JSON-dict shape means code punctuation
      (|, commas, braces) never breaks parsing.

Region-aware:  --region main | overlay,  --func NAME,  --overlay OV (overlays).

    # validate the tool against the authoritative main harness
    python3 tools/analysis/variant_grid.py src/main/semi-matched/get_grp_point.c \\
        --region main --func get_grp_point --choices /tmp/vg_53280.json

    # sweep an overlay function's loop-body forms
    python3 tools/analysis/variant_grid.py src/overlay/TAIIKU/func_801464D4.c \\
        --region overlay --overlay TAIIKU --func func_801464D4 \\
        --choices /tmp/vg_taiiku.json --json /tmp/taiiku_grid.json
"""
import argparse
import difflib
import hashlib
import itertools
import json
import os
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools" / "build"))
sys.path.insert(0, str(ROOT / "tools" / "overlay"))

EXE_PATH = ROOT / "disks" / "SLPS_000.65"
MAIN_BASE_VADDR = 0x80041000
MAIN_FILE_BASE = 0x800

OBJDUMP = "mips-linux-gnu-objdump"


# ───────────────────────────── shape metric ─────────────────────────────────
# Same computation as tools/overlay/shape_align.py: difflib SequenceMatcher
# ratio over the mnemonic (opcode-name) stream, register/immediate agnostic.
def _objdump(data: bytes):
    """[(mnemonic, full-text)] per word via objdump (single source of truth for
    both the shape ratio and the side-by-side)."""
    with tempfile.NamedTemporaryFile(suffix=".bin", delete=False) as fh:
        fh.write(data)
        path = fh.name
    try:
        r = subprocess.run([OBJDUMP, "-D", "-b", "binary", "-m", "mips:isa32",
                            "-EL", path], capture_output=True, text=True)
    finally:
        os.unlink(path)
    out = []
    for ln in r.stdout.splitlines():
        m = re.match(r"^\s*([0-9a-f]+):\t([0-9a-f]{8}) \t(.*)$", ln)
        if m:
            txt = m.group(3).strip().replace("\t", " ")
            out.append((txt.split()[0], txt))
    return out


def shape_pct(target: bytes, compiled: bytes) -> float:
    """Mnemonic-shape alignment, ignoring register fields (0..100)."""
    n = min(len(target), len(compiled))
    n -= n % 4
    a = [x[0] for x in _objdump(target[:n])]
    b = [x[0] for x in _objdump(compiled[:n])]
    if not a:
        return 0.0
    return difflib.SequenceMatcher(None, a, b, autojunk=False).ratio() * 100.0


def _trim_trailing_zero_words(data: bytes) -> int:
    """Real (non-zero) word count, dropping IDO's trailing .text zero padding."""
    words = [struct.unpack("<I", data[i:i + 4])[0]
             for i in range(0, len(data) - 3, 4)]
    while words and words[-1] == 0:
        words.pop()
    return len(words)


# ───────────────────────────── main region ──────────────────────────────────
def _parse_addr_size(text: str):
    addr = size = 0
    for line in text.splitlines():
        if "*/" in line and addr and size:
            break
        m = re.search(r"Address:\s*(0x[0-9a-fA-F]+)", line)
        if m:
            addr = int(m.group(1), 16)
        m = re.search(r"Size:\s*(\d+)\s*bytes", line)
        if m:
            size = int(m.group(1))
    return addr, size


def eval_main(source_text: str, func_name: str, addr: int, size: int,
              want_bytes: bool = False) -> dict:
    """Compile ONE main-binary variant through build_level3_c's authoritative
    path and return its metrics.  Mirrors tools/analysis/test_single_match.py."""
    from build_level3_c import (compile_c_function, fixup_relocations)
    from parse_postproc import (parse_postproc_header, parse_compile_flags,
                                parse_compiler, parse_compile_env,
                                parse_symbol_start, parse_symbol_size)

    with tempfile.TemporaryDirectory() as td:
        src = Path(td) / f"{func_name}.c"
        src.write_text(source_text)

        func_addr, func_size = addr, size
        ss = parse_symbol_start(str(src))
        sz = parse_symbol_size(str(src))
        if ss is not None and ss <= func_addr:
            func_size += (func_addr - ss)
            func_addr = ss
        if sz is not None:
            func_size = sz

        postproc = parse_postproc_header(str(src))
        flags = parse_compile_flags(str(src))
        compiler = parse_compiler(str(src)) or "ido52"
        env = parse_compile_env(str(src))

        try:
            compiled = compile_c_function(str(src), func_name, func_addr,
                                          postproc_steps=postproc,
                                          extra_flags=flags, compiler=compiler,
                                          compile_env=env)
        except Exception as e:  # noqa: BLE001
            return {"ok": False, "error": str(e)[:160]}
        if compiled is None:
            return {"ok": False, "error": "compile returned None"}

    exe = EXE_PATH.read_bytes()
    file_off = (func_addr - MAIN_BASE_VADDR) + MAIN_FILE_BASE
    target = exe[file_off:file_off + func_size]

    raw_body_words = _trim_trailing_zero_words(compiled)
    if len(compiled) > func_size:
        compiled = compiled[:func_size]

    audit = []
    compiled = fixup_relocations(compiled, func_addr, exe, file_off,
                                 func_name=func_name, audit=audit)

    n = min(len(target), len(compiled)) // 4
    exact = 0
    for i in range(n):
        tw = struct.unpack("<I", target[i * 4:i * 4 + 4])[0]
        cw = struct.unpack("<I", compiled[i * 4:i * 4 + 4])[0]
        if tw == cw:
            exact += 1
    total = len(target) // 4
    raw_delta = raw_body_words - total
    nonreloc = len(audit)
    win = (exact == total and raw_delta == 0 and nonreloc == 0)
    out = {"ok": True, "exact": exact, "total": total,
           "shape": shape_pct(target, compiled), "raw_delta": raw_delta,
           "nonreloc": nonreloc, "resolved_exact": exact, "win": win}
    if want_bytes:
        out["target_bytes"] = bytes(target[:total * 4])
        out["compiled_bytes"] = bytes(compiled[:total * 4])
    return out


# ─────────────────────────── overlay region ─────────────────────────────────
def _overlay_raw_delta(source_text: str, overlay: str, func_name: str) -> int:
    """Reuse tools/overlay/audit_raw_count logic: strip PostProc (keep the
    removal-only cse_lui_at), compile with the file's own env/flags, read the
    ELF SYMBOL size, return (raw_words - target_window_words).  A non-zero delta
    means the body is genuinely the wrong LENGTH (too short/padded)."""
    from audit_raw_count import strip_postproc, COUNT_CHANGING_ALLOWED
    cc = ROOT / "tools/ido-static-recomp/build/5.2/out/cc"
    flags = ["-O2", "-G0", "-mips1", "-EL", "-non_shared",
             f"-I{ROOT/'include'}", f"-I{ROOT/'include'/'psyq'}"]

    env_extra = {}
    for m in re.finditer(r"@CompileEnv:\s*(.+)", source_text):
        for kv in m.group(1).split():
            if "=" in kv:
                k, v = kv.split("=", 1)
                v = v.strip()
                if v.endswith("*/"):
                    v = v[:-2].rstrip()
                env_extra[k] = v
    extra_flags = []
    for m in re.finditer(r"@Compile(?:r)?Flags:\s*(.+)", source_text):
        extra_flags += m.group(1).replace("*/", "").split()
    sym_size = None
    m = re.search(r"@SymbolSize:\s*(\d+)", source_text)
    if m:
        sym_size = int(m.group(1))

    target_words = None
    for ln in (ROOT / "config/overlay" / f"{overlay}.sym").read_text().splitlines():
        p = ln.strip().split(":")
        if len(p) >= 3 and p[2].strip() == func_name:
            target_words = int(p[1], 0) // 4
            break
    if sym_size is not None:
        target_words = sym_size // 4

    raw = strip_postproc(source_text, COUNT_CHANGING_ALLOWED)
    with tempfile.TemporaryDirectory() as td:
        rawc = Path(td) / f"{func_name}.c"
        rawc.write_text(raw)
        obj = Path(td) / "o.o"
        env = os.environ.copy()
        env["TMPDIR"] = td
        env.update(env_extra)
        r = subprocess.run([str(cc)] + flags + extra_flags +
                           ["-c", str(rawc), "-o", str(obj)],
                           capture_output=True, text=True, env=env, cwd=td,
                           timeout=120)
        if r.returncode != 0:
            return 999
        nm = subprocess.run(["mipsel-linux-gnu-readelf", "-sW", str(obj)],
                            capture_output=True, text=True)
        sym_bytes = None
        for ln in nm.stdout.splitlines():
            f = ln.split()
            if len(f) >= 8 and f[7] == func_name and f[3] == "FUNC":
                sym_bytes = int(f[2])
    if sym_bytes is None or target_words is None:
        return 999
    return sym_bytes // 4 - target_words


def eval_overlay(source_text: str, overlay: str, func_name: str,
                 want_bytes: bool = False) -> dict:
    """Compile ONE overlay variant through the authoritative overlay pipeline.

    exact%/shape% come from test_overlay_match (the masked harness, so exact%
    is directly comparable to `test_overlay_match.py -v`).  The WIN oracle and
    resolved_exact come from resolve_overlay_relocs (name-derived strict link).
    nonreloc = words the masked verdict counts as effectively-matched that the
    strict resolver does NOT confirm -- the over-masking honesty flag.
    """
    from test_overlay_match import test_overlay_function
    from resolve_overlay_relocs import resolve_function

    with tempfile.TemporaryDirectory() as td:
        src = Path(td) / f"{func_name}.c"
        src.write_text(source_text)

        res = test_overlay_function(overlay, func_name, verbose=False,
                                    return_bytes=True, src_override=src)
        if "error" in res:
            return {"ok": False, "error": res["error"][:160]}

        rr = resolve_function(overlay, func_name, src_override=src)
        raw_delta = _overlay_raw_delta(source_text, overlay, func_name)

    total = res["n_target"]
    exact = res["exact_match"]
    reloc_match = res["reloc_match"]
    resolved_exact = rr.get("exact_after", 0) if "error" not in rr else 0
    resolve_matched = bool(rr.get("matched")) if "error" not in rr else False
    unresolved = rr.get("unresolved", []) if "error" not in rr else ["<err>"]
    # over-masking: reloc-masked words the strict resolver cannot back.
    nonreloc = max(0, reloc_match - resolved_exact)
    compiled = res.get("compiled_bytes", b"")
    target = res.get("target_bytes", b"")
    win = (resolve_matched and raw_delta == 0 and not unresolved
           and not res.get("postproc_failed") and not res.get("truncated_surplus"))
    out = {"ok": True, "exact": exact, "total": total,
           "shape": shape_pct(target, compiled) if target else 0.0,
           "raw_delta": raw_delta, "nonreloc": nonreloc,
           "resolved_exact": resolved_exact, "win": win}
    if want_bytes:
        out["target_bytes"] = target
        out["compiled_bytes"] = compiled
    return out


def leader_bytes(task: dict):
    """Recompile ONE variant and return (target_bytes, compiled_bytes) for the
    side-by-side.  Cheap (single leader) and reuses the region evaluator."""
    if task["region"] == "main":
        r = eval_main(task["source"], task["func"], task["addr"], task["size"],
                      want_bytes=True)
    else:
        r = eval_overlay(task["source"], task["overlay"], task["func"],
                         want_bytes=True)
    if not r.get("ok"):
        return None
    return r.get("target_bytes"), r.get("compiled_bytes")


# ─────────────────────────── worker dispatch ────────────────────────────────
def _evaluate(task: dict) -> dict:
    vid = task["vid"]
    try:
        if task["region"] == "main":
            m = eval_main(task["source"], task["func"], task["addr"], task["size"])
        else:
            m = eval_overlay(task["source"], task["overlay"], task["func"])
    except Exception as e:  # noqa: BLE001
        m = {"ok": False, "error": f"{type(e).__name__}: {e}"[:160]}
    m["vid"] = vid
    m["choices"] = task.get("choices")
    return m


# ───────────────────────────── expansion ────────────────────────────────────
def expand_matrix(base_text: str, choices: dict, cap: int):
    """Cartesian product of choice-points -> list of (vid, choices_dict, text).

    Placeholders in base_text look like /*@VG:name@*/.  Identical expansions
    (different labels, same resulting source) are deduped by content hash.
    """
    names = sorted(choices)
    # A placeholder may live in the base OR inside another choice's option
    # (nested levers, resolved by the fixed-point loop below).
    haystack = base_text + "\n".join(o for opts in choices.values() for o in opts)
    for nm in names:
        if f"/*@VG:{nm}@*/" not in haystack:
            raise SystemExit(f"placeholder /*@VG:{nm}@*/ for choice '{nm}' "
                             f"appears nowhere (base or options)")
    combos = list(itertools.product(*[range(len(choices[n])) for n in names]))
    if len(combos) > cap:
        raise SystemExit(f"cartesian product is {len(combos)} variants > --max "
                         f"{cap}; narrow your choices or raise --max")
    seen = {}
    out = []
    for combo in combos:
        pick = {n: choices[n][idx] for n, idx in zip(names, combo)}
        label = {n: idx for n, idx in zip(names, combo)}
        # Fixed-point substitution: a chosen option may itself reference another
        # /*@VG:name@*/ placeholder (e.g. a loop-FORM option that embeds the
        # cast/end levers), so keep replacing until the text is stable.
        text = base_text
        for _ in range(len(names) + 2):
            new = text
            for n, code in pick.items():
                new = new.replace(f"/*@VG:{n}@*/", code)
            if new == text:
                break
            text = new
        if "/*@VG:" in text:
            raise SystemExit("unresolved placeholder after expansion (cyclic "
                             "or unknown /*@VG:...@*/ reference)")
        h = hashlib.sha256(text.encode()).hexdigest()
        if h in seen:
            continue
        seen[h] = True
        vid = "|".join(f"{n}={idx}" for n, idx in zip(names, combo))
        out.append((vid, label, text))
    return out, names


def load_variants_dir(d: Path):
    out = []
    for p in sorted(d.glob("variant_*.c")):
        out.append((p.stem, None, p.read_text()))
    if not out:
        raise SystemExit(f"no variant_*.c files in {d}")
    return out


# ───────────────────────────── reporting ────────────────────────────────────
def _rank_key(r):
    return (r.get("win", False), r.get("exact", -1), r.get("shape", -1.0),
            -abs(r.get("raw_delta", 999)))


def print_table(results, names=None, choices=None):
    rows = sorted(results, key=_rank_key, reverse=True)
    print()
    print(f"{'variant-id':40s} {'exact':>10s} {'shape%':>7s} "
          f"{'rawΔ':>5s} {'nrfix':>5s}  WIN")
    print("─" * 78)
    for r in rows:
        if not r.get("ok"):
            print(f"{r['vid']:40s} {'ERR':>10s}  {r.get('error','')[:40]}")
            continue
        ex = f"{r['exact']}/{r['total']}"
        exp = 100.0 * r["exact"] / max(r["total"], 1)
        win = "WIN" if r["win"] else ""
        print(f"{r['vid']:40s} {ex:>10s} {exp:5.1f}% {r['shape']:6.1f} "
              f"{r['raw_delta']:>5d} {r['nonreloc']:>5d}  {win}")
    return rows


def ablation(results, names, choices):
    """Hold every choice-point at the current-best and vary ONE site; print the
    marginal shape%/exact% each option yields.  Shows WHICH lever moved."""
    ok = [r for r in results if r.get("ok") and r.get("choices") is not None]
    if not ok or not names:
        return
    best = max(ok, key=_rank_key)
    bx = best["choices"]
    idx = {tuple(sorted(r["choices"].items())): r for r in ok}
    print("\n" + "═" * 78)
    print("ABLATION  (others held at leader; vary one site)")
    print(f"leader: {best['vid']}  exact {best['exact']}/{best['total']}  "
          f"shape {best['shape']:.1f}%")
    for nm in names:
        print(f"\n  choice '{nm}':")
        for opt in range(len(choices[nm])):
            key = dict(bx)
            key[nm] = opt
            r = idx.get(tuple(sorted(key.items())))
            if r is None:
                print(f"    [{opt}] (deduped/absent)")
                continue
            mark = "  <- leader" if opt == bx[nm] else ""
            dsh = r["shape"] - best["shape"]
            dex = r["exact"] - best["exact"]
            snippet = choices[nm][opt].strip().splitlines()
            snippet = (snippet[0][:46] + "…") if snippet else ""
            print(f"    [{opt}] exact {r['exact']:>3d} (Δ{dex:+d})  "
                  f"shape {r['shape']:5.1f}% (Δ{dsh:+.1f})  {win_flag(r)}{mark}")
            print(f"          {snippet}")


def win_flag(r):
    return "WIN" if r.get("win") else ""


def emit_shape_sbs(target: bytes, compiled: bytes, func: str):
    """Aligned mnemonic side-by-side of target vs leader (same difflib
    alignment as tools/overlay/shape_sbs.py), built from bytes we control so
    it never touches src/.  '**' marks a structural (mnemonic) divergence."""
    n = min(len(target), len(compiled))
    n -= n % 4
    t = _objdump(target[:n])
    c = _objdump(compiled[:n])
    print("\n" + "═" * 78)
    print(f"LEADER shape side-by-side  (target | {func} leader)   '**'=shape diff")
    sm = difflib.SequenceMatcher(None, [x[0] for x in t], [x[0] for x in c],
                                 autojunk=False)
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        span = max(i2 - i1, j2 - j1)
        for k in range(span):
            ti = i1 + k if i1 + k < i2 else None
            ci = j1 + k if j1 + k < j2 else None
            a = f"{ti:4d} {t[ti][1]}" if ti is not None else " " * 34
            b = f"{ci:4d} {c[ci][1]}" if ci is not None else ""
            mark = "  " if (tag == "equal" and ti is not None and ci is not None
                            and t[ti][1] == c[ci][1]) else \
                   ("~ " if tag == "equal" else "**")
            print(f"{a:<40}{mark}{b}")


# ─────────────────────── generic safe transforms ────────────────────────────
# BATCH-TRIAGE auto-library.  When a batch entry supplies NO per-entry
# choices.json we synthesise a small library of GENERIC, conservative,
# text-level source variants and test each through the SAME authoritative
# evaluator.  These are codegen PROBES, not applied patches: a variant that
# fails to compile is scored ok=False (skip) and NEVER crashes the run.  We
# apply exactly ONE change per variant (from the pristine source each time) so
# a probe's effect is attributable, and we cap the library per function.
def _find_matching_brace(text: str, i: int) -> int:
    depth = 0
    while i < len(text):
        c = text[i]
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                return i
        i += 1
    return -1


def _find_matching_paren(text: str, i: int) -> int:
    depth = 0
    while i < len(text):
        c = text[i]
        if c == "(":
            depth += 1
        elif c == ")":
            depth -= 1
            if depth == 0:
                return i
        i += 1
    return -1


def _gen_do_while(text: str):
    """(a) do{...}while(c);  ->  while(c){...}   (one variant per unambiguous do)."""
    out = []
    for m in re.finditer(r"\bdo\b\s*\{", text):
        brace = text.index("{", m.start())
        close = _find_matching_brace(text, brace)
        if close < 0:
            continue
        wm = re.match(r"\s*while\s*\(", text[close + 1:])
        if not wm:
            continue
        popen = close + 1 + wm.end() - 1
        pclose = _find_matching_paren(text, popen)
        if pclose < 0:
            continue
        sm = re.match(r"\s*;", text[pclose + 1:])
        if not sm:
            continue
        cond = text[popen + 1:pclose].strip()
        body = text[brace + 1:close]
        end = pclose + 1 + sm.end()
        new = f"while ({cond}) {{{body}}}"
        out.append((f"do2while@{m.start()}", text[:m.start()] + new + text[end:]))
    return out


def _gen_while_do(text: str):
    """(a) while(c){...}  ->  do{...}while(c);   (skips the trailing while of an
    existing do-while, which is followed by ';' not '{')."""
    out = []
    for m in re.finditer(r"\bwhile\s*\(", text):
        popen = m.end() - 1
        pclose = _find_matching_paren(text, popen)
        if pclose < 0:
            continue
        bm = re.match(r"\s*\{", text[pclose + 1:])
        if not bm:
            continue
        brace = pclose + 1 + bm.end() - 1
        close = _find_matching_brace(text, brace)
        if close < 0:
            continue
        cond = text[popen + 1:pclose].strip()
        body = text[brace + 1:close]
        new = f"do {{{body}}} while ({cond});"
        out.append((f"while2do@{m.start()}", text[:m.start()] + new + text[close + 1:]))
    return out


def _gen_increment(text: str):
    """(b) lone `x++;` <-> `++x;` statements (one variant per site)."""
    out = []
    for m in re.finditer(r"(?m)^([ \t]*)([A-Za-z_]\w*)\+\+;", text):
        new = f"{m.group(1)}++{m.group(2)};"
        out.append((f"post2pre@{m.start()}", text[:m.start()] + new + text[m.end():]))
    for m in re.finditer(r"(?m)^([ \t]*)\+\+([A-Za-z_]\w*);", text):
        new = f"{m.group(1)}{m.group(2)}++;"
        out.append((f"pre2post@{m.start()}", text[:m.start()] + new + text[m.end():]))
    return out


_ASSIGN_RE = re.compile(r"^\s*[A-Za-z_]\w*\s*=\s*[^;{}()]+;\s*$")


def _gen_swap_adjacent(text: str):
    """(c) swap two ADJACENT simple scalar `lhs = rhs;` statements (rhs has no
    call/paren/brace), for each adjacent pair (one variant per pair)."""
    out = []
    lines = text.split("\n")
    for i in range(len(lines) - 1):
        if _ASSIGN_RE.match(lines[i]) and _ASSIGN_RE.match(lines[i + 1]):
            swapped = lines[:]
            swapped[i], swapped[i + 1] = swapped[i + 1], swapped[i]
            out.append((f"swap@{i + 1}", "\n".join(swapped)))
    return out


def gen_generic_variants(text: str, cap: int = 32):
    """Best-effort generic library (deduped, capped).  Returns [(label, src)]."""
    cands = (_gen_do_while(text) + _gen_while_do(text) +
             _gen_increment(text) + _gen_swap_adjacent(text))
    seen = {hashlib.sha256(text.encode()).hexdigest()}  # exclude no-op == base
    out = []
    for label, src in cands:
        h = hashlib.sha256(src.encode()).hexdigest()
        if h in seen:
            continue
        seen.add(h)
        out.append((label, src))
        if len(out) >= cap:
            break
    return out


# ─────────────────────────── batch-triage engine ────────────────────────────
def _pct(r: dict) -> float:
    return 100.0 * r.get("exact", 0) / max(r.get("total", 0), 1)


def _default_source(region: str, overlay: str, func: str):
    if region == "overlay":
        p = ROOT / "src" / "overlay" / overlay / f"{func}.c"
        return p if p.exists() else None
    hits = list((ROOT / "src" / "main").rglob(f"{func}.c"))
    return hits[0] if hits else None


def _resolve_main_addr_size(func: str, sample_text: str):
    addr = size = 0
    cfg = ROOT / "config/level3_functions.json"
    if cfg.exists():
        info = json.loads(cfg.read_text()).get(func)
        if info:
            a = info.get("address", 0)
            addr = int(a, 16) if isinstance(a, str) else a
            size = info.get("size", 0)
    if not addr or not size:
        ta, tsz = _parse_addr_size(sample_text)
        addr = addr or ta
        size = size or tsz
    return addr, size


def classify_entry(base_r: dict, others: list, threshold: float):
    """Sort one function into WIN / C-REACHABLE / LIKELY-WALL.

    WIN         some variant (or the baseline) is a genuine clean win.
    C-REACHABLE best generic/matrix variant beats baseline exact% OR shape% by
                >= threshold points AND is length-honest (raw_delta == 0).
    LIKELY-WALL nothing generic moves it -> flag for function-specific / hook work.
    """
    ok_others = [r for r in others if r.get("ok")]
    all_ok = ([base_r] if base_r.get("ok") else []) + ok_others
    if any(r.get("win") for r in all_ok):
        best = max((r for r in all_ok if r.get("win")), key=_rank_key)
        return "WIN", best
    if not base_r.get("ok"):
        return "BASELINE-BROKEN", base_r
    best = max(ok_others, key=_rank_key, default=None)
    if best is None:
        return "LIKELY-WALL", base_r
    improved = ((_pct(best) - _pct(base_r) >= threshold or
                 best["shape"] - base_r["shape"] >= threshold) and
                best.get("raw_delta") == 0)
    return ("C-REACHABLE" if improved else "LIKELY-WALL"), best


def run_batch(args):
    entries = json.loads(Path(args.batch).read_text())
    if not isinstance(entries, list):
        raise SystemExit("batch.json must be a LIST of entries")
    worklist = []
    for entry in entries:
        func = entry["func"]
        region = entry["region"]
        overlay = (entry.get("overlay") or "").upper()
        if region == "overlay" and not overlay:
            raise SystemExit(f"entry {func}: overlay region needs 'overlay'")
        base_path = entry.get("base") or _default_source(region, overlay, func)
        if not base_path or not Path(base_path).exists():
            raise SystemExit(f"entry {func}: cannot locate source (set 'base')")
        base_text = Path(base_path).read_text()

        variants = [("baseline", base_text)]
        mode = ""
        if entry.get("choices"):
            choices = json.loads(Path(entry["choices"]).read_text())
            vs, _ = expand_matrix(base_text, choices, args.max)
            variants += [(vid, text) for vid, _lbl, text in vs]
            mode = f"matrix({len(vs)})"
        else:
            gen = gen_generic_variants(base_text, cap=args.batch_cap)
            variants += gen
            mode = f"generic({len(gen)})"

        addr, size = (0, 0)
        if region == "main":
            addr, size = _resolve_main_addr_size(func, base_text)
            if not addr or not size:
                raise SystemExit(f"entry {func}: could not determine addr/size")

        tasks = [{"vid": vid, "choices": None, "source": text,
                  "region": region, "func": func, "overlay": overlay,
                  "addr": addr, "size": size} for vid, text in variants]
        print(f"[{region}/{func}] evaluating {len(tasks)} variant(s) "
              f"[{mode}] on {args.jobs} job(s) ...")
        if len(tasks) == 1 or args.jobs <= 1:
            results = [_evaluate(t) for t in tasks]
        else:
            import multiprocessing as mp
            with mp.Pool(args.jobs) as pool:
                results = pool.map(_evaluate, tasks)

        base_r = next(r for r in results if r["vid"] == "baseline")
        others = [r for r in results if r["vid"] != "baseline"]
        klass, best = classify_entry(base_r, others, args.improve_threshold)
        n_ok = sum(1 for r in others if r.get("ok"))
        row = {
            "func": func, "region": region, "overlay": overlay,
            "class": klass, "mode": mode,
            "n_variants": len(others), "n_compiled": n_ok,
            "baseline_exact": base_r.get("exact"),
            "baseline_total": base_r.get("total"),
            "baseline_exact_pct": round(_pct(base_r), 1) if base_r.get("ok") else None,
            "baseline_shape": round(base_r.get("shape", 0.0), 1),
            "baseline_raw_delta": base_r.get("raw_delta"),
            "baseline_nrfix": base_r.get("nonreloc"),
            "best_vid": best.get("vid") if best else None,
            "best_exact": best.get("exact") if best else None,
            "best_exact_pct": round(_pct(best), 1) if best and best.get("ok") else None,
            "best_shape": round(best.get("shape", 0.0), 1) if best else None,
            "best_raw_delta": best.get("raw_delta") if best else None,
            "best_nrfix": best.get("nonreloc") if best else None,
        }
        worklist.append(row)

    print_worklist(worklist, args.improve_threshold)
    if args.json:
        Path(args.json).write_text(json.dumps(worklist, indent=2))
        print(f"\nWrote {args.json}")
    return worklist


def print_worklist(worklist, threshold):
    print("\n" + "═" * 100)
    print(f"BATCH-TRIAGE WORKLIST  (improve-threshold = +{threshold:.1f} pts; "
          f"C-REACHABLE also requires raw_delta == 0)")
    print("─" * 100)
    print(f"{'function':22s} {'region':7s} {'base ex%':>8s} {'base sh%':>8s}"
          f" {'best ex%':>8s} {'best sh%':>8s} {'rawΔ':>5s} {'nrfix':>5s}  CLASS")
    print("─" * 100)
    for r in worklist:
        bx = f"{r['baseline_exact_pct']:.1f}" if r['baseline_exact_pct'] is not None else "ERR"
        bs = f"{r['baseline_shape']:.1f}"
        ex = f"{r['best_exact_pct']:.1f}" if r['best_exact_pct'] is not None else "-"
        sh = f"{r['best_shape']:.1f}" if r['best_shape'] is not None else "-"
        rd = r['best_raw_delta'] if r['best_raw_delta'] is not None else "-"
        nr = r['best_nrfix'] if r['best_nrfix'] is not None else "-"
        print(f"{r['func']:22s} {r['region']:7s} {bx:>8s} {bs:>8s}"
              f" {ex:>8s} {sh:>8s} {str(rd):>5s} {str(nr):>5s}  {r['class']}")
    print("─" * 100)
    wins = [r for r in worklist if r["class"] == "WIN"]
    creach = [r for r in worklist if r["class"] == "C-REACHABLE"]
    walls = [r for r in worklist if r["class"] == "LIKELY-WALL"]
    print(f"WIN: {len(wins)}   C-REACHABLE (keep hand-rewriting): {len(creach)}"
          f"   LIKELY-WALL (stop C attempts, do hook/function-specific work): "
          f"{len(walls)}")
    if wins:
        print("  WIN candidates MUST be verified independently "
              "(test_*_match.py + audit_raw_count + audit_paint_*) before applying.")
    print("This tool did NOT edit src/ and did NOT auto-apply any variant.")


# ───────────────────────────────── main ─────────────────────────────────────
def main():
    ap = argparse.ArgumentParser(
        description="Directed enumerative permuter: bulk-test C variants through "
                    "the authoritative match pipeline. Ranks + ablates; never "
                    "edits src/ and never auto-applies.")
    ap.add_argument("base", nargs="?", help="base .c (MATRIX mode)")
    ap.add_argument("--variants-dir", help="FULL-VARIANT mode: dir of variant_*.c")
    ap.add_argument("--choices", help="MATRIX mode: choices.json (name -> [opts])")
    ap.add_argument("--region", choices=["main", "overlay"],
                    help="required unless --batch is used")
    ap.add_argument("--func", help="required unless --batch is used")
    ap.add_argument("--batch", help="BATCH-TRIAGE mode: JSON list of entries "
                    "[{func, region, overlay, base?, choices?}]; bulk-sorts each "
                    "function into WIN / C-REACHABLE / LIKELY-WALL")
    ap.add_argument("--batch-cap", type=int, default=32,
                    help="max auto-generated generic variants per function")
    ap.add_argument("--improve-threshold", type=float, default=2.0,
                    help="points a variant must beat baseline exact%%/shape%% by "
                    "(with raw_delta==0) to be C-REACHABLE (default 2.0)")
    ap.add_argument("--overlay", help="overlay name (overlay region)")
    ap.add_argument("--addr", help="main func address (hex); else header/config")
    ap.add_argument("--size", type=int, help="main func size bytes; else header")
    ap.add_argument("--jobs", type=int, default=os.cpu_count())
    ap.add_argument("--max", type=int, default=400, help="cartesian cap")
    ap.add_argument("--json", help="write machine-readable results here")
    ap.add_argument("--no-sbs", action="store_true", help="skip leader side-by-side")
    args = ap.parse_args()

    if args.batch:
        run_batch(args)
        return

    if not args.region or not args.func:
        raise SystemExit("--region and --func are required (unless --batch)")
    if args.region == "overlay" and not args.overlay:
        raise SystemExit("--overlay is required for --region overlay")

    # Build the variant list.
    names = None
    choices = None
    if args.variants_dir:
        variants = load_variants_dir(Path(args.variants_dir))
    else:
        if not args.base or not args.choices:
            raise SystemExit("MATRIX mode needs BASE.c and --choices JSON")
        base_text = Path(args.base).read_text()
        choices = json.loads(Path(args.choices).read_text())
        variants, names = expand_matrix(base_text, choices, args.max)

    # main region addr/size resolution
    addr, size = 0, 0
    if args.region == "main":
        if args.addr:
            addr = int(args.addr, 16)
        if args.size:
            size = args.size
        if not addr or not size:
            cfg = ROOT / "config/level3_functions.json"
            if cfg.exists():
                info = json.loads(cfg.read_text()).get(args.func)
                if info:
                    a = info.get("address", 0)
                    addr = addr or (int(a, 16) if isinstance(a, str) else a)
                    size = size or info.get("size", 0)
        if not addr or not size:
            ta, tsz = _parse_addr_size(variants[0][2])
            addr = addr or ta
            size = size or tsz
        if not addr or not size:
            raise SystemExit("could not determine main func addr/size "
                             "(pass --addr/--size)")

    tasks = []
    for vid, label, text in variants:
        tasks.append({"vid": vid, "choices": label, "source": text,
                      "region": args.region, "func": args.func,
                      "overlay": (args.overlay or "").upper(),
                      "addr": addr, "size": size})

    print(f"Evaluating {len(tasks)} variant(s) on {args.jobs} job(s) "
          f"[{args.region}/{args.func}] ...")
    if len(tasks) == 1 or args.jobs <= 1:
        results = [_evaluate(t) for t in tasks]
    else:
        import multiprocessing as mp
        with mp.Pool(args.jobs) as pool:
            results = pool.map(_evaluate, tasks)

    rows = print_table(results, names, choices)
    if names:
        ablation(results, names, choices)

    winners = [r for r in rows if r.get("ok") and r.get("win")]
    print("\n" + "═" * 78)
    if winners:
        print(f"WIN candidate(s): {len(winners)} — VERIFY INDEPENDENTLY before "
              f"applying (test_*_match.py + audit_raw_count + audit_paint_bans "
              f"+ audit_paint_isomorphism). This tool did NOT touch src/.")
        for w in winners:
            print(f"    {w['vid']}")
    else:
        best = max((r for r in rows if r.get("ok")), key=_rank_key, default=None)
        if best:
            print(f"No clean win. Best: {best['vid']}  "
                  f"exact {best['exact']}/{best['total']}  "
                  f"shape {best['shape']:.1f}%  rawΔ {best['raw_delta']}  "
                  f"nrfix {best['nonreloc']}")

    if not args.no_sbs and rows:
        best = max((r for r in rows if r.get("ok")), key=_rank_key, default=None)
        if best is not None:
            btask = next(t for t in tasks if t["vid"] == best["vid"])
            pair = leader_bytes(btask)
            if pair and pair[0] and pair[1]:
                emit_shape_sbs(pair[0], pair[1], args.func)

    if args.json:
        Path(args.json).write_text(json.dumps(
            [{k: v for k, v in r.items() if k != "choices"} for r in results],
            indent=2))
        print(f"\nWrote {args.json}")


if __name__ == "__main__":
    main()
