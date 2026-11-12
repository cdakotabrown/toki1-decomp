/* Function: date_cw_fade_in */
/* Overlay: DATE */
/* @PostProc: scheduler_normalizer */
/* @PostProc: rename_register_chain --chain v1:v0 */
#include "types.h"

extern u8 cw98_disp;
extern u8 cal_cw_bright;
extern u8 cw98_bright;
extern u8 cw99_disp;
extern u8 cw99_bright;

extern s32 date_fade_in(void);

s32 date_cw_fade_in(void)
{
    u8 t6, t8;
    s32 v0;

    date_fade_in();
    t6 = cw98_disp;
    v0 = cal_cw_bright;
    cw98_disp = t6 | 0x80;
    t8 = cw99_disp;
    cw99_disp = t8 | 0x80;
    cw98_bright = (u8)v0;
    cw99_bright = (u8)v0;
    return v0;
}
