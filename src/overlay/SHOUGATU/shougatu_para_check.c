/* Address: 0x8013D924 (true start 0x8013D8F0, .sym mislabeled the boundary -- */
/* Size: 268 bytes (67 instructions) from the true start */
/* @SymbolStart: 0x8013D8F0 */
/* @SymbolSize: 268 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x18:rt:t8:t0 0x1c:rt:v0:v1 0x20:rd:v0:v1 0x24:rd:t9:v0 0x24:rt:v0:v1 0x28:rd:t9:v0 0x28:rs:t9:v0 0x28:rt:v0:v1 0x2c:rd:t9:v0 0x2c:rt:t9:v0 0x30:rd:t9:v0 0x30:rs:t9:v0 0x30:rt:v0:v1 0x34:rs:t9:v0 0x34:rt:t0:v0 0x38:rs:t8:t0 0x38:rt:t8:t0 0x3c:rd:t1:t8 0x3c:rt:t0:v0 0x40:rd:t2:t9 0x40:rt:t1:t8 0x48:rs:t8:t0 0x48:rt:t2:t9 0x54:rt:t3:t1 0x58:rs:t3:t1 0x58:rt:t3:t1 0x60:rd:t4:t2 0x60:rt:t3:t1 0x64:rd:t5:t3 0x64:rt:t4:t2 0x68:rs:t5:t3 0x6c:rs:v0:v1 0x6c:rt:t9:t6 0x98:rs:v0:v1 0x98:rt:t9:t6 0x9c:rs:t9:t6 0x9c:rt:t9:t6 0xa0:rd:t0:t7 0xa0:rt:t9:t6 0xa4:rd:t7:t5 0xa4:rt:v0:v1 0xa8:rd:t7:t5 0xa8:rs:t7:t5 0xa8:rt:v0:v1 0xac:rd:t0:t7 0xac:rs:t0:t7 0xac:rt:t9:t6 0xb0:rd:t0:t7 0xb0:rt:t0:t7 0xb4:rd:t7:t5 0xb4:rt:t7:t5 0xb8:rd:t1:t8 0xb8:rs:t7:t5 0xb8:rt:t0:t7 0xbc:rs:t1:t8 0xbc:rt:t8:t9 0xc0:rt:t6:t4 0xc4:rs:t6:t4 0xc4:rt:t6:t4 0xc8:rd:t2:t0 0xc8:rt:t8:t9 0xcc:rd:t3:t1 0xcc:rt:t2:t0 0xd0:rs:t6:t4 0xd0:rt:t3:t1 */
#include "types.h"

extern void sub_sub_step_up();
extern s32 girl12_flag;
extern s16 para_un;
extern s32 girl14_flag;
extern s16 para_you;

void shougatu_para_check(void)
{
    u32 v1;
    s16 t9;
    s32 t3;
    u32 flag;
    s16 t1;

    v1 = ((u32)girl12_flag << 20) >> 29;
    t9 = (s16)(v1 * 25 + 150);

    if (para_un >= t9) {
        t3 = ((u32)girl14_flag << 20) >> 29;
        if (t3 != 0) {
            sub_sub_step_up();
            sub_sub_step_up();
            sub_sub_step_up();
            sub_sub_step_up();
            return;
        }
    }

    flag = (v1 >= 2);
    t1 = (s16)(v1 * 30 - flag * 10 + 70);

    if (para_you >= t1) {
        sub_sub_step_up();
    } else {
        sub_sub_step_up();
        sub_sub_step_up();
        sub_sub_step_up();
        sub_sub_step_up();
    }
}
