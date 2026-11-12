#!/usr/bin/env python3
"""Differential validation of the recompiled IDO 5.2 (the build's compiler)
against the IDO 5.2 binaries it was recompiled from, run under qemu-irix.

The binaries are tools/ido-static-recomp/ido/5.2/: a set collected from
several sources rather than installed from one IRIX 5.2 disc (each compiler
pass carries an IRIX 5.2 build stamp; cfe is a June 1994 IRIX 5.2 patch
build).  This tool does not show that the set equals any particular SGI
release; it shows that the static recompilation behaves like those same
binaries.

Compiles every translation unit in the repo BOTH ways with an identical
command line and byte-compares the resulting ``.o``.  Any difference is a
fidelity gap in ``ido-static-recomp``'s 5.2 port (which, unlike 5.3/7.1, is
this project's own work and has no upstream validation).

Prerequisites::

    make setup-qemu-irix        # fetches pinned source -> build/qemu-irix
    make setup-irix-rootfs      # stages ido/5.2 as an executable rootfs

Usage::

    python3 tools/analysis/validate_ido52.py --corpus overlay
    python3 tools/analysis/validate_ido52.py --corpus main --no-env
    python3 tools/analysis/validate_ido52.py --corpus all -j 8 \
        --native /path/to/other/build/5.2/out/cc

``--no-env`` drops each file's ``@CompileEnv`` (the env-gated uopt/as1 hook
switches).  The IRIX binaries have no hooks, so a divergence under
``--no-env`` is a REAL recomp gap, while a divergence with the env applied is
just the hook doing its job.  Running both ways shows that the hook patches
are inert when unset.
"""
import argparse
import hashlib
import json
import os
import shutil
import subprocess
import sys
import tempfile
from collections import Counter
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools" / "build"))
from ido_flags import base_flags, compile_flag_tail          # noqa: E402
from parse_postproc import (parse_compile_env,               # noqa: E402
                            parse_compile_flags, parse_compiler)

DEFAULT_NATIVE = ROOT / "tools/ido-static-recomp/build/5.2/out/cc"
QEMU_BIN = ROOT / "build/qemu-irix/irix-linux-user/qemu-irix"
ROOTFS = ROOT / "build/irix52-rootfs"
INCLUDE = ROOT / "include"


def overlay_corpus():
    """Every overlay function with a .c file, from the status JSON."""
    status = json.loads((ROOT / "config/overlay_match_status.json").read_text())
    out = []
    for overlay, funcs in status.items():
        for fn in funcs:
            src = ROOT / "src/overlay" / overlay / f"{fn}.c"
            if src.exists():
                out.append(str(src))
    return sorted(out)


def main_corpus():
    return sorted(str(p) for d in ("matched", "semi-matched")
                  for p in (ROOT / "src/main" / d).glob("*.c"))


def flags_for(src: Path):
    """Reproduce the project's real command line for this translation unit."""
    extra = parse_compile_flags(src)
    if "src/overlay/" in str(src):
        return (base_flags([INCLUDE, INCLUDE / "psyq"])
                + compile_flag_tail(src, compile_flags=extra))
    # Main binary: build_level3_c.py's line (no PIC mode wired up yet).
    return ["-O2"] + list(extra) + ["-G0", "-mips1", "-EL", "-non_shared",
                                    "-I", str(INCLUDE)]


