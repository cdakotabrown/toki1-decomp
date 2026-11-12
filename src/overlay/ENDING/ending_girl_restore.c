/* @SymbolStart: 0x80133A50 */
/* @SymbolSize: 44 */
/* @PostProc: swap_registers t7:at */
/* ending_girl_restore - ENDING overlay, Address: 0x80133A50, Size: 52 bytes (13 instructions) */
#include "types.h"

extern u8 ending_save_girl;
extern void sub_sub_step_up();

void ending_girl_restore(void)
{
    u8 value = ending_save_girl;

    *(u8 *)0x800EF6BFU = value;
    sub_sub_step_up();
}
