/* Address: 0x80141B90 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern u8 bustup_cd_read_tbl0;

void shougatu_bustup_flag_set(void)
{
    bustup_cd_read_tbl0 = 1;
    sub_sub_step_up();
}
