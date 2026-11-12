/* @SymbolStart: 0x801352DC */
/* @SymbolSize: 100 */
/* @PostProc: patch_insn_reg_field 0x0:rt:v1:v0 */
/* @PostProc: patch_insn_reg_field 0x4:rs:v1:v0 */
/* Address: 0x801352E4 */
/* Size: 92 bytes (23 insns) */
#include "types.h"

extern s32 step_counter;

extern s32 pad_trig;

extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32 a0);
extern void white_light(s32 a0);

void white_light_down(void)
{
    s32 sel = step_counter;

    if ((u32)sel < 255) {
        if (pad_trig != 0) {
            goto_sub_sub_step(5);
        } else {
            white_light(255 - sel);
        }
    } else {
        sub_sub_step_up();
    }
}
