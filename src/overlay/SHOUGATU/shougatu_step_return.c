/* @PostProc: pad_trailing_nop --count 3 */
/* @SymbolSize: 68  * Status: BYTE-PERFECT 🎉 */
/* Address: 0x8014180C */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void goto_sub_step();
extern void goto_sub_sub_step();
extern u8 ret_sub_step;
extern u8 ret_sub_sub_step;

void shougatu_step_return(void)
{
    goto_sub_step(ret_sub_step);
    goto_sub_sub_step(ret_sub_sub_step);
}
