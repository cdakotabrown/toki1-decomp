/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x8013BE98 */
/* @SymbolSize: 84 */
#include "types.h"

extern s32 option_menu3_girl_init();
extern s32 option_menu3_main1();
extern u8 now_sub_sub_step;

s32 option_menu3(void)
{
    s32 ret;

    ret = now_sub_sub_step;
    switch (now_sub_sub_step) {
    case 0:
        ret = option_menu3_girl_init();
        break;
    case 1:
        ret = option_menu3_main1();
        break;
    }
    return ret;
}
