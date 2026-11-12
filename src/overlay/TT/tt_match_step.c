/* @SymbolStart: 0x8013BAD0 */
/* @SymbolSize: 88 */
/* Address: 0x8013BAD0 (cross-prologue head absorbed), Size: 88 bytes (22 insns) */
#include "types.h"

extern void tt_match_shot_step(void);
extern void tt_aim_burst(void);
extern u8 *tt_boss_wk;

void tt_match_step(void)
{
    u8 *t6 = tt_boss_wk;
    u16 val;

    val = *(u16 *)(t6 + 782);

    switch (val) {
        case 0:
            tt_match_shot_step();
            break;
        case 1:
            tt_aim_burst();
            break;
        default:
            break;
    }
}
