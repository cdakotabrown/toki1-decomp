/* func_8013DADC -- EVENT overlay, Address: 0x8013DADC */
/* @SymbolStart: 0x8013DAB8 */
/* @SymbolSize: 308 */
#include "types.h"

extern s32 D_8012033C;
extern s32 D_80120350;
extern s32 D_80120364;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_800EAFD8;
extern s16 D_800EAFB6;
extern volatile s16 D_800B0A3A;
extern volatile s16 D_800B0A3E;
extern volatile s16 D_800B0A42;
extern s32 D_80122254;
extern s32 D_80122258;
extern s32 D_8012225C;
extern s16 D_80122260;
extern s32 D_80122264;
extern s32 D_80122268;
extern s32 D_8012226C;
extern s32 D_80122270;
extern s32 D_80122274;
extern s32 D_80122278;
extern s32 D_8012227C;

extern void func_801013B0(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_8013DADC(void) {
    D_80120678 = D_8012033C;
    D_8012067C = D_80120350;
    D_80120680 = D_80120364;
    func_801013B0();
    func_80012D64(D_80122264, 17, 1, 2, 0);
    func_8004C46C(D_80122268, D_8012226C, D_80122270, D_80122274,
                  D_80122278, D_8012227C);
    func_8004C6B0(D_80122258, D_8012225C, D_80122254, D_80122260);
    D_800EAFD8 |= 0x80000000;
    D_800EAFB6 = 0;
    D_800B0A3A = (s16)(D_800B0A3A + 2);
    D_800B0A3E = (s16)(D_800B0A3E + 2);
    D_800B0A42 = (s16)(D_800B0A42 - 20);
    func_8004C250();
    func_80011DFC();
}
