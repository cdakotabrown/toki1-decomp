/* @SymbolStart: 0x80138A38 */
/* @SymbolSize: 120 */
#include "types.h"

extern void serifu_main(s32, s32, s32);

extern u8 now_serifuev[];
extern u8 now_serifuev_in[];
extern s32 geko_ss_serifuev;
extern s32 geko_ss_serifuev_kind;
extern s32 geko_sss_soundev;

extern s32 now_s;
extern s32 now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;

void geko_serifuev_run_end(void)
{
    now_s = (s32)now_serifuev;
    now_s_in = (s32)now_serifuev_in;
    ss_s = geko_ss_serifuev;
    ss_s_kind = geko_ss_serifuev_kind;
    sss_so = geko_sss_soundev;
    serifu_main(255, 1, 0);
}
