/* date2_birthday_serifu_load - DATE2 overlay, Address: 0x80137064, Size: 56 bytes (14 insns) */
#include "types.h"

void cd_read_start2(s32 arg0, void *arg1, s32 arg2);
void date2_script_table_init(void);
void sub_sub_step_up(void);

void date2_birthday_serifu_load(void)
{
    cd_read_start2(3, (void *)0x801B0000, 0xAF43);
    date2_script_table_init();
    sub_sub_step_up();
}
