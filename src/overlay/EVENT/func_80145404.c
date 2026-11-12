/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=24:t4,35:t6 */
/* @PostProc: patch_stack_complete --old 128 --new 136 */
/* Address: 0x80145404, Size: 768 bytes (192 insns) */
#include "types.h"

typedef struct {
    s32 pad0[6];
    s16 f24;
    s16 pad1[6];
    s16 f38;
    s16 pad2;
    s16 f42;
    s32 pad3[6];
} Rec68;

typedef struct {
    s16 v[10];
} Tbl20;

extern void func_80108330(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_80011DFC(void);

extern u8 D_800B0860[];
extern s8 D_800EAFA1;
extern s8 D_800EAFA4;
extern s8 D_800EAFE5;
extern s8 D_800EAFE8;
extern s8 D_800EB029;
extern s8 D_800EB02A;
extern u8 D_800EB02F;
extern s16 D_800EB040;
extern s16 D_800EB04E;
extern s16 D_800EB052;

extern Rec68 D_800EAFA0;
extern Rec68 D_800EB028;
extern Rec68 D_800E9620[];

extern Tbl20 D_80123B7C;
extern Tbl20 D_80123B90;
extern Tbl20 D_80123BA4;

extern s32 D_8012395C;
extern s32 D_80123960;
extern s32 D_80123964;
extern s16 D_80123968;
extern s32 D_8012396C;
extern s32 D_80123970;
extern s32 D_80123974;
extern s32 D_80123978;
extern s32 D_8012397C;
extern s32 D_80123980;
extern s32 D_80123984;

void func_80145404(void)
{
    Tbl20 t0;
    Tbl20 t1;
    Tbl20 t2;
    s16 i;

    t0 = D_80123B7C;
    t1 = D_80123B90;
    t2 = D_80123BA4;

    func_80108330();
    func_80012D64(D_8012396C, 17, 1, 2, 0);
    func_8004C46C(D_80123970, D_80123974, D_80123978, D_8012397C,
                  D_80123980, D_80123984);

    for (i = 0; i < 6; i++) {
        D_800B0860[4241 + i] = 12;
    }

    func_8004C6B0(D_80123960, D_80123964, D_8012395C, D_80123968);

    D_800EAFE8 = 8;
    D_800EAFA4 = 8;
    D_800EAFE5 = 12;
    D_800EAFA1 = 12;

    D_800EB028 = D_800EAFA0;

    D_800EB029 = 11;
    D_800EB02A = 64;
    D_800EB040 = 20;
    D_800EB04E = -6;
    D_800EB052 = -48;
    D_800EB02F = 128;

    for (i = 0; i < 10; i++) {
        D_800E9620[99 + i] = D_800EB028;
        D_800E9620[99 + i].f24 = t0.v[i];
        D_800E9620[99 + i].f38 = t1.v[i] - 160;
        D_800E9620[99 + i].f42 = t2.v[i] - 120;
    }

    func_80011DFC();
}
