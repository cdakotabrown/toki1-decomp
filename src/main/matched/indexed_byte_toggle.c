/* Address: 0x80057760, Size: 32 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 bg_change_tbl0[];

void indexed_byte_toggle(s32 a)
{
    bg_change_tbl0[a] = (u8)(1 - bg_change_tbl0[a]);
}
