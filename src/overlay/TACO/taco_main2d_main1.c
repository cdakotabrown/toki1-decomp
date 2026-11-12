/* @SymbolStart: 0x8013B368 */
/* @SymbolSize: 44 */
/* Address: 0x8013B370 (window starts at the cross-prologue head 0x8013B368) */
/* Size: 52 bytes (13 insns) including the absorbed 2-insn head */
#include "types.h"

extern s32 pad_trig;
extern void sub_sub_step_up();

void taco_main2d_main1(void)
{
    s32 flag = pad_trig;

    if (flag != 0) {
        sub_sub_step_up();
    }
}
