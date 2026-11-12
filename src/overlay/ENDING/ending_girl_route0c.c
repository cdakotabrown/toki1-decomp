/* @SymbolStart: 0x80139A78 */
/* @SymbolSize: 72 */
/* ending_girl_route0c - ENDING overlay, Address: 0x80139A78, Size: 72 bytes (18 instructions) */
#include "types.h"

extern u8 now_girl;
extern void goto_sub_step();
extern void goto_sub_sub_step();

void ending_girl_route0c(void)
{
    u8 sel = now_girl;

    if (sel == 13) {
        goto_sub_step(7);
        goto_sub_sub_step(15);
        return;
    }
    goto_sub_step(8);
}
