/* @SymbolStart: 0x8013D3FC */
/* @SymbolSize: 144 */
#include "types.h"

extern s32 serifu_main(s32, s32, s32);
extern void sub_sub_step_up(void);
extern u8 serifu_no;
extern s32 shougatu_menu_ss_serifu;
extern s32 shougatu_menu_ss_serifu_kind;
extern s32 shougatu_menu_sss_sound;
extern s16 shougatu_menu_now_serifu;
extern s16 shougatu_menu_now_serifu_in;
extern s16 *now_s;
extern s16 *now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;

void shougatu_menu_serifu_wait(void)
{
    now_s = &shougatu_menu_now_serifu;
    now_s_in = &shougatu_menu_now_serifu_in;
    ss_s = shougatu_menu_ss_serifu;
    ss_s_kind = shougatu_menu_ss_serifu_kind;
    sss_so = shougatu_menu_sss_sound;
    if (serifu_main((s32)serifu_no, 1, 0) == 1) {
        sub_sub_step_up();
    }
}
