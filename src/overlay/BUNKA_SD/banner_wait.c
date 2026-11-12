/* @SymbolSize: 68 */
/* @PostProc: swap_registers t7:t6 */
/* @PostProc: swap_registers v0:t7 */
/* @PostProc: patch_insn_reg_field 0x20:rt:t6:t7 0x24:rs:t6:t7 */
/* banner_wait - BUNKA_SD overlay, Address: 0x80132DB8, Size: 76 bytes (19 insns) */
#include "types.h"

extern void banner_scroll(void);
extern void sub_sub_step_up(void);

void banner_wait(s32 param0)
{
    s32 counter;
    banner_scroll();
    counter = *(s32 *)0x800EF864U;
    if ((u32)param0 < (u32)counter) {
        sub_sub_step_up();
    }
}
