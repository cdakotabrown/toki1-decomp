/* ending_common_load - ENDING overlay, Address: 0x80137148, Size: 56 bytes (14 instructions)  * Status: BYTE-PERFECT 🎉 */
#include "types.h"

extern void cd_read_start2();
extern void ending_common_init();
extern void sub_sub_step_up();

void ending_common_load(void)
{
    cd_read_start2(61, 0x80180000, 31515);
    ending_common_init();
    sub_sub_step_up();
}
