/* @CompileFlags: -cckr */
/* @SymbolSize: 684 */
#include "types.h"
typedef struct {
    u8 pad0[38];
    s16 f38;
    u8 pad40[2];
    s16 f42;
    u8 pad44[4];
    u32 b0 : 1;
    u32 b1 : 1;
    u32 rest : 30;
    u8 pad52[4];
} Rec;
typedef union {
    Rec r[80];
    u8 b[4480];
    u32 w[1120];
    s16 h[2240];
} Big;
extern Big D_800F53A0;
extern s16 D_800F54AE;
extern u8 D_801486E4;
extern u8 D_80148754;
extern u8 D_80148758;
extern s16 D_80148620;
extern s16 D_8014875C[];
extern s16 D_8014876C[];
#define SEL (D_800F53A0.b[3887])

void func_8013F2A4()
{
    short count;
    short i;
    int targ;
    int ov;
    int bit;
    u32 r;

    count = 0;
    for (i = 0; i < 11; i++) count += D_800F53A0.r[i + 7].b1 > 0;
    targ = D_80148754;
    i = 0;
loop:
    SEL = (D_800F53A0.w[1074] + i) % 10 + 1;
    i++;
    bit = D_800F53A0.r[SEL + 7].b1;
    if (bit == 0 && SEL >= 9) goto loop;
    if (targ == SEL) goto loop;
    if (bit == 0 && count >= 3) goto loop;
    ov = D_801486E4;
    if ((ov == 7 || ov == 9) && targ != ov) {
        SEL = ov;
    }
    D_80148758 = SEL;
    D_80148620 = 0;
    func_80141550();
    func_8008F618(SEL);
    if (!D_800F53A0.r[SEL + 7].b1) {
        D_800F53A0.r[SEL + 7].b1 = 1;
        D_800F53A0.b[1808] = D_800F53A0.b[4312];
        D_800F53A0.b[1809] = D_800F53A0.b[4313];
        D_800F53A0.b[1810] = 24;
        func_80090960(4, 0);
        D_800F53A0.b[1841 + D_800F53A0.b[3886] * 8] = 2;
        func_8004DEAC(5);
    } else {
        D_800F53A0.r[SEL + 7].f42 += 4;
        for (i = 0; i < 7; i++) {
            if (D_800F54AE < D_8014875C[i]) {
                break;
            }
        }
        D_800F53A0.r[SEL + 7].f38 += D_8014876C[i];
        func_80090960(4, 0);
        func_8004DE1C();
    }
}
