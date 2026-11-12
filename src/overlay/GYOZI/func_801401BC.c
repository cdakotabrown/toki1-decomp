/* @SymbolStart: 0x801401B4 */
/* @SymbolSize: 1088 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_COPYPROP_VAR=-8,-32 IDO52_COPYPROP_SITE=lod:2:deny,lod:3:deny,lod:5:deny,lod:11:deny IDO52_AS1_NO_PASS1_PROP=7 IDO52_FORCE_WEB_COLOR=33:a0,87:a0,5:t0,149:a2 */
#include "types.h"

extern u32 D_800F6468;
extern void func_8005BE34();
extern s32 func_8005C1B8();
extern s32 func_8005B32C();
extern void func_8005B06C();
extern void func_8004DE1C();

typedef struct {
    u8 pad[0x3E];
    u8 week;
} Save;

extern Save D_800F53A0;
extern s16 D_800F549A;
extern s16 D_800F54A2;
extern u16 D_800F5A10;
extern s32 D_800F62F8;
extern u8 D_800F6306;
extern u8 D_800F6316;
extern u8 D_800F6326;
extern u8 D_800F6336;
extern u8 D_80129F43;
extern s16 D_80129F66;
extern s16 D_80129F6A;
extern u8 D_8012B907;
extern s16 D_8012B91C;
extern s16 D_8012B920;
extern s16 D_8012B92A;
extern s16 D_8012B92E;
extern s32 D_8012E6C0;
extern s16 D_80148620;
extern s16 D_80148624;
extern u8 D_80148754;
extern u8 D_80148758;
extern u8 D_80148818;
extern u8 D_80148820;
u8 D_80148828[4] = {0};

s32 func_801401BC()
{
    u32 seed;
    u32 ten;
    u32 a;
    u32 b;
    u32 c;
    u8 d;
    u8 x1;
    u8 x2;
    u8 idx;
    s16 i;
    s32 j;
    s16 k;
    s32 v;
    Save *sv;

    seed = D_800F6468;
    a = seed;
    a = a % 4;
    i = 1;
    for (;;) {
        b = seed * 2 + i;
        b = (u8)(b % 4);
        j = i;
        if (a != b) {
            break;
        }
        i++;
    }
    ten = 10;
    for (;;) {
        c = seed / ten + j;
        c = (u8)(c % 4);
        if (a != c && b != c) {
            break;
        }
        i++;
        j = i;
        ten = 10;
    }
    for (;;) {
        d = i % 4;
        if (a != d && (s32)b != d) {
            D_80148828[3] = d;
            if ((s32)c != d) {
                D_80148828[0] = a;
                D_80148828[1] = b;
                D_80148828[2] = c;
                break;
            }
        }
        i++;
    }

    x1 = D_80148754;
    x2 = D_80148758;
    for (k = 0; k < 4; k++) {
        if (D_80148828[k] == 1) {
            D_80148828[k] = 11;
        } else if (D_80148828[k] == 2) {
            D_80148828[k] = x1;
        } else if (D_80148828[k] == 3) {
            D_80148828[k] = x2;
        }
    }

    func_8005BE34(0, D_80129F66, D_80129F6A);
    func_8005C1B8(0);

    if ((D_800F62F8 & 0x600060) != 0) {
        if (func_8005B32C() != 0) {
        if (D_800F6306 != 0) {
            D_80148818 = 0;
        } else if (D_800F6316 != 0) {
            D_80148818 = 1;
        } else if (D_800F6326 != 0) {
            D_80148818 = 2;
        } else if (D_800F6336 != 0) {
            D_80148818 = 3;
        } else {
            return 0;
        }
        D_80129F43 = D_80129F43 & 0xFF7F;
        D_8012E6C0 = 0;
        func_8005B06C(1);
        D_8012B907 = D_8012B907 | 0x80;
        idx = D_80148818;
        sv = &D_800F53A0;
        v = (D_80148828[idx] == 11);
        if (v) {
            v = (sv->week == 95);
        }
        D_8012B91C = D_80148828[idx] + (sv->week == 96) * 11 + (sv->week == 97) * 23 - v + 1;
        D_8012B92A = idx * 78 - idx - 117;
        D_8012B92E = -60;
        D_8012B920 = 0;
        D_80148620 += D_80148828[idx] * 3 + sv->week - 95;
        D_80148820 = 1;
        D_80148818 = idx;
        if ((sv->week == 95 && D_80148828[idx] == 0 && D_800F54A2 >= 100) ||
            (((D_80148828[idx] == 0) & (sv->week == 95)) && D_800F5A10 >= 100) ||
            (((D_80148828[idx] == 1) & (sv->week == 95)) && D_800F549A >= 70) ||
            (((D_80148828[idx] == 1) & (sv->week == 97)) && D_800F549A >= 100) ||
            (((D_80148828[idx] == 2) & (sv->week == 96)) && D_800F549A >= 100)) {
            D_80148624 = 1;
        }
        func_8004DE1C();
        }
    }
}
