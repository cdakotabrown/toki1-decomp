/* @SymbolStart: 0x801376F0 */
/* @SymbolSize: 108 */
/* Address: 0x801376F0 (head) / 0x80137734 (recorded symbol) */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 serifu_no;
extern s8 date2_serifu_no_save;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern u32 date2_res_tbl2[][13];

void date2_serifu_init(void)
{
    date2_serifu_no_save = serifu_no;
    now_serifu = 0;
    now_serifu_in = 0;
    ss_serifu = (*(s32 *)&date2_res_tbl2[0][1]);
    ss_serifu_kind = (*(s32 *)&date2_res_tbl2[1][1]);
    sss_sound = (*(s32 *)&date2_res_tbl2[2][1]);
    sub_sub_step_up();
}
