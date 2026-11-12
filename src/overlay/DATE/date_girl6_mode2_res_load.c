/* Function: date_girl6_mode2_res_load */
/* Overlay: DATE */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void date_girl6_mode2_bufptr_init(void);

void date_girl6_mode2_res_load(void)
{
    cd_read_start2(61, 0x801B0000, 0x8984);
    date_girl6_mode2_bufptr_init();
    sub_sub_step_up();
}
