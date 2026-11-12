/* @SymbolStart: 0x801413A4 */
/* @SymbolSize: 72 */
/* shougatu_girl4_menu_serifu_check - SHOUGATU overlay, Address: 0x801413AC, Size: 64 bytes (16 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern s16 shougatu_menu_now_serifu;
extern u8 girl4_basyo;

void shougatu_girl4_menu_serifu_check(void)
{
    u32 a0_param = girl4_basyo;

    if ((a0_param >> 4) == 7) {
        shougatu_menu_now_serifu = shougatu_menu_now_serifu + 3;
    }
    sub_sub_step_up();
}
