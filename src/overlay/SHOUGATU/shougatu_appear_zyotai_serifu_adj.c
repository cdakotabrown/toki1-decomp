/* @CompileFlags: -cckr */
/* @SymbolSize: 148 */
/* Address: 0x8013BE2C, Size: 148 bytes (37 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 get_g_zyotai_s(s32);
extern u8 now_girl;
extern s16 shougatu_appear_now_serifu;

void shougatu_appear_zyotai_serifu_adj(void)
{
    s32 v0;
    s32 v1;
    u8 masked;

    v0 = get_g_zyotai_s(now_girl);
    v1 = v0;
    masked = (u8)(v1 & 0x7F);
    v1 = masked;
    if ((u32)v1 >= 2U) {
        shougatu_appear_now_serifu = shougatu_appear_now_serifu + 2;
    }
    if ((u32)v1 >= 3U) {
        shougatu_appear_now_serifu = shougatu_appear_now_serifu + 2;
    }
    if (v1 == 4) {
        shougatu_appear_now_serifu = shougatu_appear_now_serifu + 2;
    }
    sub_sub_step_up();
}
