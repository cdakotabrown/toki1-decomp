/* Address: 0x8014F790 (absorbed cross-prologue head) .. 0x8014F88C */
/* Size: 252 bytes (63 instructions) */
/* @SymbolStart: 0x8014F790 */
/* @SymbolSize: 252 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:5 */
#include "types.h"

extern void strcpy();
extern void rpg_bat_anim_set();
extern s32  rpg_bat_koe_set();
extern void rpg_bat_talk_clear();
extern s32  rpg_bat_talk_wait();
extern s32  rpg_bat_msg_set();
extern s32  rpg_bat_act_next();

extern u8 rpg_bat_msg_buf4[];
extern u8 himo_robo_lose_talk_str0[];
extern u8 rpg_bat_msg_tbl4[];
extern s32 rpg_bat_talk_work_step;

s32 himo_robo_lose_talk(void)
{
    s32 r;
    s32 sel;

    sel = rpg_bat_talk_work_step;
    r = sel;
    switch (sel) {
    case 0:
        strcpy(rpg_bat_msg_buf4, himo_robo_lose_talk_str0);
        rpg_bat_anim_set(29, 25, 1, 1);
        r = rpg_bat_msg_set(rpg_bat_msg_tbl4, 1, 200);
        rpg_bat_talk_work_step++;
        break;
    case 1:
        r = rpg_bat_koe_set(56, 0x09000041, 0);
        rpg_bat_talk_work_step++;
        break;
    case 2:
        r = rpg_bat_talk_wait(0);
        break;
    case 3:
        rpg_bat_anim_set(29, 24, 1, 1);
        rpg_bat_talk_clear();
        r = rpg_bat_act_next();
        break;
    }
    return r;
}
