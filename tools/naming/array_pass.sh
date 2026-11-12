#!/bin/bash
# PRE step for tools/naming/land_overlay_data.sh: env ARRAY_OV=<OV>,
# optional ARRAY_SKIP="file1 file2" (files whose codegen the rewrite changes).
set -e
S=""; for f in ${ARRAY_SKIP:-}; do S="$S --skip-file $f"; done
python3 tools/naming/array_pass.py "$ARRAY_OV" $S --apply
