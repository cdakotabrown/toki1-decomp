/* Address: 0x80138EF4 */
/* Size: 76 bytes (19 instructions) */
#include "types.h"

extern u8 D_800F19AB;
extern u8 D_800EB02F;
extern u8 D_800EB02B;

extern void func_8004C984();

void func_80138EF4(void) {
    u8 val;

    func_8004C984();

    val = D_800F19AB;
    D_800EB02F = val;

    if ((u32)val < 8u) {
        D_800EB02B = D_800EB02B & 0xFF7F;
    }
}
