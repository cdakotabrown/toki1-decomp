/* Address: 0x80154B4C */
/* Size: 608 bytes (152 instructions) */
#include "types.h"

typedef struct {
    s16 v[5];
} EvtTbl5;

extern EvtTbl5 D_80125360;

extern s32 D_800EECBC;
extern s16 D_8012438C[];
extern s16 D_8012439C[];
extern u8 D_800B0860[];
extern u8 D_800B1746;

extern void func_8004A8EC();
extern void func_80033E88();
extern void func_80118DAC();
extern void func_8004C250();
extern void func_80011DFC();

void func_80154B4C(void) {
    s16 *q;
    s16 outA;
    s16 outB;
    u8 *p;
    EvtTbl5 tbl;

    tbl = D_80125360;
    func_8004A8EC(D_8012438C[D_800EECBC]);
    func_80033E88();
    q = D_8012439C;

    func_80118DAC(3 - q[D_800EECBC], &outA, &outB);
    p = &D_800B0860[D_800B1746 * 52];
    *(s32 *)(p + 420) +=
        (((3 - q[D_800EECBC]) * outA * outB) << 16) / 16;

    func_80118DAC(2 - q[D_800EECBC | 0], &outA, &outB);
    p = &D_800B0860[D_800B1746 * 52];
    *(s32 *)(p + 424) +=
        (((2 - q[D_800EECBC]) * outA * outB) << 16) / 16;

    func_80118DAC(2 - q[D_800EECBC | 0], &outA, &outB);
    p = &D_800B0860[D_800B1746 * 52];
    *(s32 *)(p + 424) +=
        ((tbl.v[q[D_800EECBC]] * outA * outB) << 16) / 16;

    func_8004C250();
    func_80011DFC();
}
