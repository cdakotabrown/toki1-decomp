/* @PostProc: scheduler_normalizer --reloc-symbol-fp --const-fp --load-fp */
/* @PostProc: patch_insn_reg_field 0x2e8:rt:t8:v0 0x2f0:rt:t8:v0 */
/* @RodataBase: 0x8013BF70 */
/* Address: 0x80138088 */
/* Size: 748 bytes (187 instructions) */
#include "types.h"

extern u8 screen_w[];
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u8 now_youbi;
extern u8 girl4_basyo;
extern volatile u8 rec_idx;
extern volatile u8 now_girl;

void master_log_girl_add(void)
{
    u32 v;

    screen_w[rec_idx * 8 + 1886] = now_girl;
    screen_w[rec_idx * 8 + 1888] = 21;
    screen_w[rec_idx * 8 + 1887] = 6;
    screen_w[rec_idx * 8 + 1890] = now_year;
    screen_w[rec_idx * 8 + 1891] = now_month;
    screen_w[rec_idx * 8 + 1892] = now_day;
    screen_w[rec_idx * 8 + 1893] = now_youbi;

    switch ((u32)now_girl) {
    case 0:
        screen_w[rec_idx * 8 + 1889] = 1;
        break;
    case 1:
        screen_w[rec_idx * 8 + 1889] = 1;
        break;
    case 2:
        screen_w[rec_idx * 8 + 1889] = 1;
        break;
    case 3:
        screen_w[rec_idx * 8 + 1889] = 1;
        break;
    case 4:
        if (6U == ((u32)girl4_basyo >> 4)) {
            screen_w[rec_idx * 8 + 1889] = 5;
            screen_w[rec_idx * 8 + 1893] = 255;
        } else {
            screen_w[rec_idx * 8 + 1889] = 1;
        }
        break;
    case 5:
        screen_w[rec_idx * 8 + 1889] = 1;
        break;
    case 6:
        screen_w[rec_idx * 8 + 1889] = 0;
        break;
    case 7:
        screen_w[rec_idx * 8 + 1889] = 0;
        break;
    case 8:
        screen_w[rec_idx * 8 + 1889] = 1;
        break;
    case 9:
        screen_w[rec_idx * 8 + 1889] = 5;
        screen_w[rec_idx * 8 + 1893] = 255;
        break;
    case 10:
        screen_w[rec_idx * 8 + 1889] = 1;
        break;
    case 11:
        screen_w[rec_idx * 8 + 1889] = 1;
        break;
    case 12:
        screen_w[rec_idx * 8 + 1889] = 1;
        break;
    }

    v = (u8)(rec_idx + 1);
    rec_idx = (u8)v;
    if (v >= 255U) {
        rec_idx = 254;
    }
}
