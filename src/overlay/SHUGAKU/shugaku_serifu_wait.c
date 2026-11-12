/* @SymbolStart: 0x80138C58 */
/* @SymbolSize: 108 */
#include "types.h"
extern s32 ss_serifu_shugaku_kind;
extern s16 now_serifu_shugaku;

extern void sub_sub_step_up(void);
extern void step_wait(s32 a0);

extern s16  ss_serifu_shugaku;
extern s16  now_serifu_shugaku_in;
extern s32 *shugaku_place;

void shugaku_serifu_wait(void)
{
    s32 idx;
    s32 base;

    idx = now_serifu_shugaku;
    base = ss_serifu_shugaku_kind;
    if (*(u8 *)(((s32 *)base)[idx] + now_serifu_shugaku_in) == 9) {
        sub_sub_step_up();
    } else {
        step_wait(128);
    }
}
