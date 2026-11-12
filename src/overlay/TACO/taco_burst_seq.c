/* @PostProc: patch_insn_reg_field 0x18:rt:t6:t7 0x1c:rs:t6:t7 0x20:rt:t7:t6 0x28:rt:t7:t6 0x1c0:rt:t7:t6 0x1c4:rs:t7:t6 0x1c8:rt:t6:t7 0x1cc:rs:t6:t7 0x1d0:rs:t6:t7 0x1d8:rs:t6:t7 */
/* Address: 0x80153FE4 */
#include "types.h"

extern s32 taco_drop_bright;
extern s32 taco_burst_flag;
extern s32 taco_burst_cnt;

extern void taco_drop(s32, s32);
extern void taco_burst_disp(s32, s32, s32);
extern void taco_band_disp(s32, s32, s32, s32);

s32 taco_burst_seq(void)
{
    s32 s0;
    s32 m;

    s0 = taco_burst_cnt;
    taco_drop_bright = 128;
    if (s0 < 50) {
        taco_drop(0, 2);
    } else if (taco_burst_cnt >= 50 && taco_burst_cnt < 220) {
        taco_drop(0, 3);
    }
    if (s0 >= 50 && s0 < 90) {
        taco_burst_disp(s0 * 3 - 150, 2, 0xA0A0A0);
    }
    if (s0 >= 90 && s0 < 150) {
        taco_burst_disp(120, 2, 0xA0A0A0);
    }
    if (s0 >= 150 && s0 < 180) {
        taco_burst_disp(270 - s0, 2, 0xA0A0A0);
    }
    if (s0 >= 180 && s0 < 210) {
        taco_burst_disp(630 - s0 * 3, 2, 0xA0A0A0);
    }
    if (s0 < 50) {
        m = (s0 * 2) & 0xFF;
        taco_band_disp(2, m, (u8)m, m & 0xFF);
    } else if (s0 >= 50 && s0 < 210) {
        if ((s0 - 50) % 3 == 0) {
            taco_band_disp(1, 160, 160, 192);
        } else {
            taco_band_disp(2, 100, 100, 100);
        }
    } else if (s0 >= 210 && s0 < 260) {
        m = (520 - s0 * 2) & 0xFF;
        taco_band_disp(2, m, (u8)m, m & 0xFF);
    }
    if (s0 == 260) {
        taco_burst_cnt = 0;
        taco_burst_flag = 0;
    } else {
        taco_burst_cnt = taco_burst_cnt + 1;
    }
    return taco_burst_flag;
}
