/* Address: 0x80057780 */
/* Size: 20 bytes (5 instructions) */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 bg_change_tbl0[];

void indexed_byte_lsb_set(s32 a0, s32 a1)
{
    bg_change_tbl0[a0] = (u8)(a1 & 1);
}
