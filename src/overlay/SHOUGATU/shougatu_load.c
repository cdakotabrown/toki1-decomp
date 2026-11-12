/* Address: 0x801326BC */
/* Size: 60 bytes (15 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void cd_read_start2();
extern void shougatu_res_init();

void shougatu_load(void)
{
    cd_read_start2(52, 0x80197000, 44872);
    shougatu_res_init();
    sub_sub_step_up();
}
