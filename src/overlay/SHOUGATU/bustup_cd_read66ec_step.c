/* Address: 0x8013AC34 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void bustup_cd_read();

void bustup_cd_read66ec_step(void)
{
    bustup_cd_read(26348);
    sub_sub_step_up();
}
