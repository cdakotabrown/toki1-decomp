/* @PostProc: swap_registers t9:t0 --bidirectional */
/* @PostProc: swap_registers t8:t9 --bidirectional */
/* @PostProc: swap_registers t7:t8 --bidirectional */
/* @PostProc: swap_registers t6:t7 --bidirectional */
/* @PostProc: swap_registers v1:t6 --bidirectional */
/* Address: 0x8015360C */
/* Size: 112 bytes (28 instructions) */
#include "types.h"

extern u16 D_80094730;
extern u16 D_80094734;
extern u8 D_800B1746;
extern u16 D_8012439C;
extern u16 D_8012439E;
extern u16 D_801243A0;

extern u8 func_8002328C(u8 arg);
extern void func_80011DFC(void);

void func_8015360C(void) {
    u32 t6;
    u8 v0;

    v0 = func_8002328C(D_800B1746);
    t6 = (u32)(v0 & 0x7F);
    D_80094730 = (u16)((((t6 >= 2) + (t6 >= 3))) << 1);
    D_80094734 = 0;
    D_8012439E = 0;
    D_8012439C = 0;
    D_801243A0 = 0;
    func_80011DFC();
}
