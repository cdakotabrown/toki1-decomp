/* @PostProc: scheduler_normalizer --block-aware --reloc-symbol-fp --const-fp --load-fp --store-fp --fence 160-163,164-175,176-176,177-177,212-212,215-215 */
/* @PostProc: patch_insn_reg_field 0x14:rt:v1:a1 0x18:rs:v1:a1 0x18:rt:v1:a1 0x40:rs:v1:a1 0x50:rt:t0:a2 0x54:rs:t0:a2 0x54:rt:t0:a2 0x5c:rs:t0:a2 0x60:rt:t6:t8 0x68:rs:v1:a1 0x68:rt:v1:a1 0x6c:rt:t6:t8 0x70:rd:t7:t6 0x70:rt:v1:a1 0x7c:rd:v1:a1 0x7c:rt:t7:t6 0x8c:rs:t0:a2 0x90:rt:t4:t3 0x94:rs:v1:a1 0x94:rt:v1:a1 0x98:rd:t1:t0 0x98:rt:v1:a1 0x9c:rt:t3:t2 0xa4:rd:v1:a1 0xa4:rt:t1:t0 0xac:rt:t3:t2 0xb0:rs:t4:t3 0xb0:rt:t4:t3 0xb8:rs:t4:t3 0xc4:rs:t0:a2 0xc8:rs:v1:a1 0xcc:rd:t5:t4 0xd0:rs:t5:t4 0xdc:rs:v1:a1 0xec:rt:v1:a1 0xf0:rs:v1:a1 0xf0:rt:v1:a1 0xf8:rt:v1:a1 0xfc:rt:v1:a1 0x108:rd:a0:v1 0x120:rd:t1:t0 0x124:rs:t1:t0 0x124:rt:t2:t1 0x12c:rs:t2:t1 0x130:rs:a0:v1 0x130:rt:t1:t0 0x13c:rd:t4:t3 0x13c:rt:v1:a1 0x140:rt:t3:t2 0x144:rs:t4:t3 0x144:rt:t3:t2 0x150:rt:v1:a1 0x154:rt:v1:a1 0x158:rs:v1:a1 0x158:rt:v1:a1 0x15c:rt:t6:t5 0x160:rs:t6:t5 0x160:rt:t6:t5 0x164:rd:t5:t4 0x164:rt:v1:a1 0x168:rd:a0:v1 0x168:rs:t5:t4 0x168:rt:t6:t5 0x170:rt:t7:t6 0x174:rs:t7:t6 0x17c:rs:t7:t6 0x17c:rt:t7:t6 0x180:rs:t7:t6 0x180:rt:t8:t7 0x188:rs:a0:v1 0x188:rt:t8:t7 0x194:rd:t1:t9 0x194:rt:v1:a1 0x198:rt:t9:t8 0x19c:rs:t1:t9 0x19c:rt:t9:t8 0x1a8:rt:v1:a1 0x1ac:rt:v1:a1 0x1b0:rs:v1:a1 0x1b0:rt:v1:a1 0x1b4:rt:t3:t1 0x1b8:rs:t3:t1 0x1b8:rt:t3:t1 0x1bc:rd:t2:t0 0x1bc:rt:v1:a1 0x1c0:rd:a0:v1 0x1c0:rs:t2:t0 0x1c0:rt:t3:t1 0x1c8:rt:t4:t2 0x1cc:rs:t4:t2 0x1d4:rs:t4:t2 0x1d4:rt:t4:t2 0x1d8:rs:t4:t2 0x1d8:rt:t5:t3 0x1dc:rs:a0:v1 0x1dc:rt:t5:t3 0x1e0:rs:a0:v1 0x1e4:rt:t1:t8 0x1e8:rt:t6:t4 0x1ec:rs:t6:t4 0x1ec:rt:t6:t4 0x1f0:rd:t7:t5 0x1f0:rt:t6:t4 0x1f4:rd:t8:t6 0x1f4:rs:t7:t5 0x1f8:rs:t8:t6 0x1f8:rt:t9:t7 0x1fc:rs:t1:t8 0x1fc:rt:t9:t7 0x200:rd:t3:t0 0x200:rt:v1:a1 0x204:rs:t9:t7 0x214:rs:t9:t7 0x21c:rs:t1:t8 0x22c:rt:t0:a2 0x230:rt:t3:t0 0x234:rs:t0:a2 0x234:rt:t0:a2 0x238:rd:t2:t9 0x23c:rt:t2:t9 0x248:rt:v1:a1 0x24c:rs:v1:a1 0x24c:rt:a3:a0 0x254:rs:a3:a0 0x274:rt:t9:t8 0x278:rs:t9:t8 0x278:rt:t9:t8 0x27c:rd:t1:t6 0x280:rs:t1:t6 0x280:rt:t9:t8 0x284:rt:t2:t7 0x288:rd:t4:t1 0x28c:rs:t0:a2 0x28c:rt:t2:t7 0x290:rd:t4:t1 0x290:rs:t4:t1 0x294:rt:t5:t2 0x298:rs:t5:t2 0x298:rt:t5:t2 0x29c:rd:t4:t1 0x29c:rt:t4:t1 0x2a0:rt:t8:t3 0x2a4:rs:t8:t3 0x2a4:rt:t8:t3 0x2a8:rs:t4:t1 0x2a8:rt:t5:t2 0x2ac:rt:t6:t4 0x2b0:rs:t8:t3 0x2b4:rd:t7:t5 0x2b4:rs:t6:t4 0x2c0:rt:t7:t5 0x2c8:rt:t7:t2 0x2cc:rt:t6:t5 0x2d0:rs:t6:t2 0x2d0:rt:t6:t2 0x2d4:rs:t7:t5 0x2d4:rt:t7:t5 0x2d8:rt:t6:t2 0x2e0:rt:t7:t5 0x2e4:rt:t8:t3 0x2e8:rs:t2:t7 0x2f4:rd:t3:t9 0x2f8:rs:t3:t9 0x300:rt:t4:t0 0x308:rs:t4:t0 0x308:rt:t5:t1 0x30c:rt:t5:t1 0x314:rt:a1:t3 0x318:rt:a2:t6 0x31c:rd:a0:t4 0x31c:rs:a1:t3 0x320:rt:t3:t2 0x324:rt:a0:t4 0x328:rd:t8:t7 0x328:rs:a2:t6 0x328:rt:a0:t4 0x32c:rt:t2:t1 0x330:rd:t1:t9 0x330:rt:t8:t7 0x334:rd:t9:t0 0x334:rs:t1:v1 0x334:rt:v1:t9 0x338:rs:t3:t2 0x33c:rt:t9:t0 0x344:rt:t2:t1 0x35c:rt:t4:t8 0x360:rt:t4:t8 0x364:rd:t5:t3 0x368:rs:t5:t3 0x368:rt:t6:t4 0x36c:rt:t6:t4 0x370:rt:t7:t5 0x378:rt:t7:t5 0x37c:rt:t8:t6 0x384:rt:t8:t6 0x38c:rt:t1:t7 0x390:rs:t1:t7 0x398:rs:t1:t7 0x398:rt:t1:t7 0x39c:rt:t1:t7 0x3a0:rt:t2:t0 0x3a8:rs:t2:t0 0x3b0:rt:t3:t1 0x3b4:rd:t4:t2 0x3bc:rs:t4:t2 0x3bc:rt:t3:t1 0x3c4:rt:t5:t3 0x3c8:rs:t5:t3 0x3d0:rs:t5:t3 0x3d0:rt:t5:t3 0x3d4:rs:t5:t3 0x3d4:rt:t6:t4 0x3dc:rt:t6:t4 0x3e0:rt:t7:t5 0x3e4:rd:t8:t6 0x3ec:rs:t8:t6 0x3ec:rt:t7:t5 0x3f4:rt:t1:t8 0x3f8:rs:t1:t8 0x400:rs:t1:t8 0x400:rt:t1:t8 0x404:rs:t1:t8 0x404:rt:t9:t7 0x408:rt:t9:t7 0x410:rt:t6:t3 0x414:rt:t2:t9 0x418:rs:t2:t9 0x418:rt:t2:t9 0x41c:rd:t3:t0 0x41c:rt:t2:t9 0x420:rd:t4:t1 0x420:rs:t3:t0 0x424:rs:t4:t1 0x424:rt:t5:t2 0x428:rs:t6:t3 0x428:rt:t5:t2 0x42c:rt:v1:a1 0x430:rs:v1:a1 0x430:rt:v1:a1 0x434:rt:t0:a2 0x438:rs:t5:t2 0x448:rs:t5:t2 0x450:rs:t6:t3 0x45c:rs:v1:a1 0x45c:rt:a3:a0 0x460:rd:t7:t4 0x464:rt:t7:t4 0x468:rs:t0:a2 0x468:rt:t0:a2 0x474:rd:t8:t5 0x478:rt:t8:t5 0x47c:rt:a3:a0 0x488:rs:t0:a2 0x488:rt:t0:a2 0x490:rt:t0:a2 */
/* func_80142018 -- GYOZI overlay, Address: 0x80142018, Size: 1204 bytes (301 insns) */
#include "types.h"

