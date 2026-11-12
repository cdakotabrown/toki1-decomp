/* @SymbolStart: 0x80133FE0 */
/* @SymbolSize: 60 */
/* shougatu_serifu6_flag_skip_check - SHOUGATU overlay, Address: 0x80133FE8, Size: 60 bytes (15 insns) */
#include "types.h"

extern void sub_sub_step_up();
extern void shougatu_serifu_run();
extern u8 shougatu_serifu6_flag;

void shougatu_serifu6_flag_skip_check(void)
{
    if (shougatu_serifu6_flag == 0) {
        sub_sub_step_up();
    } else {
        shougatu_serifu_run();
    }
}
