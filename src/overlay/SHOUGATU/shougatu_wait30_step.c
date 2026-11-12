/* Address: 0x8013D8C8 */
/* Size: 40 bytes (10 instructions) */
/* @SymbolSize: 40 */
#include "types.h"

extern void sub_sub_step_up();
extern s32 k_wait_cnt;

void shougatu_wait30_step(void)
{
    k_wait_cnt = 30;
    sub_sub_step_up();
}
