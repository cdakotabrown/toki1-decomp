/* Address: 0x8013D314 */
/* Size: 308 bytes (77 insns) */
/* @PostProc: patch_insn_reg_field 0x44:rt:s3:s0 0x48:rt:s0:s3 0x58:rt:t1:t0 0x60:rs:t1:t0 0x80:rt:t2:t1 0x84:rt:t5:t4 0x8c:rs:t2:t1 0x90:rs:t5:t4 0x94:rd:t6:t5 0x98:rd:t3:t2 0xa0:rt:t3:t2 0xa4:rt:t6:t5 0xb0:rt:v0:v1 0xb4:rt:v1:t7 0xb8:rt:a0:t8 0xbc:rt:a1:t0 0xc0:rt:a2:t1 0xc4:rt:a3:t3 0xc8:rt:t0:t4 0xcc:rs:v0:v1 0xcc:rt:t3:a0 0xd0:rs:t3:a0 0xd0:rt:t4:a0 0xd4:rs:v0:v1 0xd4:rt:t2:t6 0xd8:rd:t8:t9 0xd8:rs:v1:t7 0xd8:rt:a0:t8 0xdc:rd:t9:t2 0xdc:rs:a1:t0 0xdc:rt:a2:t1 0xe0:rd:t1:t5 0xe0:rs:a3:t3 0xe0:rt:t0:t4 0xe4:rt:t8:t9 0xe8:rt:t9:t2 0xec:rt:t1:t5 0xf0:rs:t4:a0 0xf4:rt:t2:t6 0x100:rt:t6:a0 0x104:rs:t6:a0 0x104:rt:t7:a0 0x108:rt:t5:t7 0x10c:rs:t7:a0 0x110:rt:t5:t7 */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 pad0;
    s16 dx;
    s16 dy;
    s16 dz;
    s16 pad1;
    u8 pad2;
    u8 counter1;
    u8 counter2;
    u8 active;
} ParticleData;

extern ParticleData taco_spark2_buf;
extern ParticleData taco_pickup_tbl;

extern s32 rand(void);
extern void taco_spark_disp2(s16, s16, s16, s32);

void taco_spark2(void)
{
    s16 randX;
    s16 randY;
    ParticleData *p;
    ParticleData *end;
    s16 x, y, z, dx, dy, dz;
    u8 c1, c2;
    s32 cond;

    randX = (s16)(rand() & 0x7F);
    randY = (s16)(rand() & 0x3F);

    end = &taco_pickup_tbl;
    p = &taco_spark2_buf;
    do {
        if (p->active != 0) {
            taco_spark_disp2(p->x, p->y, p->z, p->counter1 - 1);

            taco_spark_disp2((s16)(p->x + randX), (s16)(p->y + randY), p->z, p->counter1 + 2);

            c2 = p->counter2;
            x = p->x;
            dx = p->dx;
            y = p->y;
            dy = p->dy;
            z = p->z;
            dz = p->dz;

            cond = ((u32)c2 < 5U) ^ 1;
            p->x = x + dx;
            p->y = y + dy;
            p->z = z + dz;
            p->counter2 = c2 + 1;

            if (cond) {
                c1 = p->counter1;
                p->counter2 = 0;
                p->counter1 = c1 + 1;
                if (((u32)c1 < 7U) ^ 1) {
                    p->active = 0;
                    p->counter1 = 0;
                }
            }
        }
        p++;
    } while (p != end);
}
