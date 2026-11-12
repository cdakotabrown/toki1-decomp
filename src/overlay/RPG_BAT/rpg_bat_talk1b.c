/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x801480F0 */
/* @SymbolSize: 216 */
/* Address: 0x801480F8, Size: 216 bytes (54 insns) */
#include "types.h"

extern void strcpy(s32 a0, s32 a1);
extern s32 rpg_bat_koe_set(s32 a0, s32 a1, s32 a2);
extern void rpg_bat_talk_clear(void);
extern s32 rpg_bat_talk_wait(s32 a0);
extern s32 rpg_bat_msg_set(s32 a0, s32 a1, s32 a2);
extern s32 rpg_bat_act_next(void);

extern char rpg_bat_talk1b_str0[];
extern char rpg_bat_msg_buf3[];
extern char rpg_bat_msg_tbl3[];
extern s32 rpg_bat_talk_work_step;

s32 rpg_bat_talk1b(void)
{
    s32 ret;

    ret = rpg_bat_talk_work_step;
    switch (rpg_bat_talk_work_step) {
    case 0:
        strcpy((s32)rpg_bat_msg_buf3, (s32)rpg_bat_talk1b_str0);
        ret = rpg_bat_msg_set((s32)rpg_bat_msg_tbl3, 1, 170);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 1:
        ret = rpg_bat_koe_set(18, 0x04000019, 0);
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
