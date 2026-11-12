/* Address: 0x8013A100 */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void FUN_80011DFC();
extern void FUN_80015D28();
extern void FUN_800FDC20();

void func_8013A100(void)
{
    FUN_80015D28(61, 0x801B0000, 32554);
    FUN_800FDC20();
    FUN_80011DFC();
}
