/* @SymbolStart: 0x80155A70 */
/* @SymbolSize: 588 */
/* @RodataBase: 0x8015DAFC */
/* Address: 0x80155A78 (window starts at the absorbed cross-prologue head */
/* Size: 588 bytes (147 insns) including the 2-insn head */
#include "types.h"

extern s32 get_CD_State();
extern void strcpy();
extern void rpg_bat_player_pose_set();
extern void rpg_bat_anim_set();
extern void rpg_bat_se_set();
extern void rpg_bat_koe_set();
extern void rpg_bat_msg_set();
extern void rpg_bat_act_next();
extern void rpg_bat_reset();
extern void rpg_bat_act_wait();

extern u32 rpg_bat_unit0;
extern u32 rpg_bat_unit0_flag;
extern u32 rpg_bat_unit0_hp_max;
extern u32 rpg_bat_unit1_flag;
extern u32 rpg_bat_unit2_flag;
extern u32 rpg_bat_unit2_step;
extern s32 rpg_bat_msg_time;
extern s32 rpg_bat_msg_ctl_cnt;
extern u8 bat_result_step_str0[];
extern u8 rpg_bat_msg_buf3[];
extern u8 rpg_bat_msg_tbl3[];

s32 bat_result_step(void)
{
    u32 flags;
    s32 elapsed;
    u32 step;

    if ((rpg_bat_unit0_flag & 0xFF) == 0 && (rpg_bat_unit1_flag & 0xFF) == 0) {
        flags = rpg_bat_unit2_flag;
        if ((flags & 0xFF) == 0 && (flags & 0x80000000) != 0) {
            elapsed = rpg_bat_unit0_hp_max;
            if (elapsed / 10 < elapsed - (s32)rpg_bat_unit0) {
                rpg_bat_player_pose_set();
                rpg_bat_unit2_flag = 1;
            }
        }
    }

    if (rpg_bat_unit2_flag == 1) {
        step = rpg_bat_unit2_step;
        switch (step) {
        case 0:
            rpg_bat_anim_set(29, 5, 1, 1);
            strcpy(rpg_bat_msg_buf3, bat_result_step_str0);
            rpg_bat_msg_set(rpg_bat_msg_tbl3, 1, 500);
            rpg_bat_koe_set(0x03000008, 0x02000010, 0);
            rpg_bat_act_next();
            break;
        case 1:
            rpg_bat_act_wait(70);
            break;
        case 2:
            rpg_bat_anim_set(29, 6, 1, 1);
            rpg_bat_se_set(1297, 1, 0);
            rpg_bat_act_next();
            break;
        case 3:
            if (get_CD_State() == 2) {
                rpg_bat_msg_ctl_cnt = rpg_bat_msg_time + 1;
                rpg_bat_act_next();
            }
            if (rpg_bat_msg_ctl_cnt >= 501) {
                rpg_bat_msg_ctl_cnt = rpg_bat_msg_time + 1;
                rpg_bat_act_next();
            }
            break;
        case 4:
            rpg_bat_act_wait(20);
            break;
        case 5:
            rpg_bat_anim_set(29, 7, 1, 1);
            rpg_bat_se_set(1298, 1, 0);
            rpg_bat_act_next();
            break;
        case 6:
            rpg_bat_act_wait(30);
            break;
        case 7:
            rpg_bat_reset();
            rpg_bat_unit2_flag = 4096;
            break;
        }
    }
}
