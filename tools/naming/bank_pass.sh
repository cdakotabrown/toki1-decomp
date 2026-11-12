#!/bin/bash
# PRE step for tools/naming/land_overlay_data.sh: a landed void *[3][13]
# serifu/kind/sound bank becomes an s32 array (its cells are 32-bit PS1
# address cells): the init's void * cells are re-declared s32
# (normalize_ptr_cells.py range mode), the interior tokens become host[r][c],
# and the ledger type follows.  env BANK_OV, BANK_ADDR (0x...), BANK_DIMS
# (default 3,13), BANK_SIZE (default 156).
set -e
python3 tools/naming/normalize_ptr_cells.py --ov "$BANK_OV" --range "$BANK_ADDR" "${BANK_SIZE:-156}" --apply
python3 tools/naming/apply_interior.py "$BANK_ADDR" s32 "${BANK_DIMS:-3,13}" --ov "$BANK_OV" --no-volatile --apply
python3 - "$BANK_OV" "$BANK_ADDR" "${BANK_DIMS:-3,13}" <<'PY'
import os, sys
sys.path.insert(0, 'tools/analysis')
import symbol_names as SN
ov, addr, dims = sys.argv[1:]
doc = SN.load_names(all_scopes=True)
rec = doc['symbols']['%s:0x%08X' % (ov, int(addr, 16))]
rec['type'] = 's32' + ''.join('[%s]' % d for d in dims.split(','))
SN.save_names(doc)
PY
