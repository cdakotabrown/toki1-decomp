/* @SymbolStart: 0x8013BA44 */
/* @SymbolSize: 64 */
/* /* shougatu_serifu_appear_add -- SHOUGATU overlay, Address: 0x8013BA4C, Size: 64 bytes (16 insns) */
#include "types.h"

extern void sub_sub_step_up(void);

extern s16 shougatu_appear_now_serifu;
extern u8 shougatu_appear_any;

void shougatu_serifu_appear_add(void)
{
    if (shougatu_appear_any != 0) goto label0;
    shougatu_appear_now_serifu = (shougatu_appear_now_serifu + 1);

label0:
    sub_sub_step_up();
}
