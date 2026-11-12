/* Function: date_girl8_mode3_cd_load */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 cd_read_start2();
extern s32 date_bufptr7e10_init();

void date_girl8_mode3_cd_load(void)
{
    cd_read_start2(69, 0x801B0000, 32272);
    date_bufptr7e10_init();
    sub_sub_step_up();
}
