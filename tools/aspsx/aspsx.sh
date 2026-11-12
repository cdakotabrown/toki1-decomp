#!/bin/bash
# ASPSX Wine Wrapper Script
# Invokes SN Systems ASPSX assembler via Wine
#
# Usage: aspsx.sh [--version VERSION] input.s -o output.obj
#
# Available versions: 2.67, 2.77, 2.79, 2.81, 2.86
# Default: 2.67 (earliest working Win32 version)
#
# The script handles:
#   - Line ending conversion (Unix LF -> DOS CR+LF)
#   - Wine path translation
#   - Temporary file cleanup

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
VERSION="2.67"
ARGS=()

# Parse --version flag
while [[ $# -gt 0 ]]; do
    case "$1" in
        --version)
            VERSION="$2"
            shift 2
            ;;
        *)
            ARGS+=("$1")
            shift
            ;;
    esac
done

ASPSX_EXE="$SCRIPT_DIR/aspsx-${VERSION}.exe"
if [ ! -f "$ASPSX_EXE" ]; then
    echo "Error: ASPSX version $VERSION not found at $ASPSX_EXE" >&2
    echo "Available versions:" >&2
    ls "$SCRIPT_DIR"/aspsx-*.exe 2>/dev/null | sed 's/.*aspsx-//;s/\.exe//' >&2
    exit 1
fi

# Find the input .s file and output .obj file
INPUT_FILE=""
OUTPUT_FILE=""
PASS_ARGS=()
i=0
while [ $i -lt ${#ARGS[@]} ]; do
    arg="${ARGS[$i]}"
    if [ "$arg" = "-o" ]; then
        i=$((i+1))
        OUTPUT_FILE="${ARGS[$i]}"
    elif [[ "$arg" == *.s ]] || [[ "$arg" == *.S ]]; then
        INPUT_FILE="$arg"
    else
        PASS_ARGS+=("$arg")
    fi
    i=$((i+1))
done

if [ -z "$INPUT_FILE" ]; then
    echo "Error: No input .s file specified" >&2
    exit 1
fi

if [ -z "$OUTPUT_FILE" ]; then
    OUTPUT_FILE="${INPUT_FILE%.s}.obj"
fi

# Convert to absolute paths
INPUT_FILE="$(realpath "$INPUT_FILE")"
OUTPUT_FILE="$(realpath -m "$OUTPUT_FILE")"

# Create temp file with DOS line endings
TMPFILE=$(mktemp /tmp/aspsx_XXXXXX.s)
trap "rm -f '$TMPFILE'" EXIT

sed 's/$/\r/' "$INPUT_FILE" > "$TMPFILE"

# Convert paths to Wine format
WINE_INPUT="Z:$(echo "$TMPFILE" | sed 's|/|\\|g')"
WINE_OUTPUT="Z:$(echo "$OUTPUT_FILE" | sed 's|/|\\|g')"

# Run ASPSX under Wine
WINEDEBUG=-all wine "$ASPSX_EXE" "$WINE_INPUT" -o "$WINE_OUTPUT" "${PASS_ARGS[@]}" 2>&1

# Check if output was created
if [ ! -f "$OUTPUT_FILE" ]; then
    echo "Error: ASPSX failed to create output file" >&2
    exit 1
fi
