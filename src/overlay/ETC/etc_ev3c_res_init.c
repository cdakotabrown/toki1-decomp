/* Address: 0x80135540 */
/* Size: 208 bytes (52 instructions) */
/* @SymbolSize: 208 */
#include "types.h"
#include "tokimeki/etc_ev_res.h"

extern ETC_EV_RES etc_ev3c_res;
extern ETC_EV_RES_BLK etc_ev0_res;

void etc_ev3c_res_init(void)
{
    etc_ev3c_res.hdr = 0x801D22F4;
    etc_ev3c_res.cels = 0x801D22F8;
    etc_ev3c_res.anim_tbl = 0x801D2318;
    etc_ev3c_res.scale = *(s16 *)0x801D232C;
    etc_ev3c_res.pal = 0x801B0000;
    etc_ev0_res.blk = 0x801D2000;
    etc_ev3c_res.bg[0] = 0x801B2000;
    etc_ev3c_res.bg[1] = 0x801B6000;
    etc_ev3c_res.bg[2] = 0x801BA000;
    etc_ev3c_res.bg[3] = 0x801BE000;
    etc_ev3c_res.bg[4] = 0x801C2000;
    etc_ev3c_res.bg[5] = 0x801C6000;
    etc_ev0_res.blk_prev = 0x801CE000;
}
