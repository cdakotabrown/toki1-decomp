/* @SymbolSize: 412 */
/* @CompileFlags: -Wc,-pic0 */
/* tt_marker_step - TT overlay, Address: 0x8013FF04, Size: 412 bytes (103 insns) */
#include "types.h"

extern u8 *tt_player_wk;
extern u8 *tt_boss_wk;

extern s32 tt_angle_between(s32 *p0, s32 *p1);
extern void tt_shot_heading_make(s32 x, s32 y, s32 a2, s32 a3, s32 a4);
extern void shot_fan(s32 x, s32 y, s32 a2, s32 a3, s32 a4);

void tt_marker_step(void)
{
    u8 *s;
    u8 *o;
    s32 v;
    s32 loc[8];
    s32 unused[5];

    s = tt_boss_wk;
    o = tt_player_wk;
    switch ((u32)*(u16 *)(s + 8)) {
    case 0:
        *(s16 *)(s + 870) = 0;
        *(s16 *)(s + 8) = 64;
        break;
    case 64:
        if ((u32)*(u16 *)(tt_boss_wk + 870) % 384U == 192) {
            shot_fan(*(s16 *)(s + 20), *(s16 *)(s + 22) + 8, 128, 48,
                          170);
        }
        if ((*(u16 *)(s + 870) & 7) == 7) {
            loc[3] = *(s16 *)(s + 20) << 16;
            loc[4] = (*(s16 *)(s + 22) + 8) << 16;
            v = tt_angle_between(loc, (s32 *)(o + 20));
            tt_shot_heading_make(*(s16 *)(s + 20), *(s16 *)(s + 22) + 8, 24,
                          *(u16 *)(s + 932) * *(s16 *)(s + 934) + v, 160);
            if ((*(u16 *)(s + 932) += 64) >= 769U) {
                *(s16 *)(s + 932) = 0;
            }
            if (*(u16 *)(s + 934) == 1) {
                *(s16 *)(s + 934) = -1;
            } else {
                *(s16 *)(s + 934) = 1;
            }
        }
        if (*(u16 *)(s + 870) >= 384U) {
            *(s16 *)(s + 8) = 160;
        }
        break;
    case 160:
        *(s16 *)(s + 6) = 1;
        *(s16 *)(s + 8) = 0;
        break;
    }
    *(s16 *)(s + 870) = (s16)(*(u16 *)(s + 870) + 1);
}
