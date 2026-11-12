/* @SymbolStart: 0x8013401C */
/* @SymbolSize: 84 */
/* shougatu_girl5_serifu_check - SHOUGATU overlay, Address: 0x80134024, Size: 84 bytes (21 insns) */
#include "types.h"

extern void sub_sub_step_up();
extern void shougatu_serifu_run();
extern u8 shougatu_serifu6_flag;
extern u8 now_girl;

void shougatu_girl5_serifu_check(void)
{
    if (now_girl == 5) {
        if (shougatu_serifu6_flag == 0) {
            shougatu_serifu_run();
            return;
        }
    }
    sub_sub_step_up();
}
