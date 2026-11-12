#!/usr/bin/env python3
"""Hook honesty check: does any `@CompileEnv` hook change WHAT a function does,
rather than HOW the compiler allocates/schedules it?

A hooked byte-perfect object is retail's own code, so it is correct by
construction. The residual honesty risk is the inverse one: a hook that
miscompiles (deletes a load, drops a store, folds a call argument) could turn
C that is NOT faithful into retail's bytes. `IDO52_SKIP_PASSES=codemotion`
did exactly that (CLAUDE.md, 2026-08-29): it deleted loads and made an
over-count function count-exact.

Method: for every source that carries a live `@CompileEnv`, compile it twice
with the real pipeline (`tools/build/ci_compile_check.py`-style flags via
`gccwrap`/cc), once with the env and once with it stripped, and compare the
two objects on semantic invariants that allocation and scheduling hooks must
preserve:

  loads/stores  number of memory loads and stores (per width), and the
                multiset of relocated symbols they touch
  calls         multiset of `jal` targets
  arith         number of mul/div (multu/divu/mult/div) instructions

A hook family that only steers colouring, temp choice, delay-slot fills,
statement order or const materialisation leaves all three unchanged. A
difference is not automatically dishonest (a KEEP_ANDI keeps an `andi`, a
NO_DEADSTORE keeps a store the C really has, NO_CSE recomputes) but every
difference is listed with the hook set that produced it, so each can be
explained or fixed.

Usage:
    python3 tools/analysis/audit_hook_semantics.py                # all
    python3 tools/analysis/audit_hook_semantics.py --overlay DATE # one overlay
    python3 tools/analysis/audit_hook_semantics.py --files a.c b.c
Writes translation-free JSON to build/analysis/hook_semantics.json.
"""
from __future__ import annotations

import argparse
import hashlib
import collections
import json
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO / "tools" / "build"))
from parse_postproc import directive_bodies   # the build's own prose-immune parser
import ido_flags as _ido_flags
CC = REPO / "tools/ido-static-recomp/build/5.2/out/cc"
OBJDUMP = "mipsel-linux-gnu-objdump"
ENV_RE = re.compile(r"@CompileEnv:\s*(.*)")
FLAGS_RE = re.compile(r"@CompileFlags:\s*(.*)")
LOADS = {"lw", "lh", "lhu", "lb", "lbu", "lwl", "lwr"}
STORES = {"sw", "sh", "sb", "swl", "swr"}
MULDIV = {"mult", "multu", "div", "divu"}


def parse_header(src: Path):
    """Use the build's directive parser (only lines that OPEN with the
    directive count; prose mentions are ignored) and the build's flag
    derivation (region/PIC flags), so the audit compiles what the build
    compiles.  The first version of this tool regex-scraped across line
    breaks and used an approximated command line; that produced 25 false
    'stale' tokens and one phantom banned token (HOOK-HONESTY-AUDIT.md)."""
    txt = src.read_text(errors="replace")
    env = {}
    for body in directive_bodies(txt, "@CompileEnv:"):
        for tok in body.replace("*/", "").split():
            if tok.startswith("IDO52_"):
                k, _, v = tok.partition("=")
                env[k] = v
    flags = []
    for body in directive_bodies(txt, "@CompileFlags:"):
        flags += body.replace("*/", "").split()
    region = "main" if "src/main/" in str(src) else "overlay"
    return env, flags, region


def compile_obj(src: Path, env: dict, flags: list, out: Path, region: str = "overlay") -> bool:
    base = _ido_flags.ido_flags(include_dirs=[REPO / "include"], source=src,
                                compile_flags=flags, region=region)
    cmd = [str(CC)] + base + [f for f in flags if f.startswith("-")] + ["-c", str(src), "-o", str(out)]
    e = {k: v for k, v in os.environ.items() if not k.startswith("IDO52_")}
    e.update(env)
    r = subprocess.run(cmd, env=e, capture_output=True, text=True)
    return r.returncode == 0 and out.exists()


