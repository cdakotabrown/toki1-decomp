/* get_tt_free_rec - TT overlay, Address: 0x80132F10, Size: 60 bytes (15 insns) */
#include "types.h"

u8 *get_tt_free_rec(u8 *a0, u8 *a1)
{
    while (a0 < a1) {
        if (*a0 == 0) {
            return a0;
        }
        a0 += 120;
    }
    return (u8 *)0;
}
