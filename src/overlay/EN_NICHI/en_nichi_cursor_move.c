/* @SymbolSize: 736 */
/* en_nichi_cursor_move - EN_NICHI overlay, Address: 0x8013324C, Size: 736 bytes (184 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 pad_data;
extern u8 pad_type;
typedef union { s32 w; struct { u16 lo; s16 hi; } h; } Fx;
typedef struct { u8 pad[0x24]; Fx x; Fx y; } Cam;
extern Cam class_w;

void en_nichi_cursor_move(void)
{
    s32 flags;
    s32 step;
    s16 v;
    volatile s16 *ent;

    flags = pad_data;
    if (flags & 0x40) {
        step = 3;
    } else {
        step = 2;
    }
    if (pad_type != 1) {
        goto clamp;
    }
    switch (flags & ~96) {
    case 0x1000:
        class_w.y.w -= step << 16;
        break;
    case 0x2000:
        class_w.x.w += step << 16;
        break;
    case 0x4000:
        class_w.y.w += step << 16;
        break;
    case 0x8000:
        class_w.x.w -= step << 16;
        break;
    case 0x3000:
        class_w.x.w += step * 46341;
        class_w.y.w -= step * 46341;
        break;
    case 0x6000:
        class_w.x.w += step * 46341;
        class_w.y.w += step * 46341;
        break;
    case 0xC000:
        class_w.x.w -= step * 46341;
        class_w.y.w += step * 46341;
        break;
    case 0x9000:
        class_w.x.w -= step * 46341;
        class_w.y.w -= step * 46341;
        break;
    }
clamp:
    ent = (volatile s16 *)&class_w;
    v = ent[19];
    if (v < -143) {
        ent[19] = -144;
        v = ent[19];
    }
    if (v > 143) {
        ent[19] = 144;
    }
    v = ent[21];
    if (v < -103) {
        ent[21] = -104;
        v = ent[21];
    }
    if (v > 103) {
        ent[21] = 104;
        v = ent[21];
    }
    ((CLASS_W *)&class_w)[152].x.half[1] = ent[19];
    ((CLASS_W *)&class_w)[152].y.half[1] = v;
}
