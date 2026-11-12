#!/bin/bash
# PRE step for land_overlay_data.sh: correct already-landed data names.
# env RENAME_ROWS=<rows.json> (tools/naming/rename_landed_data.py)
set -e
python3 tools/naming/rename_landed_data.py "$RENAME_ROWS" --apply --by "${BY:-rename}"
