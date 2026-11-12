/* date2_birthday_jump - DATE2 overlay, Address: 0x80137948, Size: 132 bytes (33 insns) */
/* @SymbolSize: 132 */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x3C --offset2 0x40 */
/* @PostProc: patch_insn_reg_field 0x28:rt:t6:t7 0x2C:rs:t6:t7 0x2C:rt:t6:t7 0x30:rt:t7:t6 */
/* @PostProc: patch_insn_reg_field 0x38:rd:t9:t8 0x3C:rt:t8:t0 0x40:rs:t8:t0 0x40:rt:t8:t0 */
/* @PostProc: patch_insn_reg_field 0x44:rd:t0:t9 0x44:rt:t9:t8 0x48:rs:t8:t9 */
/* @PostProc: patch_insn_reg_field 0x4C:rt:t1:t2 0x50:rs:t1:t2 */
/* @PostProc: patch_insn_reg_field 0x58:rt:t2:t3 0x5C:rs:t2:t3 0x5C:rt:t2:t3 0x64:rs:t2:t3 0x64:rt:t3:t4 0x68:rt:t3:t4 */
#include "types.h"

extern u32 p_birthday;
extern u8 now_month;
extern u8 now_day;
extern u8 day_flag;

s32 get_g_zyotai_s(s32 arg0);
void goto_sub_step(s32 arg0);
void sub_sub_step_up(void);

void date2_birthday_jump(void)
{
    s32 v0;
    u32 v1;

    v0 = get_g_zyotai_s(0);
    v1 = p_birthday;
    if ((u32)now_month == (v1 & 0x0F)) {
        if ((u32)now_day == ((v1 << 23) >> 27)) {
            if (((u32)v0 & 0x7F) < 2U) {
                day_flag = day_flag | 0x04;
                goto_sub_step(4);
                return;
            }
        }
    }
    sub_sub_step_up();
}
