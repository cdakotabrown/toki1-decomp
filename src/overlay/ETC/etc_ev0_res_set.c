/* Address: 0x80133710 */
/* Size: 204 bytes (51 instructions) */
/* @SymbolSize: 204 */
#include "types.h"
#include "tokimeki/etc_ev_res.h"
extern ETC_EV_RES_BLK etc_ev0_res;


void etc_ev0_res_set(void)
{
    etc_ev0_res.hdr = 0x801D6394;
    etc_ev0_res.cels = 0x801D639C;
    etc_ev0_res.anim_tbl = 0x801D63BC;
    etc_ev0_res.scale = *(s16 *)0x801D63D0;
    etc_ev0_res.pal = 0x801B0000;
    etc_ev0_res.blk = 0x801D6000;
    etc_ev0_res.bg[0] = 0x801B2000;
    etc_ev0_res.bg[1] = 0x801B6000;
    etc_ev0_res.bg[2] = 0x801BA000;
    etc_ev0_res.bg[3] = 0x801BE000;
    etc_ev0_res.bg[4] = 0x801C2000;
    etc_ev0_res.bg[5] = 0x801C6000;
    etc_ev0_res.blk_prev = 0x801D2000;
}
