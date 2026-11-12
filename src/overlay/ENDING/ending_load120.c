/* ending_load120 - ENDING overlay, Address: 0x801349B8, Size: 56 bytes (14 instructions) */
#include "types.h"

extern void cd_read_start2();
extern void ending_class_ptr_init();
extern void sub_sub_step_up();

void ending_load120(void)
{
    cd_read_start2(120, 0x80180000, 31576);
    ending_class_ptr_init();
    sub_sub_step_up();
}
