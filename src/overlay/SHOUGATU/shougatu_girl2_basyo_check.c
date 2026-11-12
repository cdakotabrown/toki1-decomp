/* @SymbolStart: 0x801411C0 */
/* @SymbolSize: 72 */
/* Address: 0x801411C8, Size: 64 bytes (16 insns) */
#include "types.h"

extern void sub_sub_step_up(void);

extern s16 shougatu_menu_now_serifu;
extern u8 girl2_basyo;

void shougatu_girl2_basyo_check(void)
{
    u32 inputState = girl2_basyo;

    if ((inputState >> 4) == 3) {
        shougatu_menu_now_serifu = shougatu_menu_now_serifu + 1;
    }
    sub_sub_step_up();
}
