/* Address: 0x80133F2C */
/* Size: 80 bytes (20 instructions) */
/* @SymbolSize: 80 */
#include "types.h"
#include "tokimeki/etc_ev_res.h"
extern ETC_EV_RES_OBJ etc_ev13_res;


void etc_ev13_res_set(void)
{
    etc_ev13_res.hdr = 0x801C25B8;
    etc_ev13_res.cels = 0x801C25C8;
    etc_ev13_res.anim_tbl = 0x801C2674;
    etc_ev13_res.scale = *(s16 *)0x801C26A0;
    etc_ev13_res.pal = 0x801B0000;
}
