/* @PostProc: swap_instructions --offset1 0x144 --offset2 0x148 */
/* @PostProc: swap_instructions --offset1 0x148 --offset2 0x14c */
/* @PostProc: patch_insn_reg_field 0x134:rt:v0:v1 0x13c:rt:v0:v1 0x14c:rt:v0:v1 */
/* @SymbolStart: 0x8013EE54 */
/* @SymbolSize: 672 */
/* Address: 0x8013EE54 (hoisted entry block; .sym draws the start at 0x8013EE8C) */
/* Size: 672 bytes (168 instructions) including the absorbed entry block */
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
    u8    unk03;
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
    Fix32 x;
    Fix32 y;
    s32   unk2C;
    s32   unk30;
    s32   unk34[4];
} SparkRec;

typedef struct {
    u8       unk0000[6528];
    SparkRec base[2];
    SparkRec ent[20];
} SparkBlock;

extern SparkBlock D_800E9620;

extern s32 D_80120450;
extern s32 D_8012046C;
extern s32 D_80120488;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;

extern volatile s16 D_800B0B0A;
extern volatile s16 D_800B0B0E;
extern volatile s16 D_800B0B12;

extern s32 D_801226D8;
extern s32 D_801226DC;
extern s32 D_801226E0;
extern s16 D_801226E4;
extern s32 D_801226E8;
extern s32 D_801226EC;
extern s32 D_801226F0;
extern s32 D_801226F4;
extern s32 D_801226F8;
extern s32 D_801226FC;
extern s32 D_80122700;

extern u8  D_8012274C[];
extern s16 D_80122754[];
extern s16 D_80122764[];
extern u8  D_80122774[];

extern void func_8004C250();
extern void func_80102610();
extern void func_80012D64();
extern void func_8004C46C();
extern void func_8004C6B0();
extern s32  func_800789A0();
extern void func_80011DFC();

void func_8013EE8C(void)
{
    s16 i;

    D_80120678 = D_80120450;
    D_8012067C = D_8012046C;
    D_80120680 = D_80120488;
    D_800B0B0A += 2;
    D_800B0B0E += 1;
    D_800B0B12 -= 20;

    func_8004C250();
    func_80102610();
    func_80012D64(D_801226E8, 17, 1, 2, 0);
    func_8004C46C(D_801226EC, D_801226F0, D_801226F4, D_801226F8,
                  D_801226FC, D_80122700);
    func_8004C6B0(D_801226DC, D_801226E0, D_801226D8, D_801226E4);

    D_800E9620.base[0].unk04 = D_800E9620.base[1].unk04 = 8;

    for (i = 0; i < 8; i++) {
        D_8012274C[i] = 0;
    }

    for (i = 0; i < 8; i++) {
        D_800E9620.ent[i] = D_800E9620.base[0];
        D_800E9620.ent[i].unk16 = 5;
        D_800E9620.ent[i].x.h.hi = D_80122754[i] - 160;
        D_800E9620.ent[i].y.h.hi = D_80122764[i] - 120;
        D_80122774[i] = func_800789A0() % 120;
    }

    D_800E9620.ent[0].unk16 = 4;
    func_80011DFC();
}
