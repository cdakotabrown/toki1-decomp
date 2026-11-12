/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x80137BE8 */
/* @SymbolSize: 228 */
#include "types.h"

extern u8 step_mode;
extern s16 taco_time_left;
extern s32 sub_sub_step_up();
extern s32 draw2d3d();
extern s32 device_port_init2();
extern s32 get_CD_State2();
extern s32 cd_read_start2();
extern s32 class_switch();
extern s32 k_reset();
extern s32 decdct();
extern s32 dec_bg_reset();
extern s32 all_object_disp_off();
extern s32 taco_vram_clear();
extern s32 taco_mes_win_set();

s32 taco_cd_image_load()
{
    s32 ret;

    ret = step_mode;
    switch (step_mode) {
    case 0:
        device_port_init2(0, 513);
        all_object_disp_off();
        dec_bg_reset();
        cd_read_start2(9, 0x80162000, 0xB380);
        k_reset(0);
        taco_time_left = 0;
        class_switch(1);
        draw2d3d(1, 0);
        ret = taco_mes_win_set(1);
        step_mode = step_mode + 1;
        break;
    case 1:
        ret = get_CD_State2();
        if ((ret & 1) != 0) {
            decdct(704, 0, 320, 240);
            taco_vram_clear();
            ret = sub_sub_step_up();
        }
        break;
    }
    return ret;
}
