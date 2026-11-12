/* Address: 0x801398C0 */
/* Size: 204 bytes (51 insns) */
#include "types.h"
#include "tokimeki/date_obj_res_ex.h"
extern GEKO_OBJ_RES_EX geko_ptc_res;


void geko_event0_ptr_set1(void)
{
    geko_ptc_res.hdr = 0x801D6394;
    geko_ptc_res.img = 0x801D639C;
    geko_ptc_res.anim_tbl = 0x801D63BC;
    geko_ptc_res.aux = *(s16 *)0x801D63D0;
    geko_ptc_res.pal = 0x801B0000;
    geko_ptc_res.pal2 = 0x801D6000;
    geko_ptc_res.tile[0] = 0x801B2000;
    geko_ptc_res.tile[1] = 0x801B6000;
    geko_ptc_res.tile[2] = 0x801BA000;
    geko_ptc_res.tile[3] = 0x801BE000;
    geko_ptc_res.tile[4] = 0x801C2000;
    geko_ptc_res.tile[5] = 0x801C6000;
    geko_ptc_res.ext_tile = 0x801D2000;
}
