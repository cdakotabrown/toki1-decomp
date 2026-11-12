/* get_tt_ent_free - TT overlay, Address: 0x80147370, Size: 60 bytes (15 insns) */
#include "types.h"

u8 *get_tt_ent_free(u8 *a0, u8 *a1)
{
    while (a0 < a1) {
        if (a0[0] == 0) {
            return a0;
        }
        a0 += 104;
    }
    return 0;
}
