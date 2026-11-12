/* @SymbolStart: 0x80135D90 */
/* @SymbolSize: 240 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x94:rt:t7:v0 0x98:rs:t7:v0 0x98:rt:t7:v0 0xa0:rs:t7:v0 */
/* rpg_bat_slide_in_step - RPG_BAT overlay, Address: 0x80135D98, Size: 232 bytes (58 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit2;


extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_act_next();
extern s32 rpg_bat_reset();
extern s32 rpg_bat_act_wait();


void rpg_bat_slide_in_step(void)
{
    s32 v0;

    switch (rpg_bat_unit2.step) {
    case 0:
        rpg_bat_anim_set(29, 1, 1, 1);
        rpg_bat_act_next();
        break;
    case 1:
        v0 = rpg_bat_unit2.cnt + 1;
        rpg_bat_unit2.cnt = v0;
        rpg_bat_sprite_pos_set(29, 64 - v0, 0);
        if (rpg_bat_unit2.cnt >= 65) {
            rpg_bat_sprite_pos_set(29, 0, 0);
            rpg_bat_anim_set(29, 0, 1, 1);
            rpg_bat_act_next();
        }
        break;
    case 2:
        rpg_bat_act_wait(20);
        break;
    case 3:
        rpg_bat_reset();
        break;
    }
}
