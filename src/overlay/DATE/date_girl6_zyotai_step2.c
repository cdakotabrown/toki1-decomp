/* @PostProc: swap_registers t8:t9 */
/* @PostProc: swap_registers t7:t8 */
/* @PostProc: swap_registers t6:t7 */
/* @SymbolSize: 80 */
#include "types.h"

extern u8 now_girl;
extern u16 now_serifuev;
extern s32 sub_sub_step_up(void);
extern s32 get_g_zyotai_s(u8);

void date_girl6_zyotai_step2(void)
{
    s32 v0;
    v0 = get_g_zyotai_s(now_girl);
    if ((u32)(v0 & 0x7F) >= 2) {
        now_serifuev += 5;
    }
    sub_sub_step_up();
}
