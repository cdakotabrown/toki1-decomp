/* Address: 0x80139188 */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void FUN_80011DFC();
extern void FUN_80015D28();
extern void FUN_800FBDC0();

void func_80139188(void)
{
    FUN_80015D28(69, 0x801B0000, 32326);
    FUN_800FBDC0();
    FUN_80011DFC();
}
