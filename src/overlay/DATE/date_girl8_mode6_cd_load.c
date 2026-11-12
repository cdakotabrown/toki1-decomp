/* Function: date_girl8_mode6_cd_load */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 cd_read_start2();
extern s32 date_girl8_mode6_res_ptr_set();

void date_girl8_mode6_cd_load(void)
{
    cd_read_start2(69, 0x801B0000, 32463);
    date_girl8_mode6_res_ptr_set();
    sub_sub_step_up();
}
