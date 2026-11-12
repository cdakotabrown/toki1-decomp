/* @SymbolStart: 0x801357B4 */
/* @SymbolSize: 44 */
/* @PostProc: patch_insn_reg_field 0x20:rt:t7:at 0x28:rs:t7:at */
/* kangei_girl_restore2 - KANGEI overlay, Address: 0x801357BC, Size: 36 bytes (9 insns) */
#include "types.h"
extern s32 girl_no_bak;

extern void sub_sub_step_up(void);
extern u8 girl_no;

void
kangei_girl_restore2(void)
{
    s32 hidden_t6 = girl_no;

    *(u8 *)(0x800F0000 - 2369) = (u8)hidden_t6;
    sub_sub_step_up();
}
