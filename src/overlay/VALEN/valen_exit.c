/* @SymbolStart: 0x801339F8 */
/* @SymbolSize: 196 */
/* @PostProc: patch_insn_reg_field 0x58:rt:v0:v1 0x60:rs:v0:v1 0x60:rt:t7:v0 0x64:rs:t7:v0 0x64:rt:t8:v0 0x68:rs:v0:v1 0x6c:rs:t8:v0 0xa8:rt:t9:t7 0xb0:rt:t9:t7 */
/* valen_exit - VALEN overlay, Address: 0x801339F8 */
#include "types.h"

extern volatile u8 step_mode;
extern s32 step_counter;
extern void season_bgm_set(void);
extern void CDQ_init(void);
extern void device_state_init(void);
extern s32 device_load_poll(void);
extern void return_schedule(s32);

s32 valen_exit(void)
{
    s32 val;
    s32 counter;

    val = step_mode;
    if (val == 0) {
        season_bgm_set();
        val = (u8)((u8)step_mode + 1);
    }
    step_mode = (u8)val;
    if (val != 1) {
        goto call_72c1c;
    }
    counter = step_counter;
    step_counter = counter + 1;
    if ((((u32)counter < 1024) ^ 1) != 0) {
        CDQ_init();
        device_state_init();
        step_mode = 0;
    }
    if (device_load_poll() == 1) {
        val = step_mode;
        step_mode = (u8)(val + 1);
    } else {
        return 0;
    }
call_72c1c:
    return_schedule(1);
    return;
}
