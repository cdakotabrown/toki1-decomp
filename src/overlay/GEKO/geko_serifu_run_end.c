/* @SymbolStart: 0x80132364 */
/* @SymbolSize: 124 */
#include "types.h"

extern void serifu_main(s32, s32, s32);

extern u8 geko_now_serifu[];
extern u8 geko_now_serifu_in[];
extern s32 geko_ss_serifu;
extern s32 geko_ss_serifu_kind;
extern s32 geko_sss_sound;

extern s32 now_s;
extern s32 now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;

void geko_serifu_run_end(void)
{
    now_s = (s32)geko_now_serifu;
    now_s_in = (s32)geko_now_serifu_in;
    ss_s = geko_ss_serifu;
    ss_s_kind = geko_ss_serifu_kind;
    sss_so = geko_sss_sound;
    serifu_main(255, 1, 0);
}
