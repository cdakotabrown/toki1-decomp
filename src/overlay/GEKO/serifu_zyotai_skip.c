/* @CompileFlags: -cckr */
/* @SymbolSize: 184 */
#include "types.h"

extern s32 get_g_zyotai_s(s32);
extern void sub_sub_step_up(void);

extern u8 now_girl;
extern u8 geko_stat_hi;

void serifu_zyotai_skip(void)
{
    s32 v0;
    s32 v1;
    u8 masked;

    v0 = get_g_zyotai_s(now_girl);
    v1 = v0;
    masked = (u8)(v1 & 0x7F);
    v1 = masked;
    if (v1 == 4 || geko_stat_hi != 0) {
        sub_sub_step_up();
        sub_sub_step_up();
        sub_sub_step_up();
        sub_sub_step_up();
    } else if (now_girl == 6 && (u32)v1 < 2U) {
        sub_sub_step_up();
        sub_sub_step_up();
        sub_sub_step_up();
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
