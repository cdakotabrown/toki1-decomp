#!/bin/bash
# Land a batch of adjudicated MAIN data names, all-or-nothing:
#   record (tools/naming/record_data_decided.py) -> apply_data_symbols.py --apply
#   -> the data lander's gates, actually run (its --gates only prints them):
#      symbol_names check + gen-retail --check, build-c retail SHA1 with 0
#      reloc refusals, check-raw, check-clean-main, main relocs + reloc positions,
#      registry overlaps, overlay name table, whole-tree plain-vs-shipped and
#      prove_equivalence, serial verify of every overlay + reconcile with no
#      status change, then the PC catch-up (sync + module generator + clean
#      link + unit tests) -> commit.
#
# Holds landing.lock throughout; on any failure or kill every dirty path is
# restored to HEAD (main data is global, so a landing touches the whole tree).
# Never run it under a short timeout; use a detached runner.
#
# usage: tools/naming/land_data.sh LABEL [DECIDED.json ...]
#   (no decision files: a PRE-only landing, e.g. an interior-token rewrite pass)
#   env LOGDIR (default build/naming)  env BY (provenance)  env TRAILER (commit trailer)
#   env PRE (a pre-step script run under the lock, inside the all-or-nothing restore)
#   env SUBJECT (commit subject line; default "Land N main data names (LABEL)")
#   env PSX_EXTRA (extra --only addresses for the psx_globals.syms catch-up)
set -u
LABEL=$1; shift
ROOT=$(cd "$(dirname "$0")/../.." && pwd)
DECIDED=(); for f in "$@"; do DECIDED+=("$(cd "$(dirname "$f")" && pwd)/$(basename "$f")"); done
LOGDIR=${LOGDIR:-$ROOT/build/naming}; BY=${BY:-data blind-review}
mkdir -p "$LOGDIR"; L=$LOGDIR/data-${LABEL//[^A-Za-z0-9._-]/_}  # gates splice $L unquoted: keep it free of spaces
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
  git status --short | grep -v '^??' | cut -c4- | xargs -r git checkout HEAD --
  git status --short | grep '^??' | cut -c4- | grep -E '^(src|include|asm|pc|config)/' | xargs -r rm -rf
}
exec >>"$L.log" 2>&1
trap 'restore; exit 7' TERM INT HUP
echo "== data $LABEL $(date)"
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
  # the pre-step's own edits are this landing's, not someone else's: without
  # this the data lander refuses every name that shares a file with them
  for f in $(git status --short | grep -v '^??' | cut -c4-); do PRE_ALLOW="$PRE_ALLOW --allow-locked $f"; done
