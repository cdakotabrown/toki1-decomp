/* @SymbolSize: 140 */
/* @SymbolStart: 0x801396E4 */
/* Address: 0x801396EC, Size: 132 bytes (33 insns) */
#include "types.h"

extern u8 *tt_boss_wk;
extern u8 *tt_board_ptr;

extern void tt_shot_aimed_make(s16 x, s16 y, s32 width, s32 height);

void tt_board_shot_step(void)
{
    u8 *obj;
    u8 *ptr;

    obj = tt_boss_wk;
    if ((u32)*(u16 *)(obj + 870) >= 24U) {
        ptr = tt_board_ptr;
        tt_shot_aimed_make(*(s16 *)(ptr + 58), *(s16 *)(ptr + 186), 24, 168);
        ptr = ptr + 3836;
        tt_shot_aimed_make(*(s16 *)(ptr + 58), *(s16 *)(ptr + 186), 24, 168);
        *(s16 *)(obj + 870) = 0;
        return;
    }
    *(s16 *)(obj + 870) = *(u16 *)(obj + 870) + 1;
}
