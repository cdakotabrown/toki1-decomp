/* @SymbolStart: 0x8014F600 */
/* @SymbolSize: 308 */
/* Function: date_girl0_mode5_init */
/* Overlay: DATE */
/* Address: 0x8014F66C */
/* Size: 192 bytes (48 instructions) */
#include "types.h"

extern void param_clamp(void);
extern void sub_sub_step_up(void);

extern s16 girl0_tokimeki[];
extern s32 now_date_spot;
extern s16 kao[];
extern s16 kao2[];
extern s16 imp[];
extern s16 ans[];
extern u8 date_ride_kind;
extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_girl0_mode5_init(void)
{
    date_ride_kind = 2;
    date_ss_serifuev = date_serifuev_tbl[10];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[10];
    date_sss_soundev = date_soundev_tbl[10];
    girl0_tokimeki[0]++;
    girl0_tokimeki[2]++;
    girl0_tokimeki[4] -= 20;
    param_clamp();
    now_date_spot = 23;
    kao[0] = 2;
    kao[2] = 2;
    kao[1] = 2;
    kao2[0] = 2;
    kao2[2] = 2;
    kao2[1] = 2;
    imp[0] = 2;
    imp[2] = 2;
    imp[1] = 2;
    ans[0] = 1;
    ans[2] = 1;
    ans[1] = 1;
    sub_sub_step_up();
}
