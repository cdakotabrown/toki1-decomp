/* @SymbolStart: 0x8013B150 */
/* @SymbolSize: 116 */
/* Address: 0x8013B150 (cross-prologue head absorbed), Size: 116 bytes (29 insns) */
#include "types.h"

extern void tt_match_move0(void);
extern void tt_match_move1(void);
extern u8 *tt_boss_wk;

void tt_match_move(void)
{
    u8 *obj = tt_boss_wk;

    switch (*(u16 *)(obj + 782)) {
    case 0:
        tt_match_move0();
        break;
    case 1:
        tt_match_move1();
        break;
    }

    *(s16 *)(obj + 20) = *(s16 *)(obj + 34);
    *(s16 *)(obj + 22) = *(s16 *)(obj + 38);
}
