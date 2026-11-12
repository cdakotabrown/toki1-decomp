/* func_801372C4 -- EVENT overlay, Address: 0x801372C4 */
/* @SymbolStart: 0x801372A0 */
/* @SymbolSize: 252 */
#include "types.h"

extern s32 D_801202E4;
extern s32 D_80120304;
extern s32 D_80120324;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern volatile s16 D_800B0A06;
extern volatile s16 D_800B0A0A;
extern volatile s16 D_800B0A0E;
extern s32 D_80120CA8;
extern s32 D_80120CAC;
extern s32 D_80120CB0;
extern s32 D_80120CB4;
extern s32 D_80120CB8;
extern s32 D_80120CBC;
extern s32 D_80120CC0;

extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s32);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_801372C4(void) {
    D_80120678 = D_801202E4;
    D_8012067C = D_80120304;
    D_80120680 = D_80120324;
    func_80012D64(D_80120CA8, 17, 1, 2, 0);
    func_8004C46C(D_80120CAC, D_80120CB0, D_80120CB4, D_80120CB8,
                  D_80120CBC, D_80120CC0);
    func_8004C6B0(0, 0, 0, 0);
    D_800B0A06 = (s16)(D_800B0A06 + 1);
    D_800B0A0A = (s16)(D_800B0A0A + 2);
    D_800B0A0E = (s16)(D_800B0A0E - 20);
    func_8004C250();
    func_80011DFC();
}
