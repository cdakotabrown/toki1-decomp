/* @SymbolStart: 0x80136B54 */
/* @SymbolSize: 120 */
/* Address: 0x80136B98, Size: 52 bytes */
#include "types.h"

extern void serifu_main(s32, s32, s32);

extern u8 geko_now_serifu2[];
extern u8 geko_now_serifu2_in[];
extern s32 geko_ss_serifu2;
extern s32 geko_ss_serifu2_kind;
extern s32 geko_sss_sound2;

extern s32 now_s;
extern s32 now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;

void geko_serifu_run_end2(void)
{
    now_s = (s32)geko_now_serifu2;
    now_s_in = (s32)geko_now_serifu2_in;
    ss_s = geko_ss_serifu2;
    ss_s_kind = geko_ss_serifu2_kind;
    sss_so = geko_sss_sound2;
    serifu_main(255, 1, 0);
}
