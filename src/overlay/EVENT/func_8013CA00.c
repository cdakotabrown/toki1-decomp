/* Address: 0x8013CA00 (cross-prologue head 0x8013C9A0 absorbed) */
/* Size: 280 bytes (70 instructions) */
/* @SymbolStart: 0x8013C9A0 */
/* @SymbolSize: 280 */
#include "types.h"

extern volatile s16 D_800B0B72;
extern volatile s16 D_800B0B76;
extern volatile s16 D_800B0B7A;
extern s32 D_801204E4;
extern s32 D_80120500;
extern s32 D_8012051C;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_80121DD0;
extern s32 D_80121DD4;
extern s32 D_80121DD8;
extern s16 D_80121DDC;
extern s32 D_80121DE0;
extern s32 D_80121DE4;
extern s32 D_80121DE8;
extern s32 D_80121DEC;
extern s32 D_80121DF0;
extern s32 D_80121DF4;
extern s32 D_80121DF8;

extern void func_8004C250(void);
extern void func_800FFEA0(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_80011DFC(void);

void func_8013CA00(void) {
    D_80120678 = D_801204E4;
    D_8012067C = D_80120500;
    D_80120680 = D_8012051C;
    D_800B0B72 = D_800B0B72 + 3;
    D_800B0B76 = D_800B0B76 + 2;
    D_800B0B7A = D_800B0B7A - 20;

    func_8004C250();
    func_800FFEA0();
    func_80012D64(D_80121DE0, 17, 1, 2, 0);
    func_8004C46C(D_80121DE4, D_80121DE8, D_80121DEC, D_80121DF0, D_80121DF4, D_80121DF8);
    func_8004C6B0(D_80121DD4, D_80121DD8, D_80121DD0, D_80121DDC);
    func_80011DFC();
}
