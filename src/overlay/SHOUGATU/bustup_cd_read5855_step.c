/* Address: 0x8013E20C */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void bustup_cd_read();

void bustup_cd_read5855_step(void)
{
    bustup_cd_read(22613);
    sub_sub_step_up();
}
