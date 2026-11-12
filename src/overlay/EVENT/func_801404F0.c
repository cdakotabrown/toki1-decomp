/* func_801404F0 -- EVENT overlay, Address: 0x801404F0 */
/* @SymbolStart: 0x801404CC */
/* @SymbolSize: 276 */
#include "types.h"

extern s32 D_801204A0;
extern s32 D_801204B4;
extern s32 D_801204C8;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_80122B88;
extern s32 D_80122B8C;
extern s32 D_80122B90;
extern s16 D_80122B94;
extern s32 D_80122B98;
extern s32 D_80122B9C;
extern s32 D_80122BA0;
extern s32 D_80122BA4;
extern s32 D_80122BA8;
extern s32 D_80122BAC;
extern s32 D_80122BB0;
extern volatile s16 D_800B0B3E;
extern volatile s16 D_800B0B42;
extern volatile s16 D_800B0B46;

extern void func_80103DE0(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_801404F0(void) {
    D_80120678 = D_801204A0;
    D_8012067C = D_801204B4;
    D_80120680 = D_801204C8;
    func_80103DE0();
    func_80012D64(D_80122B98, 17, 1, 2, 0);
    func_8004C46C(D_80122B9C, D_80122BA0, D_80122BA4, D_80122BA8,
                  D_80122BAC, D_80122BB0);
    func_8004C6B0(D_80122B8C, D_80122B90, D_80122B88, D_80122B94);
    D_800B0B3E = (s16)(D_800B0B3E + 3);
    D_800B0B42 = (s16)(D_800B0B42 + 2);
    D_800B0B46 = (s16)(D_800B0B46 - 20);
    func_8004C250();
    func_80011DFC();
}
