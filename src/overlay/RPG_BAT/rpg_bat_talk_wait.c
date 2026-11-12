/* @SymbolStart: 0x8013F250 */
/* @SymbolSize: 256 */
/* @CompileEnv: IDO52_AS1_NO_SCHED_LOAD_HOIST=strict:-0 */
/* Address: 0x8013F258 (true entry 0x8013F250: the rpg_bat_talk_work_busy read is */
/* Size: 248 bytes (62 insns) + 8 hoisted = 256 */
#include "types.h"

extern s32 rpg_bat_talk_wait_cnt;
extern s32 rpg_bat_talk_work_step;
extern s32 rpg_bat_talk_work_busy;
extern s32 rpg_bat_msg_ctl;
extern s32 rpg_bat_msg_ctl_mode;

extern s32 device_cmd_send();
extern s32 get_CD_State();

void rpg_bat_talk_wait(s32 a0)
{
    s32 v0;

    if (rpg_bat_talk_work_busy == 0) {
        rpg_bat_talk_wait_cnt++;
        v0 = get_CD_State();
        if (v0 == 4 || rpg_bat_talk_wait_cnt >= 61) {
            if (rpg_bat_talk_wait_cnt >= 61) {
                device_cmd_send(116);
            }
            rpg_bat_msg_ctl_mode = 4;
            rpg_bat_talk_work_busy = 1;
            rpg_bat_talk_wait_cnt = 0;
        }
    }

    if (rpg_bat_msg_ctl_mode == 0) {
        rpg_bat_talk_work_busy = 0;
        rpg_bat_talk_work_step++;
    }

    if (a0 != 0) {
        if ((rpg_bat_msg_ctl - 3) >= a0) {
            rpg_bat_talk_work_busy = 0;
            rpg_bat_talk_work_step++;
        }
    }
}
