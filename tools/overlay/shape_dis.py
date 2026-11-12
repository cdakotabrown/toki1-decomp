#!/usr/bin/env python3
#
# Written 2026-08-14. Part of the
# mnemonic-SHAPE alignment harness: shape_tdis.py (target disasm) + shape_dis.py
# (explicit-env-only compile, dodges the @CompileEnv prose-scrape trap) +
# shape_align.py (difflib opcode-stream alignment) + shape_sbs.py (side-by-side).
# Shape alignment finds real holes that exact%% hides — exact%% moves wholesale on
# allocation shifts and rewards count-compensating hooks. Intermediates in /tmp.
"""dis.py <OV> <FUNC> [ENV=...]...  -- direct compile with EXPLICIT env only.

Strips @PostProc directive lines, IGNORES any @CompileEnv in the file (prose
trap), applies only the env given on the command line.  Prints ELF symbol size
and writes the disassembled body to /tmp/<FUNC>.dis (one insn per line).
"""
import sys, os, re, subprocess

ROOT = "."
os.chdir(ROOT)

ov, fn = sys.argv[1], sys.argv[2]
envs = {}
extra_flags = []
for a in sys.argv[3:]:
    if a.startswith('-'):
        extra_flags.append(a)
    else:
        k, v = a.split('=', 1)
        envs[k] = v

src = f"src/overlay/{ov}/{fn}.c"
text = open(src).read()
# ⚠️ Only STRICT directive lines may be stripped.  A header that merely
# MENTIONS "@PostProc:"/"@CompileEnv:" in prose used to be deleted too, and
# when such a line opened a comment block the block was left unterminated and
# the whole function stopped compiling (six files hit this on 2026-08-23).
# parse_postproc.py applies the same rule; keep them in step.
_DIRECTIVE = re.compile(r'^\s*(?:/\*|\*)\s*@(?:PostProc|CompileEnv):')
out = []
for ln in text.splitlines():
    if _DIRECTIVE.match(ln):
        op = ln.lstrip().startswith("/*")
        cl = ln.rstrip().endswith("*/")
        if op and cl:
            continue
        out.append("/* */" if op and not cl else ("*/" if cl and not op else ""))
        continue
    out.append(ln)
tmpc = f"/tmp/dis_{fn}.c"
open(tmpc, "w").write("\n".join(out) + "\n")

# @CompileFlags (excluding pic which we control)
# Same rule for @CompileFlags: a prose mention is NOT a directive.
cf = re.findall(r'(?m)^\s*(?:/\*|\*)\s*@CompileFlags:\s*([^*\n]*)', text)
flags = []
for g in cf:
    flags += g.split()
picflag = "-Wc,-pic1"
for f in list(flags):
    if f.startswith("-Wc,-pic"):
        picflag = f
        flags.remove(f)
for f in extra_flags:
    if f.startswith("-Wc,-pic"):
        picflag = f
        extra_flags.remove(f)

env = dict(os.environ)
for k in list(env):
    if k.startswith("IDO52_"):
        del env[k]
env.update(envs)

obj = f"/tmp/dis_{fn}.o"
cmd = ["./tools/ido-static-recomp/build/5.2/out/cc", "-O2", "-G0", "-mips1",
       "-EL", "-non_shared", picflag] + flags + extra_flags + \
      ["-Iinclude", "-Iinclude/psyq", "-c", tmpc, "-o", obj]
p = subprocess.run(cmd, env=env, capture_output=True, text=True)
if p.returncode != 0:
    sys.stderr.write(p.stdout + p.stderr)
    sys.exit(1)

# symbol size
r = subprocess.run(["mips-linux-gnu-readelf", "-sW", obj], capture_output=True, text=True)
size = None
for l in r.stdout.splitlines():
    parts = l.split()
    if len(parts) >= 8 and parts[-1] == fn:
        size = int(parts[2])
print(f"ELF symbol size = {size} bytes = {size//4 if size else '?'} words")

r = subprocess.run(["mips-linux-gnu-objdump", "-d", "-EL", obj],
                   capture_output=True, text=True)
lines = []
started = False
for l in r.stdout.splitlines():
    m = re.match(r'^\s*([0-9a-f]+):\t([0-9a-f]{8}) \t(.*)$', l)
    if m:
        lines.append((int(m.group(1), 16), m.group(2), m.group(3).strip()))
# body = words within [0, size) offset of func symbol; func starts where?
# find symbol value
r2 = subprocess.run(["mips-linux-gnu-readelf", "-sW", obj], capture_output=True, text=True)
val = 0
for l in r2.stdout.splitlines():
    parts = l.split()
    if len(parts) >= 8 and parts[-1] == fn:
        val = int(parts[1], 16)
body = [(a, w, t) for (a, w, t) in lines if val <= a < val + (size or 10**9)]
with open(f"/tmp/{fn}.dis", "w") as f:
    for a, w, t in body:
        f.write(f"{(a-val)//4:5d} {w} {t}\n")
print(f"body words = {len(body)}   -> /tmp/{fn}.dis")
