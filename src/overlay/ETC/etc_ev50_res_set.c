/* Address: 0x80133F7C */
/* Size: 112 bytes (28 instructions) */
/* @SymbolSize: 112 */
#include "types.h"

extern void * etc_ev50_res;
extern void * etc_ev50_res_bg0;
extern void * etc_ev50_res_bg1;
extern void * etc_ev50_res_bg2;
extern void * etc_ev50_res_bg3;
extern void * etc_ev50_res_bg4;
extern void * etc_ev50_res_bg5;

void etc_ev50_res_set(void)
{
    etc_ev50_res = (void *)0x801B0000;
    etc_ev50_res_bg0 = (void *)0x801B2000;
    etc_ev50_res_bg1 = (void *)0x801B6000;
    etc_ev50_res_bg2 = (void *)0x801BA000;
    etc_ev50_res_bg3 = (void *)0x801BE000;
    etc_ev50_res_bg4 = (void *)0x801C2000;
    etc_ev50_res_bg5 = (void *)0x801C6000;
}
