/* Function: set_XAsector */
/* Address: 0x80046560 */
/* Size: 44 bytes (11 instructions) */
/* Region: Game (0x80046560 < 0x80086000) */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 xa_sector_ofs_tbl[];
s32 XAsector[4] = { 0, 0, 0, 0 };

void set_XAsector(s32 a0, s32 a1, s32 a2) {
    s32 idx;
    idx = a2 & 0xF;
    XAsector[0] = idx;
    XAsector[2] = a0;
    XAsector[3] = a1;
    XAsector[1] = xa_sector_ofs_tbl[idx];
}
