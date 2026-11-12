/* Address: 0x80138300 */
/* Size: 364 bytes (91 instructions) */
#include "types.h"

extern u8 D_800EB06D;
extern u8 D_800EB06E;
extern u8 D_800EB06F;
extern u8 D_800EB070;
extern u8 D_800EB071;
extern u8 D_800EB072;
extern u8 D_800EB073;
extern s32 D_800EB078;
extern s32 D_800EB07C;
extern s16 D_800EB080;
extern s16 D_800EB082;
extern s16 D_800EB084;
extern s16 D_800EB092;
extern s16 D_800EB096;
extern s32 D_800EB0A0;
extern s32 D_800EB0A4;
extern u8 D_800EB0AF;
extern s32 D_800B1C70;

extern s32 D_801211E0;
extern s32 D_801211E4;
extern s32 D_801211E8;
extern s16 D_801211EC;
extern s32 D_801211F0;
extern s32 D_801211F4;
extern s32 D_801211F8;
extern s32 D_801211FC;
extern s32 D_80121200;
extern s32 D_80121204;
extern s32 D_80121208;

extern void func_800FBBF0();
extern void func_80012D64();
extern void func_8004C46C();
extern void func_8004C6B0();
extern void func_800189C4();
extern void func_80011DFC();

void func_80138300(void) {
    func_800FBBF0();
    func_80012D64(D_801211F0, 17, 1, 2, 0);
    func_8004C46C(D_801211F4, D_801211F8, D_801211FC, D_80121200,
                  D_80121204, D_80121208);
    func_8004C6B0(0, 0, 0, 0);
    func_800189C4(99);
    D_800EB06D = 10;
    D_800EB06E = 0;
    D_800EB0A4 = 0x41000000;
    D_800EB06F = 4;
    D_800EB078 = D_801211E4;
    D_800EB07C = D_801211E8;
    D_800EB0A0 = D_801211E0;
    D_800EB080 = D_801211EC;
    D_800EB082 = 4;
    D_800EB084 = 0;
    D_800EB072 = 1;
    D_800EB0AF = 17;
    D_800EB092 = -160;
    D_800EB096 = -120;
    D_800EB073 = 0;
    D_800EB071 = 0;
    D_800EB070 = 0;
    D_800B1C70 = -1;
    func_80011DFC();
}
