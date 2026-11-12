/* @CompileFlags: -cckr */
/* @RodataBase: 0x80145C60 */
/* @SymbolSize: 740 */
/* func_8013C824 -- GYOZI overlay, Address: 0x8013C824, Size: 740 bytes (185 insns) */
/* in-function IDO switch, and @RodataBase: 0x80145C60 places the compiled */
#include "types.h"

extern s32 system_config_disp(s32);
extern void func_8004DE1C(void);

extern u8 D_800F62CF;

extern s32 D_80148060;
extern s32 D_80148064;
extern s32 D_80148068;
extern s32 D_80148070;
extern s32 D_80148074;
extern s32 D_80148078;
extern s32 D_8014807C;
extern s32 D_80148080;
extern s32 D_80148084;
extern s32 D_80148088;
extern s32 D_80148090;
extern s32 D_80148094;
extern s32 D_80148098;
extern s32 D_8014809C;
extern s32 D_801480A4;
extern s32 D_801480A8;
extern s32 D_801480AC;
extern s32 D_801480B0;
extern s32 D_801480B4;
extern s32 D_801480B8;
extern s32 D_801480BC;
extern s32 D_801480C4;
extern s32 D_801480C8;
extern s32 D_801480CC;
extern s32 D_801480D0;
extern s32 D_801480D8;
extern s32 D_801480DC;
extern s32 D_801480E0;
extern s32 D_801480E4;
extern s32 D_801480E8;
extern s32 D_801480EC;
extern s32 D_801480F0;
extern s32 D_801480F8;
extern s32 D_801480FC;
extern s32 D_80148100;
extern s32 D_80148104;
extern s16 D_80148108;

void func_8013C824(void)
{
    s32 v0;
    s32 v1;
    u8 masked;

    v0 = system_config_disp(D_800F62CF);
    v1 = v0;
    masked = (u8)(v1 & 0x7F);
    v1 = masked;
    D_80148108 = 0;
    if ((u32)v1 >= 2U) {
        D_80148108 += 2;
    }
    if ((u32)v1 >= 3U) {
        D_80148108 += 2;
    }
    if (v1 == 4) {
        D_80148108 += 2;
    }

    switch ((u32)D_800F62CF) {
    case 0:
        D_801480FC = D_80148064;
        D_80148100 = D_80148098;
        D_80148104 = D_801480CC;
        break;
    case 1:
        D_801480FC = D_80148078;
        D_80148100 = D_801480AC;
        D_80148104 = D_801480E0;
        break;
    case 2:
        D_801480FC = D_80148068;
        D_80148100 = D_8014809C;
        D_80148104 = D_801480D0;
        break;
    case 3:
        D_801480FC = D_80148070;
        D_80148100 = D_801480A4;
        D_80148104 = D_801480D8;
        break;
    case 4:
        D_801480FC = D_80148088;
        D_80148100 = D_801480BC;
        D_80148104 = D_801480F0;
        break;
    case 5:
        D_801480FC = D_8014807C;
        D_80148100 = D_801480B0;
        D_80148104 = D_801480E4;
        break;
    case 6:
        D_801480FC = D_80148080;
        D_80148100 = D_801480B4;
        D_80148104 = D_801480E8;
        break;
    case 7:
        D_801480FC = D_80148074;
        D_80148100 = D_801480A8;
        D_80148104 = D_801480DC;
        break;
    case 8:
        D_801480FC = D_80148060;
        D_80148100 = D_80148094;
        D_80148104 = D_801480C8;
        break;
    case 9:
        D_801480FC = D_80148084;
        D_80148100 = D_801480B8;
        D_80148104 = D_801480EC;
        break;
    case 10:
        D_801480FC = D_80148090;
        D_80148100 = D_801480C4;
        D_80148104 = D_801480F8;
        break;
    }

    func_8004DE1C();
}
