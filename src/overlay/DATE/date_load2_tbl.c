/* Function: date_load2_tbl */
/* Overlay: DATE */
#include "types.h"

extern void cd_read_start2(s32, s32, s32);
extern void date_ptrtbl2_init(void);
extern void sub_sub_step_up(void);

void date_load2_tbl(void)
{
    cd_read_start2(2, 0x80197000, 0xAF46);
    date_ptrtbl2_init();
    sub_sub_step_up();
}
