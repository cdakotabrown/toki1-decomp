/* @SymbolStart: 0x8013785C */
/* @SymbolSize: 52 */
/* @PostProc: pad_trailing_nop --count 1 */
/* bunka_sd_count_wait2 - BUNKA_SD overlay, Address: 0x8013785C, Size: 52 bytes (13 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 step_counter;

void bunka_sd_count_wait2(s32 param0)
{
    if ((u32)param0 < (u32)step_counter) {
        sub_sub_step_up();
    }
}
