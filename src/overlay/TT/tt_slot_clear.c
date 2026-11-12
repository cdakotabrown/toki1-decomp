/* Size: 80 bytes */
/* @SymbolSize: 80 */
#include "types.h"
extern u8 *tt_anim_buf;
extern u8 *tt_boss_wk;
void tt_slot_clear(void)
{
    s32 i;
    u8 *p;

    p = tt_anim_buf + 3200;
    tt_boss_wk[0] = 0;
    for (i = 0; i < 31; i++) {
        p[0] = 0;
        p += 100;
    }
}
