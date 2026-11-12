/* @SymbolSize: 108 */
/* @PostProc: swap_registers t8:t9 */
/* @PostProc: swap_registers t7:t8 */
/* @PostProc: swap_registers t6:t7 */
/* Address: 0x801338E0 */
/* Size: 108 bytes (27 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 get_g_zyotai_s(s32);
extern u8 now_girl;
extern s16 shougatu_meet_now_serifu;

void shougatu_zyotai4_check(void)
{
    s32 r;
    s16 v;

    r = get_g_zyotai_s((s32)now_girl);
    if ((r & 0x7F) != 4) {
        v = shougatu_meet_now_serifu;
        shougatu_meet_now_serifu = v + 1;
        sub_sub_step_up();
        sub_sub_step_up();
        sub_sub_step_up();
        return;
    }
    sub_sub_step_up();
}
