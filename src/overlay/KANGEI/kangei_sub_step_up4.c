/* kangei_sub_step_up4 - KANGEI overlay, Address: 0x80135AE0, Size: 56 bytes (14 insns) */
#include "types.h"

extern void sub_step_up(void);

void
kangei_sub_step_up4(void)
{
    sub_step_up();
    sub_step_up();
    sub_step_up();
    sub_step_up();
}
