/* geko_girl9_60_load3 -- GEKO overlay, Address: 0x8013E330, Size: 56 bytes (14 insns) */
#include "types.h"

extern void cd_read_start2(s32 a0, s32 a1, u32 a2);
extern void geko_loadbuf_init(void);
extern void sub_sub_step_up(void);

void geko_girl9_60_load3(void)
{
    cd_read_start2(69, 0x801B0000, 0x8A88);
    geko_loadbuf_init();
    sub_sub_step_up();
}
