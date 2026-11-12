/* @SymbolStart: 0x801383FC */
/* @SymbolSize: 64 */
/* Address: 0x801383FC (head) / 0x80138404 (body) */
#include "types.h"

extern void sub_sub_step_up(void);
extern s16 shougatu_date_now_serifu;
extern u8 shougatu_girl2_basyo3;

void
shougatu_girl2_basyo_add(void)
{
    if (shougatu_girl2_basyo3 != 0) {
        shougatu_date_now_serifu = shougatu_date_now_serifu + 1;
    }
    sub_sub_step_up();
}
