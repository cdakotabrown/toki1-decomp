/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=28:v1,84:a2,59:a3 */
/* @SymbolStart: 0x8013BB7C */
/* @SymbolSize: 432 */
#include "types.h"

extern void season_bgm_set();
extern void CDQ_init();
extern void device_state_init();
extern s32 device_load_poll();
extern void param_clamp();
extern void return_schedule();
extern u8 step_mode;
extern u32 step_counter;
extern u8 now_girl;
extern u8 screen_w[];

s32 shougatu_schedule_return3()
{
    u8 sel;
    s16 i;
    s32 flags;
    u8 *rec;
    u8 *p;

    if (step_mode == 0) {
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
    for (i = 0; i < 11; i++) {
        rec = screen_w + i * 56;
        flags = rec[457];
        if (flags & 1) {
            rec[457] = flags & 0xFFFE;
            if (i != (&now_girl)[0]) {
                *(s16 *)(rec + 450) += 1;
            }
        }
    }
    p = (u8 *)((s32)screen_w + (s32)now_girl * 56U);
    *(s16 *)(p + 446) += 8;
    p = (u8 *)((s32)screen_w + (s32)now_girl * 56U);
    *(s16 *)(p + 450) += 8;
    p = (u8 *)((s32)screen_w + (s32)now_girl * 56U);
    *(s16 *)(p + 454) += -10;
    param_clamp();
    return_schedule(1);
}
