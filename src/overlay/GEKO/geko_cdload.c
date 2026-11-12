/* geko_cdload -- GEKO overlay, Address: 0x8013B1E0, Size: 56 bytes (14 insns) */
#include "types.h"

extern void cd_read_start2(s32 a0, s32 a1, u32 a2);
extern void geko_ptr_table_init(void);
extern void sub_sub_step_up(void);

void geko_cdload(void)
{
    cd_read_start2(69, 0x801B0000, 0x80FF);
    geko_ptr_table_init();
    sub_sub_step_up();
}
