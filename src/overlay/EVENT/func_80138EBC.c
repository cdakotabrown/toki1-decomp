/* Address: 0x80138EBC */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void FUN_80015D28(s32 resourceId, s32 baseAddr, s32 offset);
extern void FUN_800FBD10(void);
extern void FUN_80011DFC(void);

void func_80138EBC(void) {

    FUN_80015D28(61, 0x801B0000, 32265);
    FUN_800FBD10();
    FUN_80011DFC();
}
