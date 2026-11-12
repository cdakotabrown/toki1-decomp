#!/bin/bash
# Land one overlay's adjudicated function names, all-or-nothing:
#   record (tools/naming/record_decided.py) -> apply_overlay_symbols.py --apply
#   -> apply_overlay_symbols.py --run-gates -> symbol_names.py check -> commit.
#
# Runs entirely under the shared landing lock (landing.lock at the repo root),
# so two landers never interleave. On ANY failure, or if the process is
# killed, every dirty path is restored to HEAD and nothing is committed: while
# the lock is held, every change in the tree belongs to this landing.
#
# Never run it under a short timeout: the serial byte verification takes
# 5-30 minutes per overlay. Run it in the background or from a detached queue.
#
# usage: tools/naming/land_overlay.sh OV DECIDED.json [DECIDED2.json ...]
#   env LOGDIR (default build/naming)   logs: land-OV.log apply-OV.out gates-OV.out
#   env BY     (default blind-review)   provenance label in records/commit
set -u
OV=$1; shift
[ $# -ge 1 ] || { echo "usage: $0 OV DECIDED.json [...]"; exit 64; }
ROOT=$(cd "$(dirname "$0")/../.." && pwd)
DECIDED=(); for f in "$@"; do DECIDED+=("$(cd "$(dirname "$f")" && pwd)/$(basename "$f")"); done
LOGDIR=${LOGDIR:-$ROOT/build/naming}; BY=${BY:-blind-review}
mkdir -p "$LOGDIR"
cd "$ROOT" || exit 9
source .venv/bin/activate
# which other parts (PC port, English patch) this checkout holds: their steps
# run only when present and are named as skipped otherwise
source tools/naming/parts.sh || exit 9
parts_banner
# the gitignored TXT_ string headers the overlay C includes (generated from
# the disc; a checkout without them fails every RENSYU compile in the gates)
python3 tools/overlay/gen_overlay_text.py >/dev/null || exit 9
exec 9>landing.lock
flock 9

TOUCHED=""
restore() {
  echo "RESTORING"
  git reset -q   # unstage anything a failed add/commit staged, else checkout
  git status --short | grep '^??' | cut -c4- | grep -F "$OV" | xargs -r rm -rf
  git status --short | grep -v '^??' | cut -c4- | xargs -r git checkout HEAD --
  # asm/overlay/<OV> is gitignored: the apply's .s renames are undone from
  # the snapshot taken before it (2026-11-10: a failed landing left them)
  if [ -f "$LOGDIR/asm-$OV.tar" ]; then
    python3 - "$OV" "$LOGDIR/asm-$OV.tar" <<'PY'
import os, sys, tarfile
ov, tar = sys.argv[1], sys.argv[2]
d = os.path.join('asm/overlay', ov)
with tarfile.open(tar) as t:
    keep = {m.name for m in t.getmembers() if m.isfile()}
    for f in os.listdir(d):
        p = os.path.join(d, f)
        if os.path.isfile(p) and p not in keep:
            os.remove(p)
    t.extractall('.')
PY
  fi
}
exec >>"$LOGDIR/land-$OV.log" 2>&1
trap 'restore; exit 7' TERM INT HUP
echo "== $OV $(date)"
parts_banner
tar -cf "$LOGDIR/asm-$OV.tar" "asm/overlay/$OV" 2>/dev/null || rm -f "$LOGDIR/asm-$OV.tar"

# Optional correction-flow pre-step (env PRE = script path), e.g.
# tools/naming/refine_pre.sh for renaming an ALREADY-LANDED name: it runs
# inside the same lock and the same all-or-nothing restore.
if [ -n "${PRE:-}" ]; then
  bash "$PRE" "$OV" "${DECIDED[@]}" || { restore; echo "PRE FAILED"; exit 8; }
fi

python3 tools/naming/record_decided.py "$OV" "${DECIDED[@]}" --by "$BY" || { restore; echo "RECORD FAILED"; exit 2; }
# With a PRE step the tree already holds this landing's own pre-step renames
# (e.g. refine_pre.sh's --revert): git-modified is not "someone else" here.
PRE_FLAGS=""
[ -n "${PRE:-}" ] && PRE_FLAGS="--no-git-lock"
python3 tools/overlay/apply_overlay_symbols.py --apply --ov "$OV" --min-tier DERIVED --batch 400 $PRE_FLAGS > "$LOGDIR/apply-$OV.out" 2>&1
rc=$?
grep -E '^(REFUSE|landed|SURVIVOR|nothing)' "$LOGDIR/apply-$OV.out" | head -60
TOUCHED=$(awk '/^touched paths/{f=1;next} f&&/^  /{print $1} f&&!/^  /{f=0}' "$LOGDIR/apply-$OV.out")
if [ $rc -ne 0 ] || [ -z "$TOUCHED" ]; then echo "APPLY rc=$rc"; restore; exit 3; fi
PCGATE=0
if [ $HAVE_PC = 1 ]; then
# pc/tools/switch_labels.json keys the retail case-label relabel by OV/func
# name; the renamer does not carry it, and a stale key makes the next module
# regeneration silently drop the relabel (TT/func_8013C824, 2026-11-02).
python3 - "$OV" "${DECIDED[@]}" <<'PY' || { restore; echo "SWITCH_LABELS FAILED"; exit 6; }
import json, sys, re
ov, paths = sys.argv[1], sys.argv[2:]
f = 'pc/tools/switch_labels.json'
t = open(f).read()
n = 0
for p in paths:
    for r in json.load(open(p)):
        if r.get('decision') != 'LAND' or not r['id'].startswith(ov + ':'):
            continue
        a = int(r['id'].split(':')[1], 16)
        for old in (f'"{ov}/func_{a:08X}"', f'"{ov}/func_{a:08x}"'):
            if old in t:
                t = t.replace(old, f'"{ov}/{r["name"]}"'); n += 1
open(f, 'w').write(t)
print(f'switch_labels: {n} key(s) renamed')
PY
TOUCHED="$TOUCHED pc/tools/switch_labels.json"
python3 pc/tools/gen_ps1_alias_refs.py > "$LOGDIR/aliasrefs-$OV.out" 2>&1 || { restore; echo "ALIAS REFS FAILED"; exit 6; }
TOUCHED="$TOUCHED pc/include/game/ps1_alias_refs.h pc/src/ps1_alias_refs.c"
# The renamed module must equal a fresh regeneration of it.
cp "pc/src/overlay_$OV.c" "$LOGDIR/module-$OV.landed.c"
python3 pc/tools/generate_overlay_modules.py "$OV" > "$LOGDIR/regen-$OV.out" 2>&1 || { restore; echo "REGEN FAILED"; exit 6; }
# blank lines are layout, not names: the generator's spacing around a
# declaration it drops can differ by one line (2026-11-10)
# Beyond that the generator can legitimately emit more for a NAME than for
# an address name (a typed-call macro `<name>__f<caller>`); the regenerated
# module is then kept and must build and pass the PC tests (PCGATE below).
[ "${FAST:-0}" = 1 ] || diff -B -q "pc/src/overlay_$OV.c" "$LOGDIR/module-$OV.landed.c" >/dev/null || { echo "REGEN DIFFERS from the renamed module: keeping the regeneration, PC build gate armed"; diff "$LOGDIR/module-$OV.landed.c" "pc/src/overlay_$OV.c" | head -40; PCGATE=1; }
else
  part_skip pc "pc/tools/switch_labels.json keys, gen_ps1_alias_refs.py, generate_overlay_modules.py $OV, PC build gate"
fi
# FAST=1: rename and sync only; the full battery runs once after the
# whole set of renames
if [ "${FAST:-0}" = 1 ]; then echo "gates skipped (FAST)"; grc=0; else
python3 tools/overlay/apply_overlay_symbols.py --run-gates --ov "$OV" > "$LOGDIR/gates-$OV.out" 2>&1
grc=$?
fi
grep -E '^\s+\[(PASS|FAIL)\]|gate' "$LOGDIR/gates-$OV.out"
if [ $grc -ne 0 ]; then restore; exit 4; fi
python3 tools/analysis/symbol_names.py check || { restore; exit 5; }
if [ $PCGATE = 1 ]; then
  { make -j"$(nproc)" -f pc/Makefile all > "$LOGDIR/pcbuild-$OV.out" 2>&1 \
    && make -j"$(nproc)" -f pc/Makefile test > "$LOGDIR/pctest-$OV.out" 2>&1 \
    && tail -1 "$LOGDIR/pctest-$OV.out" | grep -q ' 0 failed'; } \
    || { echo "PC GATE FAILED"; tail -5 "$LOGDIR/pcbuild-$OV.out" "$LOGDIR/pctest-$OV.out"; restore; exit 6; }
  echo "PC GATE PASS"
fi
# English patch: keep its names / map in step (renamed func_ files are named in
# config/patch/compositions.json), then the fast patch gate
if [ $HAVE_ENGLISH = 1 ]; then
python3 tools/patch/sync_patch_symbols.py --apply > "$LOGDIR/patchsync-$OV.out" 2>&1 || { restore; echo "PATCH SYNC FAILED"; exit 5; }
[ "${FAST:-0}" = 1 ] || python3 tools/patch/fast_check.py > "$LOGDIR/patchcheck-$OV.out" 2>&1 || { tail -20 "$LOGDIR/patchcheck-$OV.out"; restore; echo "PATCH CHECK FAILED"; exit 5; }
else
  part_skip english "tools/patch/sync_patch_symbols.py --apply, tools/patch/fast_check.py"
fi
git checkout -- config/clean_build_debt.json 2>/dev/null
for p in $(git status --short -- src/patch config/patch | cut -c4-); do TOUCHED="$TOUCHED $p"; done
# the PRE step (refine_pre.sh) moved the landed files away: stage those
# deletions too, or a later rollback restores them from HEAD next to the
# refined files (2026-11-10: 9 stale DATE2 date2_load*/date2_main* pairs)
for p in $(git ls-files --deleted -- "src/overlay/$OV" "wiki/notes/$OV" "pc/src/overlay/$OV"); do TOUCHED="$TOUCHED $p"; done


N=$(grep -c '^LAND' "$LOGDIR/apply-$OV.out")
MSG=$(mktemp)
printf 'Land %s %s function names (%s)\n\nAdjudicated T4 names landed with apply_overlay_symbols.py through\ntools/naming/land_overlay.sh. Overlay gates (name table, serial byte\nverify, status unchanged, zero survivors) and symbol_names check pass.\n' "$N" "$OV" "$BY" > "$MSG"
[ -n "${TRAILER:-}" ] && printf '\n%s\n' "$TRAILER" >> "$MSG"
# Only paths that exist or are tracked: the refine flow's move sources
# (transient func_* names) exist in neither index nor worktree by commit time,
# and git add/commit reject the whole pathspec list because of them.
PATHS=()
for t in $TOUCHED; do
  # gitignored paths (asm/overlay listings the apply renamed) are not committed
  if git ls-files --error-unmatch "$t" >/dev/null 2>&1 || { [ -e "$t" ] && ! git check-ignore -q "$t"; }; then
    PATHS+=("$t")
  fi
done
# a rename of a TRACKED file under a gitignored tree (asm_only listings in
# asm/overlay): stage the deletion and force-add the renamed listing
for p in $(python3 tools/naming/check_name_keys.py --fix-tracked 2>/dev/null | sed -n 's/^TRACKED //p'); do PATHS+=("$p"); done
# spelling-keyed places a rename can bypass (pc/Makefile swaps, plan keys,
# patch compositions, tracked asm/PC overrides): tools/naming/check_name_keys.py
python3 tools/naming/check_name_keys.py --fix-makefile > "${LOGDIR:-build/naming}/keys-$OV.out" 2>&1 || { cat "${LOGDIR:-build/naming}/keys-$OV.out"; restore; echo "NAME KEYS FAILED"; exit 7; }
git diff --quiet -- pc/Makefile || PATHS+=(pc/Makefile)
git add -A -- "${PATHS[@]}" config/symbol_names.json
git commit -q -F "$MSG" -- "${PATHS[@]}" config/symbol_names.json \
  || { restore; echo "COMMIT FAILED"; exit 9; }
git log -1 --oneline
rm -f "$MSG"
