/* @SymbolStart: 0x8013FF28 */
/* @RodataBase: 0x8015C0C8 */
/* @SymbolSize: 680 */
/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: patch_insn_reg_field 0x120:rs:v0:t7 0x120:rt:v0:t7 0x128:rs:v0:t7 0x128:rt:v0:t8 0x12c:rt:v0:t8 0x130:rs:v0:t8 0x138:rt:v0:t9 0x13c:rs:t7:t9 0x13c:rt:t7:t9 0x144:rs:t7:t9 0x144:rt:t8:t0 0x148:rs:t8:t0 0x15c:rt:t9:t1 0x160:rs:t9:t1 0x160:rt:t9:t1 0x168:rs:t9:t1 0x190:rt:t0:t2 0x194:rs:t0:t2 0x194:rt:t0:t2 0x198:rt:t1:t3 0x1a8:rs:t1:t3 0x1a8:rt:t0:t2 0x1bc:rt:t2:t4 0x1c0:rd:t3:t5 0x1c0:rs:t2:t4 0x1c8:rs:t3:t5 */
/* rpg_bat_slide_substep - RPG_BAT overlay, Address: 0x8013FF30, Size: 672 bytes (168 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"


extern s32 rpg_bat_player_pose_set();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_se_set();
extern s32 rpg_bat_seq_step_next();
extern s32 rpg_bat_substep_next();
extern s32 rpg_bat_substep_wait();
extern s32 rpg_bat_flag_substep();

extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

void rpg_bat_slide_substep(void)
{
    s32 t;
    s32 v0;

    switch ((u32)rpg_bat_unit1.sub) {
    case 0:
        rpg_bat_anim_set2(59, 4, 1, 1);
        rpg_bat_substep_next();
        break;
    case 1:
        rpg_bat_substep_wait(7);
        break;
    case 2:
        rpg_bat_anim_set2(59, 3, 1, 1);
        rpg_bat_sprite_pos_set(58, 112, 72);
        rpg_bat_anim_set2(58, 5, 5, 1);
        rpg_bat_substep_next();
        break;
    case 3:
        rpg_bat_flag_substep(58);
        break;
    case 4:
        rpg_bat_anim_set2(58, 6, 1, 1);
        rpg_bat_se_set(1285, 1, 0);
        rpg_bat_substep_next();
        break;
    case 5:
        rpg_bat_sprite_pos_set(58, rpg_bat_unit1.wait + 112, 72);
        t = rpg_bat_unit1.wait + 8;
        rpg_bat_unit1.wait = t;
        if (t >= 145) {
            if ((rpg_bat_unit0.flag & 0x100) == 0)
                rpg_bat_anim_set(48, 10, 1, 1);
        }
        if (rpg_bat_unit1.wait < 297)
            return;
        rpg_bat_anim_set2(58, 7, 1, 1);
        rpg_bat_substep_next();
        break;
    case 6:
        rpg_bat_sprite_pos_set(58, 408 - rpg_bat_unit1.wait, 72);
        v0 = rpg_bat_unit1.wait + 8;
        rpg_bat_unit1.wait = v0;
        if (408 - v0 >= 112)
            return;
        rpg_bat_player_pose_set();
        rpg_bat_substep_next();
        break;
    case 7:
        rpg_bat_sprite_pos_set(57, 72, 96);
        rpg_bat_anim_set2(57, 8, 1, 1);
        rpg_bat_substep_next();
        break;
    case 8:
        rpg_bat_substep_wait(11);
        break;
    case 9:
        rpg_bat_anim_set2(59, 4, 1, 1);
        rpg_bat_anim_set2(58, 255, 1, 0);
        rpg_bat_anim_set(57, 255, 1, 0);
        rpg_bat_substep_next();
        break;
    case 10:
        rpg_bat_substep_wait(7);
        break;
    case 11:
        rpg_bat_anim_set2(59, 2, 1, 1);
        rpg_bat_seq_step_next();
        break;
    }
}
