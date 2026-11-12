/* Address: 0x80132BF0, Size: 104 bytes (26 insns) */
/* @SymbolSize: 104 */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x40 */
#include "types.h"

s32 en_nichi_sq_dist(s16 x0, s16 y0, s16 x1, s16 y1)
{
    s32 dx;
    s32 dy;

    dx = x0 - x1;
    dy = y0 - y1;
    if (dx < 0) {
        dx = -dx;
    }
    if (dy < 0) {
        dy = -dy;
    }
    dx = dx * dx;
    dy = dy * dy;
    return dx + dy;
}
