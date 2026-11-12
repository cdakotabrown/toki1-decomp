/* geko_event0_cd_read8 -- GEKO overlay, Address: 0x8013A514, Size: 56 bytes (14 insns) */
#include "types.h"

extern void cd_read_start2(s32 a0, s32 a1, u32 a2);
extern void geko_event0_ptr_set8(void);
extern void sub_sub_step_up(void);

void geko_event0_cd_read8(void)
{
    cd_read_start2(69, 0x801B0000, 0x80BA);
    geko_event0_ptr_set8();
    sub_sub_step_up();
}
