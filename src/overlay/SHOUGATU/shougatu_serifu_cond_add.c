/* @CompileFlags: -cckr */
/* shougatu_serifu_cond_add - SHOUGATU overlay, Size: 228 bytes (57 insns). */
#include "types.h"

extern void sub_sub_step_up();
extern s32 get_g_zyotai_s();
extern u8 now_year;
extern u8 now_girl;
extern s16 shougatu_meet_now_serifu;
extern u8 shougatu_meet_girl;
extern u8 shougatu_alone_flag;

void shougatu_serifu_cond_add()
{
    s32 v0;
    s32 v1;
    u8 masked;
    s32 k;

    k = 9;
    v0 = get_g_zyotai_s(now_girl);
    v1 = v0;
    masked = (u8)(v1 & 0x7F);
    v1 = masked;
    if ((u32)v1 >= 2U) {
        shougatu_meet_now_serifu += 2;
    }
    if (now_year == 98) {
        shougatu_meet_now_serifu += 1;
    } else if (shougatu_alone_flag == 0 && k == now_girl && shougatu_meet_girl == k &&
               (u32)v1 < 2U) {
        shougatu_meet_now_serifu += 4;
    }
    if (shougatu_alone_flag != 0) {
        shougatu_meet_now_serifu = 19;
    }
    sub_sub_step_up();
}
