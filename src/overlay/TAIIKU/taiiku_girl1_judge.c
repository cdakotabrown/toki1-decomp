/* @SymbolStart: 0x80142490 */
/* @SymbolSize: 232 */
/* Address: 0x80142498 */
/* Size: 224 bytes (56 insns) */
#include "types.h"

extern s32 sub_step_up(void);
extern void sub_sub_step_up(void);
extern void goto_sub_step(s32 a0);
extern void device_load_start(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern s32 device_load_poll(void);
extern void common_disp(void);
extern s32 get_g_zyotai_low(s32 a0);

extern s32 girl1_flag;
extern u8 ev_flag0d;
extern u8 taiiku_ninin_girl1;
extern u8 now_sub_sub_step;

void taiiku_girl1_judge(void) {
    s32 t6_hidden = now_sub_sub_step;

    s32 v0;

    if (t6_hidden == 0) {
        v0 = get_g_zyotai_low(1);
        if ((u32)v0 < 2U && ((u32)(girl1_flag << 30) >> 31) == 1 && ev_flag0d == 0) {
            ev_flag0d = 1;
            taiiku_ninin_girl1 = 1;
            device_load_start(0, 0xBFA6, 0xBF87, 0xD720, 0xD6D1, 0xD6C5);
            sub_sub_step_up();
        } else {
            taiiku_ninin_girl1 = 0;
            goto_sub_step(3);
        }
        common_disp();
        return;
    }

    if (device_load_poll() == 1) {
        sub_step_up();
    }
    common_disp();
}
