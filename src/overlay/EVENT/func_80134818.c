/* @PostProc: scheduler_normalizer --target-exe assets/overlays/EVENT.EXN --address 0x80134818 */
/* Address: 0x80134818 */
/* Size: 96 bytes (24 instructions) */
#include "types.h"

extern u8 D_800EAFA7;
extern u8 D_800EB02B;
extern u8 D_800EB02F;
extern u8 D_800EB06F;
extern u8 D_800EB073;

extern void func_8004C984(void);

void func_80134818(void) {
    u8 v0;

    func_8004C984();
    v0 = D_800EAFA7;
    if (v0 == 0) {
        D_800EB02B = D_800EB02B & 0xFF7F;
        D_800EB06F = D_800EB06F & 0xFF7F;
    }
    D_800EB02F = v0;
    D_800EB073 = v0;
}
