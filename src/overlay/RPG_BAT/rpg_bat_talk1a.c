/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x8013F4F0 */
/* @SymbolSize: 216 */
/* Address: 0x8013F4F8, Size: 216 bytes (54 insns w/ 2-insn cross-prologue tail) */
#include "types.h"

extern void strcpy(s32 a0, s32 a1);
extern s32 rpg_bat_koe_set(s32 a0, s32 a1, s32 a2);
extern void rpg_bat_talk_clear(void);
extern s32 rpg_bat_talk_wait(s32 a0);
extern s32 rpg_bat_msg_set(s32 a0, s32 a1, s32 a2);
extern s32 rpg_bat_seq_step_next(void);

extern char rpg_bat_talk1a_str0[];
extern char rpg_bat_msg_buf2[];
extern char rpg_bat_msg_tbl2[];
extern s32 rpg_bat_talk_work_step;

s32 rpg_bat_talk1a(void)
{
    s32 ret;

    ret = rpg_bat_talk_work_step;
    switch (rpg_bat_talk_work_step) {
    case 0:
        strcpy((s32)rpg_bat_msg_buf2, (s32)rpg_bat_talk1a_str0);
        ret = rpg_bat_msg_set((s32)rpg_bat_msg_tbl2, 1, 240);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 1:
        ret = rpg_bat_koe_set(26, 0x05000026, 0);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 2:
        ret = rpg_bat_talk_wait(0);
        break;
    case 3:
        rpg_bat_talk_clear();
        ret = rpg_bat_seq_step_next();
        break;
    }
    return ret;
}
