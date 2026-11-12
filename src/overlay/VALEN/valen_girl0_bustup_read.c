/* valen_girl0_bustup_read - VALEN overlay, Address: 0x801335B8, Size: 48 bytes (12 insns) */
#include "types.h"

extern void bustup_cd_read(s32);
extern void date_name_set(s32);
extern void sub_sub_step_up(void);

void valen_girl0_bustup_read(void)
{
    date_name_set(0);
    bustup_cd_read(21578);
    sub_sub_step_up();
}
