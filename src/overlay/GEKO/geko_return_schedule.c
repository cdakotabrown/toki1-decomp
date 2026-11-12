/* @SymbolStart: 0x80135A84 */
/* @SymbolSize: 244 */
/* @PostProc: scheduler_normalizer --const-fp --store-fp --branch-fp */
/* @PostProc: patch_insn_reg_field 0x44:rt:v1:v0 0x4c:rs:v1:v0 0x5c:rt:v1:v0 0x60:rs:v1:v0 0x60:rt:v1:v0 0x68:rt:v1:v0 0x6c:rs:v1:v0 0x70:rd:v1:v0 0x78:rt:v1:v0 0x80:rs:v1:v0 0x88:rt:v0:v1 0x90:rs:v0:v1 0x90:rt:t9:v0 0x94:rs:t9:v0 0x94:rt:t0:v0 0x9c:rs:t0:v0 0xcc:rt:t1:v0 0xd0:rs:t1:v0 0xd0:rt:t1:v0 0xd8:rs:t1:v0 0xd8:rt:t2:t9 0xe0:rt:t2:t9 */
/* geko_return_schedule -- GEKO overlay, Address: 0x80135A8C, Size: 236 bytes */
#include "types.h"

extern void goto_step(s32 a0);
extern void device_cmd_send(s32 a0);
extern s32 device_state_init(void);
extern s32 device_load_poll(void);
extern void CDQ_init(void);
extern void season_bgm_set(void);
extern s32 return_schedule(s32 a0);

extern s32 step_counter;
extern u8 step_mode;
extern u8 day_flag;

s32 geko_return_schedule(void)
{
    s32 t6 = day_flag;

    s32 state;
    s32 count;

    if (t6 & 0x20) {
        device_cmd_send(207);
        goto_step(82);
        return 0;
    }

    state = step_mode;
    if (state == 0) {
        season_bgm_set();
        state = (u8)(step_mode + 1);
    }
    step_mode = state;

    if (state == 1) {
        count = step_counter;
        step_counter = count + 1;
        if (((u32)count < 1024) ^ 1) {
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

    return return_schedule(1);
}
