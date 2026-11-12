/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    s16 v[11];
} Tbl;

typedef struct {
    u8 pad0[2];
    s16 a;
    u8 pad4[6];
    s16 b;
    u32 f0 : 4;
    u32 f4 : 1;
    u32 f5 : 9;
    u32 f14 : 1;
    u32 f15 : 17;
    u32 bmon : 4;
    u32 bday : 5;
    u32 bpad : 23;
    u8 pad14[52 - 20];
} Chr;

typedef struct {
    u8 pad0[53];
    u8 year;
    u8 month;
    u8 day;
    u8 pad38[420 - 56];
    Chr chr[25];
    u8 pad2b6[3814 - 420 - 25 * 52];
    u8 cur;
} Save;

extern Save D_800B0860;
extern Tbl D_80125330;
extern Tbl D_80125348;
extern u32 D_800B1AE4;
extern s32 D_800EECBC;
extern s16 D_80124364;
extern s16 D_80124368;
extern u8 D_801252EC;
extern u8 D_801252F0;

void func_8015367C()
{
    s32 i;
    u8 r1;
    Tbl tblA;
    Tbl tblB;
    u8 flag;
    u32 r2;
    u32 d;

    tblA = D_80125330;
    tblB = D_80125348;
    if ((D_800B0860.year | 0) == 97 && D_800B0860.month >= 6) tblB.v[3]++;
    else if (D_800B0860.year == 98) tblB.v[3]++;
    D_80124364 = D_800EECBC;
    D_80124368 = D_800B1AE4 % ((D_800EECBC == 0) * tblA.v[D_800B0860.cur] +
                               tblB.v[D_800B0860.cur] * (D_800EECBC == 1));
    flag = 0;
    D_801252EC = 0;
    if (D_800B0860.month == D_800B0860.chr[D_800B0860.cur].bmon) {
        if (D_800B0860.day < D_800B0860.chr[D_800B0860.cur].bday) {
            D_801252EC = 1;
        }
    } else if ((d = D_800B0860.chr[D_800B0860.cur].bmon - D_800B0860.month + 12) % 12 == 1) {
        D_801252EC = 1;
    }
    D_801252F0 = 0;
    if (((D_800B0860.year | 0) == 97 && D_800B0860.month >= 3) || D_800B0860.year == 98) {
        D_801252F0 = 1;
    }
    r1 = func_8002328C(D_800B0860.cur) & 0x7F;
    r2 = func_8002336C(D_800B0860.cur);
    if (r1 == 0 && r2 == 0) {
        flag = 1;
    }
    if (flag != 0 && D_800EECBC == 0) {
        D_80124364 = 4;
        D_80124368 = 0;
    } else if (D_801252EC != 0 && D_800EECBC == 0) {
        D_80124364 = 2;
        D_80124368 = D_800B0860.year - 95;
        if (D_800B0860.cur == 6) {
            if (r1 < 2 && r2 < 2) {
                D_80124368 = 1;
            } else {
                D_80124368 = 0;
            }
        }
    } else if (D_801252F0 != 0 && D_800EECBC == 0) {
        D_80124364 = 3;
        D_80124368 = 0;
    }
    if (D_800B0860.cur == 5 && D_80124364 == 0 && D_80124368 == 3) {
        D_800B0860.chr[5].f14 = 1;
    }
    func_80011DFC();
}
