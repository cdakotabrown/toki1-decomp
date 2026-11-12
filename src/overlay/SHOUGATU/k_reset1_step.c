/* Address: 0x801392B4 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void k_reset();

void k_reset1_step(void)
{
    k_reset(1);
    sub_sub_step_up();
}
