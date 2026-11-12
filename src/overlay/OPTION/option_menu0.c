/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x8013AD30 */
/* @SymbolSize: 132 */
#include "types.h"

extern s32 option_menu0_init();
extern s32 option_menu0_main1();
extern s32 option_cal_load();
extern s32 option_device_wait();
extern u8 now_sub_sub_step;

s32 option_menu0()
{
    s32 ret;

    ret = now_sub_sub_step;
    switch (now_sub_sub_step) {
    case 0:
        ret = option_menu0_init();
        break;
    case 1:
        ret = option_menu0_main1();
        break;
    case 254:
        ret = option_cal_load();
        break;
    case 255:
        ret = option_device_wait();
        break;
    }
    return ret;
}
