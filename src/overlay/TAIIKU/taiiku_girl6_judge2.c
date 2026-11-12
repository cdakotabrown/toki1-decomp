/* @SymbolSize: 144 */
/* Address: 0x80139C68 */
/* Size: 144 bytes (36 insns) */
#include "types.h"

extern s32 get_g_zyotai_low(s32 a0);
extern void sub_step_up(void);

extern s32 girl6_flag;
extern s32 taiiku_tama_girl;
extern u8  ev_flag2d;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern s32 taiiku_end;

void taiiku_girl6_judge2(void)
{
    s32 v0;

    v0 = get_g_zyotai_low(6);
    if ((u32)v0 < 2U &&
        (((u32)(girl6_flag << 30)) >> 31) == 1 &&
        taiiku_tama_girl == 6 &&
        ev_flag2d == 0)
    {
        ev_flag2d = 1;
        sub_step_up();
    } else {
        now_sub_step = 0;
        now_sub_sub_step = 0;
        taiiku_end = 1;
    }
}
