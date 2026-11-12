/* @SymbolStart: 0x8013C2F0 */
/* @SymbolSize: 88 */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 geko_ss_serifuev;
extern s32 geko_ss_serifuev_kind;
extern s32 geko_sss_soundev;
extern s8 now_event_no;
extern s32 geko_serifuev_tbl[];
extern s32 geko_serifuev_kind_tbl[];
extern s32 geko_soundev_tbl[];

void geko_serifuev_tbl_set(void)
{
    geko_ss_serifuev = geko_serifuev_tbl[23];
    geko_ss_serifuev_kind = geko_serifuev_kind_tbl[23];
    geko_sss_soundev = geko_soundev_tbl[23];
    now_event_no = 81;
    sub_sub_step_up();
}
