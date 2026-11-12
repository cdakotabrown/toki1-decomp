/* Address: 0x80065670 */
/* Size: 1072 bytes (268 instructions) */
/* Region: Game (0x80041000-0x80086000) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef union {
    s32 l;
    struct { u16 lo; s16 hi; } w;
} Fix;

typedef struct {
    u8 b0;
    u8 layer;
    u8 f2;
    u8 f3;
    u8 pad4[18];
    s16 anim;
    s16 frame;
    u8 pad1a[10];
    Fix x;
    Fix y;
    u8 pad2c[24];
} Slot;

typedef struct {
    s16 x;
    s16 y;
} Pt;

extern u8 icon_num;
extern Slot class_w[];
extern Pt icon_xy[];

extern void menu_set();

void icon_menu_set(n)
s32 n;
{
    s32 unused;
    s32 i;
    s16 x[16];
    s16 y[16];
    s16 w[16];
    s16 h[16];

    if (n != 15) {
        n = 14;
        icon_num = 14;
    }
    for (i = 0; i < n; i++) {
        x[i] = icon_xy[i].x - 10;
        y[i] = icon_xy[i].y - 10;
        w[i] = 20;
        h[i] = 16;
    }
    menu_set(0, n & 0xFF, x, y, w, h);
    for (i = 0; i < n; i++) {
        class_w[80 + i].anim = i;
        class_w[80 + i].x.w.hi = icon_xy[i].x;
        class_w[80 + i].y.w.hi = icon_xy[i].y;
    }
    for (i = 0; i < n; i++) {
        class_w[64 + i].frame = 2;
        class_w[64 + i].x.w.hi = icon_xy[i].x;
        class_w[64 + i].y.w.hi = icon_xy[i].y;
    }
    if (n == 15) {
        class_w[94].anim = 15;
        class_w[94].x.w.hi = icon_xy[14].x;
        class_w[94].y.w.hi = icon_xy[14].y;
        class_w[78].frame = 2;
        class_w[78].x.w.hi = icon_xy[14].x;
        class_w[78].y.w.hi = icon_xy[14].y;
    } else {
        class_w[78].f3 = 0;
        class_w[94].f3 = 0;
    }
}
