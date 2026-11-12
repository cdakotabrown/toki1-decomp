#!/usr/bin/env python3
"""Apply uopt Hooks 38/38b/39/40/40b (the web home/spill/formation family).

Runs uopt_web_hooks/apply_h38.py, apply_h39.py, apply_h40.py IN THAT ORDER
on build/5.2/uopt.c (or the path given as argv[1]).  Order matters for the
pre-f_globalcolor call-site stacking; this order reproduces the 2026-08-14
research build byte-for-byte (verified against its uopt_priv.o).

Hooks installed (env-gated, default-off; census in
docs/reference/UOPT-HOOKS-REFERENCE.md):
    38  IDO52_FORCE_WEB_HOME / IDO52_WEB_HOME_STAGE / IDO52_WEB_HOME_TRACE
    38b IDO52_FORCE_WEB_ADJSAVE
    39  IDO52_FORCE_WEB_SPILL / IDO52_FORCE_WEB_NOSPILL
    40  IDO52_VETO_WEB / IDO52_VETO_WEB_STAGE / IDO52_VETO_WEB_DROPLR
    40b IDO52_VETO_FORMLIV / IDO52_FORMLIV_TRACE
    37  (limits only) IDO52_FORCE_WEB_COLOR overflow -> hard error.  The
        Hook 37 parser in apply_uopt_hooks.py stops silently at
        FORCE_WEB_COLOR_MAX (24) entries and at its 512-byte token buffer,
        so a directive could look applied and be partly ignored
        (HOOK-HONESTY-AUDIT.md finding 3, 2026-09-26).  This applier adds
        the two checks after the parse loop: surplus entries or an
        over-long value print `[GC-COLOR] ERROR: ...` and exit(1), which
        the build reports as a compile failure.  Anchored on the Hook 37
        parse loop, so it must run after apply_uopt_hooks.py.

Part of the post-`make clean` chain (see CLAUDE.md Known Gotchas): run
AFTER apply_uopt_hooks.py and its sibling appliers, since the anchors
include hook-37-era lines (FORCE_WEB_NOSPLIT_MAX, GC_COLOR_TRACE parse,
the PRECOLOR_WEB call sites).  Each sub-applier is idempotent (probes for
its own env-var marker).

After applying, rebuild uopt (or pass --rebuild, which runs exactly this):
    cd tools/ido-static-recomp
    gcc -c -std=c11 -O0 -ggdb3 -fno-strict-aliasing -I. -o build/5.2/uopt.o build/5.2/uopt.c
    gcc -std=c11 -O0 -ggdb3 -fno-strict-aliasing -I. -o build/5.2/out/uopt \
        build/5.2/uopt.o build/5.2/version_info.o build/5.2/libc_impl_52.o -lm

Usage: apply_uopt_web_hooks.py [--rebuild] [path-to-uopt.c]
"""
import os
import subprocess
import sys

SD = os.path.dirname(os.path.abspath(__file__))
UOPT_C = os.path.join(SD, "build", "5.2", "uopt.c")


H37_LIMITS_MARKER = "[GC-COLOR] ERROR: IDO52_FORCE_WEB_COLOR"

# Anchors are the Hook 37 parse block emitted by apply_uopt_hooks.py.
H37_ANCHOR_BUF = (
    "        char buff[512];\n"
    "        char *tokf;\n"
    "        strncpy(buff, env, sizeof(buff) - 1);\n")
H37_ANCHOR_DONE = (
    "        if (ido52_force_web_color_count > 0) {\n"
    "            fprintf(stderr, \"[GC-COLOR] %d force entries configured\\n\",")

H37_LEN_CHECK = (
    "        if (strlen(env) >= sizeof(buff)) {\n"
    "            fprintf(stderr, \"" + H37_LIMITS_MARKER + " value is %u bytes; \"\n"
    "                    \"the parser holds %u and the rest would be silently \"\n"
    "                    \"dropped -- aborting\\n\",\n"
    "                    (unsigned)strlen(env), (unsigned)(sizeof(buff) - 1));\n"
    "            exit(1);\n"
    "        }\n")
