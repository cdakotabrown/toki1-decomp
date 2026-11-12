/* @SymbolStart: 0x8013AECC */
/* @SymbolSize: 64 */
/* Address: 0x8013AED4, Size: 72 bytes (18 instructions). */
#include "types.h"

extern void sub_sub_step_up(void);
extern s16 shougatu_date_now_serifu;
extern u8 girl4_basyo;

void shougatu_girl4_date_serifu6_check(void)
{
    u32 a0_param = girl4_basyo;

    if ((a0_param >> 4) == 7) {
        shougatu_date_now_serifu = 6;
    }
    sub_sub_step_up();
}
