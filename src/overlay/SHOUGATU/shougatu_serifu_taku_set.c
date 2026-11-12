/* @SymbolStart: 0x8013671C */
/* @SymbolSize: 52 */
/* Address: 0x80136724 */
/* Size: 44 bytes (11 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern s16 now_serifu;
extern s32 serifu_no;

void shougatu_serifu_taku_set(void)
{
    now_serifu = (serifu_no << 1) + 13;
    sub_sub_step_up();
}
