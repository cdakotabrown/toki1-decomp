/* @SymbolStart: 0x80138AD0 */
/* @SymbolSize: 64 */
/* shougatu_girl2_basyo3_serifu_inc - SHOUGATU overlay, Address: 0x80138AD8, Size: 64 bytes (16 insns) */
#include "types.h"

extern void sub_sub_step_up();
extern s16 shougatu_date_now_serifu;
extern u8 shougatu_girl2_basyo3;

void shougatu_girl2_basyo3_serifu_inc(void)
{
    if (shougatu_girl2_basyo3 != 0) {
        shougatu_date_now_serifu = shougatu_date_now_serifu + 1;
    }
    sub_sub_step_up();
}
