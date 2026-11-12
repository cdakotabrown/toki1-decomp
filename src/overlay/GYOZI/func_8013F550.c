/* @RodataBase: 0x80145D50 */
/* func_8013F550 -- GYOZI overlay, Address: 0x8013F550, 144 insns. */
#include "types.h"

extern u8 D_800F62CF;

extern s32 D_801481D0;
extern s32 D_801481D8;
extern s32 D_801481DC;
extern s32 D_801481E0;
extern s32 D_801481E4;
extern s32 D_801481E8;
extern s32 D_801481EC;
extern s32 D_801481F0;
extern s32 D_801481F4;
extern s32 D_80148200;
extern s32 D_80148204;
extern s32 D_8014820C;
extern s32 D_80148210;
extern s32 D_80148214;
extern s32 D_80148218;
extern s32 D_8014821C;
extern s32 D_80148220;
extern s32 D_80148224;
extern s32 D_80148228;
extern s32 D_80148234;
extern s32 D_80148238;
extern s32 D_80148240;
extern s32 D_80148244;
extern s32 D_80148248;
extern s32 D_8014824C;
extern s32 D_80148250;
extern s32 D_80148254;
extern s32 D_80148258;
extern s32 D_8014825C;
extern s32 D_80148268;
extern s32 D_80148614;
extern s32 D_80148618;
extern s32 D_8014861C;

void func_8013F550(void)
{
    switch (D_800F62CF) {
    case 1:
        D_80148614 = D_801481E4;
        D_80148618 = D_80148218;
        D_8014861C = D_8014824C;
        break;
    case 2:
        D_80148614 = D_801481D8;
        D_80148618 = D_8014820C;
        D_8014861C = D_80148240;
        break;
    case 3:
        D_80148614 = D_801481DC;
        D_80148618 = D_80148210;
        D_8014861C = D_80148244;
        break;
    case 4:
        D_80148614 = D_801481F4;
        D_80148618 = D_80148228;
        D_8014861C = D_8014825C;
        break;
    case 5:
        D_80148614 = D_801481E8;
        D_80148618 = D_8014821C;
        D_8014861C = D_80148250;
        break;
    case 6:
        D_80148614 = D_801481EC;
        D_80148618 = D_80148220;
        D_8014861C = D_80148254;
        break;
    case 7:
        D_80148614 = D_801481E0;
        D_80148618 = D_80148214;
        D_8014861C = D_80148248;
        break;
    case 8:
        D_80148614 = D_801481D0;
        D_80148618 = D_80148204;
        D_8014861C = D_80148238;
        break;
    case 9:
        D_80148614 = D_801481F0;
        D_80148618 = D_80148224;
        D_8014861C = D_80148258;
        break;
    case 10:
        D_80148614 = D_80148200;
        D_80148618 = D_80148234;
        D_8014861C = D_80148268;
        break;
    }
}
