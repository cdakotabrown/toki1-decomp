/* geko_zyotai_serifu_add -- GEKO overlay, Address: 0x80136ECC, Size: 152 bytes, Semi-matched */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 get_g_zyotai_s(s32 a0);

extern u8 now_girl;
extern s16 geko_now_serifu2;

void geko_zyotai_serifu_add(void)
{
    s32 v0;
    s32 v1;
    u8 masked;

    v0 = get_g_zyotai_s((s32)now_girl);
    v1 = v0;
    masked = (u8)(v1 & 0x7F);
    v1 = masked;

    if (v1 == 2) {
        geko_now_serifu2 = geko_now_serifu2 + 2;
    } else if (v1 == 3) {
        geko_now_serifu2 = geko_now_serifu2 + 4;
    } else if (v1 == 4) {
        geko_now_serifu2 = geko_now_serifu2 + 6;
    }

    sub_sub_step_up();
}
