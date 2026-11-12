/* func_801371AC -- EVENT overlay, Address: 0x801371AC */
/* @SymbolStart: 0x80137188 */
/* @SymbolSize: 280 */
#include "types.h"

extern s32 D_801202E0;
extern s32 D_80120300;
extern s32 D_80120320;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern volatile s16 D_800B0A06;
extern volatile s16 D_800B0A0A;
extern volatile s16 D_800B0A0E;
extern s32 ev_flag18;
extern s32 D_80120C7C;
extern s32 D_80120C80;
extern s32 D_80120C84;
extern s16 D_80120C88;
extern s32 D_80120C8C;
extern s32 D_80120C90;
extern s32 D_80120C94;
extern s32 D_80120C98;
extern s32 D_80120C9C;
extern s32 D_80120CA0;
extern s32 D_80120CA4;

extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_801371AC(void) {
    D_80120678 = D_801202E0;
    D_8012067C = D_80120300;
    D_80120680 = D_80120320;
    func_80012D64(D_80120C8C, 17, 1, 2, 0);
    func_8004C46C(D_80120C90, D_80120C94, D_80120C98, D_80120C9C,
                  D_80120CA0, D_80120CA4);
    func_8004C6B0(D_80120C80, D_80120C84, D_80120C7C, D_80120C88);
    D_800B0A06 = (s16)(D_800B0A06 + 1);
    D_800B0A0A = (s16)(D_800B0A0A + 1);
    D_800B0A0E = (s16)(D_800B0A0E - 20);
    func_8004C250();
    ev_flag18 = 22;
    func_80011DFC();
}