def signature(obj: Path):
    r = subprocess.run([OBJDUMP, "-dr", str(obj)], capture_output=True, text=True)
    loads = collections.Counter(); stores = collections.Counter(); calls = collections.Counter()
    syms = collections.Counter(); muldiv = 0
    pending = None
    for line in r.stdout.splitlines():
        m = re.match(r"\s*[0-9a-f]+:\s+[0-9a-f]{8}\s+(\S+)\s*(.*)", line)
        if m:
            op = m.group(1)
            if op in LOADS: loads[op] += 1; pending = "mem"
            elif op in STORES: stores[op] += 1; pending = "mem"
            elif op in MULDIV: muldiv += 1; pending = None
            elif op == "jal": pending = "call"
            else: pending = None
            continue
        rm = re.match(r"\s*[0-9a-f]+:\s+R_MIPS_\w+\s+(\S+)", line)
        if rm and pending:
            name = rm.group(1).split("+")[0].split("-")[0]
            if pending == "call": calls[name] += 1
            else: syms[name] += 1
    return {"loads": dict(loads), "stores": dict(stores), "calls": dict(calls),
            "memsyms": dict(syms), "muldiv": muldiv}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--overlay")
    ap.add_argument("--files", nargs="*")
    ap.add_argument("--limit", type=int, default=0)
    ap.add_argument("--per-token", action="store_true",
                    help="also compile with each hook token removed and report tokens whose removal leaves the object byte-identical (stale directives)")
    a = ap.parse_args()
    if a.files:
        files = [Path(f).resolve() for f in a.files]
    else:
        pats = [f"src/overlay/{a.overlay}/*.c"] if a.overlay else ["src/main/matched/*.c", "src/overlay/*/*.c"]
        files = [p for pat in pats for p in sorted(REPO.glob(pat))]
    results = []; diffs = []
    tmp = Path(tempfile.mkdtemp(prefix="hooksem_"))
    n = 0
    for src in files:
        txt = src.read_text(errors="replace")
        if "Status: ASM-ONLY" in txt or "@AsmOnly" in txt:
            continue
        env, flags, region = parse_header(src)
        if not env:
            continue
        n += 1
        if a.limit and n > a.limit:
            break
        o1 = tmp / (src.stem + ".hooked.o"); o2 = tmp / (src.stem + ".plain.o")
        ok1 = compile_obj(src, env, flags, o1, region); ok2 = compile_obj(src, {}, flags, o2, region)
        rel = str(src.relative_to(REPO))
        if not (ok1 and ok2):
            results.append({"file": rel, "status": "compile-failed", "hooked": ok1, "plain": ok2}); continue
        s1, s2 = signature(o1), signature(o2)
        keys = [k for k in ("loads", "stores", "calls", "memsyms", "muldiv") if s1[k] != s2[k]]
        # classify: symbols that exist on one side only are the suspect class
        only_h = sorted(set(s1["memsyms"]) - set(s2["memsyms"]))
        only_p = sorted(set(s2["memsyms"]) - set(s1["memsyms"]))
        cls = "clean"
        if s1["calls"] != s2["calls"]:
            cls = "SUSPECT-calls"
        elif only_p:
            cls = "SUSPECT-symbol-dropped-by-hook"
        elif only_h:
            cls = "addressing-form"      # hook rematerialises a base the plain build held in a register
        elif keys:
            cls = "stack-traffic/instruction-selection"
        rec = {"file": rel, "hooks": sorted(env), "diff": keys, "class": cls,
               "only_hooked": only_h, "only_plain": only_p, "hooked": s1, "plain": s2}
        if a.per_token and len(env) > 0:
            base_md5 = hashlib.md5(o1.read_bytes()).hexdigest()
            stale = []
            for k in list(env):
                sub = {kk: vv for kk, vv in env.items() if kk != k}
                o3 = tmp / (src.stem + ".minus.o")
                if compile_obj(src, sub, flags, o3, region) and hashlib.md5(o3.read_bytes()).hexdigest() == base_md5:
                    stale.append(k)
            rec["stale_tokens"] = stale
            if stale:
                print(f"STALE {rel}: {' '.join(stale)}")
        results.append(rec)
        if keys:
            diffs.append(rec)
            print(f"DIFF {rel}: {keys}  hooks={' '.join(sorted(env))}")
    out = REPO / "build/analysis/hook_semantics.json"
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(results, indent=1))
    print(f"\n{len(results)} hooked files compared, {len(diffs)} with semantic differences -> {out}")
    bycls = collections.Counter(d["class"] for d in diffs)
    print("  classes:", dict(bycls))
    for d in diffs:
        if d["class"].startswith("SUSPECT"):
            print(f"  {d['class']:32s} {d['file']}  only_plain={d['only_plain']} calls_h={d['hooked']['calls']} calls_p={d['plain']['calls']}")
    if a.per_token:
        st = collections.Counter(t for r in results for t in r.get("stale_tokens", []))
        print(f"  stale tokens (removal leaves the object identical): {sum(st.values())} in {sum(1 for r in results if r.get('stale_tokens'))} files")
        for t, c in st.most_common(40):
            print(f"    {t:36s} {c}")


if __name__ == "__main__":
    main()
