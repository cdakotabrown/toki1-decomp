/* @SymbolSize: 40 */
/* shugaku_serifu14_set - SHUGAKU overlay, Address: 0x80139588, Size: 48 bytes (12 insns) */
#include "types.h"

extern s16 now_serifu_shugaku;

extern void sub_sub_step_up(void);

void shugaku_serifu14_set(void)
{
    now_serifu_shugaku = 14;
    sub_sub_step_up();
}
