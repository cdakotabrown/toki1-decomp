/* @SymbolStart: 0x8013D6B0 */
/* @SymbolSize: 124 */
#include "types.h"

extern void serifu_main(s32, s32, s32);
extern u8 serifu_no;
extern s32 shougatu_ss_serifuev;
extern s32 shougatu_ss_serifuev_kind;
extern s32 shougatu_sss_soundev;
extern s16 now_serifuev;
extern s16 now_serifuev_in;
extern s16 *now_s;
extern s16 *now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;

void shougatu_serifuev_run2(void)
{
    now_s = &now_serifuev;
    now_s_in = &now_serifuev_in;
    ss_s = shougatu_ss_serifuev;
    ss_s_kind = shougatu_ss_serifuev_kind;
    sss_so = shougatu_sss_soundev;
    serifu_main((s32)serifu_no, 1, 0);
}
