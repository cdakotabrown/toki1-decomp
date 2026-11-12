/* @SymbolSize: 60 */
/* Address: 0x801320F0, Size: 60 bytes (15 insns) */
#include "types.h"

extern void get_angle(s32 y, s32 x);

void tt_angle_between(s32 *p0, s32 *p1)
{
    s32 dx, dy;

    dx = p1[3] - p0[3];
    dy = p1[4] - p0[4];

    get_angle(dy >> 16, dx >> 16);
}
