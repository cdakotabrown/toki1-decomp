/* geko_ev_ta02_cd_load -- GEKO overlay, Address: 0x80140E84, Size: 48 bytes (12 insns) */
#include "types.h"

extern void cd_read_start2(s32 a0, s32 a1, u32 a2);
extern void sub_sub_step_up(void);

void geko_ev_ta02_cd_load(void)
{
    cd_read_start2(53, 0x801B0000, 0x8F43);
    sub_sub_step_up();
}
