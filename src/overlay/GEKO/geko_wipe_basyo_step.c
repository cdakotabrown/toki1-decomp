/* @SymbolStart: 0x8013BBA0 */
/* @SymbolSize: 72 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
#include "types.h"

extern void wipe_main(void);
extern void basyo_string_step(void);

extern u8 now_sub_sub_step;

void geko_wipe_basyo_step(void)
{
    s32 t6 = now_sub_sub_step;
    s32 t7;

    wipe_main();
    t7 = (s32)now_sub_sub_step;
    if (t6 == t7) goto lbl_done;
    now_sub_sub_step = (u8)(t7 - 1);
    basyo_string_step();
lbl_done:
    return;
}