typedef struct {
    s32 unk00;
    s16 unk04;
    s16 unk06;
    u8 unk08[6];
    u8 unk0E;
    u8 unk0F[5];
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17[13];
} GyoziEnt;

typedef union {
    s32 w;
    struct {
        u16 lo;
        s16 hi;
    } h;
} GyoziFx;

extern s32 func_800BCE40(void);

extern u16 D_800D9250;
extern u8 D_800F53A0[];
extern GyoziEnt D_8012CA40[];
extern s16 D_8012CAD4;
extern volatile u8 D_801317EB;
extern s32 D_80148BF0[];
extern GyoziFx D_80148CB8[];
extern s32 D_80148D80[];
extern s16 D_80148E48[];
extern volatile s16 D_80148EAC;
extern u32 D_80148EB0;

s32 func_80142018(void)
{
    s16 n;
    s16 m;
    s16 i;
    s32 r;
    s32 v;
    s32 a;
    s32 b;
    s32 c;

    n = D_80148EAC;
    if (n == 0) {
        return 0;
    }
    if (D_80148EB0 == 30) {
        D_8012CAD4 = 180;
        n = n + 1;
        D_80148E48[0] = 1;
    } else if (D_80148EB0 == 90) {
        n = n + 1;
        D_8012CAD4 = 40;
    } else if (D_800D9250 >= 6U && D_80148EB0 % 20 == 0 && n < 50) {
        D_80148EAC = n + 1;
        r = func_800BCE40() % 2;
        m = D_80148EAC;
        D_8012CA40[m + 3].unk0E = r * 8;
        if (D_8012CA40[m + 3].unk0E == 8) {
            D_800F53A0[m + 4216] = 12;
            D_80148EAC = m;
            r = func_800BCE40() % 2;
            n = D_80148EAC;
            D_80148E48[n - 1] = r + 3;
        } else {
            D_800F53A0[m + 4216] = 9;
            D_80148EAC = m;
            r = func_800BCE40() % 2;
            n = D_80148EAC;
            D_80148E48[n - 1] = r + 1;
        }
        v = D_80148E48[n - 1];
        D_80148BF0[n - 1] = 0x10000 / (v + (v >= 3) * 2 + 1);
    }
    D_80148EAC = n;

    for (i = 4; i < n + 4; i++) {
        D_8012CA40[i].unk00 = D_8012CA40[i].unk00 & 0x7FFFFFFF;
        D_8012CA40[i].unk16 = D_801317EB;
        D_8012CA40[i].unk15 = D_8012CA40[i].unk16;
        D_8012CA40[i].unk14 = D_8012CA40[i].unk16;
        if (D_80148EB0 % D_80148E48[i - 4] == 0) {
            D_8012CA40[i].unk06 = D_8012CA40[i].unk06 + 1;
        }
        a = D_80148BF0[i - 4];
        b = D_80148CB8[i - 4].w;
        c = D_80148D80[i - 4];
        D_80148CB8[i - 4].w = b + a;
        D_80148BF0[i - 4] = a + ((c - (b + a)) >> 8);
        D_8012CA40[i].unk04 = D_80148CB8[i - 4].h.hi;
        if (D_8012CA40[i].unk06 >= 41) {
            r = func_800BCE40() % 320;
            D_80148CB8[i - 4].h.lo = 0;
            D_8012CA40[i].unk06 = -120;
            D_8012CA40[i].unk04 = r - 159;
            D_80148CB8[i - 4].h.hi = D_8012CA40[i].unk04;
            D_80148D80[i - 4] = D_80148CB8[i - 4].w;
            r = func_800BCE40() % 2;
            D_8012CA40[i].unk0E = r * 8;
            if (D_8012CA40[i].unk0E == 8) {
                D_800F53A0[i + 4213] = 12;
                r = func_800BCE40() % 2;
                D_80148E48[i - 4] = r + 3;
            } else {
                D_800F53A0[i + 4213] = 9;
                r = func_800BCE40() % 2;
                D_80148E48[i - 4] = r + 1;
            }
            v = D_80148E48[i - 4];
            n = D_80148EAC;
            D_80148BF0[i - 4] = 0x10000 / (v + (v >= 3) * 2 + 1);
        }
    }
    D_80148EB0 = D_80148EB0 + 1;
}
