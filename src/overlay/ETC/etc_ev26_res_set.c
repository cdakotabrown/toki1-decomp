/* Address: 0x801350B0 */
/* Size: 112 bytes (28 instructions) */
/* @SymbolSize: 112 */
#include "types.h"
#include "tokimeki/etc_ev_res.h"
extern ETC_EV_RES_BG etc_ev26_res;


void etc_ev26_res_set(void)
{
    etc_ev26_res.pal = 0x801B0000;
    etc_ev26_res.bg[0] = 0x801B2000;
    etc_ev26_res.bg[1] = 0x801B6000;
    etc_ev26_res.bg[2] = 0x801BA000;
    etc_ev26_res.bg[3] = 0x801BE000;
    etc_ev26_res.bg[4] = 0x801C2000;
    etc_ev26_res.bg[5] = 0x801C6000;
}
