/* @SymbolStart: 0x8013AD50 */
/* @SymbolSize: 460 */
/* @RodataBase: 0x8015BB54 */
/* @PostProc: swap_instructions --offset1 0xe4 --offset2 0xe8 */
/* @PostProc: patch_insn_reg_field 0x2c:rt:t8:t9 0x34:rs:t8:t9 0x34:rt:t9:t0 0x38:rs:t9:t0 0x5c:rt:t0:t1 0x60:rs:t0:t1 0x60:rt:t0:t1 0x68:rs:t0:t1 0x68:rt:t1:t2 0x6c:rt:t1:t2 0x70:rt:t2:t3 0x74:rs:t2:t3 0x74:rt:t2:t3 0x7c:rs:t2:t3 0x84:rd:t2:t3 0x84:rt:t2:t3 0x8c:rt:t2:t3 0x90:rt:t2:t3 0x98:rs:t2:t3 0xa0:rt:t3:t4 0xa4:rs:t3:t4 0xa4:rt:t3:t4 0xa8:rt:t5:t6 0xac:rs:t3:t4 0xac:rt:t4:t5 0xb0:rs:t4:t5 0xb8:rs:t5:t6 0xb8:rt:t5:t6 0xbc:rt:t7:t8 0xc0:rs:t5:t6 0xc0:rt:t6:t7 0xc4:rs:t6:t7 0xcc:rs:t7:t8 0xcc:rt:t7:t8 0xd0:rt:t8:t9 0xd4:rs:t7:t8 0xe4:rt:t8:t9 0x108:rd:t9:t0 0x10c:rt:t9:t0 0x110:rs:t9:t0 */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
/* rpg_bat_reward_step - RPG_BAT overlay, Address: 0x8013AD58, Size: 452 bytes */
/* `@CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all` (Hook 27) then restores the */
#include "types.h"

extern void strcpy(u8 *, u8 *);
extern void strcat(u8 *, u8 *);
extern s32  get_rpg_bat_turn_wait(s32);
extern void rpg_bat_player_pose_set(void);
extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern void rpg_bat_slot_adjust(s32, s32, s32, s32);
extern void rpg_bat_se_set(s32, s32, s32);
extern void rpg_bat_msg_set(u8 *, s32, s32);
extern void rpg_bat_step_next(void);
extern void rpg_bat_clear(void);
extern void rpg_bat_timer_step(s32);
extern void rpg_bat_step_gate(s32);

extern u8  p_sei[];
extern u8  rpg_bat_reward_step_str0[];
extern u8  rpg_bat_msg_buf[];
extern u8  rpg_bat_msg_tbl[];
extern s32 rpg_bat_unit0_flag;
extern s32 rpg_bat_unit0_aux08;
extern s32 rpg_bat_unit0_hp_max;
extern s32 rpg_bat_unit0_aux10;
extern volatile s32 rpg_bat_unit0_step;
extern volatile s32 rpg_bat_unit0_cnt;
extern s32 rpg_bat_unit1_flag;
extern s32 rpg_bat_unit2_flag;

void rpg_bat_reward_step(void)
{
    if ((rpg_bat_unit2_flag & 0xFF) == 0) {
        if ((rpg_bat_unit0_flag & 0x100) != 0) {
            rpg_bat_anim_set(48, 3, 1, 1);
        } else {
            rpg_bat_player_pose_set();
        }
    }

    rpg_bat_unit0_cnt = rpg_bat_unit0_cnt + 1;

    switch (rpg_bat_unit0_step) {
    case 0:
        if ((rpg_bat_unit1_flag & 0xFF) != 0) {
            break;
        }
        if ((rpg_bat_unit2_flag & 0xFF) != 0) {
            break;
        }
        if (rpg_bat_unit0_cnt < 151) {
            break;
        }
        rpg_bat_unit0_flag = 260;
        rpg_bat_unit0_aux10 = rpg_bat_unit0_hp_max / 20;
        if (rpg_bat_unit0_aux10 == 0) {
            rpg_bat_unit0_aux10 = 1;
        }
        strcpy(rpg_bat_msg_buf, p_sei);
        strcat(rpg_bat_msg_buf, rpg_bat_reward_step_str0);
        rpg_bat_msg_set(rpg_bat_msg_tbl, 1, 60);
        rpg_bat_step_next();
        break;
    case 1:
        rpg_bat_step_gate(1);
        break;
    case 2:
        rpg_bat_slot_adjust(0, 33, rpg_bat_unit0_aux10, 0);
        rpg_bat_se_set(1282, 1, 0);
        rpg_bat_step_next();
        break;
    case 3:
        rpg_bat_timer_step(20);
        break;
    case 4:
        rpg_bat_clear();
        rpg_bat_unit0_aux08 = get_rpg_bat_turn_wait(1);
        break;
    }
}
