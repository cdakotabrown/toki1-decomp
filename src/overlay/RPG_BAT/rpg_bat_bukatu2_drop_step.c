/* @SymbolStart: 0x801426B0 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=4:t0,10:t1 IDO52_PRECOLOR_WEB=v0:14 */
/* @SymbolSize: 308 */
/* @PostProc: patch_insn_reg_field 0x7c:rt:t0:a3 0x84:rd:a0:v1 0x84:rt:t0:a3 0x88:rs:a0:v1 0x90:rd:v1:v0 0x90:rt:t0:a3 0x94:rs:v1:v0 0xbc:rs:t0:a3 0xcc:rt:t0:a3 0xd4:rt:t0:a3 0xdc:rs:t0:a3 */
/* rpg_bat_bukatu2_drop_step - RPG_BAT overlay, Address: 0x801426B8, Size: 300 bytes (75 insns) */
/* rpg_bat_bukatu2_drop_step - RPG_BAT overlay, Address: 0x801426B8, Size: 300 bytes. */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;

extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_step_next();

void rpg_bat_bukatu2_drop_step(void)
{
    s32 val;
    s32 scaled;
    s32 a2_val;

    if (rpg_bat_unit0.cnt == 0) {
        rpg_bat_anim_set2(43, 4, 1, 1);
        rpg_bat_anim_set2(44, 19, 1, 1);
        rpg_bat_sprite_pos_set(44, 272, 144);
    }

    val = rpg_bat_unit0.cnt;
    rpg_bat_sprite_pos_set(43, 272, (val << 2) - 192);
    val = rpg_bat_unit0.cnt;
    scaled = (val << 14) / 292;
    a2_val = (val << 2) - 192;
    class_w[140].scale_x = (s16)scaled;
    class_w[140].scale_y = (s16)scaled;
    if (a2_val >= 85) {
        rpg_bat_anim_set2(43, 21, 1, 1);
        val = rpg_bat_unit0.cnt;
        a2_val = (val << 2) - 192;
    }
    rpg_bat_unit0.cnt = val + 1;
    a2_val += 4;
    if (a2_val >= 101) {
        rpg_bat_anim_set(43, 6, 1, 1);
        rpg_bat_anim_set(44, 255, 1, 0);
        rpg_bat_sprite_pos_set(43, 0, 0);
        rpg_bat_step_next();
    }
}
