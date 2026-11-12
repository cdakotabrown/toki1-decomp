/* Function: date_cd_load */
/* Overlay: DATE */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void date_buf_tbl_init(void);

void date_cd_load(void)
{
    cd_read_start2(134, 0x801B0000, 0xBC43);
    device_cmd_send(512);
    date_buf_tbl_init();
    sub_sub_step_up();
}
