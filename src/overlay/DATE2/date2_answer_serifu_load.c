/* date2_answer_serifu_load - DATE2 overlay, Address: 0x8013768C, Size: 56 bytes (14 insns) */
#include "types.h"

void cd_read_start2(s32 arg0, void *arg1, s32 arg2);
void date2_ptr_set2(void);
void sub_sub_step_up(void);

void date2_answer_serifu_load(void)
{
    cd_read_start2(22, (void *)0x801B0000, 0xAF0D);
    date2_ptr_set2();
    sub_sub_step_up();
}
