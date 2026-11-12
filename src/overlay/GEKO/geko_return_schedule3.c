/* @SymbolStart: 0x8014298C */
/* @SymbolSize: 196 */
/* @PostProc: patch_insn_reg_field 0x14:rt:v1:v0 0x1c:rs:v1:v0 0x2c:rt:v1:v0 0x30:rs:v1:v0 0x30:rt:v1:v0 0x38:rs:v1:v0 0x38:rt:v1:v0 0x3c:rs:v1:v0 0x40:rd:v1:v0 0x48:rt:v1:v0 0x50:rs:v1:v0 0x64:rt:t7:v0 0x6c:rs:t7:v0 0x9c:rt:t8:v0 0xa0:rs:t8:v0 0xa0:rt:t8:v0 0xa8:rs:t8:v0 0xa8:rt:t9:t7 0xb0:rt:t9:t7 */
/* geko_return_schedule3 - GEKO overlay, Address: 0x80142994, Size: 188 bytes (47 insns) */
#include "types.h"

extern s32 device_state_init(void);
extern s32 device_load_poll(void);
extern void CDQ_init(void);
extern void season_bgm_set(void);
extern s32 return_schedule(s32 a0);

extern s32 step_counter;
extern u8 step_mode;

s32 geko_return_schedule3(void)
{
    s32 v0 = step_mode;

    s32 stateVal;
    s32 counter;

    if (v0 == 0) {
        season_bgm_set();
        v0 = (u8)(step_mode + 1);
    }
    step_mode = v0;
    stateVal = v0;

    if (stateVal == 1) {
        s32 over;
        counter = step_counter;
        over = ((u32)counter < 1024) ^ 1;
        step_counter = counter + 1;
        if (over != 0) {
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

    return return_schedule(1);
}
