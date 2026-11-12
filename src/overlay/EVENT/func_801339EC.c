/* @SymbolSize: 188 */
/* Address: 0x801339EC */
/* Size: 188 bytes (47 instructions) */
#include "types.h"

extern s16 D_800EAFB6;
extern s32 D_800EECEC;
extern s32 D_801206A0;
extern s32 D_801206A4;
extern s32 D_801206A8;
extern s32 D_801206AC;
extern s32 D_801206B0;
extern s32 D_801206B4;
extern s32 D_801206B8;
extern s32 D_80120694;
extern s32 D_80120698;
extern s16 D_8012069C;
extern s32 D_80120690;

extern void func_800F7220();
extern void func_80012D64();
extern void func_8004C46C();
extern void func_8004C6B0();
extern void func_80011DFC();

void func_801339EC(void) {
    func_800F7220();
    func_80012D64(D_801206A0, 17, 1, 2, 0);
    func_8004C46C(D_801206A4, D_801206A8, D_801206AC, D_801206B0, D_801206B4, D_801206B8);
    func_8004C6B0(D_80120694, D_80120698, D_80120690, D_8012069C);
    D_800EAFB6 = 4;
    D_800EECEC = 1;
    func_80011DFC();
}
