/* date2_girl_serifu_load - DATE2 overlay, Address: 0x80132BB0, Size: 60 bytes (15 insns) */
#include "types.h"

void cd_read_start2(s32 arg0, void *arg1, s32 arg2);
void date2_ptr_tbl_init(void);
void sub_sub_step_up(void);

void date2_girl_serifu_load(void)
{
    cd_read_start2(17, (void *)0x80197000, 0xAEEC);
    date2_ptr_tbl_init();
    sub_sub_step_up();
}
