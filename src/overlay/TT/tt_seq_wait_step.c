/* @SymbolStart: 0x80135AC4 */
/* @SymbolSize: 152 */
/* tt_seq_wait_step - TT overlay, Address: 0x80135ACC, Size: 144 bytes (36 insns) */
#include "types.h"
#include "tokimeki/tt_box.h"

extern u8 tt_wait_seq_tbl[];

extern u8 *tt_seq_step(u8 *a0, s32 a1);
extern u8 *tt_boss_wk;
extern TT_BOX tt_hit_box_tbl[];

void tt_seq_wait_step(void)
{
    u8 *obj = tt_boss_wk;

    switch (*(u16 *)(obj + 8)) {
    case 0:
        obj[84] = 128;
        *(s16 *)(obj + 8) = 64;
        break;
    case 64:
        tt_seq_step(tt_wait_seq_tbl, 1);
        obj[84] &= 1;
        if (obj[84] != 0) {
            *(s32 *)(obj + 536) = (s32)((u8 *)&tt_hit_box_tbl[6].x);
            obj[835] = 1;
            *(s16 *)(obj + 8) = 0;
            *(s16 *)(obj + 4) = 64;
        }
        break;
    }
}
