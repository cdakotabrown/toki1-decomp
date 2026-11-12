/* Address: 0x80140AA0 */
/* Size: 112 bytes (28 insns) */
#include "types.h"

extern s32 geko_bg_buf_tbl[];

void geko_bg_buf_tbl_init(void)
{
    geko_bg_buf_tbl[0] = 0x801B0000;
    geko_bg_buf_tbl[1] = 0x801B2000;
    geko_bg_buf_tbl[2] = 0x801B6000;
    geko_bg_buf_tbl[3] = 0x801BA000;
    geko_bg_buf_tbl[4] = 0x801BE000;
    geko_bg_buf_tbl[5] = 0x801C2000;
    geko_bg_buf_tbl[6] = 0x801C6000;
}
