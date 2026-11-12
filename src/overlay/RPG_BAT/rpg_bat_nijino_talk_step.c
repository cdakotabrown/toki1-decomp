/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x80136D50 */
/* @SymbolSize: 220 */
#include "types.h"

extern s32 rpg_bat_talk_work_step;
extern u8 rpg_bat_msg_buf3[];
extern u8 rpg_bat_nijino_talk_step_str0[];
extern u8 rpg_bat_msg_tbl3[];
extern s32 strcpy();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_koe_set();
extern s32 rpg_bat_talk_wait();
extern s32 rpg_bat_talk_clear();
extern s32 rpg_bat_act_next();

s32 rpg_bat_nijino_talk_step()
{
    s32 ret;

    ret = rpg_bat_talk_work_step;
    switch (rpg_bat_talk_work_step) {
    case 0:
        strcpy(rpg_bat_msg_buf3, rpg_bat_nijino_talk_step_str0);
        ret = rpg_bat_msg_set(rpg_bat_msg_tbl3, 1, 130);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 1:
        ret = rpg_bat_koe_set(0x06000032, 0x08000034, 0);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 2:
        ret = rpg_bat_talk_wait(1);
        break;
    case 3:
        rpg_bat_talk_clear();
        ret = rpg_bat_act_next();
        break;
    }
    return ret;
}
