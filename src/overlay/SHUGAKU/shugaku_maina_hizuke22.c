/* @PostProc: scheduler_normalizer --const-fp --store-fp --branch-fp */
/* @PostProc: patch_insn_reg_field 0x28:rt:t7:t6 0x2c:rs:t7:t6 0x2c:rt:t7:t6 0x30:rt:t6:t7 0x38:rt:t0:t8 0x3c:rs:t0:t8 0x3c:rt:t0:t8 0x44:rd:t9:t0 0x44:rt:t8:t9 0x48:rs:t9:t8 0x40:rd:t8:t9 */
/* @SymbolSize: 172 */
#include "types.h"

extern s32 girl0_birth;
extern u8  now_month;
extern u8  now_day;
extern u8  ret_sub_step;
extern u8  ret_sub_sub_step;
extern u8  now_girl;
extern u8  girl_no;

extern void shugaku_cmd_send(void);
extern void goto_sub_step(s32 a0);

void shugaku_maina_hizuke22(void)
{
    s32 v0;

    shugaku_cmd_send();
    v0 = girl0_birth;
    if ((s32)now_month == (v0 & 0xF) &&
        (s32)now_day == (s32)((u32)(v0 << 23) >> 27)) {
        ret_sub_step = (u8)(ret_sub_step + 1);
        ret_sub_sub_step = 0;
        girl_no = now_girl;
        now_girl = 0;
        goto_sub_step(6);
    } else {
        goto_sub_step((ret_sub_step + 1) & 0xFF);
    }
}
