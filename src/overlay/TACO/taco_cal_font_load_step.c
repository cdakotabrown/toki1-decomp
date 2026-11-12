/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x80142E0C */
/* @SymbolSize: 180 */
#include "types.h"

extern u8 now_year;
extern u8 now_month;
extern u8 step_mode;
extern s32 sub_sub_step_up();
extern s32 get_CD_State2();
extern s32 cd_read_start2();
extern s32 cal_font_load();

s32 taco_cal_font_load_step()
{
    s32 ret;

    ret = step_mode;
    switch (step_mode) {
    case 0:
        ret = cd_read_start2(149, 0x801A0000, 0x9B94);
        step_mode = step_mode + 1;
        break;
    case 1:
        ret = get_CD_State2();
        if ((ret & 1) != 0) {
            ret = cal_font_load(now_year, now_month, 0);
            step_mode = step_mode + 1;
        }
        break;
    case 2:
        ret = sub_sub_step_up();
        break;
    }
    return ret;
}
