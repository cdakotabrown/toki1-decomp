/* @SymbolStart: 0x801349C4 */
/* @SymbolSize: 124 */
#include "types.h"

extern void serifu_main(s32, s32, s32);

extern u8 geko_now_serifu_icon[];
extern u8 geko_now_serifu_icon_in[];
extern s32 geko_ss_serifu_icon;
extern s32 geko_ss_serifu_icon_kind;
extern s32 geko_sss_sound_icon;

extern s32 now_s;
extern s32 now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;

extern u8 serifu_no;

void icon_serifu_run(void)
{
    now_s = (s32)geko_now_serifu_icon;
    now_s_in = (s32)geko_now_serifu_icon_in;
    ss_s = geko_ss_serifu_icon;
    ss_s_kind = geko_ss_serifu_icon_kind;
    sss_so = geko_sss_sound_icon;
    serifu_main(serifu_no, 1, 0);
}
