/* @SymbolStart: 0x801338B8 */
/* @SymbolSize: 52 */
/* date2_mode_next -- DATE2 overlay, Address: 0x801338B8, Size: 52 bytes (13 insns) */
#include "types.h"

extern u8 date2_mode_no;

void goto_sub_sub_step(s32 arg0);

void date2_mode_next(void)
{
    date2_mode_no = (u8)(date2_mode_no + 1);
    goto_sub_sub_step(27);
}
