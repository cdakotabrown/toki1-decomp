/* @SymbolSize: 208 */
/* Address: 0x80141920 */
/* Size: 208 bytes (52 instructions) */
#include "types.h"

extern u32 shougatu_ev06_res_pal2, shougatu_ev06_res_ext_tile, shougatu_ev34_res, shougatu_ev34_res_cels, shougatu_ev34_res_anim_tbl;
extern s16 shougatu_ev34_res_scale;
extern u32 shougatu_ev34_res_pal, shougatu_ev34_res_bg0, shougatu_ev34_res_bg1, shougatu_ev34_res_bg2, shougatu_ev34_res_bg3,
           shougatu_ev34_res_bg4, shougatu_ev34_res_bg5;

void shougatu_buf_init2(void)
{
    shougatu_ev34_res = 0x801D2430;
    shougatu_ev34_res_cels = 0x801D2434;
    shougatu_ev34_res_anim_tbl = 0x801D2498;
    shougatu_ev34_res_scale = *(s16 *)0x801D24B4;
    shougatu_ev34_res_pal = 0x801B0000;
    shougatu_ev06_res_pal2 = 0x801D2000;
    shougatu_ev34_res_bg0 = 0x801B2000;
    shougatu_ev34_res_bg1 = 0x801B6000;
    shougatu_ev34_res_bg2 = 0x801BA000;
    shougatu_ev34_res_bg3 = 0x801BE000;
    shougatu_ev34_res_bg4 = 0x801C2000;
    shougatu_ev34_res_bg5 = 0x801C6000;
    shougatu_ev06_res_ext_tile = 0x801CE000;
}
