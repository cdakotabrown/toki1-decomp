/* rensyu_basyo8_res_set - RENSYU overlay, Address: 0x8013401C, 84 bytes. */
#include "types.h"

extern s32 rensyu_cal_cw_res[];
extern u32 rensyu_res_pal_ptr;

void rensyu_basyo8_res_set(void)
{
    rensyu_cal_cw_res[0] = 0x801A432C;
    rensyu_cal_cw_res[1] = 0x801A4338;
    rensyu_cal_cw_res[2] = 0x801A4394;
    (*(s16 *)&rensyu_cal_cw_res[3]) = *(s16 *)0x801A43A4;
    rensyu_res_pal_ptr = 0x80197000;
}
