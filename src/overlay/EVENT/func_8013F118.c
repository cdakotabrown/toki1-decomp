/* func_8013F118 -- EVENT overlay, Address: 0x8013F118 */
/* @SymbolStart: 0x8013F0F4 */
/* @SymbolSize: 276 */
#include "types.h"

extern s32 D_80120454;
extern s32 D_80120470;
extern s32 D_8012048C;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_80122704;
extern s32 D_80122708;
extern s32 D_8012270C;
extern s16 D_80122710;
extern s32 D_80122714;
extern s32 D_80122718;
extern s32 D_8012271C;
extern s32 D_80122720;
extern s32 D_80122724;
extern s32 D_80122728;
extern s32 D_8012272C;
extern volatile s16 D_800B0B0A;
extern volatile s16 D_800B0B0E;
extern volatile s16 D_800B0B12;

extern void func_801026C0(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_8013F118(void) {
    D_80120678 = D_80120454;
    D_8012067C = D_80120470;
    D_80120680 = D_8012048C;
    func_801026C0();
    func_80012D64(D_80122714, 17, 1, 2, 0);
    func_8004C46C(D_80122718, D_8012271C, D_80122720, D_80122724,
                  D_80122728, D_8012272C);
    func_8004C6B0(D_80122708, D_8012270C, D_80122704, D_80122710);
    D_800B0B0A = (s16)(D_800B0B0A + 3);
    D_800B0B0E = (s16)(D_800B0B0E + 2);
    D_800B0B12 = (s16)(D_800B0B12 - 20);
    func_8004C250();
    func_80011DFC();
}
