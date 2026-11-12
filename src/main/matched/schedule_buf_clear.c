/* Address: 0x8006CFD4 */
/* Size: 72 bytes (18 instructions) */
/* Region: Game */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef union {
    u32 w;
    u8 b;
} SlotEnt;

extern u8 screen_w[];
extern u8 schedule_cursor;

#define SLOT_TBL ((SlotEnt *)(screen_w + 1692))

void schedule_buf_clear(void) {
    s32 i;

    for (i = 0; i < 32; i++) {
        SLOT_TBL[i].w = 0;
        SLOT_TBL[i].b = 0xFF;
    }
    schedule_cursor = 0;
}
