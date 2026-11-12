/* @SymbolStart: 0x80134070 */
/* @SymbolSize: 60 */
/* shougatu_serifu6_flag_run_check - SHOUGATU overlay, Address: 0x80134078, Size: 60 bytes (15 insns) */
#include "types.h"

extern void shougatu_serifu_run();
extern void sub_sub_step_up();
extern u8 shougatu_serifu6_flag;

void shougatu_serifu6_flag_run_check(void)
{
    if (shougatu_serifu6_flag != 0) {
        shougatu_serifu_run();
        return;
    }
    sub_sub_step_up();
}
