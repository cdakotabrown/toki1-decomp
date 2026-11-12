/* Address: 0x80143F90 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern u8 D_800B1746;

extern void func_80011DFC();

void func_80143F90(void) {
    D_800B1746 = 4;
    func_80011DFC();
}
