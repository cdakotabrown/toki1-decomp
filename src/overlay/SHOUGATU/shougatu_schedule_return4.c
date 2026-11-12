/* Address: 0x8013F498 (true start 0x8013F490: the step_mode state-byte */
/* @SymbolStart: 0x8013F490 */
/* @SymbolSize: 212 */
/* @PostProc: patch_insn_reg_field 0x14:rt:v1:v0 0x1c:rs:v1:v0 0x34:rt:v1:v0 0x38:rs:v1:v0 0x38:rt:v1:v0 0x40:rs:v1:v0 0x40:rt:v1:v0 0x44:rs:v1:v0 0x48:rd:v1:v0 0x50:rt:v1:v0 0x58:rs:v1:v0 0xa4:rt:t7:v0 0xa8:rs:t7:v0 0xa8:rt:t7:v0 0xb0:rs:t7:v0 0xb0:rt:t8:t7 0xb8:rt:t8:t7 */
#include "types.h"

extern void prim_init(void);
extern void device_state_init(void);
extern s32 device_load_poll(void);
extern void CDQ_init(void);
extern void text_sys_init(void);
extern void season_bgm_set(void);
extern void return_schedule(s32);
extern s32 step_counter;
extern u8 step_mode;

s32 shougatu_schedule_return4(void)
{
    s32 v0;
    s32 v1;
    s32 cond;

    v0 = step_mode;
    if (v0 == 0) {
        text_sys_init();
        season_bgm_set();
        v0 = (step_mode + 1) & 0xFF;
    }
    step_mode = (u8)v0;
    if (v0 == 1) {
        v1 = step_counter;
        cond = ((u32)v1 >= 1024U);
        step_counter = v1 + 1;
        if (cond) {
            CDQ_init();
            device_state_init();
            step_mode = 0;
        }
        if (device_load_poll() == 1) {
            step_mode = step_mode + 1;
        } else {
            return 0;
        }
    }
    prim_init();
    return_schedule(1);

}
