/* @SymbolStart: 0x80139DC8 */
/* @SymbolSize: 96 */
/* ending_bs_load - ENDING overlay, Address: 0x80139DC8, Size: 96 bytes (24 instructions) */
#include "types.h"

extern u8 now_girl;
extern void cd_read_start();
extern void sub_sub_step_up();

void ending_bs_load(void)
{
    u8 sel = now_girl;

    if (sel != 13) {
        cd_read_start(9, 0x80162000, 18812);
    } else {
        cd_read_start(10, 0x80162000, 18821);
    }
    sub_sub_step_up();
}
