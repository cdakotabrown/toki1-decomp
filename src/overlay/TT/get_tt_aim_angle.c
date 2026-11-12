/* @SymbolSize: 60 */
/* Address: 0x80133BE0, Size: 60 bytes (15 insns) */
#include "types.h"

extern void get_angle(s32 y, s32 x);

void get_tt_aim_angle(s32 *p0, s32 *p1)
{
    s32 dx, dy;

    dx = p1[8] - p0[8];
    dy = p1[9] - p0[9];

    get_angle(dy >> 16, dx >> 16);
}
