/* @SymbolStart: 0x8013F5E8 */
/* @SymbolSize: 88 */
/* Address: 0x8013F5E8 (cross-prologue head absorbed), Size: 88 bytes (22 insns) */
#include "types.h"
#include "tokimeki/tt_box.h"

extern s32 tt_seq_step();
extern u8 tt_intro_seq_tbl[];
extern u8 *tt_boss_wk;
extern TT_BOX tt_cell_box_tbl[];

void tt_intro_seq_step(void)
{
    u8 *ptr = tt_boss_wk;

    tt_seq_step(tt_intro_seq_tbl, 0);

    if ((ptr[64] & 1) != 0) {
        *(u8 **)(ptr + 536) = ((u8 *)&tt_cell_box_tbl[30].x);
        *(s16 *)(ptr + 4) = 64;
    }
}
