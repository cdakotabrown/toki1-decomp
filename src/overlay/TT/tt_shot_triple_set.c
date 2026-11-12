/* @SymbolStart: 0x801454C8 */
/* @SymbolSize: 48 */
/* tt_shot_triple_set - TT overlay, Address: 0x801454C8 (cross-prologue head absorbed) */
/* Size: 48 bytes (12 insns) */
#include "types.h"

extern u8 *tt_player_wk;

extern void tt_effect_make(u8 *a0);

void tt_shot_triple_set(u8 *a0)
{
    u8 *t7 = tt_player_wk;

    t7[85] = 10;
    tt_effect_make(a0 + 20);
}
