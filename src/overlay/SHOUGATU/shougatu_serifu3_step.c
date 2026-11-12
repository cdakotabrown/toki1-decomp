/* @SymbolSize: 40 */
/* shougatu_serifu3_step - SHOUGATU overlay, Address: 0x8013AA1C, Size: 60 bytes (15 insns) */
#include "types.h"

extern void sub_sub_step_up();
extern s16 shougatu_date_now_serifu;

void shougatu_serifu3_step(void)
{
    shougatu_date_now_serifu = 3;
    sub_sub_step_up();
}
