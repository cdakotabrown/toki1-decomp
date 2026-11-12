#!/bin/bash
# PRE step for tools/naming/land_overlay_data.sh: the ETC per-event resource
# record struct pass (include/tokimeki/etc_ev_res.h).  The init files' void *
# cells become s32 first, so every field token rewrites without a cast.
set -e
L="tools/naming/layouts/etc_ev_res.json tools/naming/layouts/etc_ev_res_blk.json
   tools/naming/layouts/etc_ev_res_bg.json tools/naming/layouts/etc_ev_res_obj.json"
python3 tools/naming/normalize_ptr_cells.py $L --apply
for l in $L; do python3 tools/naming/apply_interior_struct.py "$l" --apply; done
