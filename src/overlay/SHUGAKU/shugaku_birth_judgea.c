/* @SymbolStart: 0x80134C88 */
/* @SymbolSize: 244 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x5c */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xbc */
/* @PostProc: swap_instructions --offset1 0xb4 --offset2 0xb8 */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xc8 */
/* @PostProc: swap_instructions --offset1 0xc0 --offset2 0xc4 */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xcc */
/* @PostProc: patch_insn_reg_field 0x30:rt:t9:t7 0x34:rs:t9:t7 0x34:rt:t9:t7 0x38:rd:t7:t8 0x3c:rd:t8:t9 0x3c:rt:t7:t8 0x40:rs:t8:t7 0x88:rt:t3:t5 0x90:rs:v0:t2 0x90:rt:t2:v0 0x94:rd:t4:t3 0x98:rs:t3:t5 0x98:rt:t3:t5 0x9c:rd:t5:t4 0x9c:rt:t4:t3 0xa0:rs:t3:t4 0xb8:rt:t7:t8 0xbc:rs:t7:t8 0xbc:rt:t7:t8 0xc4:rt:t8:t7 0xc8:rt:t7:t8 0xcc:rs:t8:t7 0xcc:rt:t8:t7 0xd4:rs:t8:t7 */
#include "types.h"

extern s32 girl0_birth;
extern u8  now_month;
extern u8  now_day;
extern u8  ret_sub_step;
extern u8  ret_sub_sub_step;
extern u8  now_girl;
extern u8  girl_no;
extern u8  day_flag;
extern s32 p_birthday;

extern void goto_sub_step(s32 a0);
extern void sub_sub_step_up(void);

void shugaku_birth_judgea(void)
{
    s32 packed;
    s32 code;

    packed = girl0_birth;
    code = now_month;
    if (code == (packed & 0xF) &&
        (s32)now_day == (s32)((u32)(packed << 23) >> 27)) {
        ret_sub_step = 5;
        ret_sub_sub_step = 0;
        girl_no = now_girl;
        now_girl = 0;
        goto_sub_step(6);
        return;
    }
    packed = p_birthday;
    if (code == (packed & 0xF) &&
        (s32)((u32)(packed << 23) >> 27) == (s32)now_day) {
        ret_sub_step = 5;
        ret_sub_sub_step = 0;
        girl_no = now_girl;
        day_flag = (u8)(day_flag | 0x08);
        goto_sub_step(7);
        return;
    }
    sub_sub_step_up();
}
