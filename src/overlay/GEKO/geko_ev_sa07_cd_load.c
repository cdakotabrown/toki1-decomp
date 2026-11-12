/* geko_ev_sa07_cd_load -- GEKO overlay, Address: 0x8013FA3C, Size: 56 bytes (14 insns) */
#include "types.h"

extern void cd_read_start2(s32 a0, s32 a1, u32 a2);
extern void geko_scene73_ptr_set(void);
extern void sub_sub_step_up(void);

void geko_ev_sa07_cd_load(void)
{
    cd_read_start2(69, 0x801B0000, 0x8EFE);
    geko_scene73_ptr_set();
    sub_sub_step_up();
}
