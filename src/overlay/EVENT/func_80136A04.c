/* @PostProc: patch_insn_reg_field 0x1a4:rd:v1:v0 0x1a8:rt:v1:v0 0x1bc:rt:v1:v0 */
/* Address: 0x80136A04 */
/* Size: 508 bytes (127 instructions) */
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

extern void func_800F9900(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s32);
extern s32 func_800789A0(void);
extern void func_80011DFC(void);

extern u8 D_800B188D[];
extern EventObj D_800EC190[];
extern s32 D_80120BF0;
extern s32 D_80120BF4;
extern s32 D_80120BF8;
extern s16 D_80120BFC;
extern s32 D_80120C00;
extern s32 D_80120C04;
extern s32 D_80120C08;
extern s32 D_80120C0C;
extern s32 D_80120C10;
extern s32 D_80120C14;
extern s32 D_80120C18;
extern s32 D_80120C1C;
extern s16 D_80120CE8[];
extern s16 D_80120D08[];

void func_80136A04(void)
{
    EventObj *p;
    s16 i;

    func_800F9900();
    func_80012D64(D_80120C00, 17, 1, 2, 0);
    func_8004C46C(D_80120C08, D_80120C0C, D_80120C10, D_80120C14,
                  D_80120C18, D_80120C1C);
    func_8004C6B0(D_80120BF4, D_80120BF8, D_80120BF0, D_80120BFC);
    func_80012D64(D_80120C04, 18, 1, 3, 0);

    for (i = 4; i < 19; i++) {
        p = &D_800EC190[i];
        p->flags = (s32)0xC1000000;
        p->x = func_800789A0() % 320 - 159;
        p->y = -120;
        p->unk08 = 8;
        p->unk0A = 8;
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
        D_800B188D[i] = 12;
        D_80120CE8[i] = 0;
        D_80120D08[i] = 0;
    }

    func_80011DFC();
}