class Runner:
    def __init__(self, native, no_env, keep_dir):
        self.native = Path(native)
        self.no_env = no_env
        self.keep_dir = Path(keep_dir) if keep_dir else None
        self.tmproot = Path(tempfile.mkdtemp(prefix="ido52val_"))

    def __call__(self, path):
        src = Path(path)
        rec = {"src": str(src)}
        if parse_compiler(src).startswith("gcc"):
            rec["verdict"] = "GCC-SKIP"
            return rec
        flags = flags_for(src)
        env_over = {} if self.no_env else parse_compile_env(src)
        td = Path(tempfile.mkdtemp(dir=str(self.tmproot)))
        try:
            nat, gen = td / "native.o", td / "irix.o"
            env = os.environ.copy()
            env["TMPDIR"] = str(td)
            env.update(env_over)
            subprocess.run([str(self.native)] + flags + ["-c", str(src), "-o", str(nat)],
                           capture_output=True, timeout=600, env=env, cwd=str(td))
            subprocess.run([str(QEMU_BIN), "-L", str(ROOTFS), str(ROOTFS / "usr/bin/cc")]
                           + flags + ["-c", str(src), "-o", str(gen)],
                           capture_output=True, timeout=1800, env=env, cwd=str(td))
            if not nat.exists() or not gen.exists():
                rec["verdict"] = ("BOTH-FAIL" if not nat.exists() and not gen.exists()
                                  else "NATIVE-FAIL" if not nat.exists() else "IRIX-FAIL")
                return rec
            a, b = nat.read_bytes(), gen.read_bytes()
            if a == b:
                rec.update(verdict="IDENTICAL", size=len(a))
                return rec
            rec.update(verdict="DIFF", native_size=len(a), irix_size=len(b),
                       native_sha=hashlib.sha1(a).hexdigest()[:12],
                       irix_sha=hashlib.sha1(b).hexdigest()[:12])
            if len(a) == len(b):
                offs = [i for i in range(len(a)) if a[i] != b[i]]
                rec.update(diff_bytes=len(offs), first=offs[0], last=offs[-1])
            if self.keep_dir:
                keep = self.keep_dir / src.stem
                keep.mkdir(parents=True, exist_ok=True)
                shutil.copy(nat, keep / "native.o")
                shutil.copy(gen, keep / "irix.o")
            return rec
        except Exception as exc:                                # noqa: BLE001
            rec.update(verdict="ERROR", msg=str(exc)[:200])
            return rec
        finally:
            shutil.rmtree(td, ignore_errors=True)

    def cleanup(self):
        shutil.rmtree(self.tmproot, ignore_errors=True)


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--corpus", choices=("overlay", "main", "all"), default="all")
    ap.add_argument("--native", default=str(DEFAULT_NATIVE),
                    help="cc driver to validate (default: the live build/5.2 one)")
    ap.add_argument("--no-env", action="store_true",
                    help="ignore each file's @CompileEnv (hook switches)")
    ap.add_argument("-j", "--jobs", type=int, default=8)
    ap.add_argument("-o", "--out", default=None, help="write per-file JSON results here")
    ap.add_argument("--keep-diffs", default=None,
                    help="directory to save the diverging object pairs into")
    ap.add_argument("--limit", type=int, default=0, help="only the first N files")
    args = ap.parse_args()

    for path, what in ((QEMU_BIN, "qemu-irix (make setup-qemu-irix)"),
                       (ROOTFS / "usr/bin/cc", "IRIX rootfs (make setup-irix-rootfs)")):
        if not path.exists():
            sys.exit(f"error: missing {what}: {path}")

    files = []
    if args.corpus in ("overlay", "all"):
        files += overlay_corpus()
    if args.corpus in ("main", "all"):
        files += main_corpus()
    if args.limit:
        files = files[:args.limit]

    runner = Runner(args.native, args.no_env, args.keep_diffs)
    results = []
    try:
        with ThreadPoolExecutor(max_workers=args.jobs) as ex:
            for i, rec in enumerate(ex.map(runner, files)):
                results.append(rec)
                if (i + 1) % 250 == 0:
                    print(f"  {i + 1}/{len(files)}", flush=True)
    finally:
        runner.cleanup()

    counts = Counter(r["verdict"] for r in results)
    print(f"\ncorpus={args.corpus} files={len(files)} "
          f"env={'ignored' if args.no_env else 'applied'}")
    for verdict, n in counts.most_common():
        print(f"  {verdict:<13} {n}")
    for rec in results:
        if rec["verdict"] not in ("IDENTICAL", "GCC-SKIP"):
            print(f"    {rec['verdict']}: {rec['src']} "
                  f"{rec.get('diff_bytes', '')}")
    if args.out:
        Path(args.out).write_text(json.dumps(results, indent=1))
        print(f"\nwrote {args.out}")
    return 0 if not (counts["ERROR"] or counts["BOTH-FAIL"]) else 1


if __name__ == "__main__":
    sys.exit(main())
