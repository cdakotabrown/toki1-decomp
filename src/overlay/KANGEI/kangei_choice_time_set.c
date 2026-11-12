/* @SymbolStart: 0x80136A2C */
/* @SymbolSize: 76 */
/* @PostProc: patch_insn_reg_field 0x24:rt:t9:t8 0x30:rs:t9:t8 0x30:rt:t0:t8 0x34:rt:t8:t7 0x3c:rd:t1:t9 0x3c:rs:t8:t7 0x3c:rt:t0:t8 0x48:rt:t1:t9 */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 kangei_choice_timer;
extern s16 kangei_now_s;
extern s16 kangei_now_s_in;

void kangei_choice_time_set(void)
{
    s32 v = kangei_choice_timer;
    kangei_now_s = 12;
    kangei_now_s_in = (s16)((((u32)v < 0x97) ^ 1) + (((u32)v < 0x12d) ^ 1));
    sub_sub_step_up();
}
