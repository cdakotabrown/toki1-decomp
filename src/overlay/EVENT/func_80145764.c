/* Address: 0x80145764 (cross-prologue head 0x80145704 absorbed) */
/* Size: 280 bytes (70 instructions) */
/* @SymbolStart: 0x80145704 */
/* @SymbolSize: 280 */
#include "types.h"

extern volatile s16 D_800B0C0E;
extern volatile s16 D_800B0C12;
extern volatile s16 D_800B0C16;
extern s32 D_801205CC;
extern s32 D_801205E8;
extern s32 D_80120604;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_801238AC;
extern s32 D_801238B0;
extern s32 D_801238B4;
extern s16 D_801238B8;
extern s32 D_801238BC;
extern s32 D_801238C0;
extern s32 D_801238C4;
extern s32 D_801238C8;
extern s32 D_801238CC;
extern s32 D_801238D0;
extern s32 D_801238D4;

extern void func_8004C250(void);
extern void func_80108070(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_80011DFC(void);

void func_80145764(void) {
    D_80120678 = D_801205CC;
    D_8012067C = D_801205E8;
    D_80120680 = D_80120604;
    D_800B0C0E = D_800B0C0E + 1;
    D_800B0C12 = D_800B0C12 + 1;
    D_800B0C16 = D_800B0C16 - 10;

    func_8004C250();
    func_80108070();
    func_80012D64(D_801238BC, 17, 1, 2, 0);
    func_8004C46C(D_801238C0, D_801238C4, D_801238C8, D_801238CC, D_801238D0, D_801238D4);
    func_8004C6B0(D_801238B0, D_801238B4, D_801238AC, D_801238B8);
    func_80011DFC();
}
