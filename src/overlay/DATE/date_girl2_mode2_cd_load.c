/* Function: date_girl2_mode2_cd_load */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 cd_read_start2();
extern s32 date_bufptr8144_init();

void date_girl2_mode2_cd_load(void)
{
    cd_read_start2(53, 0x801B0000, 0x8144);
    date_bufptr8144_init();
    sub_sub_step_up();
}
