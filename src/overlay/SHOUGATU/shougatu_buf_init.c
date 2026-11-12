/* @SymbolSize: 208 */
/* Address: 0x80141850 */
/* Size: 208 bytes (52 instructions) */
#include "types.h"
#include "tokimeki/date_obj_res_ex.h"

extern u32 shougatu_ev06_res, shougatu_ev06_res_img, shougatu_ev06_res_anim_tbl;
extern u32 shougatu_ev06_res_pal, shougatu_ev06_res_pal2, shougatu_ev06_res_tile0, shougatu_ev06_res_tile1, shougatu_ev06_res_tile2,
           shougatu_ev06_res_tile3, shougatu_ev06_res_tile4, shougatu_ev06_res_tile5, shougatu_ev06_res_ext_tile;

void shougatu_buf_init(void)
{
    shougatu_ev06_res = 0x801DA680;
    shougatu_ev06_res_img = 0x801DA68C;
    shougatu_ev06_res_anim_tbl = 0x801DA72C;
    (*(DATE_OBJ_RES_EX *)&shougatu_ev06_res).aux = *(s16 *)0x801DA754;
    shougatu_ev06_res_pal = 0x801B0000;
    shougatu_ev06_res_pal2 = 0x801DA000;
    shougatu_ev06_res_tile0 = 0x801B2000;
    shougatu_ev06_res_tile1 = 0x801B6000;
    shougatu_ev06_res_tile2 = 0x801BA000;
    shougatu_ev06_res_tile3 = 0x801BE000;
    shougatu_ev06_res_tile4 = 0x801C2000;
    shougatu_ev06_res_tile5 = 0x801C6000;
    shougatu_ev06_res_ext_tile = 0x801D6000;
}
