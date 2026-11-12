/* Function: date_girl3_mode3_res_load */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 cd_read_start2();
extern s32 date_ptrtbl6_init();

void date_girl3_mode3_res_load(void)
{
    cd_read_start2(69, 0x801B0000, 0x833C);
    date_ptrtbl6_init();
    sub_sub_step_up();
}