fi
ADDRS=""; N=0
if [ ${#DECIDED[@]} -gt 0 ]; then
  python3 tools/naming/record_data_decided.py "${DECIDED[@]}" --by "$BY" || { restore; exit 2; }
  ADDRS=$(python3 - "${DECIDED[@]}" <<'PY'
import json,sys
a=[]
for f in sys.argv[1:]:
    a+=[f"0x{int(r['addr'],16):08X}" for r in json.load(open(f)) if r.get('decision')=='LAND']
print(' '.join('--only '+x for x in a))
PY
  )
  python3 tools/analysis/apply_data_symbols.py --apply --min-tier DERIVED --batch 1000 $ADDRS $PRE_ALLOW > "$L.apply.out" 2>&1
  rc=$?
  grep -E '^(REFUSE|LAND|landed)' "$L.apply.out" | head -200
  N=$(grep -c '^LAND ' "$L.apply.out")
  if [ "$N" -eq 0 ]; then echo "nothing landed (rc=$rc)"; restore; exit 3; fi
elif [ -z "$PRE_ALLOW" ]; then
  echo "no decisions and no PRE changes"; restore; exit 3
else
  echo "PRE-only landing: $(echo $PRE_ALLOW | wc -w) files"
fi

# English patch (src/patch, config/patch): keep its names and the name->address
# map in step with this landing, then the fast patch gate (compile + link +
# hook read-back + text gates, ~45 s warm; tools/patch/fast_check.py)
if [ $HAVE_ENGLISH = 1 ]; then
python3 tools/patch/sync_patch_symbols.py --apply > "$L.patchsync.out" 2>&1 || { tail -20 "$L.patchsync.out"; restore; exit 5; }
else
  part_skip english "tools/patch/sync_patch_symbols.py --apply"
fi
gate names       "python3 tools/analysis/symbol_names.py check && python3 tools/analysis/symbol_names.py gen-retail --check"
# build_patched reuses build/patch/gen/dte.json across runs; a stale one
# makes a row "not fit" and fails the gate for nothing (2026-11-09)
en_gate patch    "rm -f build/patch/gen/dte.json && python3 tools/patch/fast_check.py"
gate build_c     "python3 tools/build/build_level3_c.py > $L.build_c.full 2>&1; grep -q 'Output SHA1:       2d8d474816e4bf937b09dc3b017f550f7ce4e14b' $L.build_c.full && grep -q 'Reloc refusals:    0' $L.build_c.full"
gate check_raw   "make check-raw"
gate clean_main  "make check-clean-main"
gate main_relocs "python3 tools/analysis/audit_main_relocs.py"
gate reloc_pos   "python3 tools/analysis/audit_reloc_positions.py --all"
gate overlaps    "python3 tools/analysis/audit_registry_overlaps.py"
gate name_table  "python3 tools/overlay/resolve_overlay_relocs.py --check-name-table"
gate plain       "python3 tools/analysis/audit_plain_vs_shipped.py --jobs $(nproc)"
gate prove       "python3 tools/analysis/prove_equivalence.py --jobs $(nproc)"
for ov in $(ls config/overlay/*.sym | xargs -n1 basename | sed 's/\.sym$//'); do
  gate "verify_$ov" "python3 tools/overlay/verify_overlay_bytes.py $ov -j 1 > $L.v.$ov 2>&1; grep -q 'Count mismatch: 0' $L.v.$ov && grep -q 'Errors:         0' $L.v.$ov"
done
gate reconcile   "python3 tools/overlay/apply_resolved_status.py --demote > $L.rec 2>&1; grep -q 'Demoted codegen-residual reloc_only -> semi_matched: 0' $L.rec && ! grep -q 'Pruned [1-9]' $L.rec"
gate status      "python3 - <<'PY'
import json,subprocess,collections
h=json.loads(subprocess.run(['git','show','HEAD:config/overlay_match_status.json'],capture_output=True,text=True).stdout)
n=json.load(open('config/overlay_match_status.json'))
c=lambda d: collections.Counter(v.get('status') for o in d.values() for v in o.values())
raise SystemExit(0 if c(h)==c(n) else 1)
PY"
git checkout -- config/overlay_match_status.json config/clean_build_debt.json 2>/dev/null
# Scoped sync: only this batch's addresses.  A whole-tree `--apply` is NOT
# idempotent at HEAD (2026-11-02: it rewrote the deliberate address spellings
# in pc/Makefile SEMI_SHADOWED and pc/hal/gte_address_entries.c, and the link
# failed), so it must never run unscoped from a lander.
# pc/ld/psx_globals.syms (the absolute-address globals model) is touched by
# neither sync nor the landers; a missing name fails the link and a MOVED one
# links silently at its old address.  env PSX_EXTRA adds addresses a pre-step
# reverted (e.g. "--only 0x8012B8D0").
# never unscoped: with no addresses (a PRE-only landing) both syncs are skipped
SYNC="true"
if [ $HAVE_PC = 1 ]; then
[ -n "$ADDRS${PSX_EXTRA:-}" ] && SYNC="python3 tools/naming/sync_psx_globals.py $ADDRS ${PSX_EXTRA:-}"
[ -n "$ADDRS" ] && SYNC="$SYNC && python3 pc/tools/sync_symbol_names.py --apply $ADDRS"
else
  part_skip pc "tools/naming/sync_psx_globals.py, pc/tools/sync_symbol_names.py --apply $ADDRS"
fi
# the syncs run even under FAST (2026-11-10: a FAST landing skipped them and
# 316 names never reached psx_globals.syms), then the spelling-keyed places
bash -c "$SYNC" > "$L.sync.out" 2>&1 || { cat "$L.sync.out"; restore; exit 5; }
python3 tools/naming/check_name_keys.py --fix-makefile > "$L.keys.out" 2>&1 || { cat "$L.keys.out"; restore; exit 5; }
pc_gate pc       "python3 pc/tools/gen_sony_bios_alias.py && python3 pc/tools/generate_overlay_modules.py && python3 pc/tools/gen_ps1_alias_refs.py && make -f pc/Makefile clean >/dev/null && make -j$(nproc) -f pc/Makefile all && make -j$(nproc) -f pc/Makefile test > $L.pctest 2>&1; tail -1 $L.pctest | grep -q ' 0 failed'"
# a real new game on the PC port, the way a player runs it (enhanced profile;
# new_game in Japanese -- its name checks are kana -- and the English tour): the unit tests cannot see a PC-only crash in the game code (the
# 2026-11-07 class_w layout crash at name entry passed every other gate)
# the Windows exe builds too (MinGW; the player's main platform): a file the
# Linux build compiles can still fail there (2026-11-07: <SDL.h> in main.c)
pc_gate windows  "make -j$(nproc) -f pc/Makefile windows"
pc_gate playthrough "python3 pc/tools/playthrough.py pc/tools/playthrough/new_game.txt --profile enhanced --out $L.pt > $L.pt.out 2>&1 && grep -q '^PASS' $L.pt.out && ! grep -q '\\[CRASH\\]' $L.pt/*.pc.log && python3 pc/tools/playthrough.py pc/tools/playthrough/english_tour.txt --profile enhanced --set TOKI_LANG=en --out $L.pten > $L.pten.out 2>&1 && grep -q '^PASS' $L.pten.out && ! grep -q '\\[CRASH\\]' $L.pten/*.pc.log"

MSG=$(mktemp)
if [ -n "${SUBJECT:-}" ]; then printf '%s\n' "$SUBJECT" > "$MSG"; else printf 'Land %s main data names (%s)\n' "$N" "$LABEL" > "$MSG"; fi
printf '\nAdjudicated data names landed with apply_data_symbols.py through\ntools/naming/land_data.sh. Gates run: retail SHA1 (build-c, check-raw,\nclean main), main relocs and positions, registry overlaps, name table,\nplain-vs-shipped and prove_equivalence, every overlay verified serially\nwith no status change, PC re-sync with a clean link and unit tests.\n' >> "$MSG"
[ -n "${TRAILER:-}" ] && printf '\n%s\n' "$TRAILER" >> "$MSG"
git add -A -- $(git status --short | cut -c4- | grep -E '^(src|include|asm|pc|config|tools|Makefile|docs|wiki)')
git commit -q -F "$MSG" && git log -1 --oneline
rm -f "$MSG"
