/* @SymbolStart: 0x8013B2C4 */
/* Address: 0x8013B2CC */
/* Size: 60 bytes (15 instructions) */
/* @SymbolSize: 68 */
#include "types.h"

extern void sub_sub_step_up(void);
extern s16 shougatu_date_now_serifu;
extern u8 girl13_tel_cnt;

void shougatu_girl13_tel_serifu_inc(void)
{
    if ((u32)girl13_tel_cnt >= 2) {
        shougatu_date_now_serifu++;
    }
    sub_sub_step_up();
}
