/* @SymbolStart: 0x80134228 */
/* @SymbolSize: 56 */
/* @PostProc: pad_trailing_nop --count 2 */
/* bunka_sd_count_wait - BUNKA_SD overlay, Address: 0x80134228, Size: 56 bytes (14 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 step_counter;

void bunka_sd_count_wait(s32 param0)
{
    if ((u32)param0 < (u32)step_counter) {
        sub_sub_step_up();
    }
}
