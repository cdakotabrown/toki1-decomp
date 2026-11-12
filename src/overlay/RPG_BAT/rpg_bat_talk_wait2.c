/* Address: 0x8013F350 */
/* Size: 160 bytes */
/* @SymbolSize: 160 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x54 */
#include "types.h"

extern volatile s32 rpg_bat_talk_work_busy;
extern s32 rpg_bat_msg_time;
extern s32 rpg_bat_msg_ctl_mode;
extern s32 rpg_bat_talk_work_step;
extern s32 rpg_bat_msg_ctl;

s32 rpg_bat_talk_wait2(s32 a0, s32 a1)
{
    s32 old;

    old = rpg_bat_talk_work_busy;
    if (old == 0) {
        old = rpg_bat_msg_time;
        rpg_bat_msg_time = a1;
    }
    if (rpg_bat_msg_ctl_mode == 0) {
        rpg_bat_msg_time = old;
        rpg_bat_talk_work_step++;
        old = 0;
    }
    rpg_bat_talk_work_busy = old;
    if (a0 != 0) {
        rpg_bat_talk_work_busy = old;
        if (rpg_bat_msg_ctl - 3 >= a0) {
            rpg_bat_msg_time = old;
            rpg_bat_talk_work_step++;
            old = 0;
            rpg_bat_talk_work_busy = 0;
        }
    }
    return old;
}
