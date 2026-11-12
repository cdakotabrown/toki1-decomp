/* @SymbolStart: 0x80153914 */
/* @SymbolSize: 124 */
/* Function: date_girl7_mode6_init */
/* Overlay: DATE */
#include "types.h"

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s16 girl7_tokimeki[];
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];
extern s32 param_clamp(void);
extern s32 sub_sub_step_up(void);

void date_girl7_mode6_init(void)
{
    date_ss_serifuev = date_serifuev_tbl[35];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[35];
    date_sss_soundev = date_soundev_tbl[35];
    girl7_tokimeki[0] = girl7_tokimeki[0] + 3;
    girl7_tokimeki[2] = girl7_tokimeki[2] + 2;
    param_clamp();
    sub_sub_step_up();
}
