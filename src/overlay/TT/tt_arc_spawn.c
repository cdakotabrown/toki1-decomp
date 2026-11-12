#include "types.h"

extern u8 *tt_slot_get(u8 *base, u8 *end);
extern void tt_slot_kind_set();
extern u8 *tt_slot_buf;

void tt_arc_spawn(u8 flag)
{
    u8 *p;
    s32 i;
    s32 sh;

    p = tt_slot_buf;
    for (i = 0; i < 6; i++) {
        p = tt_slot_get(p, p + 7680);
        if (p == 0) {
            break;
        }
        sh = i * 16;
        if (flag != 0) {
            *(s32 *)(p + 32) = (sh + 144) << 16;
            *(s32 *)(p + 40) = 0xFFFC0000;
        } else {
            *(s32 *)(p + 32) = (-144 - sh) << 16;
            *(s32 *)(p + 40) = 0x00040000;
        }
        *(s32 *)(p + 36) = 0x00380000;
        *(s32 *)(p + 44) = 0;
        *(u8 *)(p + 99) = (u8)flag;
        *(s16 *)(p + 20) = *(s16 *)(p + 34);
        *(s16 *)(p + 22) = *(s16 *)(p + 38);
        tt_slot_kind_set(p, 68, 9);
        p += 120;
        *(u8 *)(p - 56) |= 0x40;
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
