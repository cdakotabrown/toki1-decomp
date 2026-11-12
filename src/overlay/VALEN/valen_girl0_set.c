/* valen_girl0_set - VALEN overlay, Address: 0x80132810, Size: 36 bytes (9 insns) */
#include "types.h"

extern u8 now_girl;
extern void sub_sub_step_up(void);

void valen_girl0_set(void)
{
    now_girl = 0;
    sub_sub_step_up();
}
