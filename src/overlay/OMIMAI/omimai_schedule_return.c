/* @SymbolStart: 0x801323F8 */
/* @SymbolSize: 196 */
/* @CompileFlags: -Wo,-notail */
/* @PostProc: patch_insn_reg_field 0x14:rt:v1:v0 0x1c:rs:v1:v0 0x2c:rt:v1:v0 0x30:rs:v1:v0 0x30:rt:v1:v0 0x38:rs:v1:v0 0x38:rt:v1:v0 0x3c:rs:v1:v0 0x40:rd:v1:v0 0x48:rt:v1:v0 0x50:rs:v1:v0 0x64:rt:t7:v0 0x6c:rs:t7:v0 0x9c:rt:t8:v0 0xa0:rs:t8:v0 0xa0:rt:t8:v0 0xa8:rs:t8:v0 0xa8:rt:t9:t7 0xb0:rt:t9:t7 */
/* omimai_schedule_return - OMIMAI overlay, Address: 0x80132400, Size: 188 bytes (47 insns) */
#include "types.h"

extern u8 step_mode;
extern s32 step_counter;
extern void season_bgm_set(void);
extern void CDQ_init(void);
extern void device_state_init(void);
extern s32 device_load_poll(void);
extern s32 return_schedule(s32);

s32 omimai_schedule_return(void)
{
    s32 v0_hidden = step_mode;
    s32 v1;
    s32 t7;

    if (v0_hidden == 0) {
        season_bgm_set();
        v0_hidden = (u8)(step_mode + 1);
    }
    step_mode = (u8)v0_hidden;
    if (v0_hidden != 1) {
        goto do_transition;
    }
    v1 = step_counter;
    t7 = ((u32)v1 < 1024) ^ 1;
    v1 = v1 + 1;
    step_counter = v1;
    if (t7 != 0) {
        CDQ_init();
        device_state_init();
        step_mode = 0;
    }
    if (device_load_poll() == 1) {
        t7 = step_mode + 1;
        step_mode = t7;
        goto do_transition;
    }
    return 0;

do_transition:
    return return_schedule(1);
}
