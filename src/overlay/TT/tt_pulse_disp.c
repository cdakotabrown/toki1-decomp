/* @SymbolStart: 0x801364EC */
/* @SymbolSize: 216 */
/* tt_pulse_disp - TT overlay, Address: 0x801364F4 */
#include "types.h"

extern u8 *tt_boss_wk;

extern void tt_shot_aimed_make(s16 x, s16 y, s32 w, s32 h);
extern void tt_shot_spread_make(s16 x, s16 y, s32 w, s32 h, s32 e);

void tt_pulse_disp(void)
{
    u8 *v1 = tt_boss_wk;

    switch (*(u16 *)(v1 + 6)) {
    case 0:
    case 1:
        if ((u32)*(u16 *)(tt_boss_wk + 868) >= 96U) {
            tt_shot_aimed_make(*(s16 *)(v1 + 20), *(s16 *)(v1 + 22), 24, 160);
        }
        break;
    case 2:
        if ((u32)*(u16 *)(tt_boss_wk + 868) >= 96U) {
            tt_shot_spread_make(*(s16 *)(v1 + 20), *(s16 *)(v1 + 22), 256, 24, 160);
        }
        break;
    }

    if ((u32)*(u16 *)(v1 + 868) >= 96U) {
        *(s16 *)(v1 + 868) = 0;
    } else {
        *(s16 *)(v1 + 868) = *(u16 *)(v1 + 868) + 1;
    }
}
