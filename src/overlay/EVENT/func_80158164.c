/* @PostProc: swap_instructions --offset1 0xd0 --offset2 0xd4 */
/* @PostProc: swap_instructions --offset1 0xd4 --offset2 0xd8 */
/* @PostProc: swap_instructions --offset1 0xe0 --offset2 0xe4 */
/* @PostProc: swap_instructions --offset1 0xec --offset2 0xf0 */
/* @PostProc: swap_instructions --offset1 0xf0 --offset2 0xf4 */
/* @PostProc: patch_insn_reg_field 0xb8:rt:a0:t7 0xc0:rs:a0:t7 0xc0:rt:t7:t8 0xc4:rt:t7:t8 0xc8:rt:a1:t8 0xcc:rt:a2:t7 0xd4:rd:t8:t9 0xd4:rs:a2:t7 0xd4:rt:a1:t8 0xd8:rt:a3:t0 0xdc:rt:t8:t9 0xe0:rt:t0:t1 0xe8:rd:v1:t4 0xe8:rs:t3:t0 0xe8:rt:a3:t3 0xec:rd:t4:t6 0xec:rt:v1:t4 0xf0:rs:t0:t1 0xf4:rt:v1:t4 0xfc:rt:t4:t6 0x11c:rd:t6:t0 0x120:rd:t7:t3 0x120:rt:t6:t0 0x124:rd:t8:t4 0x124:rt:t7:t3 0x128:rs:t8:t4 0x128:rt:t9:t5 0x12c:rt:t1:t6 0x130:rs:t1:t6 0x130:rt:t9:t5 0x138:rd:t5:t2 0x13c:rt:t2:t7 0x140:rs:t5:t2 0x144:rs:t9:t5 0x154:rs:t9:t5 0x15c:rs:t1:t6 0x168:rt:t2:t7 0x184:rd:t3:t8 0x188:rs:t3:t8 0x188:rt:t4:t9 0x18c:rt:t4:t9 0x190:rt:t5:t1 0x198:rt:t5:t1 0x1ac:rt:t6:t2 0x1b0:rs:t6:t2 0x1b8:rs:t6:t2 0x1b8:rt:t6:t2 0x1bc:rd:t7:t0 0x1bc:rt:t6:t2 0x1c0:rs:t7:t0 0x1c0:rt:t8:t3 0x1c8:rt:t1:t4 0x1cc:rs:t1:t4 0x1d4:rs:t1:t4 0x1d4:rt:t1:t4 0x1d8:rs:t8:t3 0x1d8:rt:t1:t4 0x1dc:rt:t6:t1 0x1e0:rt:t4:t8 0x1e4:rd:t9:t6 0x1e8:rs:t9:t6 0x1e8:rt:t2:t5 0x1f0:rs:t2:t5 0x20c:rs:t2:t5 0x218:rd:t3:t7 0x21c:rd:t5:t9 0x21c:rs:t4:t8 0x21c:rt:t3:t7 0x220:rt:t5:t9 0x224:rs:t6:t1 0x240:rs:t6:t1 0x24c:rd:t7:t2 0x250:rt:t7:t2 0x25c:rd:t8:t0 0x260:rt:t8:t0 */
/* Address: 0x80158164 */
/* Size: 656 bytes (164 instructions) */
#include "types.h"

typedef union {
    s32 v;
    struct {
        s16 lo;
        s16 hi;
    } h;
} Fix32;

typedef struct {
    u8    unk00[3];
    volatile u8 unk03;
    u8    unk04;
    u8    unk05[2];
    u8    unk07;
    u8    unk08[14];
    s16   unk16;
    s16   unk18;
    s16   unk1A;
    s16   unk1C;
    s16   unk1E;
    s16   unk20;
    s16   unk22;
    volatile Fix32 x;
    volatile Fix32 y;
    volatile s32 unk2C;
    volatile s32 unk30;
    s32   unk34[4];
} SparkRec;

typedef struct {
    u8       unk0000[6528];
    SparkRec base[2];
    SparkRec ent[20];
} SparkBlock;

extern SparkBlock D_800E9620;

extern Fix32 D_80125868[];

extern u8 D_8012531C;
extern u8 D_800B0896;

extern s32 func_800789A0();

s32 func_80158164(void)
{
    u32 flags;
    s32 xv;
    s32 yv;
    s32 u30;
    s32 u2c;
    s32 nu;
    s32 scale;
    s16 yhi;
    s32 a;
    s32 b;
    s16 dir;
    s16 i;

    if (D_8012531C == 1) {
        return 0;
    }
    if (D_8012531C != 2 && (D_800B0896 < 9U || D_800B0896 >= 12U)) {
        return 0;
    }

    for (i = 0; i < 20; i++) {
        flags = D_800E9620.ent[i].unk03;
        xv = D_800E9620.ent[i].x.v;
        D_800E9620.ent[i].unk03 = flags | 0x80;
        u30 = D_800E9620.ent[i].unk30;
        yv = D_800E9620.ent[i].y.v;
        u2c = D_800E9620.ent[i].unk2C;
        D_800E9620.ent[i].y.v = yv + u30;
        yhi = D_800E9620.ent[i].y.h.hi;
        nu = u2c + ((D_80125868[i].v - xv) >> 8);
        D_800E9620.ent[i].unk2C = nu;
        D_800E9620.ent[i].x.v = xv + nu;
        if (yhi < 41) {
            continue;
        }
        D_800E9620.ent[i].y.h.hi = -120;

        dir = func_800789A0() % 2;
        scale = 4096 / (dir * 2 + 2);
        D_800E9620.ent[i].unk1A = scale;
        D_800E9620.ent[i].unk1C = scale;
        D_800E9620.ent[i].unk07 = 128;
        D_800E9620.ent[i].x.h.hi = func_800789A0() % 300 - 150;
        D_80125868[i].v = D_800E9620.ent[i].x.v;
        a = func_800789A0();
        b = func_800789A0();
        D_80125868[i].h.hi += (((b % 2) * 2 - 1) * (a % 8) + 8) / (dir + 1);
        D_800E9620.ent[i].unk30 = 32768 / (dir + 1);
    }
}
