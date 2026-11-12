/* @SymbolStart: 0x8013D418 */
/* rpg_bat_slide1 - RPG_BAT overlay, Address: 0x8013D420, Size: 380 bytes (95 insns) */
/* @SymbolSize: 388 */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:6,lod:2 */
/* @PostProc: patch_insn_reg_field 0x94:rt:v1:v0 0x98:rt:v1:v0 0x98:rs:v1:v0 0xa0:rt:v1:v0 0xa0:rs:v1:v0 0xa4:rt:v1:v0 0xa8:rs:v1:v0 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"


extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_se_set();
extern s32 rpg_bat_step_next();
extern s32 rpg_bat_state_next();
extern s32 rpg_bat_rec_sub();

void rpg_bat_slide1(void)
{
    s32 v0;

    switch (rpg_bat_unit0.sub) {
    case 0:
        rpg_bat_sprite_pos_set(48, 0, 0);
        rpg_bat_anim_set(48, 1, 1, 1);
        rpg_bat_state_next();
        break;
    case 1:
        v0 = rpg_bat_unit0.wait;
        rpg_bat_sprite_pos_set(48, -v0, 0);
        v0 = rpg_bat_unit0.wait;
        v0 += 2;
        rpg_bat_unit0.wait = v0;
        if (v0 < 33) {
            break;
        }
        rpg_bat_anim_set(48, 0, 1, 0);
        rpg_bat_sprite_pos_set(52, 0, 0);
        if (rpg_bat_unit1.flag & 0x00600000) {
            class_w[148].rate = 130;
            rpg_bat_anim_set(52, 1, 5, 1);
        } else {
            class_w[148].rate = 130;
            rpg_bat_anim_set(52, 0, 5, 1);
        }
        rpg_bat_state_next();
        break;
    case 2:
        v0 = rpg_bat_unit0.wait;
        v0 += 1;
        rpg_bat_unit0.wait = v0;
        if (v0 == 1) {
            rpg_bat_se_set(1283, 1, 0);
        }
        rpg_bat_rec_sub(52);
        break;
    case 3:
        class_w[148].rate = 1;
        rpg_bat_step_next();
        break;
    }
}
