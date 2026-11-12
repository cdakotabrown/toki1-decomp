/* @SymbolStart: 0x8013AFB4 */
/* @SymbolSize: 84 */
/* Address: 0x8013AFC4 */
/* Size: 76 bytes (19 instructions) */
#include "types.h"
extern u8 girl4_basyo;
extern u16 basyo_pack;

extern void sub_sub_step_up(void);
extern u8 now_sub_sub_step;

void shougatu_girl4_basyo_pack_skip_check(void)
{
    u32 a0_param;
    u32 a1_param;
    u32 temp1;
    u32 temp2;

    a1_param = girl4_basyo;
    a0_param = basyo_pack;
    temp2 = a1_param >> 4;
    temp1 = a0_param >> 12;

    if (temp2 == temp1) {
        now_sub_sub_step = now_sub_sub_step + 13;
    } else {
        sub_sub_step_up();
    }
}
