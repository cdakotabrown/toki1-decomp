/* geko_step_up -- GEKO overlay, Address: 0x8013BF9C, Size: 32 bytes (8 insns) */
#include "types.h"

extern void sub_sub_step_up(void);

void geko_step_up(void)
{
    sub_sub_step_up();
}
