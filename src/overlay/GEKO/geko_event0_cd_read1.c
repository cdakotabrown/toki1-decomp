/* geko_event0_cd_read1 -- GEKO overlay, Address: 0x80139BE8, Size: 56 bytes (14 insns) */
#include "types.h"

extern void cd_read_start2(s32 a0, s32 a1, u32 a2);
extern void geko_event0_ptr_set1(void);
extern void sub_sub_step_up(void);

void geko_event0_cd_read1(void)
{
    cd_read_start2(77, 0x801B0000, 32532);
    geko_event0_ptr_set1();
    sub_sub_step_up();
}