H37_OVERFLOW_CHECK = (
    "        /* The loop above stops at FORCE_WEB_COLOR_MAX entries; a leftover\n"
    "           token means the directive was longer than the table and the\n"
    "           surplus would be silently ignored.  Refuse instead. */\n"
    "        if (tokf != NULL) {\n"
    "            fprintf(stderr, \"" + H37_LIMITS_MARKER + " has more than %d \"\n"
    "                    \"entries (first ignored: '%s') -- aborting; split the \"\n"
    "                    \"directive or raise FORCE_WEB_COLOR_MAX\\n\",\n"
    "                    FORCE_WEB_COLOR_MAX, tokf);\n"
    "            exit(1);\n"
    "        }\n")


def apply_h37_limits(path):
    """Hard-error on IDO52_FORCE_WEB_COLOR overflow (idempotent)."""
    with open(path, "rb") as f:
        raw = f.read()
    s = raw.decode("latin-1")
    if H37_LIMITS_MARKER in s:
        print("[uopt web hooks] Hook 37 limits already applied")
        return 0
    if s.count(H37_ANCHOR_BUF) != 1 or s.count(H37_ANCHOR_DONE) != 1:
        print("[uopt web hooks] ERROR: Hook 37 parse-loop anchors not found "
              f"(buf={s.count(H37_ANCHOR_BUF)}, done={s.count(H37_ANCHOR_DONE)}); "
              "run apply_uopt_hooks.py first")
        return 1
    s = s.replace(H37_ANCHOR_BUF,
                  H37_ANCHOR_BUF[:H37_ANCHOR_BUF.index("        strncpy")]
                  + H37_LEN_CHECK
                  + H37_ANCHOR_BUF[H37_ANCHOR_BUF.index("        strncpy"):])
    s = s.replace(H37_ANCHOR_DONE, H37_OVERFLOW_CHECK + H37_ANCHOR_DONE)
    with open(path, "wb") as f:
        f.write(s.encode("latin-1"))
    print("[uopt web hooks] Hook 37 limits (FORCE_WEB_COLOR overflow -> error) applied")
    return 0


def rebuild_uopt():
    flags = ["-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I."]
    cmds = [
        ["gcc", "-c"] + flags + ["-o", "build/5.2/uopt.o", "build/5.2/uopt.c"],
        ["gcc"] + flags + ["-o", "build/5.2/out/uopt.new", "build/5.2/uopt.o",
                           "build/5.2/version_info.o", "build/5.2/libc_impl_52.o",
                           "-lm"],
    ]
    for cmd in cmds:
        r = subprocess.run(cmd, cwd=SD, capture_output=True, text=True)
        if r.returncode != 0:
            print(r.stderr[-2000:])
            return 1
    os.replace(os.path.join(SD, "build/5.2/out/uopt.new"),
               os.path.join(SD, "build/5.2/out/uopt"))
    print("[uopt web hooks] uopt rebuilt")
    return 0


def main():
    args = [a for a in sys.argv[1:] if a != "--rebuild"]
    rebuild = "--rebuild" in sys.argv[1:]
    path = args[0] if args else UOPT_C
    if not os.path.exists(path):
        print(f"[uopt web hooks] ERROR: {path} not found. "
              "Run 'make setup-ido52' first.")
        return 1
    for script in ("apply_h38.py", "apply_h39.py", "apply_h40.py"):
        full = os.path.join(SD, "uopt_web_hooks", script)
        r = subprocess.run([sys.executable, full, path])
        if r.returncode != 0:
            print(f"[uopt web hooks] ERROR: {script} failed (anchor missing? "
                  "run the base applier chain first)")
            return 1
    if apply_h37_limits(path) != 0:
        return 1
    print(f"[uopt web hooks] Hooks 38/38b/39/40/40b applied to {path}")
    if rebuild:
        return rebuild_uopt()
    return 0


if __name__ == "__main__":
    sys.exit(main())
