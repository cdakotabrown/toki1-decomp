/* @SymbolStart: 0x80149E34 */
/* @SymbolSize: 92 */
/* @PostProc: patch_insn_reg_field 0x34:rd:t8:t7 0x38:rd:t9:t7 0x38:rt:t8:t7 0x40:rd:t0:t8 0x40:rs:t9:t7 0x44:rt:a1:t0 0x48:rt:t0:t8 0x4c:rt:a0:t9 0x50:rs:a1:t0 0x58:rt:a0:t9 */
/* Address: 0x80149E34 (cross-prologue head absorbed), Size: 100 bytes */
#include "types.h"

extern s32 rsin();
extern u8 *tt_player_wk;

void tt_menu_player_move(void)
{
    u8 *t6 = tt_player_wk;
    s32 result;
    s32 temp;
    u16 val;
    s16 y;

    val = *(u16 *)(t6 + 106);
    result = rsin(val);

    temp = result << 4;
    temp = temp - result;
    temp = temp << 3;
    temp = temp + 0x009C0000;

    *(s32 *)(t6 + 36) = temp;

    val = *(u16 *)(t6 + 106);
    *(u16 *)(t6 + 106) = val + 16;

    y = *(s16 *)(t6 + 38);
    *(s16 *)(t6 + 22) = y;
}
