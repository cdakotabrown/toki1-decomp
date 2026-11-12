/* @SymbolStart: 0x80136E18 */
/* @SymbolSize: 104 */
#include "types.h"

extern s32 taco_scroll_pos;
extern void sub_sub_step_up(void);
extern void taco_panel_disp(s32, s32);
extern void taco_drop(s32, s32);

void taco_scroll5(void)
{
    s32 t6_param = taco_scroll_pos;

    taco_scroll_pos = t6_param + 500;
    taco_drop(1, 0);
    taco_panel_disp(taco_scroll_pos, 0);
    if ((s32)taco_scroll_pos >= 0x9001) {
        taco_scroll_pos = 0;
        sub_sub_step_up();
    }
}
