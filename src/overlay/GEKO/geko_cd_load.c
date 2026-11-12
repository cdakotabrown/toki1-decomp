/* geko_cd_load -- GEKO overlay, Address: 0x8013D614, Size: 56 bytes (14 insns) */
#include "types.h"

extern void cd_read_start2(s32 a0, s32 a1, u32 a2);
extern void geko_bufptr_init(void);
extern void sub_sub_step_up(void);

void geko_cd_load(void)
{
    cd_read_start2(61, 0x801B0000, 0x87D9);
    geko_bufptr_init();
    sub_sub_step_up();
}
