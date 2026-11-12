/* Address: 0x801417E4 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void bustup_cd_read();

void bustup_cd_read56c0_step(void)
{
    bustup_cd_read(22208);
    sub_sub_step_up();
}
