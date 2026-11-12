/* @SymbolStart: 0x80138D10 */
/* @SymbolSize: 132 */
/* Address: 0x80138D10 (cross-prologue head absorbed), Size: 132 bytes */
#include "types.h"

extern s32 tt_seq_step();
extern u8 *tt_boss_wk;

extern u8 tt_seq6a_seq_tbl[];
extern u8 tt_seq6b_seq_tbl[];
extern u8 tt_seq6c_seq_tbl[];

void tt_seq6_step(void)
{
    u8 *t6 = tt_boss_wk;
    u16 val;

    val = *(u16 *)(t6 + 782);

    switch (val) {
        case 1:
            tt_seq_step(tt_seq6a_seq_tbl, 6);
            break;
        case 2:
            tt_seq_step(tt_seq6b_seq_tbl, 6);
            break;
        case 3:
            tt_seq_step(tt_seq6c_seq_tbl, 6);
            break;
        default:
            break;
    }
}
