/* @SymbolStart: 0x80135754 */
/* @SymbolSize: 108 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 */
/* Address: 0x8013575C (window starts at the cross-prologue head 0x80135754) */
/* Size: 108 bytes (27 insns) including the absorbed 2-insn head */
#include "types.h"

extern u8 now_sub_sub_step;

extern s32 taco_bg_load(void);
extern s32 taco_goto_sub2(void);
extern s32 taco_menu_show(void);

s32 taco_step_dispatch3(void)
{
    s32 sel;
    s32 ret;

    sel = now_sub_sub_step;
    ret = sel;
    switch (sel) {
        case 0:
            ret = taco_bg_load();
            break;
        case 1:
            ret = taco_menu_show();
            break;
        case 2:
            ret = taco_goto_sub2();
            break;
    }
    return ret;
}
