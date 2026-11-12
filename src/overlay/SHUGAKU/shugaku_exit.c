/* @CompileFlags: -cckr */
/* @SymbolStart: 0x8013AB4C */
/* @SymbolSize: 344 */
#include "types.h"

extern void dec_bg_reset();
extern void season_bgm_set();
extern void CDQ_init();
extern void device_state_init();
extern s32 device_load_poll();
extern void hizuke_advance();
extern void param_clamp();
extern void return_schedule();
extern u8 step_mode;
extern u32 step_counter;
extern u8 date_place;
extern s16 girl10_yuukou;
extern u8 now_girl;
typedef struct { u8 b[56]; } Row;
extern Row screen_w[];

s32 shugaku_exit()
{
    u8 sel;
    u8 *p;

    if (step_mode == 0) {
        dec_bg_reset();
        season_bgm_set();
        step_mode++;
    }
    sel = step_mode;
    if (sel == 1) {
        if (step_counter++ >= 1024) {
            CDQ_init();
            device_state_init();
            step_mode = 0;
        }
        if (device_load_poll() == 1) {
            step_mode++;
        } else {
            return 0;
        }
    }
    hizuke_advance();
    if (date_place != 0) {
        girl10_yuukou++;
    } else {
        p = (u8 *)((s32)&screen_w[0] + (s32)now_girl * 56);
        *(s16 *)(p + 446) = (s16)(*(s16 *)(p + 446) + 1);
        p = (u8 *)((s32)&screen_w[0] + (s32)now_girl * 56);
        *(s16 *)(p + 450) = (s16)(*(s16 *)(p + 450) + 1);
    }
    param_clamp();
    return_schedule(1);
}
