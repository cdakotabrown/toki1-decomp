/* @CompileEnv: IDO52_COPYPROP_VAR=-8 IDO52_PRECOLOR_WEB=v0:10 */
/* @SymbolStart: 0x80147F68 */
/* @SymbolSize: 128 */
/* Address: 0x80147F78, Size: 112 bytes (28 insns) */
#include "types.h"

extern s32 step_counter;
extern u8 now_sub_sub_step;
extern s32 etc_week_rank_disp();
extern s32 etc_week_result_disp();
extern s32 etc_panel_show();

s32 etc_week_result_main(void)
{
    s32 sel;
    s32 ret;

    step_counter = step_counter + 1;
    sel = now_sub_sub_step;
    ret = sel;
    switch (sel) {
    case 0:
        return etc_week_rank_disp();
        break;
    case 1:
        return etc_week_result_disp();
        break;
    case 2:
        return etc_panel_show();
        break;
    }
    return ret;
}
