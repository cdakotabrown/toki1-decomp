/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x80149CBC */
/* @SymbolSize: 156 */
#include "types.h"

extern u8 step_mode;
extern s32 sub_sub_step_up();
extern s32 device_load_start();
extern s32 device_load_poll();
extern s32 device_port_init();

s32 etc_siken_sound_load()
{
    s32 ret;

    ret = step_mode;
    switch (step_mode) {
    case 0:
        ret = device_load_start(1, 0xBFA6, 0xBF87, 0xCB56, 0xCB0B, 0xCB05);
        step_mode = step_mode + 1;
        break;
    case 1:
        ret = device_load_poll();
        if (ret == 1) {
            device_port_init(1, 517);
            ret = sub_sub_step_up();
        }
        break;
    }
    return ret;
}
