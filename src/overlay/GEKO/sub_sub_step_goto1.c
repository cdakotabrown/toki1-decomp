/* sub_sub_step_goto1 -- GEKO overlay, Address: 0x80135554, Size: 32 bytes (8 insns) */
#include "types.h"

extern void goto_sub_sub_step(s32);

void sub_sub_step_goto1(void)
{
    goto_sub_sub_step(1);
}
