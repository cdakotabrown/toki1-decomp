/* geko_step_skip6 -- GEKO overlay, Address: 0x80136648, Size: 72 bytes (18 insns) */
#include "types.h"

extern void sub_sub_step_up(void);

void geko_step_skip6(void)
{
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
}
