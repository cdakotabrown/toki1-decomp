/* Address: 0x80133198 */
/* Size: 52 bytes (13 instructions) */
#include "types.h"

extern s32 func_8001496C(void);
extern void func_80011DFC();

void func_80133198(void) {
    if (func_8001496C() == 1) {
        func_80011DFC();
    }
}
