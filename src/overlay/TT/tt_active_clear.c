/* Size: 16 bytes */
/* @SymbolSize: 16 */
#include "types.h"

extern u8 *tt_player_wk;

void tt_active_clear(void)
{
    tt_player_wk[0] = 0;
}
