/* @CompileEnv: IDO52_CONST_INREG_VALUES=0 IDO52_COPYPROP_SITE=lod:1:deny IDO52_FORM_MERGE=const:0:13 IDO52_FORM_SITE=const:0:37:0,const:0:52:0 IDO52_FORCE_WEB_COLOR=46:a1 IDO52_AS1_TIE_TAKE=76 */
/* @CompileFlags: -cckr */
/* @SymbolStart: 0x8013EE0C */
/* @SymbolSize: 388 */
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
extern s16 D_800F5746;
extern s32 D_801481D4;
extern s32 D_80148208;
extern s32 D_8014823C;
extern s32 D_80148614;
extern s32 D_80148618;
extern s32 D_8014861C;
extern s16 D_80148620;
extern s8 D_80148750;
extern s16 D_8014875C[];
extern s16 D_8014876C[];
#define SEL (D_800F53A0.b[3887])

void func_8013EE14()
{
    short i;
    int x;
    int y;

    D_80148750 = 0;
    if (!D_800F53A0.r[16].b1 && D_800F5746 >= 20 && (D_800F53A0.w[1074] & 1)) {
        D_80148750 = 1;
    }
    SEL = 0;
    x = 0;
    D_80148620 = (((u8)system_config_disp(x, 0) & 0x7F) == 4) << 2;
    D_80148614 = D_801481D4;
    D_80148618 = D_80148208;
    D_8014861C = D_8014823C;
    y = SEL;
    func_8008F618(y);
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
