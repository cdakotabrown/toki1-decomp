/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x80137A70 */
/* @SymbolSize: 180 */
#include "types.h"

extern u8 now_sub_sub_step;
extern s32 pad_trig;
extern s32 goto_sub_step();
extern s32 device_cmd_send();
extern s32 taco_cd_image_load();
extern s32 taco_main7_main1();
extern s32 taco_main7_main2();
extern s32 taco_cmd_advance();

s32 taco_case7_cd_image_main()
{
    s32 ret;

    ret = now_sub_sub_step;
    switch (now_sub_sub_step) {
    case 0:
        ret = taco_cd_image_load();
        break;
    case 1:
        ret = taco_main7_main1();
        break;
    case 2:
        ret = taco_main7_main2();
        break;
    case 3:
        ret = taco_cmd_advance();
        break;
    }
    if ((pad_trig & 0x40) != 0) {
        device_cmd_send(116);
        device_cmd_send(177);
        ret = goto_sub_step(2);
    }
    return ret;
}
