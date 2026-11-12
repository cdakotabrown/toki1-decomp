/* Address: 0x80141638 */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void k_reset();
extern s32 check_end_k();

void shougatu_k_end_wait(void)
{
    if (check_end_k() != 0) {
        k_reset(1);
        sub_sub_step_up();
    }
}
