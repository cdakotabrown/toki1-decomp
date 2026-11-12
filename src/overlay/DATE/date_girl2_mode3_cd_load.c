/* Function: date_girl2_mode3_cd_load */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 cd_read_start2();
extern void date_girl2_mode3_res_ptr_init();

void date_girl2_mode3_cd_load(void)
{
    cd_read_start2(61, 0x801B0000, 0x8179);
    date_girl2_mode3_res_ptr_init();
    sub_sub_step_up();
}
