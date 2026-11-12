/* @SymbolStart: 0x8013FD8C */
/* Address: 0x8013FD94 */
/* Size: 404 bytes (101 insns) */
/* @SymbolSize: 412 */
/* @PostProc: patch_insn_reg_field 0x160:rt:v1:v0 0x164:rs:v1:v0 0x164:rt:v1:v0 0x16c:rs:v1:v0 0x174:rd:v1:t9 0x174:rs:v1:v0 0x178:rt:t9:t0 0x180:rt:t9:t0 0x188:rt:v1:t9 */
/* @RodataBase: 0x8015C094 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"


extern void rpg_bat_damage_set(void);
extern void rpg_bat_effect_rec_set(s32);
extern void rpg_bat_seq_step_next(void);
extern void robo_drill_talk(void);
extern void rpg_bat_slide_substep(void);
extern void rpg_bat_seq_wait(s32);
extern void rpg_bat_talk_clear(void);
extern void rpg_bat_talk1d(void);
extern void rpg_bat_sequence(void);
extern void rpg_bat_talk1e(void);
extern void rpg_bat_anim_substep(void);
extern void rpg_bat_slot_adjust(s32, s32, s32, s32);
extern void rpg_bat_player_pose_set(void);
extern void rpg_bat_effect_clear(void);

extern s32 rpg_bat_dmg;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

void robo_attack(void) {
    s32 n;

    switch (rpg_bat_unit1.step) {
    case 0:
        rpg_bat_damage_set();
        class_w[144].prio = 6;
        rpg_bat_effect_rec_set(4);
        rpg_bat_seq_step_next();
        break;
    case 1:
        robo_drill_talk();
        rpg_bat_slide_substep();
        break;
    case 2:
        rpg_bat_seq_wait(60);
        break;
    case 3:
        rpg_bat_talk_clear();
        rpg_bat_seq_step_next();
        break;
    case 4:
        rpg_bat_talk1d();
        rpg_bat_sequence();
        break;
    case 5:
        rpg_bat_talk_clear();
        rpg_bat_seq_step_next();
        break;
    case 6:
        rpg_bat_talk1e();
        rpg_bat_anim_substep();
        break;
    case 7:
        rpg_bat_talk_clear();
        rpg_bat_seq_step_next();
        break;
    case 8:
        rpg_bat_slot_adjust(0, 33, rpg_bat_dmg, 1);
        rpg_bat_seq_step_next();
        break;
    case 9:
        rpg_bat_seq_wait(30);
        break;
    case 10:
        rpg_bat_player_pose_set();

    case 11:
        rpg_bat_seq_wait(20);
        break;
    case 12:
        n = rpg_bat_unit0.flag;
        if (n & 0x1000) {
            rpg_bat_unit0.flag = n & ~0x1000;
            rpg_bat_unit0.aux08 = 0xFFFF;
        }
        rpg_bat_effect_rec_set(255);
        rpg_bat_effect_clear();
        break;
    }
}
