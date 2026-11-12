/* tt_orbit_move - TT overlay, Address: 0x8014BD58, Size: 248 bytes (62 insns) */
#include "types.h"

typedef struct {
    u8 pad0[20];
    s16 h20;
    s16 h22;
    u8 pad24[8];
    s32 w32;
    s32 w36;
    u8 pad40[40];
    u8 b80;
    u8 b81;
    u8 pad82[12];
    u16 h94;
    u8 b96;
    u8 pad97[13];
    u16 h110;
} Obj;

extern s32 rcos(s32 angle);
extern s32 rsin(s32 angle);
extern void tt_shot_aimed_make(s16 a0, s16 a1, s32 a2, s32 a3);

void tt_orbit_move(Obj *obj)
{
    s32 sn;
    s32 cs;
    s32 c;

    sn = rcos(obj->h110 * 15 * 2);
    obj->w32 = (sn << 9) * (obj->b80 - 2);
    obj->w36 = (obj->h110 - 16) << 16;

    cs = rsin(obj->h110 * 15 * 2);
    obj->w36 += (cs << 9) * (obj->b80 - 2);
    obj->h110 = obj->h110 + 1;

    if ((u32)obj->h94 >= 25U) {
        c = obj->b96;
        if (c == 0) {
            obj->h94 = 0;
            obj->b96 = c + 1;
            tt_shot_aimed_make(obj->h20, obj->h22, 32, 160);
        }
    }
    if (obj->b81 != 0) {
        obj->h94 = obj->h94 + 1;
    }
}
