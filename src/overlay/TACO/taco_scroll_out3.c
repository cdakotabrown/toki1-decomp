/* @SymbolSize: 120 */
/* /* taco_scroll_out3 -- TACO overlay, Address: 0x80139B24 */
/* Size: 120 bytes (30 insns) */
#include "types.h"

extern s32 taco_wheel_slide;
extern s32 taco_wheel_no;

extern void sub_sub_step_up(void);
extern void taco_wheel_disp(s32, s32, s32, s32);
extern void taco_name_disp(s32);
extern void taco_drop(s32, s32);

void taco_scroll_out3(void)
{
    s32 v;

    taco_drop(0, 0);
    taco_name_disp(taco_wheel_slide);
    taco_wheel_disp(taco_wheel_no, 0, 640 - taco_wheel_slide, 0);
    v = taco_wheel_slide - 8;
    taco_wheel_slide = v;
    if (v < 0) {
        sub_sub_step_up();
    }
}
