#!/usr/bin/env python3
"""Apply the WHOLE IDO 5.2 hook chain to a freshly generated recomp, then rebuild.

`make VERSION=5.2` in this directory produces the stock recomp: none of the
env-gated uopt/ugen/as1/cfe hooks and none of the gp-save fidelity fix.  The
matched C in this repository is fitted to the HOOKED compiler (its register
paints, its @CompileEnv lines), so a stock build fails `build-c`,
`check-exn-all` and the patch build.  This script is the one place the chain
order lives; `make setup-ido52` calls it.

    python3 tools/ido-static-recomp/apply_all_hooks.py           # apply + rebuild
    python3 tools/ido-static-recomp/apply_all_hooks.py --regen   # start from pristine C
    python3 tools/ido-static-recomp/apply_all_hooks.py --check   # verify, change nothing

Order follows CLAUDE.md "Known Gotchas" (and UOPT-HOOKS-REFERENCE.md).
Every applier is idempotent (it probes for its own marker), so re-running on
an already-hooked tree is a no-op.  `apply_uopt_constfold_hooks.py` (Hook 47)
is deliberately NOT in the chain: it is a probe, see its docstring.

--regen deletes build/5.2/{uopt,ugen,as1,cfe}.c and regenerates them with
`make VERSION=5.2 c_files` before applying, which is what you want after a
`make clean` or when the C files carry hand edits you do not trust.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
B = "build/5.2"
PASSES = ("uopt", "ugen", "as1", "cfe")

# (script, args).  Paths are relative to HERE.  No applier is given --rebuild:
# the chain rebuilds every pass once at the end.
CHAIN = [
    # ---- uopt ----
    ("apply_uopt_hooks.py", [f"{B}/uopt.c"]),
    ("apply_uopt_precolor_hooks.py", [f"{B}/uopt.c"]),          # Hook 30
    ("apply_uopt_copyprop_hooks.py", [f"{B}/uopt.c"]),          # Hook 32
    ("apply_uopt_copyprop_var_hooks.py", [f"{B}/uopt.c"]),      # Hook 58, after 32
    ("apply_uopt_blocksplit_hooks.py", [f"{B}/uopt.c"]),        # Hook 59
    ("apply_uopt_defplace_hooks.py", [f"{B}/uopt.c"]),
    ("apply_uopt_websplit_hooks.py", [f"{B}/uopt.c"]),          # Hook 34
    ("apply_uopt_formmerge_hooks.py", [f"{B}/uopt.c"]),         # Hook 57, after 34
    ("apply_uopt_web_hooks.py", [f"{B}/uopt.c"]),               # Hooks 38-40b
    ("apply_uopt_splitpiece_hooks.py", [f"{B}/uopt.c"]),        # Hook 60, after web
    ("apply_uopt_readphase_hooks.py", [f"{B}/uopt.c"]),         # Hook 46
    ("apply_uopt_nocse_site_hooks.py", [f"{B}/uopt.c"]),        # Hook 1b, after 46
    ("apply_uopt_splitadmit_hooks.py", [f"{B}/uopt.c"]),        # Hook 53
    ("apply_uopt_splitblocks_hooks.py", [f"{B}/uopt.c"]),       # Hook 63, after 53+60
    ("apply_uopt_jumpmerge_hooks.py", [f"{B}/uopt.c"]),         # Hook 55
    ("apply_uopt_entryhome_hooks.py", [f"{B}/uopt.c"]),         # Hook 73
    ("apply_uopt_findinduct_occ_hooks.py", [f"{B}/uopt.c"]),    # Hook 74
    # ---- ugen ----
    ("apply_ugen_hooks.py", [f"{B}/ugen.c"]),                   # incl. Hooks 36/41
    ("apply_ugen_layout_hooks.py", [f"{B}/ugen.c"]),            # Hook 56
    # ---- as1 ----
    ("apply_as1_hooks.py", [f"{B}/as1.c"]),                     # incl. Hooks 48-52
    ("apply_jumpnext_hooks.py", ["--ugen", f"{B}/ugen.c",       # Hook 61: ugen + as1
                                 "--as1", f"{B}/as1.c"]),
    ("apply_as1_delay_hooks.py", [f"{B}/as1.c"]),
    ("apply_as1_divmacro_hooks.py", ["--as1", f"{B}/as1.c"]),   # Hook 62 / 62e
    ("apply_as1_wideimm_hooks.py", ["--as1", f"{B}/as1.c"]),    # Hook 64
    ("apply_as1_ftfforce_hooks.py", ["--as1", f"{B}/as1.c"]),   # Hook 65
    ("apply_as1_addiucombine_hooks.py", ["--as1", f"{B}/as1.c"]),  # Hook 69
    ("apply_as1_identrewrite_hooks.py", ["--as1", f"{B}/as1.c"]),  # Hook 72
    ("apply_as1_pass1vol_hooks.py", ["--as1", f"{B}/as1.c"]),   # Hook 75
    ("apply_as1_peepreg_hooks.py", ["--as1", f"{B}/as1.c"]),    # Hook 76
    ("apply_as1_repltgt_hooks.py", ["--as1", f"{B}/as1.c"]),    # Hook 77
    # ---- ugen, after the as1/jumpnext pair ----
    ("apply_ugen_moveloop_hooks.py", ["--ugen", f"{B}/ugen.c"]),     # Hook 66
    ("apply_ugen_movenoalias_hooks.py", ["--ugen", f"{B}/ugen.c"]),  # Hook 71
    ("apply_ugen_jumpover_hooks.py", ["--ugen", f"{B}/ugen.c"]),     # Hook 79
    ("apply_ugen_cmpbr_order_hooks.py", ["--ugen", f"{B}/ugen.c"]),  # Hook 80
    # ---- cfe ----
    ("apply_cfe_switchchain_hooks.py", ["--cfe", f"{B}/cfe.c"]),  # Hook 67
    ("apply_cfe_switchmin_hooks.py", ["--cfe", f"{B}/cfe.c"]),    # Hook 78, after 67
    ("apply_cfe_bitfield_hooks.py", ["--cfe", f"{B}/cfe.c"]),     # Hook 68
    ("apply_cfe_frametop_hooks.py", ["--cfe", f"{B}/cfe.c"]),     # Hook 70
    # ---- recomp fidelity fix (default ON), every pass ----
    ("apply_gp_save_hooks.py", []),
]

CFLAGS = ["-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I."]


def run(cmd):
    r = subprocess.run(cmd, cwd=HERE, capture_output=True, text=True)
    return r.returncode, (r.stdout + r.stderr).strip()


def snapshot():
    out = {}
    for p in PASSES:
        with open(os.path.join(HERE, B, f"{p}.c"), "rb") as f:
            out[p] = f.read()
    return out


def main():
    argv = sys.argv[1:]
    check = "--check" in argv
    if "--regen" in argv:
        for p in PASSES:
            path = os.path.join(HERE, B, f"{p}.c")
            if os.path.exists(path):
                os.remove(path)
        rc, out = run(["make", "VERSION=5.2", "c_files"])
        if rc:
            print(out[-2000:])
            sys.exit("[hooks] ERROR: make VERSION=5.2 c_files failed")
    for p in PASSES:
        if not os.path.exists(os.path.join(HERE, B, f"{p}.c")):
            sys.exit(f"[hooks] ERROR: {B}/{p}.c missing: run `make VERSION=5.2` "
                     f"in tools/ido-static-recomp first (or pass --regen)")

    before = snapshot()
    for script, args in CHAIN:
        rc, out = run([sys.executable, script] + args)
        last = out.splitlines()[-1] if out else ""
        if rc:
            print(out[-3000:])
            if check:
                for p, s in before.items():
                    with open(os.path.join(HERE, B, f"{p}.c"), "wb") as f:
                        f.write(s)
            sys.exit(f"[hooks] ERROR: {script} failed (rc={rc})")
        print(f"[hooks] {script}: {last[:150]}")

    after = snapshot()
    changed = [p for p in PASSES if before[p] != after[p]]
    if check:
        for p, s in before.items():
            with open(os.path.join(HERE, B, f"{p}.c"), "wb") as f:
                f.write(s)
        if changed:
            sys.exit(f"[hooks] CHECK FAILED: the chain would still change "
                     f"{', '.join(changed)} -- the compiler is not fully hooked")
        print("[hooks] CHECK OK: every hook already applied")
        return

    out_dir = os.path.join(HERE, B, "out")
    for p in PASSES:
        exe = os.path.join(out_dir, p)
        if p not in changed and os.path.exists(exe):
            continue
        for cmd in (
            ["gcc", "-c"] + CFLAGS + ["-o", f"{B}/{p}.o", f"{B}/{p}.c"],
            ["gcc"] + CFLAGS + ["-o", f"{B}/out/{p}.new", f"{B}/{p}.o",
                                f"{B}/version_info.o", f"{B}/libc_impl_52.o", "-lm"],
        ):
            rc, out = run(cmd)
            if rc:
                print(out[-3000:])
                sys.exit(f"[hooks] ERROR: rebuilding {p} failed")
        os.replace(os.path.join(out_dir, f"{p}.new"), exe)
        print(f"[hooks] rebuilt {B}/out/{p}")
    print(f"[hooks] chain complete ({len(CHAIN)} appliers; changed: "
          f"{', '.join(changed) or 'nothing'})")


if __name__ == "__main__":
    main()
