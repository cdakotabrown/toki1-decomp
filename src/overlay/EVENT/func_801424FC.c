/* Address: 0x801424FC (cross-prologue head 0x8014249C absorbed) */
/* Size: 280 bytes (70 instructions) */
/* @SymbolStart: 0x8014249C */
/* @SymbolSize: 280 */
#include "types.h"

extern volatile s16 D_800B0BDA;
extern volatile s16 D_800B0BDE;
extern volatile s16 D_800B0BE2;
extern s32 D_80120580;
extern s32 D_8012059C;
extern s32 D_801205B8;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_80122FF4;
extern s32 D_80122FF8;
extern s32 D_80122FFC;
extern s16 D_80123000;
extern s32 D_80123004;
extern s32 D_80123008;
extern s32 D_8012300C;
extern s32 D_80123010;
extern s32 D_80123014;
extern s32 D_80123018;
extern s32 D_8012301C;

extern void func_8004C250(void);
extern void func_80105300(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_80011DFC(void);

void func_801424FC(void) {
    D_80120678 = D_80120580;
    D_8012067C = D_8012059C;
    D_80120680 = D_801205B8;
    D_800B0BDA = D_800B0BDA + 1;
    D_800B0BDE = D_800B0BDE + 1;
    D_800B0BE2 = D_800B0BE2 - 20;

    func_8004C250();
    func_80105300();
    func_80012D64(D_80123004, 17, 1, 2, 0);
    func_8004C46C(D_80123008, D_8012300C, D_80123010, D_80123014, D_80123018, D_8012301C);
    func_8004C6B0(D_80122FF8, D_80122FFC, D_80122FF4, D_80123000);
    func_80011DFC();
}
