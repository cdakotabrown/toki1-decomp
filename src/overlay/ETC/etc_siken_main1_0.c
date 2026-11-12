/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x801498A8 */
/* @SymbolSize: 224 */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 k_sub_reset();
extern s32 k_sub_disp_start();
extern s32 x_taku_menu_set();
extern s32 x_taku_string_set();
extern s32 gnsx();
extern s32 sndisp();
extern s32 xa_wait();
extern char p_sei[];
extern u8 sinro;
extern u8 now_icon;
extern u8 step_mode;
extern char etc_siken_main1_0_str1[];
extern char etc_siken_main1_0_str2[];
extern char etc_siken_main1_0_str3[];
extern char etc_siken_main1_0_str0[];

s32 etc_siken_main1_0()
{
    s32 ret;

    ret = step_mode;
    switch (step_mode) {
    case 0:
        sinro = 0;
        k_sub_reset();
        gnsx(p_sei);
        ret = sndisp(etc_siken_main1_0_str0, 0, 31);
        step_mode = step_mode + 1;
        break;
    case 1:
        ret = xa_wait();
        break;
    case 2:
        k_sub_reset();
        x_taku_menu_set(0, 3);
        x_taku_string_set(etc_siken_main1_0_str1, etc_siken_main1_0_str2, etc_siken_main1_0_str3, 3);
        k_sub_disp_start(3);
        now_icon = 0;
        ret = sub_sub_step_up();
        break;
    }
    return ret;
}
