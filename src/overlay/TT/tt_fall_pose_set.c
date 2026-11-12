/* @SymbolSize: 132 */
#include "types.h"

extern u8 *tt_boss_wk;

void tt_fall_pose_set(void)
{
    u8 *p;
    s16 v;

    p = tt_boss_wk;
    v = *(s16 *)(p + 22);
    if (v >= 63) {
        *(s16 *)(p + 932) = 11;
        *(s16 *)(p + 934) = 11;
        return;
    }
    if (v >= 59) {
        *(s16 *)(p + 932) = 10;
        *(s16 *)(p + 934) = 10;
        return;
    }
    if (v >= 55) {
        *(s16 *)(p + 932) = 9;
        *(s16 *)(p + 934) = 9;
        return;
    }
    if (v >= 51) {
        *(s16 *)(p + 932) = 8;
        *(s16 *)(p + 934) = 8;
    }
}
