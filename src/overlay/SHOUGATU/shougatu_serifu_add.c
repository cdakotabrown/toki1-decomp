/* @SymbolStart: 0x8013B280 */
/* @SymbolSize: 68 */
/* shougatu_serifu_add - SHOUGATU overlay, Address: 0x8013B288, Size: 60 bytes (15 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern s16 shougatu_date_now_serifu;
extern u8 girl13_tel_cnt;

void shougatu_serifu_add(void)
{
    if ((u32)girl13_tel_cnt < 2) {
        shougatu_date_now_serifu = shougatu_date_now_serifu + 1;
    }
    sub_sub_step_up();
}
