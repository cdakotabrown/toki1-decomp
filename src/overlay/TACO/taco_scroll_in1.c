/* @SymbolStart: 0x80139A80 */
/* @SymbolSize: 164 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x64:rt:t9:t8 0x78:rs:t9:t8 0x78:rt:t8:v0 0x7c:rt:t8:v0 0x80:rs:t8:v0 0x80:rt:t8:v0 */
/* Address: 0x80139A88 */
/* Size: 156 bytes (39 insns) */
#include "types.h"

extern s32 taco_drop_bright;
extern s32 taco_wheel_slide;

extern void sub_sub_step_up(void);
extern void taco_wheel_disp(s32 a0, s32 a1, s32 a2, s32 a3);
extern void taco_drop(s32 a0, s32 a1);

void taco_scroll_in1(void)
{
    s32 sel = taco_wheel_slide;

    s32 v;

    if (sel < 256) {
        taco_drop_bright = sel / 2;
    } else {
        taco_drop_bright = 128;
    }

    taco_drop(0, 0);

    v = taco_wheel_slide;
    taco_wheel_disp(65, 0, 640 - v, 0);

    v = taco_wheel_slide + 4;
    taco_wheel_slide = v;
    if (v < 481) {
        return;
    }
    sub_sub_step_up();
}
