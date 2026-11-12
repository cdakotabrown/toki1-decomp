/* Address: 0x8009F5C4 */
/* Size: 60 bytes (15 instructions) */
/* Region: Library (0x80086000+), Psy-Q GCC */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --fill-load-delay */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct GsCOORD0 {
    s32 rx, ry, rz;
    s16 sx, sy, sz;
    s16 pad;
    s32 tx, ty, tz;
    struct GsCOORD0 *super;
    struct GsCOORD0 *sub;
} GsCOORD0;

typedef struct {
    s16 flg;
    s16 pad;
    GsCOORD0 *coord;
    u8 rest[32];
} Rec40;

extern Rec40 GsDSTACK[];

s32 get_dstack2(GsCOORD0 *c) {
    s32 n = 0;
    s32 i;

    for (i = 0; c != 0; i++, n++) {
        if (c == (GsCOORD0 *)1) {
            break;
        }
        GsDSTACK[i].coord = c;
        c = c->super;
    }
    return n - 1;
}
