/* @SymbolStart: 0x80133F5C */
/* @SymbolSize: 184 */
/* @PostProc: patch_insn_reg_field 0x30:rt:t9:t7 0x34:rs:t9:t7 0x34:rt:t9:t7 0x38:rd:t7:t8 0x3c:rd:t8:t9 0x3c:rt:t7:t8 0x40:rs:t8:t7 0x74:rt:t2:t4 0x7c:rs:v0:t1 0x7c:rt:t1:v0 0x80:rd:t3:t2 0x84:rs:t2:t4 0x84:rt:t2:t4 0x88:rd:t4:t3 0x88:rt:t3:t2 0x8c:rs:t2:t3 */
/* shugaku_birth_judge9 - SHUGAKU overlay, Address: 0x80133F5C, Size: 184 bytes */
#include "types.h"

extern s32 girl0_birth;
extern u8  now_month;
extern u8  now_day;
extern u8  now_girl;
extern u8  girl_no;
extern s32 p_birthday;

extern void goto_sub_step(s32 a0);
extern void sub_sub_step_up(void);

void shugaku_birth_judge9(void)
{
    s32 packed;
    s32 code;

    packed = girl0_birth;
    code = now_month;
    if (code == (packed & 0xF) && (s32)now_day == (s32)((u32)(packed << 23) >> 27)) {
        girl_no = now_girl;
        now_girl = 0;
        goto_sub_step(6);
        return;
    }
    packed = p_birthday;
    if (code == (packed & 0xF) && (s32)((u32)(packed << 23) >> 27) == (s32)now_day) {
        girl_no = now_girl;
        goto_sub_step(7);
        return;
    }
    sub_sub_step_up();
}
