/* olh_exit - OLH overlay, Address: 0x801323AC, Size: 32 bytes (8 insns) */
#include "types.h"

extern void goto_step(s32);

void
olh_exit(void)
{

    goto_step(52);
}
