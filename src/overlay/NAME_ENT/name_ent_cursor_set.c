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

name_ent_cursor_set(idx)
s32 idx;
{
    s16 x;
    s16 y;

    x = idx / 7 * 31 - 130;
    y = idx % 7 * 16 - 11;
    class_w[0].x = x;
    class_w[0].y = y;
    class_w[119].f3 |= 0x80;
    class_w[119].x = x;
    class_w[119].y = y;
}
