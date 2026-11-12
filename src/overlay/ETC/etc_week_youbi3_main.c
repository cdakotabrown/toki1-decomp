/* @SymbolStart: 0x801477E0 */
/* @RodataBase: 0x8014D850 */
/* @SymbolSize: 140 */
#include "types.h"

extern void etc_week_bustup_load(void);
extern void etc_week_main3_1(void);
extern void etc_week_cw_wait(void);
extern void etc_week_grade_disp(void);
extern void etc_week_step_up(void);

extern u8 now_sub_sub_step;

void etc_week_youbi3_main(void) {
    s32 cmd = now_sub_sub_step;

    switch (cmd) {
    case 0:
        etc_week_bustup_load();
        break;
    case 1:
        etc_week_main3_1();
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
