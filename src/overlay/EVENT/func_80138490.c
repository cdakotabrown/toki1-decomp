/* func_80138490 -- EVENT overlay, Address: 0x80138490 */
/* @SymbolStart: 0x8013846C */
/* @SymbolSize: 256 */
#include "types.h"

extern s32 D_80120370;
extern s32 D_80120388;
extern s32 D_801203A0;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_8012120C;
extern s32 D_80121210;
extern s32 D_80121214;
extern s32 D_80121218;
extern s32 D_8012121C;
extern s32 D_80121220;
extern s32 D_80121224;
extern volatile s16 D_800B0A6E;
extern volatile s16 D_800B0A72;
extern volatile s16 D_800B0A76;

extern void func_800FBCA0(void);
extern void func_80012D64(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void func_8004C46C(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern void func_8004C6B0(s32 a, s32 b, s32 c, s32 d);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_80138490(void) {
    D_80120678 = D_80120370;
    D_8012067C = D_80120388;
    D_80120680 = D_801203A0;
    func_800FBCA0();
    func_80012D64(D_8012120C, 17, 1, 2, 0);
    func_8004C46C(D_80121210, D_80121214, D_80121218, D_8012121C,
                  D_80121220, D_80121224);
    func_8004C6B0(0, 0, 0, 0);
    D_800B0A6E = (s16)(D_800B0A6E + 3);
    D_800B0A72 = (s16)(D_800B0A72 + 2);
    D_800B0A76 = (s16)(D_800B0A76 - 20);
    func_8004C250();
    func_80011DFC();
}
