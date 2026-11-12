/* ending_load17 - ENDING overlay, Address: 0x80133438, Size: 56 bytes (14 instructions) */
#include "types.h"

extern void cd_read_start2();
extern void ending_ptr_init();
extern void sub_sub_step_up();

void ending_load17(void)
{
    cd_read_start2(17, 0x801F0000, 0xAE07);
    ending_ptr_init();
    sub_sub_step_up();
}
