#!/bin/bash
# Land one overlay's adjudicated DATA names, all-or-nothing:
#   record (tools/naming/record_overlay_data.py) -> apply_overlay_data.py --apply
#   -> PC catch-up (psx_globals.syms lines, the overlay's module regenerated)
#   -> gates -> commit.
# Gates: symbol_names check, the resolver's name table, the overlay's serial
# byte verify + reconcile with that overlay's status counts unchanged,
# plain-vs-shipped and prove_equivalence on every function whose file the
# landing touched, the clean EXN build of the overlay, a clean PC link + the
# unit tests, and a second module regeneration that must change nothing.
#
# Holds landing.lock; on any failure or kill every dirty path is restored.
# Never run it under a short timeout; use a detached runner.
#
# usage: tools/naming/land_overlay_data.sh OV DECIDED.json [...]
#   env LOGDIR (default build/naming)  env BY (provenance)  env TRAILER (commit trailer)
#   env PRE (a pre-step script run under the lock, inside the all-or-nothing
#       restore, e.g. an overlay struct pass; with no DECIDED files the
#       landing is PRE-only and the gates cover the files the pre-step changed)
#   env SUBJECT (commit subject line for a PRE-only landing)
set -u
OV=$1; shift
[ $# -ge 1 ] || [ -n "${PRE:-}" ] || { echo "usage: $0 OV DECIDED.json [...]  (or PRE=script $0 OV)"; exit 64; }
ROOT=$(cd "$(dirname "$0")/../.." && pwd)
DECIDED=(); for f in "$@"; do DECIDED+=("$(cd "$(dirname "$f")" && pwd)/$(basename "$f")"); done
LOGDIR=${LOGDIR:-$ROOT/build/naming}; BY=${BY:-overlay data blind-review}
mkdir -p "$LOGDIR"; L=$LOGDIR/odata-$OV
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
restore() {
  echo "RESTORING"
  git reset -q
  git status --short | grep -v '^??' | cut -c4- | xargs -r git checkout HEAD --
  git status --short | grep '^??' | cut -c4- | grep -E '^(src|include|asm|pc|config)/' | xargs -r rm -rf
}
exec >>"$L.log" 2>&1
trap 'restore; exit 7' TERM INT HUP
echo "== overlay data $OV $(date)"
parts_banner
# FAST=1: rename and sync only; the full battery runs once after the
# whole set of renames -- every gate is skipped here
if [ "${FAST:-0}" = 1 ]; then gate() { echo "-- $1 (skipped: FAST)"; }
else gate() { local name=$1; shift; echo "-- $name"; if ! bash -c "$*" > "$L.$name.out" 2>&1; then echo "FAIL $name"; tail -20 "$L.$name.out"; restore; exit 4; fi; echo "PASS $name"; }
fi
# a gate of another part runs only when that part is in this checkout
pc_gate() { if [ $HAVE_PC = 1 ]; then gate "$@"; else part_skip pc "gate $1"; fi; }
en_gate() { if [ $HAVE_ENGLISH = 1 ]; then gate "$@"; else part_skip english "gate $1"; fi; }

PRE_ALLOW=""
if [ -n "${PRE:-}" ]; then
  bash "$PRE" || { echo "PRE FAILED"; restore; exit 8; }
  # the pre-step's edits belong to this landing: let the applier touch them
  for f in $(git status --short | grep -v '^??' | cut -c4-); do PRE_ALLOW="$PRE_ALLOW --allow-locked $f"; done
fi
N=0; ONLY=""
if [ ${#DECIDED[@]} -gt 0 ]; then
TOKI_LOCK_HELD=1 python3 tools/naming/record_overlay_data.py "${DECIDED[@]}" --ov "$OV" --by "$BY" --apply || { restore; exit 2; }
ONLY=$(python3 - "$OV" "${DECIDED[@]}" <<'PY'
import json, sys
ov, a = sys.argv[1], []
for f in sys.argv[2:]:
    for r in json.load(open(f)):
        if r.get('decision') != 'LAND':
            continue
        k = r.get('id') or r.get('addr')
        if ':' in k and not k.startswith(ov + ':'):
            continue
        a.append(f"{ov}:0x{int(k.split(':')[-1], 16):08X}")
print(' '.join('--only ' + x for x in a))
PY
)
[ -n "$ONLY" ] || { echo "no LAND rows for $OV"; restore; exit 3; }
python3 tools/naming/apply_overlay_data.py --ov "$OV" --apply $ONLY $PRE_ALLOW > "$L.apply.out" 2>&1
grep -E '^(REFUSE|LAND|SURVIVOR)' "$L.apply.out" | head -100
N=$(grep -c '^LAND ' "$L.apply.out")
if [ "$N" -eq 0 ] || grep -q '^SURVIVOR' "$L.apply.out"; then echo "nothing landed or survivors"; restore; exit 3; fi
fi
# every changed source, the pre-step's included (the gates prove each function)
TOUCHED=$(git status --short | grep -v '^??' | cut -c4-)
[ -n "$TOUCHED" ] || { echo "nothing changed"; restore; exit 3; }
# only files that define their own function: a data-only file (RENSYU's
# rensyu_main_data.c) is not a function and would read as UNMEASURABLE
FUNCS=$(for p in $TOUCHED; do case $p in src/overlay/$OV/*.c) n=$(basename "$p" .c); [ -f "$p" ] && grep -qE "\b${n}[[:space:]]*\(" "$p" && echo "--only $OV/$n";; esac; done | sort -u | tr '\n' ' ')

if [ $HAVE_PC = 1 ]; then
[ -z "$ONLY" ] || python3 tools/naming/sync_psx_globals.py $ONLY > "$L.psx.out" 2>&1 || { cat "$L.psx.out"; restore; exit 5; }
python3 pc/tools/generate_overlay_modules.py "$OV" > "$L.gen.out" 2>&1 || { tail "$L.gen.out"; restore; exit 5; }
else
  part_skip pc "tools/naming/sync_psx_globals.py, pc/tools/generate_overlay_modules.py $OV"
fi

# English patch (src/patch, config/patch): keep its names and the name->address
# map in step with this landing, then the fast patch gate (compile + link +
# hook read-back + text gates, ~45 s warm; tools/patch/fast_check.py)
if [ $HAVE_ENGLISH = 1 ]; then
python3 tools/patch/sync_patch_symbols.py --apply > "$L.patchsync.out" 2>&1 || { tail -20 "$L.patchsync.out"; restore; exit 5; }
else
  part_skip english "tools/patch/sync_patch_symbols.py --apply"
fi
gate names      "python3 tools/analysis/symbol_names.py check"
# build_patched reuses build/patch/gen/dte.json across runs; a stale one
# makes a row "not fit" and fails the gate for nothing (2026-11-09)
en_gate patch   "rm -f build/patch/gen/dte.json && python3 tools/patch/fast_check.py"
gate name_table "python3 tools/overlay/resolve_overlay_relocs.py --check-name-table"
gate verify     "python3 tools/overlay/verify_overlay_bytes.py $OV -j 1 > $L.v 2>&1; grep -q 'Count mismatch: 0' $L.v && grep -q 'Errors:         0' $L.v"
gate reconcile  "python3 tools/overlay/apply_resolved_status.py $OV --demote > $L.rec 2>&1; grep -q 'Demoted codegen-residual reloc_only -> semi_matched: 0' $L.rec && ! grep -q 'Pruned [1-9]' $L.rec"
gate status     "python3 - $OV <<'PY'
import json,subprocess,collections,sys
ov=sys.argv[1]
h=json.loads(subprocess.run(['git','show','HEAD:config/overlay_match_status.json'],capture_output=True,text=True).stdout)
n=json.load(open('config/overlay_match_status.json'))
c=lambda d: collections.Counter(v.get('status') for v in d.get(ov,{}).values())
print(c(h), c(n)); raise SystemExit(0 if c(h)==c(n) else 1)
PY"
git checkout -- config/overlay_match_status.json config/clean_build_debt.json 2>/dev/null
gate plain      "python3 tools/analysis/audit_plain_vs_shipped.py --jobs $(nproc) $FUNCS"
gate prove      "python3 tools/analysis/prove_equivalence.py --jobs $(nproc) $FUNCS > $L.prove 2>&1; ! grep -E 'UNPROVEN [1-9]|REFUTED [1-9]|UNMEASURABLE [1-9]' $L.prove"
gate clean_exn  "python3 tools/overlay/build_clean_exn.py $OV --verify"
git checkout -- config/clean_build_debt.json 2>/dev/null
pc_gate pc      "python3 pc/tools/gen_ps1_alias_refs.py && make -f pc/Makefile clean >/dev/null && make -j$(nproc) -f pc/Makefile all && make -j$(nproc) -f pc/Makefile test > $L.pctest 2>&1; tail -1 $L.pctest | grep -q ' 0 failed'"
# a real new game on the PC port, the way a player runs it (enhanced profile;
# new_game in Japanese -- its name checks are kana -- and the English tour): the unit tests cannot see a PC-only crash in the game code (the
# 2026-11-07 class_w layout crash at name entry passed every other gate)
# the Windows exe builds too (MinGW; the player's main platform): a file the
# Linux build compiles can still fail there (2026-11-07: <SDL.h> in main.c)
pc_gate windows  "make -j$(nproc) -f pc/Makefile windows"
pc_gate playthrough "python3 pc/tools/playthrough.py pc/tools/playthrough/new_game.txt --profile enhanced --out $L.pt > $L.pt.out 2>&1 && grep -q '^PASS' $L.pt.out && ! grep -q '\\[CRASH\\]' $L.pt/*.pc.log && python3 pc/tools/playthrough.py pc/tools/playthrough/english_tour.txt --profile enhanced --set TOKI_LANG=en --out $L.pten > $L.pten.out 2>&1 && grep -q '^PASS' $L.pten.out && ! grep -q '\\[CRASH\\]' $L.pten/*.pc.log"
[ $HAVE_PC = 1 ] && cp "pc/src/overlay_$OV.c" "$L.module.c"
pc_gate regen   "python3 pc/tools/generate_overlay_modules.py $OV >/dev/null && cmp -s pc/src/overlay_$OV.c $L.module.c"

# spelling-keyed places a rename can bypass (pc/Makefile swaps, plan keys,
# patch compositions, tracked asm/PC overrides): tools/naming/check_name_keys.py
python3 tools/naming/check_name_keys.py --fix-makefile > "${LOGDIR:-build/naming}/keys-$OV.out" 2>&1 || { cat "${LOGDIR:-build/naming}/keys-$OV.out"; restore; echo "NAME KEYS FAILED"; exit 7; }
git diff --quiet -- pc/Makefile || PATHS="$PATHS pc/Makefile"

MSG=$(mktemp)
printf 'Land %s %s overlay data names (%s)\n\nAdjudicated overlay data names landed with tools/naming/apply_overlay_data.py\nthrough tools/naming/land_overlay_data.sh. Gates: name table, serial byte\nverify + reconcile with the overlay status unchanged, plain-vs-shipped and\nprove on every touched function, clean EXN build, PC clean link + unit\ntests, module regeneration idempotent.\n' "$N" "$OV" "$BY" > "$MSG"
[ -n "${SUBJECT:-}" ] && { printf '%s\n\nLanded through tools/naming/land_overlay_data.sh (PRE=%s). Gates: name table,\nserial byte verify + reconcile with the overlay status unchanged,\nplain-vs-shipped and prove on every touched function, clean EXN build, PC\nclean link + unit tests, Windows build, playthroughs, module regeneration\nidempotent.\n' "$SUBJECT" "$PRE" > "$MSG"; }
[ -n "${TRAILER:-}" ] && printf '\n%s\n' "$TRAILER" >> "$MSG"
PATHS=$(git status --short | cut -c4- | grep -E '^(src|include|pc|config)/')
git add -A -- $PATHS
git commit -q -F "$MSG" -- $PATHS || { restore; echo "COMMIT FAILED"; exit 9; }
git log -1 --oneline
rm -f "$MSG"
