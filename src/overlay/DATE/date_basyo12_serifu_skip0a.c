/* @SymbolStart: 0x80139624 */
/* @SymbolSize: 176 */
/* @PostProc: scheduler_normalizer --const-fp */
/* Function: date_basyo12_serifu_skip0a */
/* Overlay: DATE */
/* Address: 0x80139624 (cross-prologue head) / 0x8013962C (body) */
/* Size: 176 bytes (44 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);

extern u8 date_first_sun_flag;
extern u8 now_girl;
extern s16 now_serifu;
extern u8 now_sub_sub_step;

void date_basyo12_serifu_skip0a(void)
{
    s32 v;

    if (date_first_sun_flag == 0) {
        v = now_girl;
        now_serifu = 9;
        if (v == 7) {
            now_serifu = 10;
        } else if (v == 9) {
            now_serifu = 8;
        } else if (v == 4 || v == 8 || v == 5 || v == 10) {
            now_serifu = 7;
        }
        now_sub_sub_step = now_sub_sub_step + 20;
    }
    sub_sub_step_up();
}
