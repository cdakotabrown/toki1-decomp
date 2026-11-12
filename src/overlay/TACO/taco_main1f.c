/* @SymbolStart: 0x80138870 */
/* @SymbolSize: 212 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-12 */
/* Address: 0x80138880 (true entry 0x80138870, 4 insns hoisted above .sym) */
#include "types.h"

extern u32 step_state;
extern u8 now_sub_sub_step;

extern void taco_score_tally(void);
extern void taco_item_reveal(void);
extern void taco_result_branch(void);
extern void taco_screen_zoom_disp(s32 a0);

s32 taco_main1f(void)
{
    u32 sel;
    u32 t;

    sel = now_sub_sub_step;
    step_state = step_state + 1;

    switch (sel) {
    case 0:
        taco_score_tally();
        break;
    case 1:
        taco_item_reveal();
        break;
    case 2:
        taco_result_branch();
        break;
    }

    if (now_sub_sub_step < 2U) {
        t = step_state;
        if (t < 50) {
            taco_screen_zoom_disp((s32)(100 - t));
        } else {
            taco_screen_zoom_disp(50);
        }
    }
}
