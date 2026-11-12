#!/bin/bash
# Correction-flow pre-step for refining ALREADY-LANDED names (the 0x80084ED0
# merge pattern).  Run as land_overlay.sh's PRE step, i.e. under landing.lock
# and the all-or-nothing restore:
#
#   1. validate the refinement rows against the live records (dry run);
#   2. pull each landed name back to func_<ADDR> with the lander's own
#      --revert (files + .sym, records marked REVERTED);
#   3. rewrite the records with tools/naming/refine_landed.py (old record
#      preserved in history, refined name pending).
#
# land_overlay.sh then lands the pending refined names forward
# (func_<ADDR> -> refined name) with the usual gates and commits.
#
# usage: refine_pre.sh OV DECIDED.json [DECIDED2.json ...]
set -u
OV=$1; shift
[ $# -ge 1 ] || { echo "usage: $0 OV DECIDED.json [...]"; exit 64; }
ROOT=$(cd "$(dirname "$0")/../.." && pwd)
cd "$ROOT" || exit 9

python3 tools/naming/refine_landed.py "$OV" "$@" --dry-run || exit 2

# ids of the LAND rows in this scope -> --only list
ONLY=$(python3 - "$OV" "$@" <<'EOF'
import json, sys
ov, paths = sys.argv[1], sys.argv[2:]
ids = []
for p in paths:
    for r in json.load(open(p)):
        if r.get('decision') == 'LAND' and r.get('name') and r['id'].startswith(ov + ':'):
            ids.append(r['id'])
print(' '.join('--only ' + i for i in ids))
EOF
)
[ -n "$ONLY" ] || { echo "refine_pre: no LAND rows for $OV"; exit 3; }

# shellcheck disable=SC2086
python3 tools/overlay/apply_overlay_symbols.py --revert --apply --any-verdict --no-git-lock \
    --batch 400 $ONLY || exit 4

python3 tools/naming/refine_landed.py "$OV" "$@" || exit 5
