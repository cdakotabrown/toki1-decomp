/* @SymbolSize: 116 */
/* Address: 0x801339EC, Size: 116 bytes (29 insns) */
#include "types.h"

extern s32 date2_intro_buf_tbl[];

void date2_buf_table_init(void)
{
    date2_intro_buf_tbl[0] = 0x801C6000;
    date2_intro_buf_tbl[1] = 0x801AE000;
    date2_intro_buf_tbl[2] = 0x801B2000;
    date2_intro_buf_tbl[3] = 0x801B6000;
    date2_intro_buf_tbl[4] = 0x801BA000;
    date2_intro_buf_tbl[5] = 0x801BE000;
    date2_intro_buf_tbl[6] = 0x801C2000;
}
