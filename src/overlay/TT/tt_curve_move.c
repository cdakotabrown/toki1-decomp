/* @CompileEnv: IDO52_UGEN_TEMP_REG=4:t8 */
/* tt_curve_move - TT overlay, Address: 0x8014B188 (sister of tt_sway_move) */
#include "types.h"

extern u8 *tt_scroll_wk;

extern void tt_shot_aimed_make(s16 a0, s16 a1, s32 a2, s32 a3);

void tt_curve_move(u8 *obj)
{
    u8 *ptr;

    ptr = tt_scroll_wk;

    if (obj[99] != 0) {
        *(s32 *)(obj + 40) = -(*(s32 *)(obj + 36) / 64);
    } else {
        *(s32 *)(obj + 40) = *(s32 *)(obj + 36) / 64;
    }

    if (ptr[11] != 0 && (u32)*(u16 *)(obj + 94) >= 25U && obj[96] == 0) {
        *(s16 *)(obj + 94) = 0;
        obj[96]++;
        tt_shot_aimed_make(*(s16 *)(obj + 20), *(s16 *)(obj + 22), 32, 160);
    }
    if (obj[81] != 0) {
        *(s16 *)(obj + 94) = *(u16 *)(obj + 94) + 1;
    }
}
