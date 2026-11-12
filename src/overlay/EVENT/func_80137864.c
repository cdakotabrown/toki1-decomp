/* Address: 0x80137864 */
/* Size: 80 bytes (20 instructions) */
#include "types.h"

extern u16 D_80094718;
extern u8 D_800B1AF6;

extern void func_800F7074();
extern void func_8004CA48(void);

void func_80137864(void) {
    func_800F7074();

    if (D_80094718 == 2) {
        func_8004CA48();
        D_800B1AF6 = D_800B1AF6 - 1;
    }
}
