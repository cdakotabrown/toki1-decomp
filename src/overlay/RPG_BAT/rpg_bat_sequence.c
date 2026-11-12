/* @RodataBase: 0x8015C0F8 */
/* @CompileFlags: -Wo,-loopunroll,4 */
/* Address: 0x80140204 */
/* Size: 2132 bytes (533 insns)  -- BYTE-PERFECT */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

typedef struct {
    u8 pad0[2];
    u8 flags;
    u8 pad1[35];
    s16 field38;
    u8 pad2[2];
    s16 counter;
    u8 pad3[24];
} BatEntry;

extern BatEntry class_w[];

extern void rpg_bat_player_pose_set(void);
extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern void rpg_bat_anim_set2(s32, s32, s32, s32);
extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern s32 get_rand_mod(s32);
extern void rpg_bat_se_set(s32, s32, s32);
extern void rpg_bat_ent_nudge(s32);
extern void rpg_bat_seq_step_next(void);
extern void rpg_bat_substep_next(void);
extern void rpg_bat_substep_wait(s32);
extern void rpg_bat_flag_substep(s32);

extern s32 rpg_bat_target_no[];
extern s32 rpg_bat_seq_work[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;


void rpg_bat_sequence(void)
{
    s32 *sp;
    s32 i;
    s32 j;
    s32 v0;

    switch (rpg_bat_unit1.sub) {
    case 0:
        rpg_bat_anim_set2(59, 10, 5, 1);
        rpg_bat_se_set(1286, 1, 0);
        rpg_bat_substep_next();
        break;
    case 1:
        rpg_bat_flag_substep(59);
        break;
    case 2:
        ((CLASS_W *)class_w)[155].anim_flag = 64;
        ((CLASS_W *)class_w)[155].frame = 25;
        rpg_bat_sprite_pos_set(57, 96, 120);
        rpg_bat_anim_set2(57, 12, 1, 1);
        rpg_bat_sprite_pos_set(56, 0, 0);
        rpg_bat_anim_set(56, 9, 5, 1);
        rpg_bat_substep_next();
        break;
    case 3:
        rpg_bat_sprite_pos_set(57, rpg_bat_unit1.wait + 98, 118 - rpg_bat_unit1.wait);
        rpg_bat_unit1.wait += 2;
        if ((((CLASS_W *)class_w)[152].anim_flag & 1) == 0) {
            rpg_bat_anim_set(56, 255, 1, 0);
        }
        if (rpg_bat_unit1.wait + 90 < 119) {
            break;
        }
        rpg_bat_anim_set(56, 255, 1, 0);
        rpg_bat_anim_set2(57, 13, 5, 1);
        ((CLASS_W *)class_w)[153].prio = 2;
        rpg_bat_substep_next();
        break;
    case 4:
        for (i = 0; i != 5; i++) {
            j = i + 4;
            rpg_bat_sprite_pos_set(j, 128, 88);
            rpg_bat_anim_set2(j, 20, 1, 1);
        }
        rpg_bat_substep_next();
        break;
    case 5:
        v0 = rpg_bat_unit1.wait % 2;
        if (v0 == 0) {
            class_w[101].field38 -= 1;
            class_w[103].field38 -= 2;
            class_w[102].counter += 1;
            class_w[104].counter += 2;
        }
        if (v0 == 0) {
            for (i = 0; i < 5; i++) {
                class_w[i + 100].field38 += 1;
                class_w[i + 100].counter -= 1;
            }
        }
        rpg_bat_unit1.wait += 1;
        if (rpg_bat_unit1.wait < 33) {
            break;
        }
        rpg_bat_substep_next();
        break;
    case 6:
        if (rpg_bat_unit1.wait >= 27) {
            rpg_bat_se_set(1287, 0, 1);
            class_w[100].field38 += 3;
            class_w[100].counter -= 3;
        } else if ((rpg_bat_unit1.wait & 1) == 0) {
            rpg_bat_ent_nudge(0);
        }
        if (rpg_bat_unit1.wait >= 14) {
            rpg_bat_se_set(1287, 0, 2);
            class_w[101].field38 += 3;
            class_w[101].counter -= 3;
        } else if ((rpg_bat_unit1.wait & 1) == 0) {
            rpg_bat_ent_nudge(1);
        }
        if (rpg_bat_unit1.wait >= 14) {
            class_w[102].field38 += 3;
            class_w[102].counter -= 3;
        } else if ((rpg_bat_unit1.wait & 1) == 0) {
            rpg_bat_ent_nudge(2);
        }
        if (rpg_bat_unit1.wait > 0) {
            rpg_bat_se_set(1287, 0, 3);
            class_w[103].field38 += 3;
            class_w[103].counter -= 3;
        }
        if (rpg_bat_unit1.wait > 0) {
            class_w[104].field38 += 3;
            class_w[104].counter -= 3;
        }
        rpg_bat_unit1.wait += 1;
        if (rpg_bat_unit1.wait < 81) {
            break;
        }
        rpg_bat_substep_next();
        break;
    case 7:
        rpg_bat_substep_wait(30);
        break;
    case 8:
        rpg_bat_unit1.wait += 1;
        rpg_bat_seq_work[40] += 1;
        sp = rpg_bat_target_no;
        for (i = 0; i != 12; i++) {
            if (sp[50] == 3 && (rpg_bat_unit0.flag & 0x100) == 0) {
                rpg_bat_anim_set(48, 5, 1, 1);
            }
            if (rpg_bat_unit1.wait < 120 && rpg_bat_seq_work[40] >= 3 && sp[50] == 0 &&
                get_rand_mod(5) == 0) {
                rpg_bat_se_set(1288, 1, 0);
                rpg_bat_seq_work[40] = 0;
                if (get_rand_mod(100) < 30) {
                    sp[50] = 1;
                    v0 = get_rand_mod(24);
                    rpg_bat_sprite_pos_set(i + 4, v0 + 260, 0);
                    rpg_bat_anim_set2(i + 4, 22, 1, 1);
                } else {
                    sp[50] = 2;
                    v0 = get_rand_mod(64);
                    rpg_bat_sprite_pos_set(i + 4, v0 + 240, 0);
                    rpg_bat_anim_set2(i + 4, 22, 1, 1);
                }
            }
            if (sp[50] == 1) {
                class_w[i + 100].counter += 5;
                if (get_rand_mod(4) - 34 < class_w[i + 100].counter) {
                    rpg_bat_anim_set2(i + 4, 14, 5, 1);
                    sp[50] = 3;
                    rpg_bat_se_set(1289, 1, 0);
                }
            }
            if (sp[50] == 2) {
                class_w[i + 100].counter += 5;
                if (get_rand_mod(4) + 8 < class_w[i + 100].counter) {
                    rpg_bat_anim_set2(i + 4, 14, 5, 1);
                    sp[50] = 3;
                    rpg_bat_se_set(1289, 1, 0);
                }
            }
            if (sp[50] == 3) {
                if ((class_w[i + 100].flags & 1) == 0) {
                    rpg_bat_anim_set2(i + 4, 255, 1, 0);
                    sp[50] = 0;
                }
            }
            sp++;
        }
        if (rpg_bat_unit1.wait < 221) {
            break;
        }
        rpg_bat_player_pose_set();
        rpg_bat_substep_next();
        break;
    case 9:
        rpg_bat_anim_set2(59, 11, 5, 1);
        for (i = 0; i != 12; i++) {
            rpg_bat_anim_set2(i + 4, 255, 1, 0);
        }
        rpg_bat_substep_next();
        break;
    case 10:
        rpg_bat_flag_substep(59);
        break;
    case 11:
        rpg_bat_seq_step_next();
        break;
    }
}
