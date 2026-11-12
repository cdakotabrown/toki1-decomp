/* Function: date_load3d */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 cd_read_start2();
extern s32 date_ptrtbl7_init();

void date_load3d(void)
{
    cd_read_start2(61, 0x801B0000, 0x8381);
    date_ptrtbl7_init();
    sub_sub_step_up();
}
