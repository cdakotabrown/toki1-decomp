/* @SymbolStart: 0x801515F0 */
/* @SymbolSize: 244 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:7 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:v0 0x14:rs:v1:v0 0x14:rt:v1:v0 0x1c:rs:v1:v0 */
/* rpg_bat_enemy_pose_set - RPG_BAT overlay, Address: 0x80151600, Size: 228 bytes (57 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern s32 rpg_bat_anim_set2(s32, s32, s32, s32);
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit1;

s32 rpg_bat_enemy_pose_set(void)
{
    s32 sel;
    s32 ret;

    sel = rpg_bat_unit1.flag & 0xFFFF0000;
    ret = sel;
    switch ((u32)sel) {
    case 0x100000: goto L1;
    case 0x200000: goto L2;
    case 0x400000: goto L3;
    }
    goto Lend;
L1:
    rpg_bat_anim_set2(53, 9, 1, 0);
    rpg_bat_anim_set2(54, 9, 1, 0);
    ret = rpg_bat_anim_set2(55, 9, 1, 0);
    class_w[155].bright = 128;
    class_w[156].bright = 128;
    class_w[157].bright = 128;
    goto Lend;
L2:
    ret = rpg_bat_anim_set2(53, 9, 1, 0);
    class_w[155].bright = 128;
    goto Lend;
L3:
    ret = rpg_bat_anim_set2(53, 9, 1, 0);
    class_w[155].bright = 128;
Lend:
    return ret;
}
