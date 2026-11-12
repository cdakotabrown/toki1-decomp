/* Address: 0x80142B70 */
/* Size: 1300 bytes (325 insns) */
/* @CompileEnv: IDO52_LDA_NOINREG_CLASSES=4:6:6 IDO52_UGEN_TEMP_REG=61:t0,71:t6,72:t7,73:t8,74:t9 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u32 unk00;
    s16 unk04;
    s16 unk06;
    u8 unk08[6];
    u8 unk0E;
    u8 unk0F[5];
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17[13];
} GyoziObj;

typedef struct {
    s16 lo;
    s16 hi;
} GyoziHalf;

typedef union {
    s32 w;
    GyoziHalf h;
} GyoziFx;

extern u8 D_800F6415[];
extern u8 D_800F6418[];
extern GyoziObj D_8012CA40[];
extern s16 D_8012CAD4;
extern s32 D_8012E694;
extern u8 D_801317EB;
extern s32 D_80148BF0[];
extern GyoziFx D_80148CB8[];
extern s32 D_80148D80[];
extern s16 D_80148E48[];
extern s16 D_80148EAC;
extern u32 D_80148EB0;

extern func_800BCE40();

func_80142B70()
{
    s16 i;
    s16 n;
    s32 t;

    if (D_80148EAC == 0) {
        return 0;
    }
    if (D_80148EB0 == 30) {
        D_80148EAC++;
        D_8012CAD4 = 180;
        D_80148E48[0] = 1;
    } else if (D_80148EB0 == 90) {
        D_80148EAC++;
        D_8012CAD4 = 40;
    } else if (D_80148EB0 > 200 && D_80148EB0 % 20 == 0 && D_80148EAC < 50) {
        D_80148EAC++;
        D_8012CA40[D_80148EAC + 3].unk0E = func_800BCE40() % 2 * 8;
        if (D_8012CA40[D_80148EAC + 3].unk0E == 8) {
            D_800F6418[D_80148EAC] = 12;
            D_80148E48[D_80148EAC - 1] = func_800BCE40() % 2 + 3;
        } else {
            D_800F6418[D_80148EAC] = 9;
            D_80148E48[D_80148EAC - 1] = func_800BCE40() % 2 + 1;
        }
        t = D_80148E48[D_80148EAC - 1];
        D_80148BF0[D_80148EAC - 1] = 65536 / ((t >= 3) * 2 + t + 1);
    }
    if (D_8012E694 == 0) {
        n = D_80148EAC;
    } else {
        n = 10;
        for (i = 14; i < 54; i++) {
            D_8012CA40[i].unk00 |= 0x80000000;
        }
    }
    for (i = 4; i < n + 4; i++) {
        D_8012CA40[i].unk14 = D_8012CA40[i].unk15 = D_8012CA40[i].unk16 = D_801317EB;
        D_8012CA40[i].unk00 &= 0x7FFFFFFF;
        if (D_80148EB0 % D_80148E48[i - 4] == 0) {
            D_8012CA40[i].unk06++;
        }
        D_80148CB8[i - 4].w += D_80148BF0[i - 4];
        D_80148BF0[i - 4] += (D_80148D80[i - 4] - D_80148CB8[i - 4].w) >> 8;
        D_8012CA40[i].unk04 = D_80148CB8[i - 4].h.hi;
        if (D_8012CA40[i].unk06 >= 41) {
            t = func_800BCE40();
            D_80148CB8[i - 4].h.lo = 0;
            D_8012CA40[i].unk06 = -120;
            D_8012CA40[i].unk04 = t % 320 - 159;
            D_80148CB8[i - 4].h.hi = D_8012CA40[i].unk04;
            D_80148D80[i - 4] = D_80148CB8[i - 4].w;
            D_8012CA40[i].unk0E = func_800BCE40() % 2 * 8;
            if (D_8012E694 == 1) {
                D_8012CA40[i].unk0E = 0;
            }
            if (D_8012CA40[i].unk0E == 8) {
                D_800F6415[i] = 12;
                D_80148E48[i - 4] = func_800BCE40() % 2 + 3;
            } else {
                D_800F6415[i] = 9;
                D_80148E48[i - 4] = func_800BCE40() % 2 + 1;
            }
            t = D_80148E48[i - 4];
            D_80148BF0[i - 4] = 65536 / ((t >= 3) * 2 + t + 1);
        }
    }
    D_80148EB0++;
}
