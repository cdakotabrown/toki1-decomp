/* @SymbolStart: 0x8013FACC */
/* @SymbolSize: 1440 */
/* func_8013FAD8 -- GYOZI overlay, Address: 0x8013FAD8, Size: 1428 bytes */
#include "types.h"

typedef struct {
    s32 raw[17];
} PieceStatus;

extern void func_801418B0(void);
extern s32 func_8004EE18(s32, s32, s32, s32, s32);
extern void func_800549E8(s32);
extern void func_80141900(void);
extern s32 func_800BCE40(void);
extern void func_8008FB00(void);
extern void func_8004DE1C(void);

extern s8 D_8014881C;
extern s8 D_80148820;
extern s8 D_80148824;

extern PieceStatus D_8012B8C0[6];
#define PS_H(k, off) (*(s16 *)((u8 *)&D_8012B8C0[k] + (off)))
extern s8 D_8012B8C1;
extern s8 D_8012B8C2;
extern s8 D_8012B8C3;
extern s8 D_8012B8C4;
extern u8 D_8012B8C5;
extern s8 D_8012B8C6;
extern u8 D_8012B8C7;
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

extern PieceStatus D_8012B904;
extern PieceStatus D_8012B948;
extern s8 D_8012B949;
extern s8 D_8012B94A;
extern s8 D_8012B94C;
extern s32 D_8012B954;
extern s32 D_8012B958;
extern s16 D_8012B95C;
extern s16 D_8012B95E;
extern s16 D_8012B960;
extern s16 D_8012B96E;
extern s16 D_8012B972;
extern s32 D_8012B97C;
extern s8 D_8012B98B;
extern PieceStatus D_8012B98C;
extern s16 D_8012B9B2;
extern PieceStatus D_8012B9D0;
extern s16 D_8012B9E6;
extern s16 D_8012B9E8;
extern PieceStatus D_8012BA14;
extern s8 D_8012BA15;
extern s8 D_8012BA16;
extern s16 D_8012BA2A;

extern u8 D_80148754;
extern u8 D_80148758;
extern s32 D_801481F8;
extern s32 D_8014822C;
extern s32 D_80148260;
extern s32 D_80148614;
extern s32 D_80148618;
extern s32 D_8014861C;
extern s16 D_80148620;
extern s32 D_801487F0;
extern s32 D_801487F4;
extern s32 D_801487F8;
extern s16 D_801487FC;
extern s32 D_80148800;
extern s32 D_80148804;
extern s32 D_80148808;
extern s32 D_8014880C;
extern s16 D_80148810;
extern s32 D_80148814;

extern s16 D_800F5552;
extern u8 D_800F53A0[];

void func_8013FAD8(void)
{
    s32 i;
    s32 r;
    PieceStatus bufB;
    PieceStatus bufA;

    D_8014881C = 0;
    D_80148820 = 0;
    D_80148824 = 0;

    func_801418B0();

    i = 0;
    do {
        func_8004EE18(D_80148800, (u8)(i + 16), 1, 2, i);
        i = (s16)(i + 1);
    } while (i < 9);

    func_800549E8(96);

    D_8012B8C1 = 8;
    D_8012B8C2 = 64;
    D_8012B8F8 = 0x01000000;
    D_8012B8C3 = 4;
    D_8012B8CC = D_801487F4;
    D_8012B8D0 = D_801487F8;
    D_8012B8F4 = D_801487F0;
    D_8012B8D4 = D_801487FC;
    D_8012B8D6 = 0;
    D_8012B8D8 = 0;
    D_8012B8C6 = 1;
    D_8012B903 = 0;
    D_8012B8E6 = 0;
    D_8012B8EA = 8;
    D_8012B8C7 = 128;
    D_8012B8C5 = 128;
    D_8012B8C4 = 0;

    D_8012B904 = D_8012B8C0[0];

    func_80141900();
    func_8004EE18(D_80148814, 31, 1, 2, 0);

    D_8012B948 = D_8012B8C0[0];

    D_8012B949 = 11;
    D_8012B94C = 8;
    D_8012B94A = 1;
    D_8012B954 = D_80148808;
    D_8012B958 = D_8014880C;
    D_8012B97C = D_80148804;
    D_8012B95C = D_80148810;

    r = func_800BCE40();
    i = 3;
    D_8012B95E = r % i;
    D_8012B960 = 0;
    D_8012B96E = -160;
    D_8012B972 = -120;
    D_8012B98B = 31;

    bufA = D_8012B948; D_8012B9D0 = bufA; bufB = bufA; D_8012B98C = bufB; D_8012BA14 = bufB;

    D_8012B9B2 = D_8012B9B2 + 72;
    r = func_800BCE40();
    PS_H(3, 0x16) = r % i;
    PS_H(3, 0x18) = 4;

    PS_H(4, 0x26) = PS_H(4, 0x26) + 144;
    r = func_800BCE40();
    D_8012B9E6 = r % i;
    D_8012B9E8 = 10;
    D_8012BA15 = 10;
    D_8012BA16 = 64;
    D_8012BA2A = 0;

    D_80148620 = 0;
    D_80148614 = D_801481F8;
    D_80148618 = D_8014822C;
    D_8014861C = D_80148260;

    D_800F5552 = D_800F5552 + 1;

    i = 56;
    ((s16 *)(D_800F53A0 + D_80148754 * i))[217] = ((s16 *)(D_800F53A0 + D_80148754 * i))[217] + 1;
    ((s16 *)(D_800F53A0 + D_80148758 * i))[217] = ((s16 *)(D_800F53A0 + D_80148758 * i))[217] + 1;

    func_8008FB00();
    func_8004DE1C();
}
