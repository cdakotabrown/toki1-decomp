/* Function: date_girl1_init4_cd_load */
/* Overlay: DATE */
#include "types.h"

extern void sub_sub_step_up(void);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void date_bufaddr_init3(void);

void date_girl1_init4_cd_load(void)
{
    cd_read_start2(61, 0x801B0000, 0x86EE);
    date_bufaddr_init3();
    sub_sub_step_up();
}
