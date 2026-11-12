/* @CompileFlags: -cckr */
/* tt_time_set - TT overlay, Address: 0x80148574, Size: 340 bytes (85 insns) */
#include "types.h"

extern u8 *tt_str_ptr_tbl[];

void tt_time_set(value, slot)
    u16 value;
    s32 slot;
{
    u8 *r;

    r = tt_str_ptr_tbl[slot];
    if (value == 0x8CA0) {
        r[0] = 9;
        r[2] = 5;
        r[3] = 9;
        r[5] = 9;
        r[6] = 9;
        return;
    }
    r[0] = value / 3600;
    r[2] = (value % 3600) / 600;
    r[3] = (value % 600) / 60;
    r[5] = (value % 60) * 10 / 60;
    r[6] = (value * 10 / 6) % 10;
}
