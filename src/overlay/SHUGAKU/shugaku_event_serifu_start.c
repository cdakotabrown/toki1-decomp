/* Address: 0x80134460 (true start 0x8013441C, .sym late by 17 insns) */
/* @SymbolStart: 0x8013441C */
/* @SymbolSize: 120 */
#include "types.h"

extern void serifu_main(s32 a0, s32 a1, s32 a2);

extern s16 with_yoshi;
extern s16 move_in;
extern s32 shugaku_ss_serifuev;
extern s32 shugaku_ss_serifuev_kind;
extern s32 shugaku_sss_soundev;

extern s32 now_s;
extern s32 now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;

void shugaku_event_serifu_start(void)
{
    now_s = (s32)&with_yoshi;
    now_s_in = (s32)&move_in;
    ss_s = shugaku_ss_serifuev;
    ss_s_kind = shugaku_ss_serifuev_kind;
    sss_so = shugaku_sss_soundev;
    serifu_main(255, 1, 0);
}
