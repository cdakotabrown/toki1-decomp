/* @SymbolStart: 0x80134B54 */
/* @SymbolSize: 48 */
/* shugaku_ret_next_goto - SHUGAKU overlay, Address: 0x80134B54, Size: 48 bytes (12 insns) */
#include "types.h"

extern void goto_sub_step(s32 a0);
extern u8 ret_sub_step;

void shugaku_ret_next_goto(void)
{
    s32 a0 = ret_sub_step;

    a0 = (a0 + 1) & 0xFF;
    goto_sub_step(a0);
}
