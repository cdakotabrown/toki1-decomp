/* Size: 40 bytes */
/* @SymbolSize: 40 */
#include "types.h"

extern u8 *tt_boss_wk;

void tt_court_id_set(s32 arg0)
{
    tt_boss_wk[62] = ((arg0 + 64) & 0xFFF) / 128;
}
