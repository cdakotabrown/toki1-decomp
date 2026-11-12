/* Address: 0x80138184 */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void FUN_80011DFC();
extern void FUN_80015D28();
extern void FUN_800FBBF0();

void func_80138184(void)
{
    FUN_80015D28(69, 0x801B0000, 32143);
    FUN_800FBBF0();
    FUN_80011DFC();
}
