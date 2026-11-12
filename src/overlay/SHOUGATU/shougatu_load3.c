/* Address: 0x8013D88C */
/* Size: 60 bytes (15 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void cd_read_start2();
extern void shougatu_ptrs2_set(void);

void shougatu_load3(void)
{
    cd_read_start2(14, (void*)0x80197000, 0xAFBC);
    shougatu_ptrs2_set();
    sub_sub_step_up();
}
