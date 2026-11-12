/* Address: 0x80157E00, Size: 796 bytes (199 insns) */
/* `@CompileEnv: IDO52_CONST_INREG_VALUES=2148439584` and a banked wall reading */
#include "types.h"

typedef union {
    s32 v;
    struct {
        s16 lo;
        s16 hi;
    } h;
} Fix32;

typedef struct {
    u8    unk00;
    u8    unk01;
    u8    unk02;
    u8    unk03;
    u8    unk04[3];
    u8    unk07;
    u8    unk08[4];
    s32   unk0C;
    s32   unk10;
    s16   unk14;
    s16   unk16;
    s16   unk18;
    s16   unk1A;
    s16   unk1C;
    s16   unk1E;
    s16   unk20;
    s16   unk22;
    Fix32 x;
    Fix32 y;
    s32   unk2C;
    s32   unk30;
    s32   unk34;
    s32   unk38;
    s32   unk3C;
    u8    unk40[3];
    u8    unk43;
} SparkRec;

typedef struct {
    u8       unk0000[6528];
    SparkRec base[2];
    SparkRec ent[20];
} SparkBlock;

extern SparkBlock D_800E9620;

extern Fix32 D_80125868[];

extern u8 D_8012531C;
extern u8 *D_801252DC;
extern u8 *D_801252E0;
extern s32 D_801252C4;
extern s32 D_801252CC;
extern s32 D_801252BC;
extern s32 D_801252C8;
extern s32 D_801252D0;
extern s32 D_801252C0;

extern void func_80012D64(u8 *, s32, s32, s32, s32);
extern void func_800189C4(s32);
extern s32 func_800789A0();
extern void func_80011DFC(void);

void func_80157E00(void)
{
    s32 scale;
    s32 a;
    s32 b;
    s16 dir;
    s16 i;

    if (D_8012531C == 2) {
        func_80012D64(D_801252DC, 18, 1, 2, 0);
    } else {
        func_80012D64(D_801252E0, 18, 1, 2, 0);
    }

    for (i = 0; i < 20; i++) {
        func_800189C4(i + 98);
        D_800E9620.ent[i].unk38 = 0x01000000;
        D_800E9620.ent[i].unk01 = 13;
        D_800E9620.ent[i].unk02 = 1;
        D_800E9620.ent[i].unk03 = 4;
        if (D_8012531C == 2) {
            D_800E9620.ent[i].unk0C = D_801252C4;
            D_800E9620.ent[i].unk10 = D_801252CC;
            D_800E9620.ent[i].unk34 = D_801252BC;
        } else {
            D_800E9620.ent[i].unk0C = D_801252C8;
            D_800E9620.ent[i].unk10 = D_801252D0;
            D_800E9620.ent[i].unk34 = D_801252C0;
        }

        D_800E9620.ent[i].unk16 = func_800789A0() % 8;
        D_800E9620.ent[i].unk43 = 18;

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
        D_800E9620.ent[i].y.h.hi = func_800789A0() % 160 - 120;
        D_800E9620.ent[i].unk30 = 32768 / (dir + 1);
    }

    func_80011DFC();
}
