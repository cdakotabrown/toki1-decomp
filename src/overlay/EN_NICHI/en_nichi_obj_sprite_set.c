/* Size: 264 bytes */
/* @SymbolSize: 264 */
#include "types.h"

typedef struct {
    s32 kind;
    s32 unk4;
    s32 unk8;
    s32 flags;
    s32 unk16;
    s16 unk20;
    s16 x;
    s16 unk24;
    s16 y;
    u8  pad28[20];
} Slot;

extern u8 class_w[][68];
extern Slot en_nichi_obj_wk[];

void en_nichi_obj_sprite_set(s32 idx, s32 mode)
{
    u8 *p;
    Slot *r;
    s32 x;
    s32 y;

    p = class_w[idx * 2];
    p[6531] = 4;
    p[6599] = 5;
    if (mode != 1) {
        return;
    }
    r = &en_nichi_obj_wk[idx];
    if (r->kind == 4) {
        if ((r->flags & 1) == 0) {
            x = r->x;
            y = r->y;
            p[6531] |= 0x80;
            p[6599] |= 0x80;
            *(s16 *)(p + 6566) = x - 16;
            *(s16 *)(p + 6570) = y - 7;
            *(s16 *)(p + 6634) = x + 16;
            *(s16 *)(p + 6638) = y + 7;
        }
    } else {
        if ((r->flags & 1) == 0) {
            x = r->x;
            y = r->y;
            p[6531] |= 0x80;
            p[6599] |= 0x80;
            *(s16 *)(p + 6566) = x - 16;
            *(s16 *)(p + 6570) = y - 7;
            *(s16 *)(p + 6634) = x + 16;
            *(s16 *)(p + 6638) = y + 7;
        }
    }
}
