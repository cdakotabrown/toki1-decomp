/* kangei_step_nop - KANGEI overlay, Address: 0x80135AC0, Size: 32 bytes (8 insns) */
#include "types.h"

extern void sub_sub_step_up(void);

void
kangei_step_nop(void)
{
    sub_sub_step_up();
}
