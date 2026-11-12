/* func_80140F6C -- GYOZI overlay, Address: 0x80140F6C, Size: 328 bytes (82 insns), Semi-matched */
#include "types.h"

extern void func_80142D00(void);
extern void func_8004EE18(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void func_800549E8(s32 a0);
extern void func_8004DE1C(void);

extern s32 D_80148900;
extern s32 D_8014861C;
extern s32 D_80148264;

extern s8 D_8012B8C1;
extern s8 D_8012B8C2;
extern u8 D_8012B8C3;
extern s8 D_8012B8C4;
extern s8 D_8012B8C5;
extern s8 D_8012B8C6;
extern s8 D_8012B8C7;
extern s32 D_8012B8CC;
extern s32 D_8012B8D0;
extern s16 D_8012B8D4;
extern s16 D_8012B8D6;
extern s16 D_8012B8D8;
extern s16 D_8012B8E6;
extern s16 D_8012B8EA;
extern s32 D_8012B8F4;
extern s32 D_8012B8F8;
extern s8 D_8012B903;
extern s32 D_801481FC;
extern s32 D_80148230;
extern s32 D_80148264;
extern s32 D_80148614;
extern s32 D_80148618;
extern s32 D_8014861C;
extern s16 D_80148620;
extern s32 D_801488F0;
extern s32 D_801488F4;
extern s32 D_801488F8;
extern s16 D_801488FC;
extern s32 D_80148900;

void func_80140F6C(void)
{
    func_80142D00();
    func_8004EE18(D_80148900, 17, 1, 2, 0);
    func_800549E8(96);
    D_8012B8C1 = 1;
    D_8012B8C2 = 0;
    D_8012B8F8 = 0x01000000;
    D_8012B8C3 = 132;
    D_8012B8CC = D_801488F4;
    D_8012B8D0 = D_801488F8;
    D_8012B8F4 = D_801488F0;
    D_8012B8D4 = D_801488FC;
    D_8012B8D6 = 0;
    D_8012B8D8 = 0;
    D_8012B8C6 = 1;
    D_8012B903 = 17;
    D_8012B8E6 = -160;
    D_8012B8EA = -120;
    D_8012B8C7 = 0;
    D_8012B8C5 = 0;
    D_8012B8C4 = 0;
    D_80148620 = 19;
    D_80148614 = D_801481FC;
    D_80148618 = D_80148230;
    D_8014861C = D_80148264;
    func_8004DE1C();
}
