/* @PostProc: patch_insn_reg_field 0x80:rt:v0:t7 0x88:rs:v0:t7 0x88:rt:v0:t8 0x8c:rs:v0:t8 0x8c:rt:t7:t9 0x90:rt:t7:t9 0x94:rt:v0:t8 0x9c:rt:t8:t0 0xa4:rs:t8:t0 0xb4:rt:v1:v0 0xbc:rs:v1:v0 0xbc:rt:v0:v1 0xc0:rs:s4:v1 0xc0:rt:v0:s4 0xc4:rs:v1:v0 0xc4:rt:t9:t1 0xc8:rs:v0:v1 0xcc:rs:v1:v0 0xcc:rt:t9:t1 0xd8:rs:t9:t1 0xe0:rt:t0:t2 0xe4:rt:t1:t3 0xe8:rt:t3:t5 0xec:rt:t4:t6 0xf0:rt:t6:t8 0xf4:rt:t7:t9 0xf8:rd:t2:t4 0xf8:rs:t0:t2 0xf8:rt:t1:t3 0xfc:rd:t5:t7 0xfc:rs:t3:t5 0xfc:rt:t4:t6 0x100:rd:t8:t0 0x100:rs:t6:t8 0x100:rt:t7:t9 0x104:rt:t2:t4 0x108:rt:t5:t7 0x10c:rt:t8:t0 0x120:rs:t9:t1 0x128:rt:t0:t2 0x12c:rt:t1:t3 0x130:rt:t3:t5 0x134:rt:t4:t6 0x138:rt:t6:t8 0x13c:rt:t7:t9 0x140:rd:t2:t4 0x140:rs:t0:t2 0x140:rt:t1:t3 0x144:rd:t5:t7 0x144:rs:t3:t5 0x144:rt:t4:t6 0x148:rd:t8:t0 0x148:rs:t6:t8 0x148:rt:t7:t9 0x14c:rt:t2:t4 0x150:rt:t5:t7 0x154:rt:t8:t0 0x16c:rt:t9:t1 0x174:rs:t9:t1 0x174:rt:t0:t2 0x178:rs:t0:t2 0x180:rt:v0:t3 0x188:rs:v0:t3 0x188:rt:v0:t4 0x18c:rs:v0:t4 0x18c:rt:v1:v0 0x190:rs:v1:v0 0x198:rt:v0:t4 0x19c:rt:t1:t5 0x1a4:rs:t1:t5 0x1a4:rt:t2:t6 0x1a8:rt:t2:t6 0x1ac:rt:t3:t7 0x1b4:rs:t3:t7 0x1b4:rt:t4:t8 0x1b8:rt:t4:t8 0x1bc:rt:t5:t9 0x1c4:rt:t5:t9 0x1c8:rt:t6:t0 0x1d0:rs:t6:t0 0x1d0:rt:t7:t1 0x1d4:rt:t7:t1 0x1d8:rt:t8:t2 0x1e0:rs:t8:t2 0x1e0:rt:t9:t3 0x1e4:rt:t9:t3 0x1e8:rt:t0:t4 0x1f0:rt:t0:t4 0x200:rt:v1:v0 0x208:rs:v1:v0 */
/* Address: 0x80140D28 */
/* Size: 568 bytes (142 insns) */
#include "types.h"

typedef struct
{
    s16 x;
    s16 y;
    s16 z;
    s16 pad6;
    s16 dx;
    s16 dy;
    s16 dz;
    s16 padE;
    u8 pad10;
    u8 pad11;
    u8 timer;
    u8 flags;
} TacoSpr;

extern TacoSpr taco_effect_buf[];
extern TacoSpr taco_effect2_buf[];
extern u8 *taco_actor_rec;

extern void taco_effect4_disp(TacoSpr *, s32);
extern void taco_splash_disp(TacoSpr *, s32);
extern void taco_shadow_disp(TacoSpr *, s32);

void _taco_effect(s32 side)
{
    TacoSpr *base;
    TacoSpr *e;
    s16 *act;
    s32 i;
    s32 t;
    s32 v;

    if (side == 0) {
        act = (s16 *)taco_actor_rec;
        base = taco_effect_buf;
    } else {
        act = (s16 *)(taco_actor_rec + 1088);
        base = taco_effect2_buf;
    }

    i = 0;
    e = base;
    do {
        t = e->timer + 1;
        e->timer = (u8)t;
        if ((t & 0xFF) == 255) {
            e->flags = 0;
        }
        if (e->z < -32000) {
            e->flags = 0;
        }
        v = e->flags;
        switch (v & 0x2F) {
        case 4:
            if ((v & 0x80) != 0) {
                e->x = (s16)(e->x + e->dx);
                e->y = (s16)(e->y + e->dy);
                e->z = (s16)(e->z + e->dz);
                taco_effect4_disp(base, i);
            }
            break;
        case 8:
            if ((v & 0x80) != 0) {
                e->x = (s16)(e->x + e->dx);
                e->y = (s16)(e->y + e->dy);
                e->z = (s16)(e->z + e->dz);
                taco_splash_disp(base, i);
            }
            break;
        }
        i++;
        e++;
    } while (i != 16);

    if ((base->flags & 0xA0) == 160) {
        v = (base->timer + 1) & 0xFF;
        base->timer = (u8)(base->timer + 1);
        if ((u32)v >= 3U) {
            base[0].x = (s16)(act[58] + 64);
            base[0].y = (s16)(act[59] + 32);
            base[0].z = act[60];
            base[1].x = (s16)(act[58] - 64);
            base[1].y = (s16)(act[59] + 32);
            base[1].z = act[60];
            taco_shadow_disp(base, side);
            v = base->timer;
        }
        if ((u32)v >= 129U) {
            base[0].flags = 0;
            base[1].flags = 0;
        }
    }
}
