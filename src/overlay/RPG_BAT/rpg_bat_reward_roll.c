/* @SymbolStart: 0x801495DC */
/* @SymbolSize: 276 */
/* @PostProc: patch_insn_reg_field 0x64:rs:t7:v0 0x64:rt:v0:t7 */
/* Address: 0x801495E4 (true start 0x801495DC: the hidden-arg halfword load */
/* Size: 268 bytes (67 insns) + 8 hoisted = 276 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 get_rpg_bat_roll2();
extern s32 get_rand_mod();

extern volatile s32 rpg_bat_dmg;
extern s32 rpg_bat_bukatu_flag;
extern s16 para_un;
extern RPG_BAT_UNIT rpg_bat_unit1;

void rpg_bat_reward_roll(void)
{
    s32 v0;
    s32 v1;
    s32 t8;
    s32 t0;

    v0 = para_un;
    if (v0 >= 301) {
        v1 = 300;
    } else {
        v1 = v0;
    }
    v0 = get_rpg_bat_roll2(100, 100);
    t8 = v0 + (v1 * 9) / 2;
    v0 = get_rand_mod(4096);
    if (v0 == 0) {
        v1 = 9999;
    } else if (v0 < 64) {
        v1 = 0;
    } else if (v0 < 270) {
        v1 = get_rpg_bat_roll2(500, 1000) + t8;
    } else {
        v1 = t8;
    }
    if (v1 >= 10000) {
        v1 = 9999;
    }
    rpg_bat_dmg = v1;
    t0 = rpg_bat_unit1.flag;
    if ((t0 & 0x00600000) != 0) {
        rpg_bat_dmg = v1;
        if (rpg_bat_bukatu_flag == 0) {
            rpg_bat_dmg = v1;
            if (v1 != 0) {
                rpg_bat_unit1.hp = rpg_bat_unit1.hp - 1500;
            }
        }
    }
}
