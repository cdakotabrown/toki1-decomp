/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
/* @SymbolSize: 928 */
/* rpg_bat_phase_run - RPG_BAT overlay, Address: 0x8013CC90, Size: 928 bytes */
/* @PostProc: patch_insn_reg_field 0x3c:rt:v0:t9 0x60:rt:t0:t1 0x70:rs:t0:t1 0x70:rt:t0:t1 0x74:rt:t2:t3 0x78:rs:t0:t1 0x78:rt:t1:t2 0x7c:rs:t1:t2 0x84:rs:t2:t3 0x84:rt:t2:t3 0x88:rt:t4:t5 0x90:rs:t3:t4 0xc8:rd:t7:t8 0xd0:rs:t7:t8 0xd4:rt:t8:t0 0xdc:rs:t7:t8 0xe0:rt:t3:t4 0xe8:rs:t7:t8 0x104:rt:t9:t1 0x108:rs:t9:t1 0x108:rt:t9:t1 0x10c:rt:t8:t0 0x114:rs:t9:t1 0x118:rt:t0:t2 0x11c:rs:t0:t2 0x11c:rt:t0:t2 0x124:rs:t0:t2 0x124:rt:t1:t3 0x128:rt:t1:t3 0x134:rt:t2:v1 0x138:rs:t2:v1 0x138:rt:t2:v1 0x140:rs:t2:v1 0x150:rt:t3:t4 0x15c:rt:t4:v1 0x160:rs:t4:v1 0x160:rt:t4:v1 0x168:rs:t4:v1 0x1c0:rt:t7:t9 0x1c8:rt:t7:t9 0x1d8:rt:t8:t7 0x1dc:rs:t8:t7 0x1e0:rs:t8:t7 0x1e8:rs:t8:t7 0x1f0:rs:t8:t7 0x1f8:rs:t8:t7 0x21c:rt:t9:t8 0x224:rs:t9:t8 0x2a4:rt:t7:t9 0x2b4:rs:t7:t9 0x2b4:rt:t7:t9 0x2bc:rs:t7:t9 0x2bc:rt:t8:t7 0x2c0:rs:t8:t7 0x2d0:rt:t9:t8 0x2d4:rs:t9:t8 0x33c:rt:t8:t7 0x340:rt:t7:t9 0x344:rs:t7:t9 0x34c:rs:t8:t7 0x34c:rt:t8:t7 0x354:rd:t9:t8 0x354:rs:t8:t7 0x358:rs:t9:t8 0x8c:rs:t2:t3 0x8c:rt:t3:t4 */
/* @PostProc: patch_insn_reg_field 0x5c:rs:v0:t9 0x5c:rt:v0:t9 0x64:rs:v0:t9 0x64:rt:t9:t0 0x68:rs:t9:t0 0x98:rs:t4:t5 0x98:rt:t4:t5 0x9c:rd:t6:t7 0x9c:rt:v0:t9 0xa0:rs:t4:t5 0xa0:rt:t5:t6 0xa4:rs:t5:t6 0xac:rs:t6:t7 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern void rpg_bat_reward_step();
extern void rpg_bat_player_attack();
extern void rpg_bat_player_pose_set();
extern void rpg_bat_bukatu_attack_sel();
extern void rpg_bat_anim_set();
extern s32 get_rand_mod();
extern s32 get_rpg_bat_turn_wait();
extern void rpg_bat_menu();

extern s32 rpg_bat_mode;
extern s32 rpg_bat_bukatu_flag;
extern s32 rpg_bat_bukatu_ready;
extern s32 rpg_bat_bukatu_gauge;
extern volatile s32 rpg_bat_unit0_flag;
extern s32 pad_data;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit2;
extern RPG_BAT_UNIT rpg_bat_unit3;

void rpg_bat_phase_run(void)
{
    s32 v1;
    s32 v0;
    s32 mode;
    s32 hi;

    rpg_bat_menu();
    if (get_rpg_bat_turn_wait(0) < rpg_bat_unit0.aux08) {
        rpg_bat_unit0_flag |= 0x10000;
        rpg_bat_unit0.aux08 = 0;
    }

    mode = rpg_bat_unit0_flag;
    hi = (s32)((u32)mode >> 24);
    if (((mode & 0xFF) == 0) && ((rpg_bat_unit1.flag & 0xFF) == 0) &&
        ((rpg_bat_unit3.flag & 0xFF) == 0) && ((rpg_bat_unit2.flag & 0xFF) == 0) &&
        (hi != 0)) {
        rpg_bat_player_pose_set();
        v1 = rpg_bat_unit0_flag & 0xFF000000;
        switch ((u32)v1) {
        case 0x01000000:
            rpg_bat_unit0_flag = 1;
            if (rpg_bat_mode == (s32)0x80000000) {
                rpg_bat_unit2.aux14 = rpg_bat_unit2.aux14 + 600;
            }
            v0 = get_rand_mod(15);
            v1 = rpg_bat_bukatu_gauge;
            v1 = v1 + v0;
            v1 = v1 + 30;
            break;
        case 0x02000000:
            rpg_bat_unit0_flag = 256;
            v0 = get_rand_mod(15);
            v1 = rpg_bat_bukatu_gauge;
            v1 = v1 + v0;
            v1 = v1 + 20;
            break;
        case 0x04000000:
            v1 = rpg_bat_bukatu_gauge;
            v1 = v1 - 100;
            rpg_bat_unit0_flag = 2;
            rpg_bat_bukatu_ready = 0;
            break;
        default:
            v1 = rpg_bat_bukatu_gauge;
            break;
        }
        if (v1 >= 171) {
            v1 = 170;
        }
        rpg_bat_bukatu_gauge = v1;
        if (v1 >= 100) {
            if (rpg_bat_bukatu_flag == 1) {
                rpg_bat_bukatu_ready = 1;
            }
        }
    }

    v1 = rpg_bat_unit0_flag & 0x1FF;
    switch ((u32)v1) {
    case 256:
        rpg_bat_reward_step();
        v1 = rpg_bat_unit0_flag & 0x1FF;
        break;
    case 260:
        rpg_bat_reward_step();
        v1 = rpg_bat_unit0_flag & 0x1FF;
        break;
    case 1:
        rpg_bat_player_attack();
        v1 = rpg_bat_unit0_flag & 0x1FF;
        break;
    case 2:
        rpg_bat_bukatu_attack_sel();
        v1 = rpg_bat_unit0_flag & 0x1FF;
        break;
    }

    if ((v1 == 0) && ((rpg_bat_unit1.flag & 0xFF) == 0) &&
        ((rpg_bat_unit2.flag & 0xFF) == 0) && ((rpg_bat_unit3.flag & 0xFF) == 0)) {
        v0 = pad_data;
        if (((v0 & 4) == 0) || ((v0 & 8) == 0) ||
            ((rpg_bat_mode & 0xF0000) == 0)) {
            rpg_bat_player_pose_set();
            v1 = rpg_bat_unit0_flag & 0x1FF;
        }
    }

    v0 = pad_data;
    if ((v1 == 0) && ((rpg_bat_unit1.flag & 0xFF) == 0) &&
        ((rpg_bat_unit3.flag & 0xFF) == 0) && ((rpg_bat_mode & 0xF0000) != 0)) {
        if (((v0 & 4) != 0) && ((v0 & 8) != 0)) {
            if (class_w[144].anim != 2) {
                rpg_bat_anim_set(48, 2, 1, 1);
            }
        } else {
            rpg_bat_player_pose_set();
        }
    }
}
