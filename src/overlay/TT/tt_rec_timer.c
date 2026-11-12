/* tt_rec_timer - TT overlay, Address: 0x80149088. */
#include "types.h"

void tt_rec_timer(u8 *p)
{
    if (++*(u16 *)(p + 70) >= 121U) {
        p[0] = 0;
    }
}
