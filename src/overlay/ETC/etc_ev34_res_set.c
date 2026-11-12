/* Address: 0x8013477C */
/* Size: 208 bytes (52 instructions) */
/* @SymbolSize: 208 */
#include "types.h"
#include "tokimeki/etc_ev_res.h"

extern ETC_EV_RES etc_ev34_res;
extern ETC_EV_RES_BLK etc_ev6_res;

void etc_ev34_res_set(void)
{
    etc_ev34_res.hdr = 0x801D2430;
    etc_ev34_res.cels = 0x801D2434;
    etc_ev34_res.anim_tbl = 0x801D2498;
    etc_ev34_res.scale = *(s16 *)0x801D24B4;
    etc_ev34_res.pal = 0x801B0000;
    etc_ev6_res.blk = 0x801D2000;
    etc_ev34_res.bg[0] = 0x801B2000;
    etc_ev34_res.bg[1] = 0x801B6000;
    etc_ev34_res.bg[2] = 0x801BA000;
    etc_ev34_res.bg[3] = 0x801BE000;
    etc_ev34_res.bg[4] = 0x801C2000;
    etc_ev34_res.bg[5] = 0x801C6000;
    etc_ev6_res.blk_prev = 0x801CE000;
}
