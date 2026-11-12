/* Address: 0x8013DDF4 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void bustup_cd_read();

void bustup_cd_read6b24_step(void)
{
    bustup_cd_read(27428);
    sub_sub_step_up();
}
