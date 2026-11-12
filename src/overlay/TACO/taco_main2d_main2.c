/* @SymbolStart: 0x8013B394 */
/* @SymbolSize: 64 */
/* Address: 0x8013B39C (window starts at the cross-prologue head 0x8013B394) */
/* Size: 64 bytes (16 insns) including the absorbed 2-insn head */
#include "types.h"

extern u8 taco_load_no;
extern void goto_sub_step();
extern s32 device_load_poll2();

void taco_main2d_main2(void)
{
    u8 flag = taco_load_no;

    if (flag == 0) {
        if (device_load_poll2() == 1) {
            goto_sub_step(8);
        }
    }
}
