/* Function: date_girl3_mode5_res_load */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 cd_read_start2();
extern void date_ptrtbl8_init();

void date_girl3_mode5_res_load(void)
{
    cd_read_start2(61, 0x801B0000, 0x83BE);
    date_ptrtbl8_init();
    sub_sub_step_up();
}
