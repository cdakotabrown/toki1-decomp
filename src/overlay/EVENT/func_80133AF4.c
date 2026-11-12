/* Address: 0x80133AA8 (cross-prologue head absorbed) .. 0x80133AF4 */
/* Size: 492 bytes (123 instructions) */
/* @SymbolStart: 0x80133AA8 */
/* @SymbolSize: 492 */
#include "types.h"

extern s32 D_8012052C;
extern s32 D_80120544;
extern s32 D_8012055C;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern volatile s16 D_800B0BAA;
extern volatile s16 D_800B0BAE;

extern u8 D_800EB029;
extern u8 D_800EB02A;
extern u8 D_800EB02B;
extern u8 D_800EB02D;
extern u8 D_800EB02E;
extern u8 D_800EB02F;
extern s32 D_800EB034;
extern s32 D_800EB038;
extern s16 D_800EB03C;
extern s16 D_800EB040;
extern s16 D_800EB04E;
extern s16 D_800EB052;
extern s32 D_800EB05C;
extern s32 D_800EB060;
extern u8 D_800EB06B;
extern s32 D_801206BC;
extern s32 D_801206C0;
extern s32 D_801206C4;
extern s16 D_801206C8;
extern s32 D_801206CC;
extern s32 D_801206D0;
extern s32 D_801206D4;
extern s32 D_801206D8;
extern s32 D_801206DC;
extern s32 D_801206E0;
extern s32 D_801206E4;
extern s32 D_80120798;
extern s32 D_8012079C;
extern s32 D_801207A0;
extern s16 D_801207A4;
extern s32 D_801207A8;
extern s16 D_801207AC;

extern void func_8004C250(void);
extern void func_800F72D0(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_800F7640(void);
extern void func_800189C4(s32);
extern void func_80011DFC(void);

void func_80133AF4(void) {
    D_80120678 = D_8012052C;
    D_8012067C = D_80120544;
    D_80120680 = D_8012055C;
    D_800B0BAA = D_800B0BAA + 3;
    D_800B0BAE = D_800B0BAE - 10;

    func_8004C250();
    func_800F72D0();
    func_80012D64(D_801206CC, 17, 1, 2, 0);
    func_8004C46C(D_801206D0, D_801206D4, D_801206D8, D_801206DC, D_801206E0, D_801206E4);
    func_8004C6B0(D_801206C0, D_801206C4, D_801206BC, D_801206C8);
    func_800F7640();
    func_80012D64(D_801207A8, 18, 1, 3, 0);
    func_800189C4(98);
    D_800EB029 = 12;
    D_800EB02A = 64;
    D_800EB060 = 0x41000000;
    D_800EB02B = 4;
    D_800EB034 = D_8012079C;
    D_800EB05C = D_80120798;
    D_800EB038 = D_801207A0;
    D_800EB03C = D_801207A4;
    D_800EB040 = 0;
    D_800EB02E = 1;
    D_800EB06B = 18;
    D_800EB04E = -160;
    D_800EB052 = -120;
    D_800EB02F = 0;
    D_800EB02D = 0;
    D_801207AC = 0;
    func_80011DFC();
}
