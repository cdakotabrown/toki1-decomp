/* indexed_byte_toggle0_step2 -- GEKO overlay, Address: 0x8013F998, Size: 40 bytes (10 insns) */
#include "types.h"

extern void indexed_byte_toggle(s32);
extern void sub_sub_step_up(void);

void indexed_byte_toggle0_step2(void)
{
    indexed_byte_toggle(0);
    sub_sub_step_up();
}
