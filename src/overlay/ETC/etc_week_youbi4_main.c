/* @SymbolStart: 0x8014786C */
/* @SymbolSize: 140 */
/* @RodataBase: 0x8014D864 */
/* Address: 0x80147874, Size: 132 bytes (33 insns) */
#include "types.h"

extern void etc_week_bustup_load(void);
extern void etc_week_main4_1(void);
extern void etc_week_cw_wait(void);
extern void etc_week_grade_disp(void);
extern void etc_week_step_up(void);

extern u8 now_sub_sub_step;

void etc_week_youbi4_main(void) {
    s32 cmd = now_sub_sub_step;

    switch (cmd) {
    case 0:
        etc_week_bustup_load();
        break;
    case 1:
        etc_week_main4_1();
        break;
    case 2:
        etc_week_cw_wait();
        break;
    case 3:
        etc_week_grade_disp();
        break;
    case 4:
        etc_week_step_up();
        break;
    }
}
