/* @SymbolSize: 60 */
/* date2_sched_str_load - DATE2 overlay, Address: 0x801380C4, Size: 60 bytes (15 insns) */
#include "types.h"

void cd_read_start2(s32 arg0, void *arg1, s32 arg2);
void date2_ptr_set3(void);
void sub_sub_step_up(void);

void date2_sched_str_load(void)
{
    cd_read_start2(2, (void *)0x801E8000, 0xBD10);
    date2_ptr_set3();
    sub_sub_step_up();
}
