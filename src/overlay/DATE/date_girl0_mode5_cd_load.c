/* Function: date_girl0_mode5_cd_load */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 cd_read_start2();
extern void date_girl0_mode5_res_ptr_set();

void date_girl0_mode5_cd_load(void)
{
    cd_read_start2(69, 0x801B0000, 32747);
    date_girl0_mode5_res_ptr_set();
    sub_sub_step_up();
}
