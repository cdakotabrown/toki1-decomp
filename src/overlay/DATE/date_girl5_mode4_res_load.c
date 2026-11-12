/* Function: date_girl5_mode4_res_load */
/* Overlay: DATE */
#include "types.h"

extern void sub_sub_step_up(void);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void date_girl5_mode4_bufptr_set(void);

void date_girl5_mode4_res_load(void)
{
    cd_read_start2(61, 0x801B0000, 0x8853);
    date_girl5_mode4_bufptr_set();
    sub_sub_step_up();
}
