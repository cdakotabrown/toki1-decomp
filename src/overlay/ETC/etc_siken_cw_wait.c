/* @SymbolStart: 0x8014A100 */
/* @SymbolSize: 216 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x8c */
/* @PostProc: patch_insn_reg_field 0x38:rt:v0:v1 0x40:rs:v0:v1 0x44:rs:v0:v1 0x44:rt:t1:v0 0x64:rt:v0:v1 0x6c:rs:v0:v1 0x6c:rt:t1:v0 0x7c:rt:v0:v1 0x84:rs:v0:v1 0x84:rt:t1:v0 0x88:rs:t1:v0 0x88:rt:t2:v0 0x8c:rs:v0:v1 0x94:rs:t2:v0 0x9c:rt:t3:t1 0xa0:rs:t3:t1 0xa0:rt:t3:t1 0xa4:rt:t4:t2 0xa8:rs:t3:t1 0xb0:rt:t6:t4 0xb4:rs:t4:t2 0xb4:rt:t4:t2 0xbc:rs:t4:t2 0xbc:rt:t5:t3 0xc4:rt:t5:t3 0xc8:rs:t6:t4 0xc8:rt:t6:t4 0xd0:rs:t6:t4 0xd0:rt:t7:t5 0xd4:rt:t7:t5 */
/* Address: 0x8014A108 */
/* Size: 208 bytes (52 instructions) */
#include "types.h"

extern void device_cmd_send(s32);
extern s32 step_counter;
extern u8 step_mode;
extern s16 cal_cw_tick;
extern s16 cal_cw_anim;
extern s32 etc_siken_qno;

extern u8 cal_cw_mode;

void etc_siken_cw_wait(void)
{
    s32 t6 = cal_cw_mode;

    s32 v1;

    if ((t6 & 1) != 0) {
        step_counter = 0;
        return;
    }
    v1 = cal_cw_tick;
    if (v1 != 0) {
        goto check;
    }
    if (cal_cw_anim == 0) {
        goto do_1281;
    }
    device_cmd_send(1282);
    v1 = cal_cw_tick;
    goto check;
do_1281:
    device_cmd_send(1281);
    v1 = cal_cw_tick;
check:
    cal_cw_tick = (s16)(v1 + 1);
    if (((v1 < 61) ^ 1) == 0) {
        return;
    }
    if (etc_siken_qno >= 10) {
        step_mode = (u8)(step_mode + 1);
        return;
    }
    step_mode = (u8)(step_mode - 1);
}
