/* Address: 0x8013C1BC */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern u8 D_800B1746;
extern s16 D_800EAFB6;

extern void func_8004BC20();
extern void func_80011DFC();

void func_8013C1BC(void) {
    func_8004BC20(D_800B1746);
    D_800EAFB6 = 2;
    func_80011DFC();
}
