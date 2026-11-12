/* @SymbolStart: 0x80138170 */
/* @RodataBase: 0x8015B84C */
/* furyou_punch_miss_seq -- RPG_BAT overlay, Address: 0x80138178, Size: 432 bytes */
/* @SymbolSize: 436 */
/* @PostProc: patch_insn_reg_field 0x44:rt:v0:v1 0x48:rs:v0:v1 0x48:rt:v0:v1 0x50:rs:v0:v1 0x5c:rs:v0:v1 0x68:rs:v0:v1 0xdc:rt:v0:v1 0xe0:rs:v0:v1 0xe0:rt:v0:v1 0xf4:rs:v0:v1 0x118:rt:v0:v1 0x11c:rs:v0:v1 0x11c:rt:v0:v1 0x124:rs:v0:v1 0x130:rt:v0:v1 0x134:rs:v0:v1 0x134:rt:v0:v1 0x148:rs:v0:v1 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern void strcpy(u8 *, u8 *);
extern void rpg_bat_msg_set(u8 *, s32, s32);
extern void rpg_bat_anim_set2(s32, s32, s32, s32);
extern void rpg_bat_se_set(s32, s32, s32);
extern void rpg_bat_seq_step_next(void);
extern void rpg_bat_flag_seq(s32);
extern void rpg_bat_seq_step_gate(s32);
extern void rpg_bat_slot_set(s32, s32, s32);
extern void rpg_bat_seq_wait(s32);
extern void rpg_bat_effect_clear(void);

extern s32 rpg_bat_target_no;
extern u8  rpg_bat_msg_buf2[];
extern u8  furyou_attack3_str0[];
extern u8  furyou_attack3_str1[];
extern u8  furyou_attack3_str2[];
extern u8  furyou_attack3_str3[];
extern u8  rpg_bat_msg_buf2_1[];
extern u8  rpg_bat_msg_tbl2[];
extern RPG_BAT_UNIT rpg_bat_unit1;

void furyou_punch_miss_seq(void)
{
    s32 v1;

    switch (rpg_bat_unit1.step) {
    case 0:
        v1 = rpg_bat_target_no;
        switch (v1) {
        case 0:
            strcpy(rpg_bat_msg_buf2, furyou_attack3_str0);
            break;
        case 1:
            strcpy(rpg_bat_msg_buf2, furyou_attack3_str1);
            break;
        case 2:
            strcpy(rpg_bat_msg_buf2, furyou_attack3_str2);
            break;
        }
        strcpy(rpg_bat_msg_buf2_1, furyou_attack3_str3);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 2, 60);
        rpg_bat_anim_set2(rpg_bat_target_no + 59, 1, 5, 1);
        rpg_bat_se_set(1285, 1, 0);
        rpg_bat_seq_step_next();
        break;
    case 1:
        rpg_bat_flag_seq(rpg_bat_target_no + 59);
        break;
    case 2:
        rpg_bat_anim_set2(rpg_bat_target_no + 59, 0, 1, 1);
        rpg_bat_seq_step_next();
        break;
    case 3:
        rpg_bat_seq_step_gate(2);
        break;
    case 4:
        rpg_bat_slot_set(0, 260, 102);
        rpg_bat_seq_step_next();
        break;
    case 5:
        rpg_bat_seq_step_gate(0);
        break;
    case 6:
        rpg_bat_seq_wait(20);
        break;
    case 7:
        rpg_bat_effect_clear();
        break;
    }
}
