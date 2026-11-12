/* shugaku_goto_sub1f - SHUGAKU overlay, Address: 0x801355C0, Size: 32 bytes (8 insns) */
#include "types.h"

extern void goto_sub_sub_step(s32);

void shugaku_goto_sub1f(void)
{
    goto_sub_sub_step(0x1F);
}
