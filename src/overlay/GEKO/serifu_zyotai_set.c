/* serifu_zyotai_set -- GEKO overlay, Address: 0x801331A0, Size: 176 bytes */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 get_g_zyotai_s(s32 a0);
extern s32 bustup_face_change(s32 a0);

extern u8 now_girl;
extern s16 geko_now_serifu;
extern u8 geko_param_up_flag;
extern u8 geko_stat_hi;

void serifu_zyotai_set(void)
{
    s32 result;
    u8 resultMasked;

    result = get_g_zyotai_s(now_girl);
    resultMasked = result & 0x7F;

    if (resultMasked == 4 || geko_stat_hi != 0) {
        geko_now_serifu += 18;
        bustup_face_change(4);
        geko_param_up_flag = 0;
    } else {
        if (resultMasked == 2) {
            geko_now_serifu += 6;
        }
        if (resultMasked == 3) {
            geko_now_serifu += 12;
        }
    }

    sub_sub_step_up();
}
