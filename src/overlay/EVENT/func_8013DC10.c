/* func_8013DC10 -- EVENT overlay, Address: 0x8013DC10 */
/* @SymbolStart: 0x8013DBEC */
/* @SymbolSize: 304 */
#include "types.h"

extern s32 D_80120340;
extern s32 D_80120354;
extern s32 D_80120368;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern volatile s16 D_800B0A3A;
extern volatile s16 D_800B0A3E;
extern volatile s16 D_800B0A42;
extern s32 D_80122280;
extern s32 D_80122284;
extern s32 D_80122288;
extern s16 D_8012228C;
extern s32 D_80122290;
extern s32 D_80122294;
extern s32 D_80122298;
extern s32 D_8012229C;
extern s32 D_801222A0;
extern s32 D_801222A4;
extern s32 D_801222A8;

extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_8013DC10(void) {
    D_80120678 = D_80120340;
    D_8012067C = D_80120354;
    D_80120680 = D_80120368;
    func_80012D64(D_80122290, 17, 1, 2, 0);
    func_80012D64(D_80122290, 18, 1, 2, 1);
    func_8004C46C(D_80122294, D_80122298, D_8012229C, D_801222A0,
                  D_801222A4, D_801222A8);
    func_8004C6B0(D_80122284, D_80122288, D_80122280, D_8012228C);
    D_800B0A3A = (s16)(D_800B0A3A + 3);
    D_800B0A3E = (s16)(D_800B0A3E + 2);
    D_800B0A42 = (s16)(D_800B0A42 - 20);
    func_8004C250();
    func_80011DFC();
}
