/* Address: 0x80144480 (absorbed head)  Size: 360 bytes (90 instructions) */
/* @SymbolStart: 0x80144480 */
/* @SymbolSize: 360 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:v0 0x14:rs:v1:v0 0x14:rt:v1:v0 0x1c:rs:v1:v0 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 get_rand_mod();

extern s32 rpg_bat_target_no;
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

s32 rpg_bat_target_set(void)
{
    s32 v0;

    v0 = rpg_bat_unit1.flag & (s32)0xFFFF0000;

    switch (v0) {
    case 0x00010000:
        rpg_bat_target_no = 16;
        break;
    case 0x00020000:
        rpg_bat_target_no = 17;
        break;
    case 0x00040000:
        rpg_bat_target_no = 18;
        break;
    case 0x00100000:
        if ((rpg_bat_unit0.flag & 1) != 0) {
            v0 = get_rand_mod(15);
            switch (v0) {
            default:
                rpg_bat_target_no = 0;
                break;
            case 0:
                rpg_bat_target_no = 1;
                break;
            case 1:
                rpg_bat_target_no = 2;
                break;
            }
        } else {
            v0 = get_rand_mod(4);
            switch (v0) {
            default:
                rpg_bat_target_no = 0;
                break;
            case 0:
                rpg_bat_target_no = 1;
                break;
            case 1:
                rpg_bat_target_no = 2;
                break;
            }
        }
        break;
    case 0x00200000:
    case 0x00400000:
        rpg_bat_target_no = 3;
        break;
    case 0x08000000:
        rpg_bat_target_no = 4;
        break;
    case (s32)0x80000000:
        rpg_bat_target_no = 5;
        break;
    }

    return v0;
}
