/* geko_serifu_cd_load -- GEKO overlay, Address: 0x80132454, Size: 60 bytes (15 insns) */
#include "types.h"

extern void cd_read_start2(s32 a0, s32 a1, u32 a2);
extern void geko_ptrs_set(void);
extern void sub_sub_step_up(void);

void geko_serifu_cd_load(void)
{
    cd_read_start2(36, 0x80197000, 0xAEC8);
    geko_ptrs_set();
    sub_sub_step_up();
}
