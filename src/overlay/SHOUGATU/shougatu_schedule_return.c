/* Address: 0x80136434 (entry at 0x8013642C: the 2-insn "func_8013642C" */
/* Size: 316 bytes + 8-byte head (81 instructions total) */
/* @SymbolStart: 0x8013642C */
/* @SymbolSize: 324 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:v0 0x14:rs:v1:v0 0x2c:rt:v1:v0 0x30:rs:v1:v0 0x30:rt:v1:v0 0x38:rs:v1:v0 0x9c:rt:v1:v0 0xa0:rs:v1:v0 0xa0:rt:v1:v0 0xa8:rs:v1:v0 */
#include "types.h"

extern void season_bgm_set(void);
extern void CDQ_init(void);
extern void device_state_init(void);
extern s32 device_load_poll(void);
extern void param_clamp(void);
extern void parameter_show_init(void);
extern void parameter_disp_switch(s32);
extern void return_schedule(s32);

extern u8 step_mode;
extern u32 step_counter;
extern u8 shougatu_alone_flag;
extern u8 now_girl;
extern u8 day_flag;

typedef struct {
    s16 h0;
    s16 cnt;
    u8 rest[52];
} Rec56;
extern Rec56 screen_w[];

s32 shougatu_schedule_return(void)
{
    u32 v0;
    u32 v1;
    u32 idx;

    v0 = step_mode;
    if (v0 == 0) {
        season_bgm_set();
        v0 = (u8)(step_mode + 1);
    }
    step_mode = v0;
    if (v0 == 1) {
        u32 over;
        v1 = step_counter;
        over = (v1 >= 1024U);
        step_counter = v1 + 1;
        if (over) {
            CDQ_init();
            device_state_init();
            step_mode = 0;
        }
        if (device_load_poll() == 1) {
            step_mode += 1;
        } else {
            return 0;
        }
    }
    if (shougatu_alone_flag == 0) {
        idx = now_girl;
        screen_w[idx + 8].cnt += 1;
    }
    param_clamp();
    if (shougatu_alone_flag == 0) {
        day_flag |= 8;
    }
    parameter_show_init();
    parameter_show_init();
    parameter_disp_switch(0);
    return_schedule(1);
}
