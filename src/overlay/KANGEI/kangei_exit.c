/* @PostProc: patch_insn_reg_field 0x54:rt:v0:v1 0x58:rs:v0:v1 0x58:rt:v0:v1 0x60:rs:v0:v1 0x60:rt:t8:v0 0x64:rs:t8:v0 0x64:rt:t9:v0 0x68:rs:v0:v1 0x68:rt:t7:v1 0x6c:rs:t9:v0 0x70:rt:t7:v1 0xa8:rt:t0:t7 0xb0:rt:t0:t7 */
#include "types.h"

extern void season_bgm_set(void);
extern s32 CDQ_init(void);
extern void device_state_init(void);
extern s32 device_load_poll(void);
extern u32 return_schedule(s32);

extern volatile u8 step_mode;
extern volatile s32 step_counter;

u32
kangei_exit(void)
{
    s32 hiddenV0 = step_mode;

    s32 value;
    s32 result;
    s32 counter;

    if (hiddenV0 == 0) {
        season_bgm_set();
        hiddenV0 = (u8)(step_mode + 1);
    }

    step_mode = hiddenV0;

    if (hiddenV0 != 1) {
        goto call_72C1C;
    }

    value = step_counter;
    result = ((u32)value < 1024) ^ 1;
    step_counter = value + 1;
    if (result != 0) {
        CDQ_init();
        device_state_init();
        step_mode = 0;
    }

    if (device_load_poll() == 1) {
        counter = step_mode;
        step_mode = counter + 1;
        goto call_72C1C;
    }
    return 0;

call_72C1C:

    return return_schedule(1);
}
