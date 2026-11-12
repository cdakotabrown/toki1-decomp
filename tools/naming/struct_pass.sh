#!/bin/bash
# PRE step for tools/naming/land_overlay_data.sh: an overlay struct pass.
# env STRUCT_LAYOUTS: space-separated layout JSONs (tools/naming/layouts/);
#   each names its header under include/.  The ledger type of every host is
#   set to the layout's type ([count] when count > 1).
# env STRUCT_SKIP: files to leave alone (codegen regressions found by a try).
# env STRUCT_CAST=1: also --array-cast (array and whole-record views).
set -e
SKIP=""; for f in ${STRUCT_SKIP:-}; do SKIP="$SKIP --skip-file $f"; done
CAST=""; [ "${STRUCT_CAST:-}" = 1 ] && CAST="--array-cast"
for l in ${STRUCT_LAYOUTS:?}; do
  python3 tools/naming/apply_interior_struct.py "$l" --apply $CAST $SKIP | head -1
  python3 - "$l" <<'PY'
import json, sys
sys.path.insert(0, 'tools/analysis')
import symbol_names as SN
lay = json.load(open(sys.argv[1]))
doc = SN.load_names(all_scopes=True)
hosts = lay.get('hosts') or [lay['host']]
n = lay.get('count', 1)
want = lay['type'] + ('[%d]' % n if n > 1 else '')
for h in hosts:
    key = '%s:%s' % (lay['scope'], h) if lay.get('scope') else h
    rec = doc['symbols'][key]
    if rec.get('type') != want:
        rec['type'] = want
SN.save_names(doc)
PY
done
