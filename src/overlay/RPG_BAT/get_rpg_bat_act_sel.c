/* @SymbolStart: 0x80144B7C */
/* @SymbolSize: 216 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:v0 0x14:rt:v1:v0 0x14:rs:v1:v0 0x1c:rs:v1:v0 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 get_rpg_bat_monster_act();
extern s32 get_rpg_bat_act();
extern s32 get_rpg_bat_banchou_power_act();
extern s32 get_rpg_bat_banchou_shade_act();
extern s32 get_rpg_bat_utyuuzin_act();
extern s32 rpg_bat_hit_cnt;
extern RPG_BAT_UNIT rpg_bat_unit1;

s32 get_rpg_bat_act_sel()
{
    s32 pad;
    s32 ret;

    pad = rpg_bat_unit1.flag & 0xFFFF0000;
    ret = pad;
    switch (pad) {
    case 0x10000:
    case 0x20000:
    case 0x40000:
        return get_rpg_bat_monster_act();
    case 0x100000:
        return get_rpg_bat_act();
    case 0x200000:
        return get_rpg_bat_banchou_power_act();
    case 0x400000:
        return get_rpg_bat_banchou_shade_act();
    case 0x8000000:
        rpg_bat_hit_cnt = rpg_bat_hit_cnt + 2;
        ret = 1;
        break;
    case (s32)0x80000000:
        return get_rpg_bat_utyuuzin_act();
    }
    return ret;
}
