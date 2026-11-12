/* Address: 0x8009F54C */
/* Size: 120 bytes (30 instructions) */
/* Region: Library (0x80086000+), Psy-Q GCC */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --load-dest-temp --konami-lib-sched */
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

s32 get_dstack(GsCOORD0 **pp) {
    s32 n = 0;
    s32 i;

    for (i = 0; *pp != 0; i++) {
        if (*pp == (GsCOORD0 *)1) {
            break;
        }
        n++;
        if (GsDSTACK[i].coord != *pp) {
            GsDSTACK[i].coord = *pp;
            GsDSTACK[i].flg = 0;
        }
        *pp = (*pp)->super;
    }
    return n - 1;
}
