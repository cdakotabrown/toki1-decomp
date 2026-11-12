/* Address: 0x801393C4 */
/* Size: 48 bytes (12 insns) */
#include "types.h"

extern void sub_sub_step_up();
extern void goto_sub_step();

void taco_goto_sub5_skip(void)
{
    goto_sub_step(5);
    sub_sub_step_up();
    sub_sub_step_up();
}
