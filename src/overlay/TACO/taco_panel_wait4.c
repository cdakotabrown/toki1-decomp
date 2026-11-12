/* Address: 0x80136F5C */
/* Size: 112 bytes (28 insns) */
#include "types.h"

extern s32 pad_data;
extern s32 pad_trig;

extern void sub_sub_step_up();
extern void taco_panel_disp();
extern void taco_drop();

void taco_panel_wait4(void)
{
    taco_drop(1, 0);
    taco_panel_disp(4096, 3);

    if (pad_data & 0x40) {
        sub_sub_step_up();
    }

    if (pad_trig & 0x20) {
        sub_sub_step_up();
    }
}
