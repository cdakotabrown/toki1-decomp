/* Function: date_girl6_mode4_res_load */
/* Overlay: DATE */
#include "types.h"

extern void sub_sub_step_up(void);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void date_girl6_mode4_bufptr_init(void);

void date_girl6_mode4_res_load(void)
{
    cd_read_start2(69, 0x801B0000, 0x8A06);
    date_girl6_mode4_bufptr_init();
    sub_sub_step_up();
}
