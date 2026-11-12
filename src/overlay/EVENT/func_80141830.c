/* Address: 0x80141830 */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void FUN_80011DFC();
extern void FUN_80015D28();
extern void FUN_801050D0();

void func_80141830(void)
{
    FUN_80015D28(69, 0x801B0000, 34536);
    FUN_801050D0();
    FUN_80011DFC();
}
