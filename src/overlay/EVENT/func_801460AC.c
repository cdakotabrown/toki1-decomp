/* Address: 0x801460AC */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void FUN_80011DFC();
extern void FUN_80015D28();
extern void FUN_801081D0();

void func_801460AC(void)
{
    FUN_80015D28(61, 0x801B0000, 35548);
    FUN_801081D0();
    FUN_80011DFC();
}
