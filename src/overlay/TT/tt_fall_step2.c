/* Size: 124 bytes */
/* @SymbolSize: 124 */
#include "types.h"
extern u8 *tt_boss_wk;
extern u8 tt_cell_box_tbl[];
void tt_fall_step2(void)
{
    u8 *p;
    p = tt_boss_wk;
    *(s32 *)(p + 32) = 0;
    *(s32 *)(p + 36) += 0x10000;
    *(s16 *)(p + 20) = *(s16 *)(p + 34);
    *(s16 *)(p + 22) = *(s16 *)(p + 38);
    if (*(s16 *)(p + 22) >= 64) {
        *(s32 *)(p + 36) = 0x400000;
        *(s16 *)(p + 22) = *(s16 *)(p + 38);
        *(u8 **)(p + 536) = tt_cell_box_tbl;
        *(s16 *)(p + 4) = 64;
        *(s16 *)(p + 10) = 0;
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 6) = 0;
        p[15] = 0;
        p[14] = 0;
        p[13] = 0;
    }
}
