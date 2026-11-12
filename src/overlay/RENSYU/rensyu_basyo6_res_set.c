/* rensyu_basyo6_res_set - RENSYU overlay, Address: 0x80134070, 84 bytes. */
#include "types.h"

extern s32 rensyu_cal_cw_res[];
extern u32 rensyu_res_pal_ptr;

void rensyu_basyo6_res_set(void)
{
    rensyu_cal_cw_res[0] = 0x801A403C;
    rensyu_cal_cw_res[1] = 0x801A4048;
    rensyu_cal_cw_res[2] = 0x801A4098;
    (*(s16 *)&rensyu_cal_cw_res[3]) = *(s16 *)0x801A40B0;
    rensyu_res_pal_ptr = 0x80197000;
}
