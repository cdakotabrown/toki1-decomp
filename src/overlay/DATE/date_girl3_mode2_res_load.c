/* Function: date_girl3_mode2_res_load */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 cd_read_start2();
extern s32 date_ptrtbl5_init();

void date_girl3_mode2_res_load(void)
{
    cd_read_start2(101, 0x801B0000, 0x82D7);
    date_ptrtbl5_init();
    sub_sub_step_up();
}
