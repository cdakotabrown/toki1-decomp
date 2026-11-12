/* Address: 0x80138630 (true start 0x801385EC, .sym late by 17 insns) */
/* @SymbolStart: 0x801385EC */
/* @SymbolSize: 120 */
#include "types.h"

extern void serifu_main(s32 a0, s32 a1, s32 a2);

extern s16 now_serifu_shugaku;
extern s16 now_serifu_shugaku_in;
extern s32 ss_serifu_shugaku;
extern s32 ss_serifu_shugaku_kind;
extern s32 sss_sound_shugaku;

extern s32 now_s;
extern s32 now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;

void shugaku_serifu_start(void)
{
    now_s = (s32)&now_serifu_shugaku;
    now_s_in = (s32)&now_serifu_shugaku_in;
    ss_s = ss_serifu_shugaku;
    ss_s_kind = ss_serifu_shugaku_kind;
    sss_so = sss_sound_shugaku;
    serifu_main(255, 1, 0);
}
