/* Address: 0x801356E0 */
/* Size: 80 bytes (20 instructions) */
#include "types.h"

extern u16 D_80094718;
extern u8 D_800B1AF6;

extern void func_800F7074();
extern void func_8004CA18(void);

void func_801356E0(void) {
    func_800F7074();

    if (D_80094718 == 4) {
        func_8004CA18();
        D_800B1AF6 = D_800B1AF6 - 1;
    }
}
