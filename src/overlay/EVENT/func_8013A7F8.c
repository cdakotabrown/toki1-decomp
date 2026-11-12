/* func_8013A7F8 -- EVENT overlay, Address: 0x8013A7F8 */
/* @SymbolStart: 0x8013A7D4 */
/* @SymbolSize: 212 */
#include "types.h"

extern s32 D_801203C4;
extern s32 D_801203DC;
extern s32 D_801203F4;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_801218B0;
extern s32 D_801218B4;
extern s32 D_801218B8;
extern s16 D_801218BC;
extern s32 D_801218C0;
extern s32 D_801218C4;
extern s32 D_801218C8;
extern s32 D_801218CC;
extern s32 D_801218D0;
extern s32 D_801218D4;
extern s32 D_801218D8;

extern void func_800FDEE0(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_80011DFC(void);

void func_8013A7F8(void) {
    D_80120678 = D_801203C4;
    D_8012067C = D_801203DC;
    D_80120680 = D_801203F4;
    func_800FDEE0();
    func_80012D64(D_801218C0, 17, 1, 2, 0);
    func_8004C46C(D_801218C4, D_801218C8, D_801218CC, D_801218D0,
                  D_801218D4, D_801218D8);
    func_8004C6B0(D_801218B4, D_801218B8, D_801218B0, D_801218BC);
    func_80011DFC();
}
