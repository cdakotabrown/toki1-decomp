/* @SymbolStart: 0x801545E4 */
/* @SymbolSize: 88 */
/* date_girl1_mode2_init -- DATE overlay, Address: 0x80154614, Size: 88 bytes (22 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern u8 date_ride_kind;
extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_girl1_mode2_init(void)
{
    date_ride_kind = 3;
    date_ss_serifuev = date_serifuev_tbl[38];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[38];
    date_sss_soundev = date_soundev_tbl[38];
    sub_sub_step_up();
}
