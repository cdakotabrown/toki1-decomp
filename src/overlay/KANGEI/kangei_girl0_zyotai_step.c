/* @SymbolSize: 76 */
/* @PostProc: rename_register_chain --chain t6:t7:t8:t9 */
#include "types.h"

extern s32 get_g_zyotai_s(s32 a0);
extern void sub_sub_step_up(void);
extern u8 now_sub_sub_step;

void kangei_girl0_zyotai_step(void)
{
    s32 result;
    u8 val;

    result = get_g_zyotai_s(0);
    if ((u32)(result & 0x7F) >= 2) {
        val = now_sub_sub_step;
        now_sub_sub_step = (u8)(val - 11);
    } else {
        sub_sub_step_up();
    }
}
