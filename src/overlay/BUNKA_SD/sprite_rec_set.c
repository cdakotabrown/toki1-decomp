/* Address: 0x80138810 */
/* Size: 80 bytes (20 insns) */
#include "types.h"

typedef struct {
    u8  pad0[3];
    u8  f3;
    u8  pad4[20];
    s16 f24;
    u8  pad26[42];
} Rec;

extern Rec class_w[];

void sprite_rec_set(s32 idx, s32 val, s32 mode) {
    Rec *r;

    if (mode == 1) {
        r = &class_w[idx];
        r[96].f3 = 128;
    } else {
        r = &class_w[idx];
        r[96].f3 = 0;
    }
    r[96].f24 = val;
}
