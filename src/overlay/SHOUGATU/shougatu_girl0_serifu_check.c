/* @PostProc: swap_registers t7:t8 */
/* Address: 0x8013E4E0 */
/* Size: 84 bytes (21 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 get_g_zyotai_s(s32);
extern void shougatu_menu_serifu_run(void);
extern u8 shougatu_girl9_appear;

void shougatu_girl0_serifu_check(void)
{
    s32 ret;

    ret = get_g_zyotai_s(0);
    if (shougatu_girl9_appear != 0 || (ret & 0x7f) == 4) {
        shougatu_menu_serifu_run();
    } else {
        sub_sub_step_up();
    }
}
