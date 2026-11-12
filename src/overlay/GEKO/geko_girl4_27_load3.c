/* geko_girl4_27_load3 -- GEKO overlay, Address: 0x8013EF54, Size: 56 bytes (14 insns) */
#include "types.h"

extern void cd_read_start2(s32 a0, s32 a1, u32 a2);
extern void geko_loadbuf2_init(void);
extern void sub_sub_step_up(void);

void geko_girl4_27_load3(void)
{
    cd_read_start2(61, 0x801B0000, 0x8C0E);
    geko_loadbuf2_init();
    sub_sub_step_up();
}
