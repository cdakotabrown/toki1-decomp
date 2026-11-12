/* Address: 0x80153304 (absorbed head)  Size: 136 bytes (34 instructions) */
/* @SymbolStart: 0x80153304 */
/* @SymbolSize: 140 */
#include "types.h"

extern u8 D_800B1746;
extern u8 *D_801251F0;
extern u8 *D_801251F4;

extern u16 D_8012439C;
extern u16 D_8012439E;
extern u16 D_801243A0;
extern u16 D_8012438C;
extern u16 D_8012438E;
extern u16 D_80124390;

extern void func_80114B0C(void);

void func_80153318(void) {
    s32 idx;
    u8 *p;
    u8 *q;

    idx = D_800B1746;
    p = D_801251F0;
    p = p + (idx * 3);
    D_8012439C = p[0];
    D_8012439E = p[1];
    D_801243A0 = p[2];

    q = D_801251F4;
    q = q + (idx * 3);
    D_8012438C = q[0];
    D_8012438E = q[1];
    D_80124390 = q[2];

    func_80114B0C();
}
