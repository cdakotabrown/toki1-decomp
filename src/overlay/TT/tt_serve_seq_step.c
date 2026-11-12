/* @SymbolStart: 0x801381E8 */
/* @SymbolSize: 88 */
/* Address: 0x801381E8 (cross-prologue head absorbed), Size: 88 bytes (22 insns) */
#include "types.h"

extern s32 tt_seq_step();

extern volatile u8 tt_serve_seq_tbl[];
extern volatile u8 tt_target_box_tbl[];
extern u8 *tt_boss_wk;

void tt_serve_seq_step(void)
{
    u8 *ptr = tt_boss_wk;

    tt_seq_step(tt_serve_seq_tbl, 0);

    if ((ptr[64] & 1) != 0) {
        *(u8 **)(ptr + 536) = tt_target_box_tbl;
        *(s16 *)(ptr + 4) = 64;
    }
}
