/* Function: date_girl4_mode3_cd_load */
/* Overlay: DATE */
#include "types.h"

extern void sub_sub_step_up(void);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void date_girl4_mode3_bufptr_init(void);

void date_girl4_mode3_cd_load(void)
{
    cd_read_start2(69, 0x801B0000, 0x8C88);
    date_girl4_mode3_bufptr_init();
    sub_sub_step_up();
}
