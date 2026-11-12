/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:4 */
/* @SymbolStart: 0x8013F5C8 */
/* @SymbolSize: 204 */
#include "types.h"

extern s32 strcpy();
extern s32 rpg_bat_koe_set();
extern s32 rpg_bat_talk_wait();
extern s32 rpg_bat_msg_set();
extern char robo_drill_talk_str0[];
extern char rpg_bat_msg_buf2[];
extern char rpg_bat_msg_tbl2[];
extern s32 rpg_bat_talk_work_step[];

s32 robo_drill_talk()
{
    s32 ret;

    if (rpg_bat_talk_work_step[0] == 3) {
        return;
    }
    ret = rpg_bat_talk_work_step[0];
    switch (rpg_bat_talk_work_step[0]) {
    case 0:
        strcpy(rpg_bat_msg_buf2, robo_drill_talk_str0);
        ret = rpg_bat_msg_set(rpg_bat_msg_tbl2, 1, 200);
        rpg_bat_talk_work_step[0] = rpg_bat_talk_work_step[0] + 1;
        break;
    case 1:
        ret = rpg_bat_koe_set(0x1C000020, 0x05000029, 1);
        rpg_bat_talk_work_step[0] = rpg_bat_talk_work_step[0] + 1;
        break;
    case 2:
        ret = rpg_bat_talk_wait(0);
        break;
    case 3:
        break;
    }
    return ret;
}
