/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x8013588C */
/* @SymbolSize: 200 */
#include "types.h"

extern u8 step_mode;

extern s32 sub_sub_step_up();
extern s32 get_CD_State2();
extern s32 cd_read_start2();
extern s32 decdct();
extern s32 dec_bg_reset();
extern s32 taco_menu_work_clear();
extern s32 taco_vram_clear();

s32 taco_bg_load()
{
    s32 ret;

    ret = step_mode;
    switch (step_mode) {
    case 0:
        dec_bg_reset();
        cd_read_start2(8, 0x80162000, 0xB378);
        ret = taco_menu_work_clear();
        step_mode = step_mode + 1;
        break;
    case 1:
        ret = get_CD_State2();
        if (ret & 0x1) {
            step_mode = step_mode + 1;
        }
        break;
    case 2:
        decdct(704, 0, 320, 240);
        taco_vram_clear();
        ret = sub_sub_step_up();
        break;
    }
    return ret;
}
