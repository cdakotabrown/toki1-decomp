/* Address: 0x8013B748 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern u8 D_800EB02A;

extern void func_80011DFC();

void func_8013B748(void) {
    D_800EB02A = 5;
    func_80011DFC();
}
