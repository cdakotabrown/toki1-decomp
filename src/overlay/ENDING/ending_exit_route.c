/* @SymbolStart: 0x801374DC */
/* @SymbolSize: 60 */
/* ending_exit_route - ENDING overlay, Address: 0x801374DC, Size: 60 bytes (15 instructions) */
#include "types.h"

extern u8 option_on;
extern void goto_step();
extern void sub_step_up();

void ending_exit_route(void)
{
    u8 flag = option_on;

    if (flag != 0) {
        goto_step(194);
        return;
    }
    sub_step_up();
}
