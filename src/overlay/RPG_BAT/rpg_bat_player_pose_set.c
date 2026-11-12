/* @SymbolStart: 0x8013D030 */
/* @SymbolSize: 164 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t7:t8 0x4c:rs:t7:t8 0x4c:rt:t8:t9 0x50:rs:t8:t9 0x80:rt:t9:t0 0x84:rs:t9:t0 0x84:rt:t9:t0 0x8c:rs:t9:t0 */
/* rpg_bat_player_pose_set - RPG_BAT overlay, Address: 0x8013D038, Size: 156 bytes (39 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit0;

extern void rpg_bat_anim_set(s32, s32, s32, s32);

void rpg_bat_player_pose_set(void)
{
    if (rpg_bat_unit0.hp == 0) {
        rpg_bat_anim_set(48, 7, 1, 1);
        return;
    }
    if (rpg_bat_unit0.flag & 0x100) {
        rpg_bat_anim_set(48, 3, 1, 1);
        return;
    }
    rpg_bat_anim_set(48, 0, 1, 1);
    if (rpg_bat_unit0.hp < 10) {
        rpg_bat_anim_set(48, 4, 1, 1);
    }
}
