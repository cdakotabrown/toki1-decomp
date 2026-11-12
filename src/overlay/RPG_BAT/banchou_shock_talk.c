/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x80137E74 */
/* @SymbolSize: 220 */
#include "types.h"

extern void strcpy(s32 a0, s32 a1);
extern s32 rpg_bat_koe_set(s32 a0, s32 a1, s32 a2);
extern void rpg_bat_talk_clear(void);
extern s32 rpg_bat_talk_wait(s32 a0);
extern s32 rpg_bat_msg_set(s32 a0, s32 a1, s32 a2);
extern s32 rpg_bat_seq_next(void);

extern char banchou_shock_talk_str0[];
extern char rpg_bat_msg_buf4[];
extern char rpg_bat_msg_tbl4[];
extern s32 rpg_bat_talk_work_step;

s32 banchou_shock_talk(void)
{
    s32 ret;

    ret = rpg_bat_talk_work_step;
    switch (rpg_bat_talk_work_step) {
    case 0:
        strcpy((s32)rpg_bat_msg_buf4, (s32)banchou_shock_talk_str0);
        ret = rpg_bat_msg_set((s32)rpg_bat_msg_tbl4, 1, 160);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 1:
        ret = rpg_bat_koe_set(0x1A000074, 0x1100007A, 0);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 2:
        ret = rpg_bat_talk_wait(0);
        break;
    case 3:
        rpg_bat_talk_clear();
        ret = rpg_bat_seq_next();
        break;
    }
    return ret;
}
