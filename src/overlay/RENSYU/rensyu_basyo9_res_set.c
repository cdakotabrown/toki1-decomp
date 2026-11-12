/* rensyu_basyo9_res_set - RENSYU overlay, Address: 0x80133FC8, 84 bytes. */
#include "types.h"

extern s32 rensyu_cal_cw_res[];
extern u32 rensyu_res_pal_ptr;

void rensyu_basyo9_res_set(void)
{
    rensyu_cal_cw_res[0] = 0x801A42E4;
    rensyu_cal_cw_res[1] = 0x801A42F0;
    rensyu_cal_cw_res[2] = 0x801A4368;
    (*(s16 *)&rensyu_cal_cw_res[3]) = *(s16 *)0x801A4370;
    rensyu_res_pal_ptr = 0x80197000;
}
