/* @SymbolStart: 0x8013BA00 */
/* @SymbolSize: 68 */
/* shougatu_serifu_year_add - SHOUGATU overlay, Address: 0x8013BA00 (head) / 0x8013BA18 (prologue), */
/* Size: 68 bytes (17 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern u8  now_year;
extern s16 shougatu_appear_now_serifu;

void shougatu_serifu_year_add(void)
{
    shougatu_appear_now_serifu = shougatu_appear_now_serifu + ((now_year >= 97U) << 2);
    sub_sub_step_up();
}
