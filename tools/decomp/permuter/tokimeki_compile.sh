#!/bin/bash
# Tokimeki Memorial IDO 5.2 compile script for decomp-permuter
# Uses absolute path resolution
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
"$PROJECT_ROOT/tools/ido-static-recomp/build/5.2/out/cc" -c -O2 -G0 -mips1 -EL -non_shared -I"$PROJECT_ROOT/include" "$@" 2>/dev/null
