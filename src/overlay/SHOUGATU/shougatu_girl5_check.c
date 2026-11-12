/* @SymbolStart: 0x80133F3C */
/* @SymbolSize: 100 */
/* shougatu_girl5_check - SHOUGATU overlay, Address: 0x80133F44, Size: 92 bytes (23 insns) */
#include "types.h"

extern void sub_step_up();
extern void sub_sub_step_up();
extern u8 shougatu_serifu6_flag;
extern u8 shougatu_alone_flag;
extern u8 now_girl;

void shougatu_girl5_check(void)
{
    if (now_girl != 5 || shougatu_alone_flag != 0 || shougatu_serifu6_flag == 0) {
        sub_step_up();
    } else {
        sub_sub_step_up();
    }
}
