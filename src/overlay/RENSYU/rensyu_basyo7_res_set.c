/* rensyu_basyo7_res_set - RENSYU overlay, Address: 0x80133F74, 84 bytes. */
#include "types.h"

extern s32 rensyu_cal_cw_res[];
extern u32 rensyu_res_pal_ptr;

void rensyu_basyo7_res_set(void)
{
    rensyu_cal_cw_res[0] = 0x801A3E3C;
    rensyu_cal_cw_res[1] = 0x801A3E48;
    rensyu_cal_cw_res[2] = 0x801A3E90;
    (*(s16 *)&rensyu_cal_cw_res[3]) = *(s16 *)0x801A3E98;
    rensyu_res_pal_ptr = 0x80197000;
}
