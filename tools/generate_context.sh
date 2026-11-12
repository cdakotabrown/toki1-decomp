#!/bin/bash
# generate_context.sh
# Generates a single monolithic header file for m2c to consume.
# This solves the "Unknown type" errors by giving m2c full visibility of
# the custom 96-byte DRAWENV and other Tokimeki-specific structs.

set -euo pipefail

# Use CC from environment or default to cc
CC="${CC:-cc}"

# Output to build directory (not tracked by git)
OUTPUT_DIR="build/m2c"
OUTPUT_FILE="${OUTPUT_DIR}/m2c_full_context.h"

mkdir -p "$OUTPUT_DIR"

echo "Generating m2c context file at $OUTPUT_FILE..."

cat > "$OUTPUT_FILE" << 'HEADER_EOF'
/*
 * AUTOMATICALLY GENERATED M2C CONTEXT
 * Regenerate with: ./tools/generate_context.sh
 *
 * Usage:
 *   python3 build/m2c/m2c.py --context build/m2c/m2c_full_context.h ...
 */

/* Neutralize _Static_assert for parsers that don't support it */
#define _Static_assert(expr, msg)

HEADER_EOF

# Preprocess the main types and libgpu headers.
# -E: Preprocess only
# -P: Inhibit generation of linemarkers (cleaner output)
# -I include: Add include dir to search path
# -D_LANGUAGE_C: Define this so headers parse correctly
# -D_Static_assert(x,y)=: Neutralize static asserts in headers
if [ -f "include/tokimeki/types.h" ] && [ -f "include/tokimeki/libgpu.h" ]; then
    "$CC" -E -P -undef -nostdinc \
        -I include \
        -D_LANGUAGE_C \
        -D__mips__ \
        '-D_Static_assert(x,y)=' \
        include/tokimeki/types.h \
        include/tokimeki/libgpu.h \
        2>/dev/null >> "$OUTPUT_FILE" || {
            echo "Warning: Preprocessing failed, using fallback types"
            cat >> "$OUTPUT_FILE" << 'FALLBACK_EOF'
/* Fallback types - preprocessing failed */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
typedef int bool;
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned long u_long;
FALLBACK_EOF
        }
else
    echo "Warning: Header files not found, using minimal types"
    cat >> "$OUTPUT_FILE" << 'MINIMAL_EOF'
/* Minimal types - header files not found */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
typedef int bool;
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned long u_long;

typedef struct {
    short x, y, w, h;
} RECT;

typedef struct {
    u_long tag;
    u_long code[15];
} DR_ENV;

/* Tokimeki Memorial uses 96-byte DRAWENV (standard is 92) */
typedef struct {
    RECT clip;
    short ofs[2];
    RECT tw;
    u_short tpage;
    u_char dtd;
    u_char dfe;
    u_char isbg;
    u_char r0, g0, b0;
    DR_ENV dr_env;
    int _tokimeki_pad;
} DRAWENV;

typedef struct {
    RECT disp;
    RECT screen;
    u_char isinter;
    u_char isrgb24;
    u_char pad0;
    u_char pad1;
} DISPENV;
MINIMAL_EOF
fi

echo "" >> "$OUTPUT_FILE"
echo "/* Additional common structs can be appended here */" >> "$OUTPUT_FILE"

# Strip any remaining _Static_assert that might have made it through
sed -i '/_Static_assert/d' "$OUTPUT_FILE"

echo "Done! Context file created:"
ls -lh "$OUTPUT_FILE"

# Also create symlink in include/ for convenience (but gitignored)
ln -sf "../${OUTPUT_FILE}" "include/m2c_full_context.h" 2>/dev/null || true
