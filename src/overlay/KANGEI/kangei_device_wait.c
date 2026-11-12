/* @SymbolSize: 128 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t6:v0 0x4c:rs:t6:v0 0x74:rt:t7:t6 0x7c:rs:t7:t6 */
/* kangei_device_wait -- KANGEI overlay, Address: 0x80133844, Size: 128 bytes (32 insns) */
#include "types.h"

extern s32 device_load_poll(void);
extern void sub_sub_step_up(void);
extern void CDQ_init(void);
extern void device_state_init(void);
extern void goto_sub_sub_step(s32);

extern s32 step_counter;
extern u8 now_sub_sub_step;

void kangei_device_wait(void)
{
    s32 v1;
    u32 cmp;

    if (device_load_poll() == 1) {
        sub_sub_step_up();
    }
    v1 = step_counter;
    cmp = ((u32)v1 < 1024U) ^ 1;
    v1++;
    step_counter = v1;
    if (cmp != 0) {
        CDQ_init();
        device_state_init();
        goto_sub_sub_step((u8)(now_sub_sub_step - 1));
    }
}
