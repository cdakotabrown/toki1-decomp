/* @SymbolStart: 0x80133878 */
/* @SymbolSize: 104 */
#include "types.h"

extern void sub_sub_step_up(void);
extern s16 shougatu_meet_now_serifu;
extern s32 shougatu_meet_ss_serifu;
extern s32 shougatu_meet_ss_serifu_kind;
extern s32 shougatu_meet_sss_sound;
extern u8 now_sub_sub_step;
extern s32 shougatu_meet_serifu_tbl[];
extern s32 shougatu_meet_serifu_kind_tbl[];
extern s32 shougatu_meet_sound_tbl[];

void shougatu_serifu7_set(void)
{
    shougatu_meet_now_serifu = 7;
    shougatu_meet_ss_serifu = shougatu_meet_serifu_tbl[2];
    shougatu_meet_ss_serifu_kind = shougatu_meet_serifu_kind_tbl[2];
    shougatu_meet_sss_sound = shougatu_meet_sound_tbl[2];
    sub_sub_step_up();
    now_sub_sub_step -= 5;
}
