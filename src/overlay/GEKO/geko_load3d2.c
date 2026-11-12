/* geko_load3d2 -- GEKO overlay, Address: 0x8013D190, Size: 56 bytes (14 insns) */
#include "types.h"

extern void cd_read_start2(s32 a0, s32 a1, u32 a2);
extern void geko_buf_init2(void);
extern void sub_sub_step_up(void);

void geko_load3d2(void)
{
    cd_read_start2(61, 0x801B0000, 0x8627);
    geko_buf_init2();
    sub_sub_step_up();
}
