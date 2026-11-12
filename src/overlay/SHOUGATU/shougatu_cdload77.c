/* Address: 0x801402F0 */
/* Size: 48 bytes (12 instructions) */
/* Size: 0xBDBA (48570) bytes */
#include "types.h"

extern void sub_sub_step_up();
extern void cd_read_start2();

void shougatu_cdload77(void)
{
    cd_read_start2(77, (void *)0x801B0000, 0xBDBA);
    sub_sub_step_up();
}
