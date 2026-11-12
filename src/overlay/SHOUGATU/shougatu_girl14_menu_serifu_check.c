/* @SymbolStart: 0x8013DB2C */
/* @SymbolSize: 80 */
/* shougatu_girl14_menu_serifu_check - SHOUGATU overlay, Address: 0x8013DB34, Size: 80 bytes (20 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void shougatu_menu_serifu_run(void);
extern s32 girl14_flag;

void shougatu_girl14_menu_serifu_check(void)
{
    u32 a0_param = girl14_flag;

    if (((a0_param << 20) >> 29) == 1) {
        shougatu_menu_serifu_run();
        return;
    }
    sub_sub_step_up();
    sub_sub_step_up();
}
