/* @SymbolStart: 0x8013D140 */
/* @SymbolSize: 144 */
#include "types.h"

extern s32 abs(s32);
extern s16 taiiku_tama_sx;
extern s16 cal_cw_x;
extern s16 taiiku_tama_sy;
extern s16 cal_cw_y;
extern s32 taiiku_tama_zone;

s32 taiiku_near_check(void)
{
    s32 a, b;
    a = abs((taiiku_tama_sx >> 3) - cal_cw_x);
    b = abs((taiiku_tama_sy >> 3) - cal_cw_y);
    if (taiiku_tama_zone != 1) {
        return 0;
    }
    if ((16 - (a >> 2)) >= (b - 16)) {
        return 1;
    }
    return 0;
}
