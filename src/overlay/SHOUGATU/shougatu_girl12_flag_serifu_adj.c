/* @SymbolStart: 0x8013E1C0 */
/* @SymbolSize: 76 */
/* Address: 0x8013E1E0 */
/* Size: 44 bytes (11 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern u32 girl12_flag;
extern s16 shougatu_menu_now_serifu;

void shougatu_girl12_flag_serifu_adj(void)
{
    shougatu_menu_now_serifu = shougatu_menu_now_serifu + ((girl12_flag << 20) >> 29) * 3 - 3;
    sub_sub_step_up();
}
