/* func_80140604 -- EVENT overlay, Address: 0x80140604 */
/* @SymbolStart: 0x801405E0 */
/* @SymbolSize: 276 */
#include "types.h"

extern s32 D_801204A4;
extern s32 D_801204B8;
extern s32 D_801204CC;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_80122BB4;
extern s32 D_80122BB8;
extern s32 D_80122BBC;
extern s16 D_80122BC0;
extern s32 D_80122BC4;
extern s32 D_80122BC8;
extern s32 D_80122BCC;
extern s32 D_80122BD0;
extern s32 D_80122BD4;
extern s32 D_80122BD8;
extern s32 D_80122BDC;
extern volatile s16 D_800B0B3E;
extern volatile s16 D_800B0B42;
extern volatile s16 D_800B0B46;

extern void func_80103E90(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_80140604(void) {
    D_80120678 = D_801204A4;
    D_8012067C = D_801204B8;
    D_80120680 = D_801204CC;
    func_80103E90();
    func_80012D64(D_80122BC4, 17, 1, 2, 0);
    func_8004C46C(D_80122BC8, D_80122BCC, D_80122BD0, D_80122BD4,
                  D_80122BD8, D_80122BDC);
    func_8004C6B0(D_80122BB8, D_80122BBC, D_80122BB4, D_80122BC0);
    D_800B0B3E = (s16)(D_800B0B3E + 2);
    D_800B0B42 = (s16)(D_800B0B42 + 2);
    D_800B0B46 = (s16)(D_800B0B46 - 10);
    func_8004C250();
    func_80011DFC();
}
