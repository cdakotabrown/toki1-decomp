/* @SymbolSize: 56 */
/* valen_return_step - VALEN overlay, Address: 0x80134238, Size: 56 bytes (14 insns) */
#include "types.h"

extern u8 ret_sub_step;
extern u8 ret_sub_sub_step;
extern void goto_sub_step(s32);
extern void goto_sub_sub_step(s32);

void valen_return_step(void)
{
    goto_sub_step(ret_sub_step);
    goto_sub_sub_step(ret_sub_sub_step);
}
