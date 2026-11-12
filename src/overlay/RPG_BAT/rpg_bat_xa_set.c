/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x8013D780 */
/* @SymbolSize: 180 */
#include "types.h"

extern u8 now_sub_sub_step;
extern s32 sub_step_up();
extern s32 sub_sub_step_up();
extern s32 device_cmd_send();
extern s32 get_CD_State();
extern s32 set_XAsector();
extern u8 now_girl;

s32 rpg_bat_xa_set()
{
    s32 ret;

    ret = now_sub_sub_step;
    switch (now_sub_sub_step) {
    case 0:
        now_girl = 14;
        set_XAsector(0x1C0000A8, 0x0F0000B7, 14);
        device_cmd_send(768);
        ret = sub_sub_step_up();
        break;
    case 1:
        device_cmd_send(228);
        ret = sub_sub_step_up();
        break;
    case 2:
        ret = get_CD_State();
        if ((ret & 0x4) != 0) {
            ret = sub_step_up();
        }
        break;
    }
    return ret;
}
