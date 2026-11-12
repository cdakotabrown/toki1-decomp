/* Function: date_girl4_mode5_cd_load */
/* Overlay: DATE */
#include "types.h"

extern void sub_sub_step_up(void);
extern void cd_read_start2(s32, s32, s32);
extern void date_girl4_mode5_bufptr_init(void);

void date_girl4_mode5_cd_load(void)
{
    cd_read_start2(61, 0x801B0000, 0x8D0A);
    date_girl4_mode5_bufptr_init();
    sub_sub_step_up();
}
