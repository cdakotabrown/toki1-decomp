#!/bin/bash
# PRE step for tools/naming/land_overlay_data.sh: the ENDING bust-up record
# struct pass (include/tokimeki/ending_girl_res.h).
set -e
python3 tools/naming/apply_interior_struct.py tools/naming/layouts/ending_girl_res.json --apply
