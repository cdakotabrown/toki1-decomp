/* @SymbolSize: 40 */
/* /* shougatu_serifu39_step - SHOUGATU overlay, Address: 0x80133A64, Size: 48 bytes (12 insns) */
#include "types.h"

extern void sub_sub_step_up();
extern s16 shougatu_meet_now_serifu;

void shougatu_serifu39_step(void)
{
    shougatu_meet_now_serifu = 39;
    sub_sub_step_up();
}
