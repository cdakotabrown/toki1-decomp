#!/bin/bash
# PRE step for the landers: retire func_<ADDR> spellings whose address has a
# landed name at exactly that address (tools/naming/retire_head_refs.py).
# env HEAD_SCOPE: space-separated list of main | <OV> | live (every live overlay)
set -e
S=""
for s in ${HEAD_SCOPE:?}; do
  if [ "$s" = live ]; then S="$S $(cd src/overlay && ls -d */ | tr -d / | grep -vxE 'EVENT|GYOZI' | tr '\n' ' ')"; else S="$S $s"; fi
done
for s in $S; do python3 tools/naming/retire_head_refs.py --scope "$s" --apply | tail -1; done
