/* @SymbolStart: 0x801337EC */
/* @SymbolSize: 260 */
/* @PostProc: pad_trailing_nop --count 3 */
/* @PostProc: patch_insn_reg_field 0x54:rt:v0:v1 0x58:rs:v0:v1 0x58:rt:v0:v1 0x60:rs:v0:v1 0x60:rt:t8:v0 0x64:rs:t8:v0 0x64:rt:t9:v0 0x68:rs:v0:v1 0x68:rt:t7:v1 0x6c:rs:t9:v0 0x70:rt:t7:v1 0xa8:rt:t0:t7 0xb0:rt:t0:t7 0xbc:rt:v1:t8 0xc0:rs:v1:t8 0xc0:rt:v1:t8 0xc4:rt:t2:t0 0xc8:rd:t1:t9 0xc8:rt:v1:t8 0xcc:rd:t1:t9 0xcc:rs:t1:t9 0xcc:rt:v1:t8 0xd0:rd:t1:t9 0xd0:rt:t1:t9 0xd4:rs:t2:t0 0xd4:rt:t2:t0 0xd8:rs:t1:t9 0xd8:rt:t2:t0 0xdc:rt:t3:t1 0xe4:rs:t3:t1 0xe4:rt:t4:t2 0xec:rt:t4:t2 */
/* geko_schedule_return - GEKO overlay, Address: 0x801337F4, Size: 252 bytes (60 insns) */
#include "types.h"

extern s32 device_state_init(void);
extern s32 device_load_poll(void);
extern void CDQ_init(void);
extern void season_bgm_set(void);
extern s32 return_schedule(s32);
extern void param_clamp(void);

extern u8 screen_w[];

extern volatile u8 now_girl;
extern volatile s32 step_counter;
extern volatile u8 step_mode;

s32 geko_schedule_return(void)
{
    s32 v0 = step_mode;

    s32 stateVal;
    s32 counter;
    u8 idx;

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

    idx = now_girl;
    *(s16 *)(screen_w + idx * 56 + 0x1C2) += 2;
    param_clamp();
    return return_schedule(1);
}
