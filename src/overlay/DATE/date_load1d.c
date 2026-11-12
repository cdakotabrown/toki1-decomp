/* Function: date_load1d */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 cd_read_start2();
extern s32 date_buf_tbl3_init();

void date_load1d(void)
{
    cd_read_start2(29, 0x801B0000, 0xBD4F);
    date_buf_tbl3_init();
    sub_sub_step_up();
}
