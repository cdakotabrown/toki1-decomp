/* ending_load9 - ENDING overlay, Address: 0x80139D44, Size: 48 bytes (12 instructions)  * Status: BYTE-PERFECT 🎉 */
#include "types.h"

extern void cd_read_start2();
extern void sub_sub_step_up();

void ending_load9(void)
{
    cd_read_start2(9, 0x80180000, 0xAE18);
    sub_sub_step_up();
}
