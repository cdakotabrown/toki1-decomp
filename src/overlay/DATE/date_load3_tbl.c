/* Function: date_load3_tbl */
/* Overlay: DATE */
#include "types.h"

extern void sub_sub_step_up(void);
extern void cd_read_start2(s32, s32, s32);
extern void date_ptr_tbl_init(void);

void date_load3_tbl(void)
{
    cd_read_start2(3, 0x801A4000, 0xAFA9);
    date_ptr_tbl_init();
    sub_sub_step_up();
}
