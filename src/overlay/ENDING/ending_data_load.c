/* ending_data_load - ENDING overlay, Address: 0x8013B914, Size: 56 bytes (14 instructions)  * Status: BYTE-PERFECT 🎉 */
#include "types.h"

extern void cd_read_start2();
extern void ending_desc_init();
extern void sub_sub_step_up();

void ending_data_load(void)
{
    cd_read_start2(69, 0x801A0000, 29306);
    ending_desc_init();
    sub_sub_step_up();
}
