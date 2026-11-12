/* date2_step_return - DATE2 overlay, Address: 0x8013765C, Size: 48 bytes (12 insns) */
#include "types.h"

extern u8 ret_sub_sub_step;

void goto_sub_step(s32 arg0);
void goto_sub_sub_step(s32 arg0);

void date2_step_return(void)
{
    goto_sub_step(2);
    goto_sub_sub_step((s32)ret_sub_sub_step);
}
