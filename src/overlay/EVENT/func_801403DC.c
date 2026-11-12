/* func_801403DC -- EVENT overlay, Address: 0x801403DC */
/* @SymbolStart: 0x801403B8 */
/* @SymbolSize: 276 */
#include "types.h"

extern s32 D_8012049C;
extern s32 D_801204B0;
extern s32 D_801204C4;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_80122B5C;
extern s32 D_80122B60;
extern s32 D_80122B64;
extern s16 D_80122B68;
extern s32 D_80122B6C;
extern s32 D_80122B70;
extern s32 D_80122B74;
extern s32 D_80122B78;
extern s32 D_80122B7C;
extern s32 D_80122B80;
extern s32 D_80122B84;
extern volatile s16 D_800B0B3E;
extern volatile s16 D_800B0B42;
extern volatile s16 D_800B0B46;

extern void func_80103D30(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_801403DC(void) {
    D_80120678 = D_8012049C;
    D_8012067C = D_801204B0;
    D_80120680 = D_801204C4;
    func_80103D30();
    func_80012D64(D_80122B6C, 17, 1, 2, 0);
    func_8004C46C(D_80122B70, D_80122B74, D_80122B78, D_80122B7C,
                  D_80122B80, D_80122B84);
    func_8004C6B0(D_80122B60, D_80122B64, D_80122B5C, D_80122B68);
    D_800B0B3E = (s16)(D_800B0B3E + 1);
    D_800B0B42 = (s16)(D_800B0B42 + 1);
    D_800B0B46 = (s16)(D_800B0B46 - 20);
    func_8004C250();
    func_80011DFC();
}
