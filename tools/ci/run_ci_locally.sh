#!/bin/bash
# Run the GitHub Actions workflow (.github/workflows/build.yaml) locally, in a
# throwaway clean checkout of a commit, before pushing it.
#
# A clean checkout is the point: CI clones only tracked files, so an untracked
# header, a gitignored generated file or a local-only tool that the main
# checkout happens to have is exactly what breaks CI and never breaks here.
#
# usage: tools/ci/run_ci_locally.sh [REV] [JOB ...]
#   REV   commit to test (default HEAD; commit first, uncommitted edits are
#         NOT tested)
#   JOB   lint | postproc | compile | pc   (default: all four, in that order;
#         `pc` only in a checkout that holds the PC port, pc/Makefile)
#   env KEEP=1 leaves the checkout in place (path printed) for debugging
#
# Mirrors the workflow step for step. If you change build.yaml, change this
# script in the same commit. The same script serves the monorepo and the
# exported toki1-decomp repository: the `pc` job and
# the pc/tools + translation/reviewer lint paths apply only where those parts
# are tracked, and say so when they are skipped. Differences, on purpose:
#   * the IDO recompiler build is copied from this checkout's
#     tools/ido-static-recomp/build/5.2 when present (CI builds it from the
#     vendored sources with `make setup-ido52`, ~10 min); set FRESH_IDO=1 to
#     build it from scratch exactly as CI does;
#   * the host's own Python/venv is used (CI pins 3.11).
set -u
ROOT=$(cd "$(dirname "$0")/../.." && pwd)
REV=${1:-HEAD}; shift || true
SHA=$(git -C "$ROOT" rev-parse --verify "$REV^{commit}") || exit 2
# Parts of the project this commit holds (tracked files, not the work tree).
has() { [ -n "$(git -C "$ROOT" ls-tree --name-only "$SHA" -- "$1")" ]; }
DEFAULT_JOBS="lint postproc compile"
if has pc/Makefile; then DEFAULT_JOBS="$DEFAULT_JOBS pc"
else echo "-- no pc/Makefile at $REV: the PC port is not part of this repository, skipping the pc job"; fi
JOBS=${*:-$DEFAULT_JOBS}
LINT_DIRS=tools
for d in pc/tools translation/reviewer; do
  if has "$d"; then LINT_DIRS="$LINT_DIRS $d"; else echo "-- no $d at $REV: not byte-compiled"; fi
done
SH_DIRS=tools
for d in pc translation; do has "$d" && SH_DIRS="$SH_DIRS $d"; done
DIR=$(mktemp -d "${TMPDIR:-/tmp}/toki-ci.XXXXXX")
cleanup() { [ "${KEEP:-0}" = 1 ] && { echo "kept: $DIR"; return; }
            git -C "$ROOT" worktree remove --force "$DIR" 2>/dev/null; rm -rf "$DIR"; }
trap cleanup EXIT
git -C "$ROOT" worktree add -q --detach "$DIR" "$SHA" || exit 2
cd "$DIR" || exit 2
echo "== CI on $(git log -1 --oneline) in $DIR"
PY=python3
[ -x "$ROOT/.venv/bin/python" ] && PY="$ROOT/.venv/bin/python"
FAIL=""
step() { local name=$1; shift; echo "-- $name"; if ! bash -c "$*"; then echo "FAIL: $name"; FAIL="$FAIL $name"; fi; }

for job in $JOBS; do case $job in
lint)
  step "byte-compile Python" "$PY -m compileall -q -x '(\\.venv|build|node_modules|__pycache__|\\.git)' $LINT_DIRS"
  step "shell syntax" "find $SH_DIRS -name '*.sh' -print0 | xargs -0 -r -n1 bash -n"
  step "tracked JSON parses" "$PY - <<'PY'
import json, subprocess
files = subprocess.run(['git', 'ls-files', '*.json'], capture_output=True, text=True, check=True).stdout.split()
bad = []
for p in files:
    try:
        json.load(open(p, encoding='utf-8'))
    except Exception as e:
        bad.append('%s: %s' % (p, e))
print('%d JSON files parsed, %d failed' % (len(files), len(bad)))
for b in bad:
    print('BAD', b)
raise SystemExit(1 if bad else 0)
PY" ;;
postproc)
  step "PostProc ban suite" "$PY tools/build/test_banned_postproc.py" ;;
compile)
  command -v mipsel-linux-gnu-as >/dev/null || { echo "FAIL: mipsel-linux-gnu-as missing (apt install binutils-mipsel-linux-gnu)"; FAIL="$FAIL compile"; continue; }
  if [ "${FRESH_IDO:-0}" != 1 ] && [ -x "$ROOT/tools/ido-static-recomp/build/5.2/out/cc" ]; then
    mkdir -p tools/ido-static-recomp/build && cp -a "$ROOT/tools/ido-static-recomp/build/5.2" tools/ido-static-recomp/build/
  else
    step "build IDO 5.2 (make setup-ido52)" "make setup-ido52"
  fi
  step "compile matched C" "$PY tools/build/ci_compile_check.py" ;;
pc)
  step "fetch SPU core" "pc/3rd/spu-glue/fetch.sh"
  step "PC build" "make -f pc/Makefile all -j$(nproc)"
  step "PC unit tests (no disc)" "PC_TESTS_NO_DISC=1 make -f pc/Makefile test" ;;
*) echo "unknown job $job"; exit 64 ;;
esac; done

if [ -n "$FAIL" ]; then echo "== CI WOULD FAIL:$FAIL"; exit 1; fi
echo "== CI would pass ($JOBS)"
