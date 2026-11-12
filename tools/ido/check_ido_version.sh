#!/bin/bash
# Check IDO compiler version from extracted IRIX files
# Usage: ./check_ido_version.sh /path/to/extracted/irix

IRIX_ROOT="${1:-.}"

echo "=== Checking IDO version in: $IRIX_ROOT ==="
echo

# Look for C frontend (cfe)
CFE_PATH=$(find "$IRIX_ROOT" -name "cfe" -type f 2>/dev/null | head -1)
if [ -n "$CFE_PATH" ]; then
    echo "Found cfe: $CFE_PATH"
    echo "Version strings:"
    strings "$CFE_PATH" | grep -E "(Version|version|[0-9]\.[0-9]+)" | head -20
    echo
    echo "ECOFF version check (looking for 3.xx):"
    strings "$CFE_PATH" | grep -E "3\.[0-9]+" | head -10
    echo
fi

# Look for code generator (ugen)
UGEN_PATH=$(find "$IRIX_ROOT" -name "ugen" -type f 2>/dev/null | head -1)
if [ -n "$UGEN_PATH" ]; then
    echo "Found ugen: $UGEN_PATH"
    echo "Version strings:"
    strings "$UGEN_PATH" | grep -E "(Version|version|[0-9]\.[0-9]+)" | head -20
    echo
fi

# Look for optimizer (uopt)
UOPT_PATH=$(find "$IRIX_ROOT" -name "uopt" -type f 2>/dev/null | head -1)
if [ -n "$UOPT_PATH" ]; then
    echo "Found uopt: $UOPT_PATH"
    echo "Version strings:"
    strings "$UOPT_PATH" | grep -E "(Version|version|[0-9]\.[0-9]+)" | head -20
    echo
fi

# Look for cc driver
CC_PATH=$(find "$IRIX_ROOT" -name "cc" -type f 2>/dev/null | head -1)
if [ -n "$CC_PATH" ]; then
    echo "Found cc: $CC_PATH"
    echo "Version strings:"
    strings "$CC_PATH" | grep -E "(IDO|IRIX|[0-9]\.[0-9]+)" | head -20
    echo
fi

# Summary of all compiler-related files found
echo "=== All compiler binaries found ==="
find "$IRIX_ROOT" -type f \( -name "cc" -o -name "cfe" -o -name "ugen" -o -name "uopt" -o -name "as*" -o -name "ld" \) 2>/dev/null

echo
echo "=== Looking for IDO package info ==="
find "$IRIX_ROOT" -name "*.idb" -o -name "*.sw" -o -name "relnotes*" 2>/dev/null | head -20
