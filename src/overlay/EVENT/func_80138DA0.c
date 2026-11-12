/* Address: 0x80138DA0 */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void FUN_80015D28(s32 resourceId, s32 baseAddr, s32 offset);
extern void FUN_800FBCA0(void);
extern void FUN_80011DFC(void);

void func_80138DA0(void) {

    FUN_80015D28(53, 0x801B0000, 32212);
    FUN_800FBCA0();
    FUN_80011DFC();
}
