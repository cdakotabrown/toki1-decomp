/* @SymbolStart: 0x8013A9DC */
/* @SymbolSize: 64 */
/* Address: 0x8013A9E4 */
#include "types.h"
extern u16 basyo_pack;

extern void sub_sub_step_up(void);
extern s16 shougatu_date_now_serifu;

void
shougatu_basyo_pack_check(void)
{
    s32 t6_in;

    t6_in = basyo_pack;

if (((u32)t6_in >> 12) == 9) {
        shougatu_date_now_serifu = 5;
    }
    sub_sub_step_up();
}
