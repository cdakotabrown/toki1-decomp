/* Function: date_girl5_mode6_res_load */
/* Overlay: DATE */
#include "types.h"

extern void sub_sub_step_up(void);
extern void cd_read_start2(s32, s32, s32);
extern void date_girl5_mode6_bufptr_set(void);

void date_girl5_mode6_res_load(void)
{
    cd_read_start2(53, 0x801B0000, 0x88CD);
    date_girl5_mode6_bufptr_set();
    sub_sub_step_up();
}
