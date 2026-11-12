/* rensyu_basyo10_res_set - RENSYU overlay, Address: 0x80133F20, 84 bytes. */
#include "types.h"

extern s32 rensyu_cal_cw_res[];
extern u32 rensyu_res_pal_ptr;

void rensyu_basyo10_res_set(void)
{
    rensyu_cal_cw_res[0] = 0x801A42D4;
    rensyu_cal_cw_res[1] = 0x801A42E0;
    rensyu_cal_cw_res[2] = 0x801A4330;
    (*(s16 *)&rensyu_cal_cw_res[3]) = *(s16 *)0x801A4338;
    rensyu_res_pal_ptr = 0x80197000;
}
