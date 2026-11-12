/* @CompileFlags: -cckr */
/* @SymbolSize: 152 */
/* shougatu_serifu_zyotai_set - SHOUGATU overlay, Size: 152 bytes. */
#include "types.h"

extern s32 get_g_zyotai_s(s32);
extern void sub_sub_step_up(void);

extern u8 now_girl;
extern s16 now_serifu;
extern s16 now_serifu_in;

void shougatu_serifu_zyotai_set(void)
{
    s32 v0;
    s32 v1;
    u8 masked;

    v0 = get_g_zyotai_s(now_girl);
    v1 = v0;
    masked = (u8)(v1 & 0x7F);
    v1 = masked;
    if ((u32)v1 < 2U) {
        now_serifu = 6;
    } else if (v1 == 2) {
        now_serifu = 8;
    } else if (v1 == 3) {
        now_serifu = 10;
    } else {
        now_serifu = 12;
    }
    now_serifu_in = 1;
    sub_sub_step_up();
}
