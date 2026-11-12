/* @SymbolStart: 0x80133980 */
/* @SymbolSize: 96 */
#include "types.h"

extern void sub_step_up(void);
extern s32 shougatu_meet_serifu_tbl;
extern s32 shougatu_meet_serifu_kind_tbl;
extern s32 shougatu_meet_sound_tbl;
extern u8 shougatu_alone_flag;
extern s16 shougatu_meet_now_serifu;
extern s32 shougatu_meet_ss_serifu;
extern s32 shougatu_meet_ss_serifu_kind;
extern s32 shougatu_meet_sss_sound;

void shougatu_serifu0_set(void)
{
    shougatu_alone_flag = 1;
    shougatu_meet_now_serifu = 0;
    shougatu_meet_ss_serifu = shougatu_meet_serifu_tbl;
    shougatu_meet_ss_serifu_kind = shougatu_meet_serifu_kind_tbl;
    shougatu_meet_sss_sound = shougatu_meet_sound_tbl;
    sub_step_up();
}
