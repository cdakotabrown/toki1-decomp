/* Address: 0x8013B710 */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void FUN_80011DFC();
extern void FUN_80015D28();
extern void FUN_800FDD80();

void func_8013B710(void)
{
    FUN_80015D28(69, 0x801B0000, 32676);
    FUN_800FDD80();
    FUN_80011DFC();
}
