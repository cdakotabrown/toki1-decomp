/* tt_arc_move - TT overlay, Address: 0x8014B798, Size: 684 bytes (171 insns) */
#include "types.h"

extern s32 rsin(s32 angle);
extern s32 rcos(s32 angle);
extern void tt_shot_aimed_make(s16 x, s16 y, s32 width, s32 height);
extern u8 tt_arc_uv_tbl[];

typedef struct {
    u8  pad00[6];
    s16 state;
    u8  pad08[12];
    s16 x;
    s16 y;
    u8  pad18[8];
    s32 vx;
    s32 vy;
    s32 vz;
    u8  pad2C[16];
    s8  col;
    u8  pad3D[20];
    u8  active;
    u8  pad52[10];
    u8  u;
    u8  v;
    u16 timer;
    u8  count;
    u8  pad61[2];
    u8  side;
    u8  pad64[10];
    s16 ang;
} Obj;

void tt_arc_move(Obj *obj)
{
    s32 v;
    s32 a;
    s32 c;
    u8 *tbl;

    switch ((u16)obj->state) {
    case 0:
        if (obj->side != 0) {
            obj->u = 208;
            obj->v = 48;
            obj->col = 0;
            if (obj->vx < 0x80001) {
                obj->state = 1;
                obj->ang = 7168;
            }
        } else {
            obj->u = 208;
            obj->v = 48;
            obj->col = 2;
            if (obj->vx >= (s32)0xFFF80000) {
                obj->state = 1;
                obj->ang = 3072;
            }
        }
        break;
    case 1:
        if (obj->side != 0) {
            v = rcos((u16)obj->ang);
            obj->vx = v * 768;
            v = rsin((u16)obj->ang);
            obj->vy = v * 768 + 0x680000;
            obj->vz = 0;
            if ((u16)obj->ang < 3072U) {
                obj->state = 2;
            }
            obj->ang = (u16)obj->ang - 64;
            a = (s16)((((u32)(((u16)obj->ang + 256) & 0xFFF)) >> 9) * 3);
        } else {
            v = rcos((u16)obj->ang);
            obj->vx = v * 768;
            v = rsin((u16)obj->ang);
            obj->vy = v * 768 + 0x680000;
            obj->vz = 0;
            if ((u16)obj->ang >= 7169U) {
                obj->state = 2;
            }
            obj->ang = (u16)obj->ang + 64;
            a = (s16)((((u32)(((u16)obj->ang + 2304) & 0xFFF)) >> 9) * 3);
        }
        tbl = tt_arc_uv_tbl + a;
        obj->u = tbl[0];
        obj->v = tbl[1];
        obj->col = tbl[2];
        break;
    case 2:
        if (obj->side != 0) {
            obj->ang = 0;
            obj->vz = (s32)0xFFFC0000;
            obj->u = 208;
            obj->v = 48;
            obj->col = 0;
        } else {
            obj->ang = 0;
            obj->vz = 0x40000;
            obj->u = 208;
            obj->v = 48;
            obj->col = 2;
        }
        break;
    default:
        break;
    }

    if (obj->timer >= 61U) {
        c = obj->count;
        if (c == 0) {
            obj->timer = 0;
            obj->count = c + 1;
            tt_shot_aimed_make(obj->x, obj->y, 32, 160);
        }
    }
    if (obj->active != 0) {
        obj->timer = obj->timer + 1;
    }
}
