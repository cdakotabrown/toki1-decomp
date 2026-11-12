#!/bin/bash
# Batch permuter runner for multiple functions
# Usage: ./batch_permute.sh [--setup-only] [--jobs N] [--iterations N]
#
# Based on xxxx.md insights:
# - Score <50: permuter solves quickly
# - Score 50-150: add PERM hints
# - Score >150: manual iteration first

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$SCRIPT_DIR/../.."
PERMUTER_DIR="$PROJECT_ROOT/build/decomp-permuter"

# Smallest Medium functions - best permuter candidates
FUNCTIONS=(
    CdGetSector   # 15 instr, delay slot
    _cmp   # 15 instr, delay slot
    GsSetAmbient   # 17 instr, delay slot
    get_g_zyotai_low   # 18 instr, scheduling
    _spu_open   # 18 instr, scheduling
    GsSetLsMatrix   # 19 instr, regalloc (s0 vs stack)
    func_8009F970   # 19 instr, delay slot
    func_8009D338   # 21 instr, delay slot
    func_8008B1F0   # 23 instr, instruction order
    dma_intr_stop   # 25 instr, register alloc
)

SETUP_ONLY=0
JOBS=2

while [[ $# -gt 0 ]]; do
    case $1 in
        --setup-only) SETUP_ONLY=1; shift ;;
        --jobs|-j) JOBS=$2; shift 2 ;;
        *) shift ;;
    esac
done

echo "=== Batch Permuter for Tokimeki Memorial ==="
echo "Functions: ${#FUNCTIONS[@]}"
echo "Jobs per function: $JOBS"
echo ""

cd "$PERMUTER_DIR"

for func in "${FUNCTIONS[@]}"; do
    echo "--- $func ---"

    if [[ $SETUP_ONLY -eq 1 ]]; then
        python3 "$SCRIPT_DIR/quick_permute.py" "$func" --setup-only
    else
        # Check if candidate dir exists
        if [[ ! -d "$func" ]]; then
            echo "  Setting up candidate dir..."
            python3 "$SCRIPT_DIR/quick_permute.py" "$func" --setup-only
        fi

        if [[ -d "$func" ]]; then
            echo "  Running: python3 permuter.py -j$JOBS --stop-on-zero $func/"
            python3 permuter.py -j$JOBS --stop-on-zero "$func/" &
            sleep 1
        fi
    fi
done

if [[ $SETUP_ONLY -eq 0 ]]; then
    echo ""
    echo "Permuters running in background. Monitor with: jobs"
    echo "Stop all with: pkill -f permuter.py"
    wait
fi
