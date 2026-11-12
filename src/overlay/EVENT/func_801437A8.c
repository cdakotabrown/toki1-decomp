/* func_801437A8 -- EVENT overlay, Address: 0x801437A8 */
/* @SymbolStart: 0x80143784 */
/* @SymbolSize: 276 */
#include "types.h"

extern s32 D_8012040C;
extern s32 D_80120424;
extern s32 D_8012043C;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern volatile s16 D_800B0AD6;
extern volatile s16 D_800B0ADA;
extern volatile s16 D_800B0ADE;
extern s32 D_801234E0;
extern s32 D_801234E4;
extern s32 D_801234E8;
extern s16 D_801234EC;
extern s32 D_801234F0;
extern s32 D_801234F4;
extern s32 D_801234F8;
extern s32 D_801234FC;
extern s32 D_80123500;
extern s32 D_80123504;
extern s32 D_80123508;

extern void func_80106EE0(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_801437A8(void) {
    D_80120678 = D_8012040C;
    D_8012067C = D_80120424;
    D_80120680 = D_8012043C;
    func_80106EE0();
    func_80012D64(D_801234F0, 17, 1, 2, 0);
    func_8004C46C(D_801234F4, D_801234F8, D_801234FC, D_80123500,
                  D_80123504, D_80123508);
    func_8004C6B0(D_801234E4, D_801234E8, D_801234E0, D_801234EC);
    D_800B0AD6 = (s16)(D_800B0AD6 + 1);
    D_800B0ADA = (s16)(D_800B0ADA + 1);
    D_800B0ADE = (s16)(D_800B0ADE - 20);
    func_8004C250();
    func_80011DFC();
}
