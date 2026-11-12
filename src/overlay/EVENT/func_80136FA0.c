/* func_80136FA0 -- EVENT overlay, Address: 0x80136FA0 */
/* @SymbolStart: 0x80136F7C */
/* @SymbolSize: 252 */
#include "types.h"

extern s32 D_801202D8;
extern s32 D_801202F8;
extern s32 D_80120318;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern volatile s16 D_800B0A06;
extern volatile s16 D_800B0A0E;
extern s32 D_80120C24;
extern s32 D_80120C28;
extern s32 D_80120C2C;
extern s16 D_80120C30;
extern s32 D_80120C34;
extern s32 D_80120C38;
extern s32 D_80120C3C;
extern s32 D_80120C40;
extern s32 D_80120C44;
extern s32 D_80120C48;
extern s32 D_80120C4C;

extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_80136FA0(void) {
    D_80120678 = D_801202D8;
    D_8012067C = D_801202F8;
    D_80120680 = D_80120318;
    func_80012D64(D_80120C34, 17, 1, 2, 0);
    func_8004C46C(D_80120C38, D_80120C3C, D_80120C40, D_80120C44,
                  D_80120C48, D_80120C4C);
    func_8004C6B0(D_80120C28, D_80120C2C, D_80120C24, D_80120C30);
    D_800B0A06 = (s16)(D_800B0A06 + 2);
    D_800B0A0E = (s16)(D_800B0A0E - 0x1c);
    func_8004C250();
    func_80011DFC();
}
