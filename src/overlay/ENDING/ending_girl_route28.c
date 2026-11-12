/* @SymbolStart: 0x80133A7C */
/* @SymbolSize: 100 */
/* ending_girl_route28 - ENDING overlay, Address: 0x80133A7C, Size: 100 bytes (25 instructions) */
#include "types.h"

extern u8 now_girl;
extern u8 sys_flags;
extern void goto_sub_step();
extern void sub_sub_step_up();

void ending_girl_route28(void)
{
    u8 step = now_girl;
    u8 t7;
    u8 t9;

    if ((u32)step < 13) {
        goto skip;
    }
    goto_sub_step(7);
    t7 = sys_flags;
    sys_flags = (u8)(t7 | 0x08);
    return;
skip:
    sub_sub_step_up();
    t9 = sys_flags;
    sys_flags = (u8)(t9 | 0x04);
}
