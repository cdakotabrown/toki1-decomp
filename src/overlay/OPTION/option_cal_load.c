/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x8013AC50 */
/* @SymbolSize: 164 */
#include "types.h"

extern s32 cd_read_start2();
extern s32 get_CD_State2();
extern s32 cal_font_load();
extern s32 season_bgm_set();
extern s32 sub_sub_step_up();
extern u8 step_mode;
extern u8 now_month;
extern u8 now_year;
extern u8 anim_fade;

s32 option_cal_load()
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
            cal_font_load(now_year, now_month, 1);
            anim_fade = 239;
            season_bgm_set();
            ret = sub_sub_step_up();
        }
        break;
    }
    return ret;
}
