/* @SymbolStart: 0x80138CC4 */
/* @SymbolSize: 108 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void fade_down_main(void);

extern s32 ss_serifu_shugaku_kind;
extern s16 now_serifu_shugaku;
extern s16 now_serifu_shugaku_in;

void shugaku_serifu_fade_down(void)
{
    s32 idx;
    s32 base;

    idx = now_serifu_shugaku;
    base = ss_serifu_shugaku_kind;
    if (*(u8 *)(((s32 *)base)[idx] + now_serifu_shugaku_in) == 9) {
        sub_sub_step_up();
    } else {
        fade_down_main();
    }
}
