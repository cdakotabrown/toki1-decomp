/* Address: 0x80145AAC (cross-prologue head 0x80145A4C absorbed) */
/* Size: 280 bytes (70 instructions) */
/* @SymbolStart: 0x80145A4C */
/* @SymbolSize: 280 */
#include "types.h"

extern volatile s16 D_800B0C0E;
extern volatile s16 D_800B0C12;
extern volatile s16 D_800B0C16;
extern s32 D_801205D8;
extern s32 D_801205F4;
extern s32 D_80120610;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_80123930;
extern s32 D_80123934;
extern s32 D_80123938;
extern s16 D_8012393C;
extern s32 D_80123940;
extern s32 D_80123944;
extern s32 D_80123948;
extern s32 D_8012394C;
extern s32 D_80123950;
extern s32 D_80123954;
extern s32 D_80123958;

extern void func_8004C250(void);
extern void func_80108280(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_80011DFC(void);

void func_80145AAC(void) {
    D_80120678 = D_801205D8;
    D_8012067C = D_801205F4;
    D_80120680 = D_80120610;
    D_800B0C0E = D_800B0C0E + 2;
    D_800B0C12 = D_800B0C12 + 1;
    D_800B0C16 = D_800B0C16 - 20;

    func_8004C250();
    func_80108280();
    func_80012D64(D_80123940, 17, 1, 2, 0);
    func_8004C46C(D_80123944, D_80123948, D_8012394C, D_80123950, D_80123954, D_80123958);
    func_8004C6B0(D_80123934, D_80123938, D_80123930, D_8012393C);
    func_80011DFC();
}
