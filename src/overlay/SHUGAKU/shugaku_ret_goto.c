/* @SymbolSize: 56 */
/* shugaku_ret_goto - SHUGAKU overlay, Address: 0x80134B1C, Size: 64 bytes (16 insns) */
#include "types.h"

extern u8 ret_sub_step;
extern u8 ret_sub_sub_step;

extern void goto_sub_step(u8);
extern void goto_sub_sub_step(u8);

void shugaku_ret_goto(void)
{
    goto_sub_step(ret_sub_step);
    goto_sub_sub_step(ret_sub_sub_step);
}
