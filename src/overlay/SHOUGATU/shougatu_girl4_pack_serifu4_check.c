/* @SymbolStart: 0x8013AF6C */
/* @SymbolSize: 72 */
/* Address: 0x8013AF7C, Size: 72 bytes (18 instructions). */
#include "types.h"
extern u16 basyo_pack;
extern u8 girl4_basyo;

extern void sub_sub_step_up(void);
extern s16 shougatu_date_now_serifu;

void shougatu_girl4_pack_serifu4_check(void)
{
    u32 t6_param;
    u32 t8_param;

    t8_param = girl4_basyo;
    t6_param = basyo_pack;

if ((t8_param >> 4) != (t6_param >> 12)) {
        shougatu_date_now_serifu = 4;
    }
    sub_sub_step_up();
}
