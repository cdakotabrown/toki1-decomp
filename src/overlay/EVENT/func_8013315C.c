/* Address: 0x8013315C */
/* Size: 60 bytes (15 instructions) */
#include "types.h"

extern s32 func_8001496C(void);
extern void func_800433D0();
extern void func_80011DFC();

void func_8013315C(void) {
    if (func_8001496C() == 1) {
        func_800433D0(512);
        func_80011DFC();
    }
}
