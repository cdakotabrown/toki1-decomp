/* Function: date_girl10_mode6_res_load */
/* Overlay: DATE */
#include "types.h"

extern void sub_sub_step_up(void);
extern void cd_read_start2(s32, s32, s32);
extern void date_girl10_mode6_bufptr_init(void);

void date_girl10_mode6_res_load(void)
{
    cd_read_start2(69, 0x801B0000, 0x8EB9);
    date_girl10_mode6_bufptr_init();
    sub_sub_step_up();
}
