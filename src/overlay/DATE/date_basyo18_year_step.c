/* @SymbolStart: 0x8013A130 */
/* @SymbolSize: 372 */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
/* Function: date_basyo18_year_step */
/* Overlay: DATE */
/* Address: 0x8013A130 (cross-prologue head) / 0x8013A138 (body) */
/* Size: 372 bytes (93 instructions) */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 goto_sub_sub_step();
extern s32 param_clamp();

extern volatile u8 now_year;
extern volatile u8 now_girl;
extern s16 now_serifu;
extern u8 screen_w[];

void date_basyo18_year_step(void)
{
    s32 evt;
    s32 girl;
    s16 *cnt;

    evt = now_year;
    if (evt == 95) {
        girl = now_girl;
        if (girl == 2 || girl == 5 || girl == 8 || girl == 10) {
            now_serifu = 1;
        }
        cnt = (s16 *)(screen_w + (s32)now_girl * 56);
        cnt[223] = (s16)(cnt[223] + 1);
        sub_sub_step_up();
    } else if (evt == 96) {
        girl = now_girl;
        if (girl == 2 || girl == 5 || girl == 8 || girl == 10) {
            now_serifu = 4;
        } else {
            now_serifu = 6;
        }
        goto_sub_sub_step(20);
    } else {
        girl = now_girl;
        if (girl == 2 || girl == 5 || girl == 8 || girl == 10) {
            now_serifu = 10;
        } else {
            now_serifu = 12;
        }
        cnt = (s16 *)(screen_w + (s32)now_girl * 56);
        cnt[223] = (s16)(cnt[223] + 1);
        goto_sub_sub_step(31);
    }
    param_clamp();
}
