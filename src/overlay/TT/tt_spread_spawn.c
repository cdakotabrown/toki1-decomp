#include "types.h"

extern u8 *tt_slot_get(u8 *base, u8 *end);
extern void tt_slot_kind_set();
extern u8 *tt_slot_buf;

void tt_spread_spawn(s32 arg0)
{
    u8 *p;
    s32 i;

    p = tt_slot_buf;
    for (i = 0; i < 5; i++) {
        p = tt_slot_get(p, p + 7680);
        if (p == 0) {
            break;
        }
        *(s32 *)(p + 32) = 0;
        *(s32 *)(p + 36) = 0xFFF00000;
        *(s32 *)(p + 40) = 0;
        *(s32 *)(p + 44) = 0x00020000;
        *(u8 *)(p + 80) = (u8)i;
        *(s16 *)(p + 20) = *(s16 *)(p + 34);
        *(s16 *)(p + 22) = *(s16 *)(p + 38);
        tt_slot_kind_set(p, 69, 10);
        p += 120;
        *(s16 *)(p - 26) = 0;
        *(u8 *)(p - 24) = 0;
        *(s16 *)(p - 116) = 64;
        *(s16 *)(p - 110) = 0;
        *(s16 *)(p - 112) = 0;
        *(s16 *)(p - 114) = 0;
        *(u8 *)(p - 105) = 0;
        *(u8 *)(p - 106) = 0;
        *(u8 *)(p - 107) = 0;
    }
}
