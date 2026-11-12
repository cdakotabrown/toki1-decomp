/* Function: date_palette_res_cd_load */
/* Overlay: DATE */
#include "types.h"

extern void sub_sub_step_up(void);
extern void cd_read_start2(s32, s32, s32);
extern void date_bufaddr_init1(void);

void date_palette_res_cd_load(void)
{
    cd_read_start2(69, 0x801B0000, 0x8664);
    date_bufaddr_init1();
    sub_sub_step_up();
}
