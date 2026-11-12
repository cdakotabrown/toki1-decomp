/* Address: 0x801397BC */
/* Size: 68 bytes (17 instructions) */
#include "types.h"

extern s32 D_800EECBC;
extern u8 D_800B1AF6;

extern void func_80011DFC();

void func_801397BC(void) {
    func_80011DFC();
    if (D_800EECBC != 0) {
        D_800B1AF6 += 19;
    }
}
