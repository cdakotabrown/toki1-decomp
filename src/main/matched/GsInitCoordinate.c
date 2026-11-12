/* Address: 0x8009EB70 */
/* Size: 72 bytes (18 instructions) */
/* Region: Library (0x80086000+), Psy-Q GCC 2.6.0 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O1 */
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

void GsInitCoordinate(GsCOORD0 *super, GsCOORD0 *base) {
    base->rx = base->ry = base->rz = 0;
    base->ty = base->tz = 0;
    base->sx = base->sy = base->sz = 4096;
    base->tx = 0;
    base->super = super;
    if (super != 0 && super != (GsCOORD0 *)1) {
        super->sub = base;
    }
}
