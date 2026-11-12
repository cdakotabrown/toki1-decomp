/* @CompileFlags: -cckr */
/* date_serifu_row_add - DATE overlay, Address: 0x80149A30, Size: 184 bytes (46 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 get_g_zyotai_s(s32);
extern u8 now_girl;
extern s16 date_now_serifu2;
extern s16 date_now_serifu2_in;

void date_serifu_row_add(void)
{
    s32 v0;
    s32 v1;
    u8 masked;

    v0 = get_g_zyotai_s(now_girl);
    v1 = v0;
    masked = (u8)(v1 & 0x7F);
    v1 = masked;
    if ((u32)v1 < 2U) {
        date_now_serifu2 = date_now_serifu2 + 2;
    } else if (v1 == 2) {
        date_now_serifu2 = date_now_serifu2 + 4;
    } else if (v1 == 3) {
        date_now_serifu2 = date_now_serifu2 + 6;
    } else {
        date_now_serifu2 = date_now_serifu2 + 8;
    }
    date_now_serifu2_in = 1;
    sub_sub_step_up();
}
