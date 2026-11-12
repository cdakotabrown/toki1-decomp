/* Address: 0x801392DC */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void cd_read_start2();
extern void shougatu_bank_set();

void shougatu_bank_load(void)
{
    cd_read_start2(113, 0x801B0000, 34230);
    shougatu_bank_set();
    sub_sub_step_up();
}
