/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
/* geko_serifu_result_step3 - GEKO overlay, Address: 0x80143BEC, Size: 488 bytes */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32 a0);
extern s32 get_g_zyotai_s(s32 a0);
extern void param_clamp(void);
extern u32 rand(void);

extern volatile u8 now_girl;
extern s32 serifu_no;
extern volatile u16 geko_now_serifu3;
extern u8 screen_w[];

void geko_serifu_result_step3(void)
{
    s16 i;
    u8 q;
    u8 m;
    u8 *rec;

    if (serifu_no != 0) {
        if (now_girl == 2) {
            geko_now_serifu3 = 24;
        } else {
            geko_now_serifu3 = 23;
        }
        rec = screen_w + (u32)now_girl * 56;
        *(s16 *)(rec + 454) += 20;
        goto_sub_sub_step(24);
    } else {
        geko_now_serifu3 = 21;
        for (i = 0; i < 11; i++) {
            rec = screen_w + i * 56;
            if ((u32)(*(s32 *)(rec + 456) << 30) >> 31 == 0) {
                continue;
            }
            if (i == now_girl) {
                continue;
            }
            q = *(s16 *)(rec + 446) / 20;
            m = rand() % (q + 1);
            if ((u8)(get_g_zyotai_s(i) & 0x7F) < 2U) {
                *(s16 *)(rec + 454) += m + 6;
            } else {
                *(s16 *)(rec + 454) += m + 4;
            }
        }
        sub_sub_step_up();
    }
    param_clamp();
}
