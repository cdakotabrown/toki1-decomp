/* @CompileFlags: -Wc,-pic0 */
/* rpg_bat_icon_set - RPG_BAT overlay, Address: 0x8014AB84 */
/* @PostProc: remap_stack_slots --map '24:44,28:40,32:48,36:52' */
/* @PostProc: patch_insn_reg_field 0x38:rt:a3:t9 0x44:rt:a3:t9 0x54:rt:a3:t9 0x60:rd:t9:t1 0x60:rt:a3:t9 0x64:rt:a3:t9 0x68:rt:t9:t1 */
#include "types.h"

extern s32 rpg_bat_se_set2(s32, s32, s32);
extern s32 rpg_bat_sprite_pos_set(s32, s32, s32);
extern s32 rpg_bat_anim_set2(s32, s32, s32, s32);

extern u8 rpg_bat_target_no[];

void rpg_bat_icon_set(s32 idx, s32 arg1, s32 arg2)
{
    s32 j;
    s32 x4, x5, x6;

    rpg_bat_se_set2(1280, 1, 0);

    *(s32 *)&rpg_bat_target_no[idx * 4 + 200] = 1;
    j = idx * 3;
    *(s32 *)&rpg_bat_target_no[idx * 8 + 220] = arg1;
    *(s32 *)&rpg_bat_target_no[idx * 8 + 224] = arg2;

    x4 = j + 4;
    rpg_bat_sprite_pos_set(x4, arg1, arg2);
    x5 = j + 5;
    rpg_bat_sprite_pos_set(x5, arg1, arg2);
    x6 = j + 6;
    rpg_bat_sprite_pos_set(x6, arg1, arg2);

    rpg_bat_anim_set2(x4, 6, 5, 1);
    rpg_bat_anim_set2(x5, 8, 1, 1);
    rpg_bat_anim_set2(x6, 13, 5, 1);
}
