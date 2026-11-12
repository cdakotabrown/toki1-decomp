/* @SymbolStart: 0x8014154C */
/* @SymbolSize: 60 */
/* @PostProc: patch_insn_reg_field 0x14:rt:t6:t7 0x1c:rs:t6:t7 0x1c:rt:t7:t8 0x24:rt:t7:t8 0x30:rt:t8:t9 0x38:rt:t8:t9 */
#include "types.h"

extern void sub_sub_step_up(void);
extern u8 girl6_flag;
extern s16 shougatu_menu_now_serifu;

void shougatu_girl6_flag_set(void)
{
    girl6_flag |= 4;
    shougatu_menu_now_serifu = 4;
    sub_sub_step_up();
}
