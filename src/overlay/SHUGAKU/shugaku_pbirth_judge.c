/* @PostProc: swap_registers t6:t7 --bidirectional --exclude-offsets 0x3C */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t8:t0 */
/* @PostProc: patch_insn_reg_field 0x48:rs:t8:t0 0x48:rt:t8:t0 */
/* @PostProc: patch_insn_reg_field 0x4C:rd:t0:t9 0x4C:rt:t9:t8 */
/* @PostProc: patch_insn_reg_field 0x50:rs:t8:t9 */
/* @PostProc: patch_insn_reg_field 0x40:rd:t9:t8 */
#include "types.h"

extern u8  girl_no;
extern u32 p_birthday;
extern u8  now_month;
extern u8  now_day;

extern s32  get_g_zyotai_s(s32 a0);
extern void goto_sub_step(s32 a0);
extern void sub_sub_step_up(void);

void shugaku_pbirth_judge(void)
{
    u32 flags;

    get_g_zyotai_s((s32)girl_no);
    flags = p_birthday;
    if ((flags & 0xF) == (u32)now_month) {
        if (((flags << 23) >> 27) == (u32)now_day) {
            goto_sub_step(7);
            return;
        }
    }
    sub_sub_step_up();
}
