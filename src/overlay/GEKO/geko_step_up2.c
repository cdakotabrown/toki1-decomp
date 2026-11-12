/* geko_step_up2 -- GEKO overlay, Address: 0x80140FEC, Size: 32 bytes (8 insns) */
#include "types.h"

extern void sub_sub_step_up(void);

void geko_step_up2(void)
{
    sub_sub_step_up();
}
