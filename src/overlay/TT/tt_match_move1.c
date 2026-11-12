/* @SymbolStart: 0x8013B0E0 */
/* @SymbolSize: 112 */
#include "types.h"

extern u8 *tt_boss_wk;
extern s32 rsin();
extern s32 rcos();

void tt_match_move1(void)
{
    u8 *p = tt_boss_wk;

    *(s32 *)(p + 32) = rsin(*(s32 *)(p + 996)) * 1280;
    *(s32 *)(p + 36) = (rcos(*(s32 *)(p + 996) << 2) << 7) + 0x00380000;
    *(s32 *)(p + 996) += 16;
}
