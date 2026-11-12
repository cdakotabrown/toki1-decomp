/* Address: 0x80141DD0 */
/* Size: 136 bytes (34 insns) */
#include "types.h"

extern void sub_step_up(void);
extern void common_disp(void);
extern u32 get_g_zyotai_low(s32);

extern s32 girl6_flag;
extern u8 ev_flag2d;
extern u8 now_sub_step;
extern u8 now_sub_sub_step;
extern s32 taiiku_end;

void taiiku_girl6_judge(void)
{
    u32 v0;

    v0 = get_g_zyotai_low(6);

    if (v0 < 2U) {
        if ((((u32)(girl6_flag << 30)) >> 31) == 1) {
            if (ev_flag2d == 0) {
                ev_flag2d = 1;
                sub_step_up();
                goto end;
            }
        }
    }

    now_sub_step = 0;
    now_sub_sub_step = 0;
    taiiku_end = 1;

end:
    common_disp();
}
