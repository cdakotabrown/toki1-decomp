/* @RodataBase: 0x8014D5D0 */
/* tt_obj_kind_set - TT overlay, Address: 0x8013BE08, Size: 148 bytes (37 insns) */
/* - `@RodataBase: 0x8014D5D0` is the in-EXN address of the jr table (from */
/* @PostProc: rename_register_chain --chain v0:t6 --chain t6:t7 --chain t7:t8 --chain t8:t9 --chain t9:t0 --chain t0:t1 --chain t1:t2 --chain t2:t3 --chain t3:t4 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x5c */
#include "types.h"

void tt_obj_kind_set(u8 *obj, s32 kind)
{
    u32 k;

    k = (u16)kind;
    *(s16 *)(obj + 2) = (s16)k;
    switch (k) {
    case 40:
        obj[86] = 0;
        obj[87] = 0;
        break;
    case 41:
        obj[86] = 64;
        obj[87] = 16;
        break;
    case 42:
        obj[86] = 64;
        obj[87] = 0;
        break;
    case 43:
        obj[87] = 32;
        obj[86] = 0;
        break;
    case 44:
        obj[87] = 16;
        obj[86] = 0;
        break;
    }
    obj[60] = 0;
    *(s16 *)(obj + 82) = 11;
    *(s16 *)(obj + 84) = 15489;
}
