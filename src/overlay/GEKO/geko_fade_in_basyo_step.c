/* @SymbolStart: 0x8013BBE8 */
/* @SymbolSize: 72 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
#include "types.h"

extern void fade_in_step(void);
extern void basyo_string2_step(void);

extern u8 now_sub_sub_step;

void geko_fade_in_basyo_step(void)
{
    s32 t6 = now_sub_sub_step;
    s32 t7;

    fade_in_step();
    t7 = (s32)now_sub_sub_step;
    if (t6 == t7) goto lbl_done;
    now_sub_sub_step = (u8)(t7 - 1);
    basyo_string2_step();
lbl_done:
    return;
}
