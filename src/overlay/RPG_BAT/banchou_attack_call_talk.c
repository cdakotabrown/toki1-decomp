/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x80157AE0 */
/* @SymbolSize: 196 */
#include "types.h"

extern s32 strcpy();
extern s32 rpg_bat_koe_set();
extern s32 rpg_bat_talk_clear();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_seq_step_next();
extern u8 rpg_bat_talk4_str0[];
extern u8 rpg_bat_msg_buf2[];
extern u8 rpg_bat_msg_tbl2[];
extern s32 rpg_bat_talk_work_step;

s32 banchou_attack_call_talk()
{
    s32 ret;

    ret = rpg_bat_talk_work_step;
    switch (rpg_bat_talk_work_step) {
    case 0:
        strcpy(rpg_bat_msg_buf2, rpg_bat_talk4_str0);
        ret = rpg_bat_msg_set(rpg_bat_msg_tbl2, 1, 120);
        rpg_bat_talk_work_step += 1;
        break;
    case 1:
        ret = rpg_bat_koe_set(0x180000A6, 0x0B0000B0, 0);
        rpg_bat_talk_work_step += 1;
        break;
    case 2:
        rpg_bat_talk_clear();
        ret = rpg_bat_seq_step_next();
        break;
    }
    return ret;
}
