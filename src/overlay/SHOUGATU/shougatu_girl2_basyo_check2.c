/* @SymbolStart: 0x80141208 */
/* @SymbolSize: 72 */
/* Address: 0x80141210, Size: 64 bytes (16 insns) */
#include "types.h"

extern void sub_sub_step_up(void);

extern s16 shougatu_menu_now_serifu;
extern u8 girl2_basyo;

void shougatu_girl2_basyo_check2(void)
{
    if (((u32)girl2_basyo >> 4) == 3) goto label0;
    shougatu_menu_now_serifu = (shougatu_menu_now_serifu + 1);

label0:
    sub_sub_step_up();
}
