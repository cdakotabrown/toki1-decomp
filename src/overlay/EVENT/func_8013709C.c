/* func_8013709C -- EVENT overlay, Address: 0x8013709C */
/* @SymbolStart: 0x80137078 */
/* @SymbolSize: 272 */
#include "types.h"

extern s32 D_801202DC;
extern s32 D_801202FC;
extern s32 D_8012031C;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern volatile s16 D_800B0A06;
extern volatile s16 D_800B0A0A;
extern volatile s16 D_800B0A0E;
extern s32 D_80120C50;
extern s32 D_80120C54;
extern s32 D_80120C58;
extern s16 D_80120C5C;
extern s32 D_80120C60;
extern s32 D_80120C64;
extern s32 D_80120C68;
extern s32 D_80120C6C;
extern s32 D_80120C70;
extern s32 D_80120C74;
extern s32 D_80120C78;

extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_8013709C(void) {
    D_80120678 = D_801202DC;
    D_8012067C = D_801202FC;
    D_80120680 = D_8012031C;
    func_80012D64(D_80120C60, 17, 1, 2, 0);
    func_8004C46C(D_80120C64, D_80120C68, D_80120C6C, D_80120C70,
                  D_80120C74, D_80120C78);
    func_8004C6B0(D_80120C54, D_80120C58, D_80120C50, D_80120C5C);
    D_800B0A06 = (s16)(D_800B0A06 + 3);
    D_800B0A0A = (s16)(D_800B0A0A + 2);
    D_800B0A0E = (s16)(D_800B0A0E - 20);
    func_8004C250();
    func_80011DFC();
}
