/* Address: 0x8015A554 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void bustup_cd_read(s32);

void bustup_cd_read6bab_step(void)
{
    bustup_cd_read(27563);
    sub_sub_step_up();
}
