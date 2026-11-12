/* Function: date_girl9_init5_cd_load */
/* Overlay: DATE */
#include "types.h"

extern void sub_sub_step_up(void);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void date_ptrtbl15_init(void);

void date_girl9_init5_cd_load(void)
{
    cd_read_start2(61, 0x801B0000, 0x8B4F);
    date_ptrtbl15_init();
    sub_sub_step_up();
}
