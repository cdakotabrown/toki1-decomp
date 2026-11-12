#!/bin/bash
# PRE step for land_data.sh: rename same-address view aliases.
# env VIEW_ROWS=<rows.json> (tools/naming/rename_views.py)
set -e
python3 tools/naming/rename_views.py "$VIEW_ROWS" --apply --by "${BY:-views}"
