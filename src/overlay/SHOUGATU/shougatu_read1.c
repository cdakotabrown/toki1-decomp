/* Address: 0x8013E234 */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void cd_read_start2();
extern void shougatu_ptr_set();

void shougatu_read1(void)
{
    cd_read_start2(13, 0x801A0000, 48557);
    shougatu_ptr_set();
    sub_sub_step_up();
}
