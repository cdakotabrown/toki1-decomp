/* Address: 0x8013DAA0 */
/* Size: 612 bytes (153 insns) */
/* @PostProc: patch_insn_reg_field 0x80:rt:v0:t2 0x84:rt:a0:v0 0x88:rt:a1:a0 0x8c:rt:a3:a1 0x90:rs:v0:t2 0x90:rt:t2:t3 0x9c:rt:a0:v0 0xa0:rt:a1:a0 0xa4:rs:t2:t3 0xa8:rt:a3:a1 0xac:rt:t3:t4 0xb0:rs:t3:t4 0xb0:rt:t3:t4 0xb4:rs:v0:t2 0xb4:rt:t0:t1 0xb8:rs:t3:t4 0xb8:rt:t4:t5 0xc0:rd:t5:t6 0xc0:rs:t4:t5 0xc0:rt:a0:v0 0xc4:rt:t5:t6 0xc8:rs:t3:t4 0xc8:rt:t6:t7 0xd0:rd:t7:t8 0xd0:rs:t6:t7 0xd0:rt:a1:a0 0xd4:rt:t7:t8 0xd8:rs:t3:t4 0xd8:rt:t8:t9 0xe0:rd:t9:t0 0xe0:rs:t8:t9 0xe0:rt:a3:a1 0xe4:rs:t0:t1 0xe8:rt:t9:t0 0xf8:rt:t1:t3 0xfc:rt:t2:t5 0x100:rd:t4:t6 0x100:rs:t1:t3 0x100:rt:t2:t5 0x104:rs:t4:t6 0x104:rt:t5:t7 0x10c:rt:t5:t7 0x110:rt:t6:t8 0x114:rt:t7:t4 0x118:rd:t3:t9 0x118:rs:t6:t8 0x118:rt:t7:t4 0x11c:rs:t3:t9 0x11c:rt:t8:t0 0x124:rt:t8:t0 0x128:rt:t9:t2 0x12c:rt:t0:t1 0x130:rd:t1:t3 0x130:rs:t9:t2 0x130:rt:t0:t1 0x134:rs:t1:t3 0x134:rt:t2:t5 0x140:rt:t2:t5 0x158:rt:t4:t6 0x15c:rt:t5:t7 0x160:rs:t5:t7 0x160:rt:t4:t6 0x164:rt:t6:t8 0x168:rt:a0:t9 0x16c:rt:a1:t2 0x178:rs:t4:t6 0x188:rs:t4:t6 0x190:rs:t5:t7 0x19c:rd:v0:a0 0x1a0:rs:v0:a0 0x1a0:rt:v0:a0 0x1a8:rs:t6:t8 0x1a8:rt:v0:a0 0x1ac:rs:v0:a0 0x1bc:rs:v0:a0 0x1c4:rs:t6:t8 0x1d0:rd:t7:t4 0x1d4:rt:t7:t4 0x1dc:rs:a0:t9 0x1dc:rt:v0:a0 0x1e0:rs:v0:a0 0x1f0:rs:v0:a0 0x1f8:rs:a0:t9 0x204:rd:t3:t0 0x208:rt:t3:t0 0x210:rs:a1:t2 0x210:rt:v0:a0 0x214:rs:v0:a0 0x224:rs:v0:a0 0x22c:rs:a1:t2 0x238:rd:t8:t1 0x23c:rt:t8:t1 0x244:rt:t0:t5 0x248:rt:t9:t3 0x254:rt:t9:t3 0x258:rt:t0:t5 0x260:rt:t0:t5 */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    s16 z;
} Pos3;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 pad6;
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad14;
    u8 pad16;
    u8 flags;
    u8 f18;
    u8 active;
} Prt;

typedef struct {
    u8 pad0[116];
    s16 cx;
    s16 cy;
    s16 cz;
} Trk;

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Vec32;

extern Prt taco_pickup_tbl[];
extern Trk *taco_actor_rec;

extern s32 rand();
extern void vector_normalize();

s32 taco_shot_spawn(Pos3 pos, s32 speed, volatile s32 flags)
{
    u8 pad;
    s32 i;
    s32 aim;
    s32 x;
    Vec32 d;
    s32 y;
    Vec32 out;
    s16 scale;
    s32 z;
    s32 k;
    Prt *e;

    scale = speed;
    if (scale == 0) {
        scale = 256;
    }

    e = taco_pickup_tbl;
    i = 0;
    do {
        if ((e->active & 0x80) == 0) {
            break;
        }
        i++;
        e++;
    } while (i != 16);

    if (i >= 16) {
        return -1;
    }

    x = pos.x;
    y = pos.y;
    z = pos.z;
    aim = flags;
    e->f18 = 0;
    e->active = 128;
    e->x = x;
    e->y = y;
    e->z = z;

    if (aim & 0x80) {
        d.vx = taco_actor_rec->cx - x;
        d.vy = taco_actor_rec->cy - y;
        d.vz = taco_actor_rec->cz - z;
        if (aim & 0x40) {
            d.vx = d.vx - (rand() & 0xFF) + 127;
            d.vy = d.vy - (rand() & 0xFF) + 127;
            d.vz = d.vz - (rand() & 0xFF) + 127;
        }
        vector_normalize(&d, &out);
        k = 4096 / scale + 1;
        x = out.vx; y = out.vy; z = out.vz;
        e->vx = (s16)(x / k);
        e->vy = (s16)(y / k);
        e->vz = (s16)(z / k);
    } else {
        e->vz = scale;
        e->vx = 0;
        e->vy = 0;
    }

    e->flags = (u8)flags;
    return i;
}
