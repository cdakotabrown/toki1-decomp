/* @SymbolStart: 0x80132398 */
/* @SymbolSize: 96 */
/* @PostProc: patch_insn_reg_field 0x34:rt:t1:t0 0x38:rs:t1:t0 0x38:rt:t1:t0 0x3c:rt:t0:t9 0x40:rs:t0:t9 0x40:rt:t1:t0 */
/* omimai_birthday_step - OMIMAI overlay, Address: 0x801323AC, Size: 84 bytes (21 insns) */
#include "types.h"

extern s32 p_birthday;
extern u8  now_day;

extern u8 now_month;
extern void sub_step_up(void);
extern void sub_sub_step_up(void);

void omimai_birthday_step(void)
{
    s32 v0_val;

    v0_val = p_birthday;
    if (now_day == (s32)((u32)(v0_val << 23) >> 27)) {
        if (now_month == (u8)(v0_val & 0xF)) {
            sub_step_up();
            return;
        }
    }
    sub_sub_step_up();
}
