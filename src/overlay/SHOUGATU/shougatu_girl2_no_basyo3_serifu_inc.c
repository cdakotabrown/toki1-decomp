/* @SymbolStart: 0x80138B10 */
/* Address: 0x80138B18 */
/* Size: 56 bytes (14 instructions) */
/* @SymbolSize: 64 */
#include "types.h"

extern void sub_sub_step_up(void);
extern s16 shougatu_date_now_serifu;
extern u8 shougatu_girl2_basyo3;

void shougatu_girl2_no_basyo3_serifu_inc(void)
{
    if (shougatu_girl2_basyo3 == 0) {
        shougatu_date_now_serifu++;
    }
    sub_sub_step_up();
}
