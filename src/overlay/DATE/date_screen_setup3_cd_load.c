/* Function: date_screen_setup3_cd_load */
/* Overlay: DATE */
#include "types.h"

extern void sub_sub_step_up(void);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void date_ptrtbl13_init(void);

void date_screen_setup3_cd_load(void)
{
    cd_read_start2(61, 0x801B0000, 0x8ACD);
    date_ptrtbl13_init();
    sub_sub_step_up();
}
