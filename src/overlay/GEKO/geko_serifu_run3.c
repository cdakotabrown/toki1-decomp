/* @SymbolStart: 0x80143A3C */
/* @SymbolSize: 124 */
#include "types.h"

extern void serifu_main(s32, s32, s32);

extern u8 geko_now_serifu3[];
extern u8 geko_now_serifu3_in[];
extern s32 geko_ss_serifu3;
extern s32 geko_ss_serifu3_kind;
extern s32 geko_sss_sound3;

extern s32 now_s;
extern s32 now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;

extern u8 serifu_no;

void geko_serifu_run3(void)
{
    now_s = (s32)geko_now_serifu3;
    now_s_in = (s32)geko_now_serifu3_in;
    ss_s = geko_ss_serifu3;
    ss_s_kind = geko_ss_serifu3_kind;
    sss_so = geko_sss_sound3;
    serifu_main(serifu_no, 1, 0);
}
