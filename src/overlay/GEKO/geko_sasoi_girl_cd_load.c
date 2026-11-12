/* geko_sasoi_girl_cd_load -- GEKO overlay, Address: 0x80142B90, Size: 60 bytes (15 insns) */
#include "types.h"

extern void cd_read_start2(s32 a0, s32 a1, u32 a2);
extern void geko_girl_ptr_init(void);
extern void sub_sub_step_up(void);

void geko_sasoi_girl_cd_load(void)
{
    cd_read_start2(16, 0x80197000, 0xAF23);
    geko_girl_ptr_init();
    sub_sub_step_up();
}
