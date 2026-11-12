/* @SymbolStart: 0x80143B30 */
/* @SymbolSize: 148 */
/* geko_serifu_result_step2 -- GEKO overlay, Address: 0x80143B38, Size: 140 bytes (35 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32 a0);
extern void param_clamp(void);

extern u8 screen_w;
extern u8 now_girl;
extern s16 geko_now_serifu3;
extern u8 geko_pick_no;
extern s32 serifu_no;

void geko_serifu_result_step2(void)
{
    s32 t6 = serifu_no;

    s16 *p;

    if (t6 != 0) {
        p = (s16 *)((u8 *)&screen_w + now_girl * 56);
        geko_now_serifu3 = 23;
        p[227] = p[227] + 15;
        goto_sub_sub_step(24);
    } else {
        geko_now_serifu3 = geko_now_serifu3 + geko_pick_no;
        sub_sub_step_up();
    }
    param_clamp();
}
