/* @CompileEnv: IDO52_PRECOLOR_WEB=a0:38 */
/* tt_box_overlap_check - TT overlay, Address: 0x801454F8, Size: 192 bytes (48 insns) */
#include "types.h"

s32 tt_box_overlap_check(u8 *p0, u8 *p1, u8 *p2, u8 *p3)
{
    s16 x0, y0, x1, y1;
    s32 tx, ty;
    s32 dx, dy;

    x0 = *(s16 *)(p1 + 0) + *(s16 *)(p0 + 0);
    y0 = *(s16 *)(p1 + 2) + *(s16 *)(p0 + 2);
    x1 = *(s16 *)(p3 + 0) + *(s16 *)(p2 + 0);
    y1 = *(s16 *)(p3 + 2) + *(s16 *)(p2 + 2);

    tx = x0 - x1;
    ty = y0 - y1;
    if (tx < 0) {
        dx = -tx;
    } else {
        dx = tx;
    }
    if ((u32)dx >= (u32)(p3[4] + p1[4])) {
        goto ret0;
    }
    if (ty < 0) {
        dy = -ty;
    } else {
        dy = ty;
    }
    if ((u32)dy >= (u32)(p3[5] + p1[5])) {
        goto ret0;
    }
    return 1;
ret0:
    return 0;
}
