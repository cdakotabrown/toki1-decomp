/* Address: 0x80147228 */
/* Size: 64 bytes (16 instructions) */
#include "types.h"

extern u16 D_80094714;
extern u16 D_80094718;

extern void func_800F7074();
extern void func_80011DFC();

void func_80147228(void) {
    func_800F7074();
    D_80094714 = D_80094714 + 1;
    D_80094718 = 0;
    func_80011DFC();
}
