/* Address: 0x80140B48 */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void cd_read_start2();
extern void shougatu_ptr_set2();

void shougatu_cd_load2(void)
{
    cd_read_start2(13, 0x801B0000, 48329);
    shougatu_ptr_set2();
    sub_sub_step_up();
}
