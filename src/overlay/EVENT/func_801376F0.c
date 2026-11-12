/* Address: 0x801376F0 */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void FUN_80011DFC();
extern void FUN_80015D28();
extern void FUN_800F99CC();

void func_801376F0(void)
{
    FUN_80015D28(69, 0x801B0000, 31729);
    FUN_800F99CC();
    FUN_80011DFC();
}
