/* Address: 0x801340D0 (cross-prologue head 0x80134070 absorbed) */
/* Size: 280 bytes (70 instructions) */
/* @SymbolStart: 0x80134070 */
/* @SymbolSize: 280 */
#include "types.h"

extern volatile s16 D_800B0BA6;
extern volatile s16 D_800B0BAA;
extern volatile s16 D_800B0BAE;
extern s32 D_80120538;
extern s32 D_80120550;
extern s32 D_80120568;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_80120740;
extern s32 D_80120744;
extern s32 D_80120748;
extern s16 D_8012074C;
extern s32 D_80120750;
extern s32 D_80120754;
extern s32 D_80120758;
extern s32 D_8012075C;
extern s32 D_80120760;
extern s32 D_80120764;
extern s32 D_80120768;

extern void func_8004C250(void);
extern void func_800F74E0(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_80011DFC(void);

void func_801340D0(void) {
    D_80120678 = D_80120538;
    D_8012067C = D_80120550;
    D_80120680 = D_80120568;
    D_800B0BA6 = D_800B0BA6 + 1;
    D_800B0BAA = D_800B0BAA + 1;
    D_800B0BAE = D_800B0BAE - 20;

    func_8004C250();
    func_800F74E0();
    func_80012D64(D_80120750, 17, 1, 2, 0);
    func_8004C46C(D_80120754, D_80120758, D_8012075C, D_80120760, D_80120764, D_80120768);
    func_8004C6B0(D_80120744, D_80120748, D_80120740, D_8012074C);
    func_80011DFC();
}
