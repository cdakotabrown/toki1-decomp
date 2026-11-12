/* @SymbolSize: 176 */
/* Address: 0x801419F0 */
/* Size: 176 bytes (44 instructions) */
#include "types.h"

extern u32 shougatu_ev42_res, shougatu_ev42_res_cels, shougatu_ev42_res_anim_tbl;
extern s16 shougatu_ev42_res_scale;
extern u32 shougatu_ev42_res_pal, shougatu_ev42_res_bg0, shougatu_ev42_res_bg1, shougatu_ev42_res_bg2, shougatu_ev42_res_bg3,
           shougatu_ev42_res_bg4, shougatu_ev42_res_bg5;

void shougatu_buf_init3(void)
{
    shougatu_ev42_res = 0x801D22D0;
    shougatu_ev42_res_cels = 0x801D22D8;
    shougatu_ev42_res_anim_tbl = 0x801D233C;
    shougatu_ev42_res_scale = *(s16 *)0x801D2358;
    shougatu_ev42_res_pal = 0x801B0000;
    shougatu_ev42_res_bg0 = 0x801B2000;
    shougatu_ev42_res_bg1 = 0x801B6000;
    shougatu_ev42_res_bg2 = 0x801BA000;
    shougatu_ev42_res_bg3 = 0x801BE000;
    shougatu_ev42_res_bg4 = 0x801C2000;
    shougatu_ev42_res_bg5 = 0x801C6000;
}
