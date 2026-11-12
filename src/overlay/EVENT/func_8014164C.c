/* Address: 0x8014164C */
/* Size: 484 bytes (121 instructions) */
#include "types.h"

typedef struct {
    s32 flags;
    s16 x;
    s16 y;
    s16 unk08;
    s16 unk0A;
    s16 unk0C;
    u8  unk0E;
    u8  unk0F;
    s16 unk10;
    s16 unk12;
    u8  r;
    u8  g;
    u8  b;
    u8  pad17;
    s16 unk18;
    s16 unk1A;
    s16 sx;
    s16 sy;
    s32 unk20;
} EventObj;

extern void func_800137AC(s32, s32, s32, s32, s32);
extern void func_801050D0(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern s32 func_800789A0(void);
extern void func_80011DFC(void);

extern s32 D_800B1C1C;
extern u8 D_800B188D[];
extern EventObj D_800EC190[];
extern s32 D_80120C04;
extern s32 D_80120C20;
extern s16 D_80120CE8[];
extern s16 D_80120D08[];

void func_8014164C(void)
{
    EventObj *p;
    s16 i;

    D_800B1C1C = 1;
    func_800137AC(320, 0, 64, 128, D_80120C20);
    func_801050D0();
    func_80012D64(D_80120C04, 18, 1, 3, 0);

    for (i = 4; i < 14; i++) {
        p = &D_800EC190[i];
        p->flags = (s32)0xC1000000;
        p->x = func_800789A0() % 84 - 159;
        p->y = func_800789A0() % 130 - 95;
        p->unk08 = 8;
        p->unk0A = 24;
        p->unk0C = 5;
        p->unk0E = 0;
        p->unk0F = 0;
        p->unk10 = 0;
        p->unk12 = 498;
        p->r = 0;
        p->g = 0;
        p->b = 0;
        p->unk18 = 0;
        p->unk1A = 0;
        p->sx = 4096;
        p->sy = 4096;
        p->unk20 = 0;
        D_800B188D[i] = 140;
        D_80120CE8[i] = 0;
        D_80120D08[i] = 0;
    }

    func_80011DFC();
}
