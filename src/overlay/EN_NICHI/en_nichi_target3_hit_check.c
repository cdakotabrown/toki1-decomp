/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8  pad0[3];
    u8  f3;
    u8  pad4[34];
    s16 x;
    u8  pad40[2];
    s16 y;
    u8  pad44[24];
} Rec;

extern Rec class_w[];

en_nichi_target3_hit_check(a, b)
s32 a;
s32 b;
{
    if (class_w[a + 98].y <= class_w[b + 105].y + 16 &&
        class_w[a + 98].y >= class_w[b + 105].y + 8 &&
        class_w[a + 98].x <= class_w[b + 105].x + 16 &&
        class_w[a + 98].x >= class_w[b + 105].x - 16 &&
        (class_w[b + 105].f3 & 0x80)) {
        return 1;
    }
    if (class_w[a + 98].y <= class_w[b + 105].y + 8 &&
        class_w[a + 98].y >= class_w[b + 105].y &&
        class_w[a + 98].x <= class_w[b + 105].x + 8 &&
        class_w[a + 98].x >= class_w[b + 105].x - 8 &&
        (class_w[b + 105].f3 & 0x80)) {
        return 1;
    }
    return 0;
}
