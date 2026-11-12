/* Address: 0x801386A8 (true start 0x80138664, .sym late by 17 insns) */
/* @SymbolStart: 0x80138664 */
/* @SymbolSize: 124 */
#include "types.h"

extern void serifu_main(s32 a0, s32 a1, s32 a2);

extern s16 now_serifu_shugaku;
extern s16 now_serifu_shugaku_in;
extern s32 ss_serifu_shugaku;
extern s32 ss_serifu_shugaku_kind;
extern s32 sss_sound_shugaku;
extern u8 serifu_no;

extern s32 now_s;
extern s32 now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;

void shugaku_serifu_run(void)
{
    now_s = (s32)&now_serifu_shugaku;
    now_s_in = (s32)&now_serifu_shugaku_in;
    ss_s = ss_serifu_shugaku;
    ss_s_kind = ss_serifu_shugaku_kind;
    sss_so = sss_sound_shugaku;
    serifu_main((s32)serifu_no, 1, 0);
}
