/* @SymbolStart: 0x801323D4 */
/* @SymbolSize: 124 */
#include "types.h"

extern void serifu_main(s32, s32, s32);
extern u8 serifu_no;
extern s32 shougatu_meet_ss_serifu;
extern s32 shougatu_meet_ss_serifu_kind;
extern s32 shougatu_meet_sss_sound;
extern s16 shougatu_meet_now_serifu;
extern s16 shougatu_meet_now_serifu_in;
extern s16 *now_s;
extern s16 *now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;

void shougatu_serifu_run(void)
{
    now_s = &shougatu_meet_now_serifu;
    now_s_in = &shougatu_meet_now_serifu_in;
    ss_s = shougatu_meet_ss_serifu;
    ss_s_kind = shougatu_meet_ss_serifu_kind;
    sss_so = shougatu_meet_sss_sound;
    serifu_main((s32)serifu_no, 1, 0);
}
