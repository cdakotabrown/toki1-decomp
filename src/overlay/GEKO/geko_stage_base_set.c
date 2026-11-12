/* Size: 112 bytes */
/* @SymbolSize: 112 */
#include "types.h"

extern u32 geko_workbuf_tbl[7];

void geko_stage_base_set(void)
{
    geko_workbuf_tbl[0] = 0x801B0000;
    geko_workbuf_tbl[1] = 0x801B2000;
    geko_workbuf_tbl[2] = 0x801B6000;
    geko_workbuf_tbl[3] = 0x801BA000;
    geko_workbuf_tbl[4] = 0x801BE000;
    geko_workbuf_tbl[5] = 0x801C2000;
    geko_workbuf_tbl[6] = 0x801C6000;
}
