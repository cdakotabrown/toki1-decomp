/* tt_slot_get - TT overlay, Address: 0x8014AEF0, Size: 60 bytes (15 insns) */
#include "types.h"

u8 *tt_slot_get(u8 *a0, u8 *a1)
{
    while (a0 < a1) {
        if (*a0 == 0) {
            return a0;
        }
        a0 += 120;
    }
    return (u8 *)0;
}
