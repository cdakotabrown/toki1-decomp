/* @SymbolSize: 48 */
/* /* @PostProc: patch_insn_reg_field 0x3c:rs:v0:t6 0x3c:rt:t6:v0 */
/* get_rpg_bat_roll2 - RPG_BAT overlay, Address: 0x8013EA00, Size: 48 bytes (12 insns). */
#include "types.h"

extern s32 get_rand_mod(s32);

s32 get_rpg_bat_roll2(s32 a0, s32 a1)
{
    s32 t6 = get_rand_mod(a0);
    return t6 + get_rand_mod(a1);
}
