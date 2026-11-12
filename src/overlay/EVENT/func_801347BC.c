/* @PostProc: scheduler_normalizer */
/* Address: 0x801347BC */
/* Size: 92 bytes (23 instructions) */
#include "types.h"

extern u8 D_800EB02B;
extern u8 D_800EB02F;
extern u8 D_800EB06F;
extern u8 D_800EB073;
extern u8 D_800EAFA7;

extern void func_8004C90C(void);

void func_801347BC(void) {
    u8 val;

    func_8004C90C();
    val = D_800EAFA7;
    D_800EB02B = D_800EB02B | 0x80;
    D_800EB06F = D_800EB06F | 0x80;
    D_800EB02F = val;
    D_800EB073 = val;
}
