/* bustup_cd_read6b51_step -- GEKO overlay, Address: 0x80141950, Size: 40 bytes (10 insns) */
#include "types.h"

extern void bustup_cd_read(s32);
extern void sub_sub_step_up(void);

void bustup_cd_read6b51_step(void)
{
    bustup_cd_read(27473);
    sub_sub_step_up();
}
