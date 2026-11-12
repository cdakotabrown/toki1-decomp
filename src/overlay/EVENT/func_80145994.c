/* Address: 0x80145948 (cross-prologue head absorbed) .. 0x80145994 */
/* Size: 260 bytes (65 instructions) */
/* @SymbolStart: 0x80145948 */
/* @SymbolSize: 260 */
#include "types.h"

extern s32 D_801205D4;
extern s32 D_801205F0;
extern s32 D_8012060C;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern volatile s16 D_800B0C12;
extern volatile s16 D_800B0C16;

extern s32 D_80123904;
extern s32 D_80123908;
extern s32 D_8012390C;
extern s16 D_80123910;
extern s32 D_80123914;
extern s32 D_80123918;
extern s32 D_8012391C;
extern s32 D_80123920;
extern s32 D_80123924;
extern s32 D_80123928;
extern s32 D_8012392C;

extern void func_8004C250(void);
extern void func_801081D0(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_80011DFC(void);

void func_80145994(void) {
    D_80120678 = D_801205D4;
    D_8012067C = D_801205F0;
    D_80120680 = D_8012060C;
    D_800B0C12 = D_800B0C12 + 2;
    D_800B0C16 = D_800B0C16 - 10;

    func_8004C250();
    func_801081D0();
    func_80012D64(D_80123914, 17, 1, 2, 0);
    func_8004C46C(D_80123918, D_8012391C, D_80123920, D_80123924, D_80123928, D_8012392C);
    func_8004C6B0(D_80123908, D_8012390C, D_80123904, D_80123910);
    func_80011DFC();
}
