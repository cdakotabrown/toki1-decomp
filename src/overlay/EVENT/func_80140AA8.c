/* Address: 0x80140AA8 */
/* Size: 88 bytes (22 instructions) */
#include "types.h"

extern u8 D_800B1746;

extern s32 func_8002328C(u8);
extern void func_8004A8EC(s32);
extern s32 func_8002336C(u8);

void func_80140AA8(void) {
    if ((u32)func_8002328C(D_800B1746) < 2) {
        func_8004A8EC(0);
    } else {
        func_8004A8EC(func_8002336C(D_800B1746));
    }
}
