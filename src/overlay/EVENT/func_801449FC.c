/* Address: 0x801449FC */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern u8 D_8008093C;

extern void func_80011DFC();

void func_801449FC(void) {
    D_8008093C = 15;
    func_80011DFC();
}
